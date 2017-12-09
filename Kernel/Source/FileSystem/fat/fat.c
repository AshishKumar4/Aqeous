/* fat.c - FAT filesystem */
/* GRUB FAT DRIVERS, to be ported */
#include <grub/fs.h>
#include <grub/disk.h>
#include <grub/file.h>
#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/err.h>
#include <grub/dl.h>
#include <grub/charset.h>
#include <grub/fat.h>
#include <grub/i18n.h>

MOD_LICENSE ("GPLv3+");

enum
  {
    FAT_ATTR_READ_ONLY = 0x01,
    FAT_ATTR_HIDDEN = 0x02,
    FAT_ATTR_SYSTEM = 0x04,
#ifndef MODE_EXFAT
    FAT_ATTR_VOLUME_ID = 0x08,
#endif
    FAT_ATTR_DIRECTORY = 0x10,
    FAT_ATTR_ARCHIVE = 0x20,

#ifndef MODE_EXFAT
    FAT_ATTR_LONG_NAME = (FAT_ATTR_READ_ONLY
			       | FAT_ATTR_HIDDEN
			       | FAT_ATTR_SYSTEM
			       | FAT_ATTR_VOLUME_ID),
#endif
    FAT_ATTR_VALID = (FAT_ATTR_READ_ONLY
			   | FAT_ATTR_HIDDEN
			   | FAT_ATTR_SYSTEM
			   | FAT_ATTR_DIRECTORY
			   | FAT_ATTR_ARCHIVE
#ifndef MODE_EXFAT
			   | FAT_ATTR_VOLUME_ID
#endif
			   )
  };

#ifdef MODE_EXFAT
struct exfat_bpb
{
  uint8_t jmp_boot[3];
  uint8_t oem_name[8];
  uint8_t mbz[53];
  uint64_t num_hidden_sectors;
  uint64_t num_total_sectors;
  uint32_t num_reserved_sectors;
  uint32_t sectors_per_fat;
  uint32_t cluster_offset;
  uint32_t cluster_count;
  uint32_t root_cluster;
  uint32_t num_serial;
  uint16_t fs_revision;
  uint16_t volume_flags;
  uint8_t bytes_per_sector_shift;
  uint8_t sectors_per_cluster_shift;
  uint8_t num_fats;
  uint8_t num_ph_drive;
  uint8_t reserved[8];
} __attribute__ ((packed));
typedef struct exfat_bpb current_fat_bpb_t;
#else
typedef struct fat_bpb current_fat_bpb_t;
#endif

#ifdef MODE_EXFAT
struct fat_dir_entry
{
  uint8_t entry_type;
  union
  {
    uint8_t placeholder[31];
    struct {
      uint8_t secondary_count;
      uint16_t checksum;
      uint16_t attr;
      uint16_t reserved1;
      uint32_t c_time;
      uint32_t m_time;
      uint32_t a_time;
      uint8_t c_time_tenth;
      uint8_t m_time_tenth;
      uint8_t a_time_tenth;
      uint8_t reserved2[9];
    }  __attribute__ ((packed)) file;
    struct {
      uint8_t flags;
      uint8_t reserved1;
      uint8_t name_length;
      uint16_t name_hash;
      uint16_t reserved2;
      uint64_t valid_size;
      uint32_t reserved3;
      uint32_t first_cluster;
      uint64_t file_size;
    }   __attribute__ ((packed)) stream_extension;
    struct {
      uint8_t flags;
      uint16_t str[15];
    }  __attribute__ ((packed))  file_name;
    struct {
      uint8_t character_count;
      uint16_t str[15];
    }  __attribute__ ((packed))  volume_label;
  }  __attribute__ ((packed)) type_specific;
} __attribute__ ((packed));

struct fat_dir_node
{
  uint32_t attr;
  uint32_t first_cluster;
  uint64_t file_size;
  uint64_t valid_size;
  int have_stream;
  int is_label;
};

typedef struct fat_dir_node fat_dir_node_t;

#else
struct fat_dir_entry
{
  uint8_t name[11];
  uint8_t attr;
  uint8_t nt_reserved;
  uint8_t c_time_tenth;
  uint16_t c_time;
  uint16_t c_date;
  uint16_t a_date;
  uint16_t first_cluster_high;
  uint16_t w_time;
  uint16_t w_date;
  uint16_t first_cluster_low;
  uint32_t file_size;
} __attribute__ ((packed));

struct fat_long_name_entry
{
  uint8_t id;
  uint16_t name1[5];
  uint8_t attr;
  uint8_t reserved;
  uint8_t checksum;
  uint16_t name2[6];
  uint16_t first_cluster;
  uint16_t name3[2];
} __attribute__ ((packed));

typedef struct fat_dir_entry fat_dir_node_t;

#endif

struct fat_data
{
  int logical_sector_bits;
  uint32_t num_sectors;

  uint32_t fat_sector;
  uint32_t sectors_per_fat;
  int fat_size;

  uint32_t root_cluster;
#ifndef MODE_EXFAT
  uint32_t root_sector;
  uint32_t num_root_sectors;
#endif

  int cluster_bits;
  uint32_t cluster_eof_mark;
  uint32_t cluster_sector;
  uint32_t num_clusters;

  uint8_t attr;
  ssize_t file_size;
  uint32_t file_cluster;
  uint32_t cur_cluster_num;
  uint32_t cur_cluster;

  uint32_t uuid;
};

static dl_t my_mod;

#ifndef MODE_EXFAT
static int
fat_log2 (unsigned x)
{
  int i;

  if (x == 0)
    return -1;

  for (i = 0; (x & 1) == 0; i++)
    x >>= 1;

  if (x != 1)
    return -1;

  return i;
}
#endif

static struct fat_data *
fat_mount (disk_t disk)
{
  current_fat_bpb_t bpb;
  struct fat_data *data = 0;
  uint32_t first_fat, magic;

  if (! disk)
    goto fail;

  data = (struct fat_data *) malloc (sizeof (*data));
  if (! data)
    goto fail;

  /* Read the BPB.  */
  if (disk_read (disk, 0, 0, sizeof (bpb), &bpb))
    goto fail;

#ifdef MODE_EXFAT
  if (memcmp ((const char *) bpb.oem_name, "EXFAT   ",
		   sizeof (bpb.oem_name)) != 0)
    goto fail;
#endif

  /* Get the sizes of logical sectors and clusters.  */
#ifdef MODE_EXFAT
  data->logical_sector_bits = bpb.bytes_per_sector_shift;
#else
  data->logical_sector_bits =
    fat_log2 (le_to_cpu16 (bpb.bytes_per_sector));
#endif
  if (data->logical_sector_bits < DISK_SECTOR_BITS
      || data->logical_sector_bits >= 16)
    goto fail;
  data->logical_sector_bits -= DISK_SECTOR_BITS;

#ifdef MODE_EXFAT
  data->cluster_bits = bpb.sectors_per_cluster_shift;
#else
  data->cluster_bits = fat_log2 (bpb.sectors_per_cluster);
#endif
  if (data->cluster_bits < 0 || data->cluster_bits > 25)
    goto fail;
  data->cluster_bits += data->logical_sector_bits;

  /* Get information about FATs.  */
#ifdef MODE_EXFAT
  data->fat_sector = (le_to_cpu32 (bpb.num_reserved_sectors)
		      << data->logical_sector_bits);
#else
  data->fat_sector = (le_to_cpu16 (bpb.num_reserved_sectors)
		      << data->logical_sector_bits);
#endif
  if (data->fat_sector == 0)
    goto fail;

#ifdef MODE_EXFAT
  data->sectors_per_fat = (le_to_cpu32 (bpb.sectors_per_fat)
			   << data->logical_sector_bits);
#else
  data->sectors_per_fat = ((bpb.sectors_per_fat_16
			    ? le_to_cpu16 (bpb.sectors_per_fat_16)
			    : le_to_cpu32 (bpb.version_specific.fat32.sectors_per_fat_32))
			   << data->logical_sector_bits);
#endif
  if (data->sectors_per_fat == 0)
    goto fail;

  /* Get the number of sectors in this volume.  */
#ifdef MODE_EXFAT
  data->num_sectors = ((le_to_cpu64 (bpb.num_total_sectors))
		       << data->logical_sector_bits);
#else
  data->num_sectors = ((bpb.num_total_sectors_16
			? le_to_cpu16 (bpb.num_total_sectors_16)
			: le_to_cpu32 (bpb.num_total_sectors_32))
		       << data->logical_sector_bits);
#endif
  if (data->num_sectors == 0)
    goto fail;

  /* Get information about the root directory.  */
  if (bpb.num_fats == 0)
    goto fail;

#ifndef MODE_EXFAT
  data->root_sector = data->fat_sector + bpb.num_fats * data->sectors_per_fat;
  data->num_root_sectors
    = ((((uint32_t) le_to_cpu16 (bpb.num_root_entries)
	 * sizeof (struct fat_dir_entry)
	 + le_to_cpu16 (bpb.bytes_per_sector) - 1)
	>> (data->logical_sector_bits + DISK_SECTOR_BITS))
       << (data->logical_sector_bits));
#endif

#ifdef MODE_EXFAT
  data->cluster_sector = (le_to_cpu32 (bpb.cluster_offset)
			  << data->logical_sector_bits);
  data->num_clusters = (le_to_cpu32 (bpb.cluster_count)
			  << data->logical_sector_bits);
#else
  data->cluster_sector = data->root_sector + data->num_root_sectors;
  data->num_clusters = (((data->num_sectors - data->cluster_sector)
			 >> data->cluster_bits)
			+ 2);
#endif

  if (data->num_clusters <= 2)
    goto fail;

#ifdef MODE_EXFAT
  {
    /* exFAT.  */
    uint16_t flags = le_to_cpu16 (bpb.volume_flags);

    data->root_cluster = le_to_cpu32 (bpb.root_cluster);
    data->fat_size = 32;
    data->cluster_eof_mark = 0xffffffff;

    if ((flags & 0x1) && bpb.num_fats > 1)
      data->fat_sector += data->sectors_per_fat;
  }
#else
  if (! bpb.sectors_per_fat_16)
    {
      /* FAT32.  */
      uint16_t flags = le_to_cpu16 (bpb.version_specific.fat32.extended_flags);

      data->root_cluster = le_to_cpu32 (bpb.version_specific.fat32.root_cluster);
      data->fat_size = 32;
      data->cluster_eof_mark = 0x0ffffff8;

      if (flags & 0x80)
	{
	  /* Get an active FAT.  */
	  unsigned active_fat = flags & 0xf;

	  if (active_fat > bpb.num_fats)
	    goto fail;

	  data->fat_sector += active_fat * data->sectors_per_fat;
	}

      if (bpb.num_root_entries != 0 || bpb.version_specific.fat32.fs_version != 0)
	goto fail;
    }
  else
    {
      /* FAT12 or FAT16.  */
      data->root_cluster = ~0U;

      if (data->num_clusters <= 4085 + 2)
	{
	  /* FAT12.  */
	  data->fat_size = 12;
	  data->cluster_eof_mark = 0x0ff8;
	}
      else
	{
	  /* FAT16.  */
	  data->fat_size = 16;
	  data->cluster_eof_mark = 0xfff8;
	}
    }
#endif

  /* More sanity checks.  */
  if (data->num_sectors <= data->fat_sector)
    goto fail;

  if (disk_read (disk,
		      data->fat_sector,
		      0,
		      sizeof (first_fat),
		      &first_fat))
    goto fail;

  first_fat = le_to_cpu32 (first_fat);

  if (data->fat_size == 32)
    {
      first_fat &= 0x0fffffff;
      magic = 0x0fffff00;
    }
  else if (data->fat_size == 16)
    {
      first_fat &= 0x0000ffff;
      magic = 0xff00;
    }
  else
    {
      first_fat &= 0x00000fff;
      magic = 0x0f00;
    }

  /* Serial number.  */
#ifdef MODE_EXFAT
    data->uuid = le_to_cpu32 (bpb.num_serial);
#else
  if (bpb.sectors_per_fat_16)
    data->uuid = le_to_cpu32 (bpb.version_specific.fat12_or_fat16.num_serial);
  else
    data->uuid = le_to_cpu32 (bpb.version_specific.fat32.num_serial);
#endif

#ifndef MODE_EXFAT
  /* Ignore the 3rd bit, because some BIOSes assigns 0xF0 to the media
     descriptor, even if it is a so-called superfloppy (e.g. an USB key).
     The check may be too strict for this kind of stupid BIOSes, as
     they overwrite the media descriptor.  */
  if ((first_fat | 0x8) != (magic | bpb.media | 0x8))
    goto fail;
#else
  (void) magic;
#endif

  /* Start from the root directory.  */
  data->file_cluster = data->root_cluster;
  data->cur_cluster_num = ~0U;
  data->attr = FAT_ATTR_DIRECTORY;
  return data;

 fail:

  free (data);
  error (ERR_BAD_FS, "not a FAT filesystem");
  return 0;
}

static ssize_t
fat_read_data (disk_t disk, struct fat_data *data,
		    disk_read_hook_t read_hook, void *read_hook_data,
		    off_t offset, size_t len, char *buf)
{
  size_t size;
  uint32_t logical_cluster;
  unsigned logical_cluster_bits;
  ssize_t ret = 0;
  unsigned long sector;

#ifndef MODE_EXFAT
  /* This is a special case. FAT12 and FAT16 doesn't have the root directory
     in clusters.  */
  if (data->file_cluster == ~0U)
    {
      size = (data->num_root_sectors << DISK_SECTOR_BITS) - offset;
      if (size > len)
	size = len;

      if (disk_read (disk, data->root_sector, offset, size, buf))
	return -1;

      return size;
    }
#endif

  /* Calculate the logical cluster number and offset.  */
  logical_cluster_bits = (data->cluster_bits
			  + DISK_SECTOR_BITS);
  logical_cluster = offset >> logical_cluster_bits;
  offset &= (1ULL << logical_cluster_bits) - 1;

  if (logical_cluster < data->cur_cluster_num)
    {
      data->cur_cluster_num = 0;
      data->cur_cluster = data->file_cluster;
    }

  while (len)
    {
      while (logical_cluster > data->cur_cluster_num)
	{
	  /* Find next cluster.  */
	  uint32_t next_cluster;
	  unsigned long fat_offset;

	  switch (data->fat_size)
	    {
	    case 32:
	      fat_offset = data->cur_cluster << 2;
	      break;
	    case 16:
	      fat_offset = data->cur_cluster << 1;
	      break;
	    default:
	      /* case 12: */
	      fat_offset = data->cur_cluster + (data->cur_cluster >> 1);
	      break;
	    }

	  /* Read the FAT.  */
	  if (disk_read (disk, data->fat_sector, fat_offset,
			      (data->fat_size + 7) >> 3,
			      (char *) &next_cluster))
	    return -1;

	  next_cluster = le_to_cpu32 (next_cluster);
	  switch (data->fat_size)
	    {
	    case 16:
	      next_cluster &= 0xFFFF;
	      break;
	    case 12:
	      if (data->cur_cluster & 1)
		next_cluster >>= 4;

	      next_cluster &= 0x0FFF;
	      break;
	    }

	  dprintf ("fat", "fat_size=%d, next_cluster=%u\n",
			data->fat_size, next_cluster);

	  /* Check the end.  */
	  if (next_cluster >= data->cluster_eof_mark)
	    return ret;

	  if (next_cluster < 2 || next_cluster >= data->num_clusters)
	    {
	      error (ERR_BAD_FS, "invalid cluster %u",
			  next_cluster);
	      return -1;
	    }

	  data->cur_cluster = next_cluster;
	  data->cur_cluster_num++;
	}

      /* Read the data here.  */
      sector = (data->cluster_sector
		+ ((data->cur_cluster - 2)
		   << data->cluster_bits));
      size = (1 << logical_cluster_bits) - offset;
      if (size > len)
	size = len;

      disk->read_hook = read_hook;
      disk->read_hook_data = read_hook_data;
      disk_read (disk, sector, offset, size, buf);
      disk->read_hook = 0;
      if (errno)
	return -1;

      len -= size;
      buf += size;
      ret += size;
      logical_cluster++;
      offset = 0;
    }

  return ret;
}

struct fat_iterate_context
{
#ifdef MODE_EXFAT
  struct fat_dir_node dir;
#else
  struct fat_dir_entry dir;
#endif
  char *filename;
  uint16_t *unibuf;
  ssize_t offset;
};

static err_t
fat_iterate_init (struct fat_iterate_context *ctxt)
{
  ctxt->offset = -sizeof (struct fat_dir_entry);

#ifndef MODE_EXFAT
  /* Allocate space enough to hold a long name.  */
  ctxt->filename = malloc (0x40 * 13 * MAX_UTF8_PER_UTF16 + 1);
  ctxt->unibuf = (uint16_t *) malloc (0x40 * 13 * 2);
#else
  ctxt->unibuf = malloc (15 * 256 * 2);
  ctxt->filename = malloc (15 * 256 * MAX_UTF8_PER_UTF16 + 1);
#endif

  if (! ctxt->filename || ! ctxt->unibuf)
    {
      free (ctxt->filename);
      free (ctxt->unibuf);
      return errno;
    }
  return ERR_NONE;
}

static void
fat_iterate_fini (struct fat_iterate_context *ctxt)
{
  free (ctxt->filename);
  free (ctxt->unibuf);
}

#ifdef MODE_EXFAT
static err_t
fat_iterate_dir_next (disk_t disk, struct fat_data *data,
			   struct fat_iterate_context *ctxt)
{
  memset (&ctxt->dir, 0, sizeof (ctxt->dir));
  while (1)
    {
      struct fat_dir_entry dir;

      ctxt->offset += sizeof (dir);

      if (fat_read_data (disk, data, 0, 0, ctxt->offset, sizeof (dir),
			      (char *) &dir)
	   != sizeof (dir))
	break;

      if (dir.entry_type == 0)
	break;
      if (!(dir.entry_type & 0x80))
	continue;

      if (dir.entry_type == 0x85)
	{
	  unsigned i, nsec, slots = 0;

	  nsec = dir.type_specific.file.secondary_count;

	  ctxt->dir.attr = cpu_to_le16 (dir.type_specific.file.attr);
	  ctxt->dir.have_stream = 0;
	  for (i = 0; i < nsec; i++)
	    {
	      struct fat_dir_entry sec;
	      ctxt->offset += sizeof (sec);
	      if (fat_read_data (disk, data, 0, 0,
				      ctxt->offset, sizeof (sec), (char *) &sec)
		  != sizeof (sec))
		break;
	      if (!(sec.entry_type & 0x80))
		continue;
	      if (!(sec.entry_type & 0x40))
		break;
	      switch (sec.entry_type)
		{
		case 0xc0:
		  ctxt->dir.first_cluster = cpu_to_le32 (sec.type_specific.stream_extension.first_cluster);
		  ctxt->dir.valid_size
		    = cpu_to_le64 (sec.type_specific.stream_extension.valid_size);
		  ctxt->dir.file_size
		    = cpu_to_le64 (sec.type_specific.stream_extension.file_size);
		  ctxt->dir.have_stream = 1;
		  break;
		case 0xc1:
		  {
		    int j;
		    for (j = 0; j < 15; j++)
		      ctxt->unibuf[slots * 15 + j]
			= le_to_cpu16 (sec.type_specific.file_name.str[j]);
		    slots++;
		  }
		  break;
		default:
		  dprintf ("exfat", "unknown secondary type 0x%02x\n",
				sec.entry_type);
		}
	    }

	  if (i != nsec)
	    {
	      ctxt->offset -= sizeof (dir);
	      continue;
	    }

	  *utf16_to_utf8 ((uint8_t *) ctxt->filename, ctxt->unibuf,
			       slots * 15) = '\0';

	  return 0;
	}
      /* Allocation bitmap. */
      if (dir.entry_type == 0x81)
	continue;
      /* Upcase table. */
      if (dir.entry_type == 0x82)
	continue;
      /* Volume label. */
      if (dir.entry_type == 0x83)
	continue;
      dprintf ("exfat", "unknown primary type 0x%02x\n",
		    dir.entry_type);
    }
  return errno ? : ERR_EOF;
}

#else

static err_t
fat_iterate_dir_next (disk_t disk, struct fat_data *data,
			   struct fat_iterate_context *ctxt)
{
  char *filep = 0;
  int checksum = -1;
  int slot = -1, slots = -1;

  while (1)
    {
      unsigned i;

      /* Adjust the offset.  */
      ctxt->offset += sizeof (ctxt->dir);

      /* Read a directory entry.  */
      if (fat_read_data (disk, data, 0, 0,
			      ctxt->offset, sizeof (ctxt->dir),
			      (char *) &ctxt->dir)
	   != sizeof (ctxt->dir) || ctxt->dir.name[0] == 0)
	break;

      /* Handle long name entries.  */
      if (ctxt->dir.attr == FAT_ATTR_LONG_NAME)
	{
	  struct fat_long_name_entry *long_name
	    = (struct fat_long_name_entry *) &ctxt->dir;
	  uint8_t id = long_name->id;

	  if (id & 0x40)
	    {
	      id &= 0x3f;
	      slots = slot = id;
	      checksum = long_name->checksum;
	    }

	  if (id != slot || slot == 0 || checksum != long_name->checksum)
	    {
	      checksum = -1;
	      continue;
	    }

	  slot--;
	  memcpy (ctxt->unibuf + slot * 13, long_name->name1, 5 * 2);
	  memcpy (ctxt->unibuf + slot * 13 + 5, long_name->name2, 6 * 2);
	  memcpy (ctxt->unibuf + slot * 13 + 11, long_name->name3, 2 * 2);
	  continue;
	}

      /* Check if this entry is valid.  */
      if (ctxt->dir.name[0] == 0xe5 || (ctxt->dir.attr & ~FAT_ATTR_VALID))
	continue;

      /* This is a workaround for Japanese.  */
      if (ctxt->dir.name[0] == 0x05)
	ctxt->dir.name[0] = 0xe5;

      if (checksum != -1 && slot == 0)
	{
	  uint8_t sum;

	  for (sum = 0, i = 0; i < sizeof (ctxt->dir.name); i++)
	    sum = ((sum >> 1) | (sum << 7)) + ctxt->dir.name[i];

	  if (sum == checksum)
	    {
	      int u;

	      for (u = 0; u < slots * 13; u++)
		ctxt->unibuf[u] = le_to_cpu16 (ctxt->unibuf[u]);

	      *utf16_to_utf8 ((uint8_t *) ctxt->filename,
				   ctxt->unibuf,
				   slots * 13) = '\0';

	      return ERR_NONE;
	    }

	  checksum = -1;
	}

      /* Convert the 8.3 file name.  */
      filep = ctxt->filename;
      if (ctxt->dir.attr & FAT_ATTR_VOLUME_ID)
	{
	  for (i = 0; i < sizeof (ctxt->dir.name) && ctxt->dir.name[i]; i++)
	    *filep++ = ctxt->dir.name[i];
	  while (i > 0 && ctxt->dir.name[i - 1] == ' ')
	    {
	      filep--;
	      i--;
	    }
	}
      else
	{
	  for (i = 0; i < 8 && ctxt->dir.name[i]; i++)
	    *filep++ = tolower (ctxt->dir.name[i]);
	  while (i > 0 && ctxt->dir.name[i - 1] == ' ')
	    {
	      filep--;
	      i--;
	    }

	  *filep++ = '.';

	  for (i = 8; i < 11 && ctxt->dir.name[i]; i++)
	    *filep++ = tolower (ctxt->dir.name[i]);
	  while (i > 8 && ctxt->dir.name[i - 1] == ' ')
	    {
	      filep--;
	      i--;
	    }

	  if (i == 8)
	    filep--;
	}
      *filep = '\0';
      return ERR_NONE;
    }

  return errno ? : ERR_EOF;
}

#endif

/* Find the underlying directory or file in PATH and return the
   next path. If there is no next path or an error occurs, return NULL.
   If HOOK is specified, call it with each file name.  */
static char *
fat_find_dir (disk_t disk, struct fat_data *data,
		   const char *path, const char *origpath,
		   fs_dir_hook_t hook, void *hook_data)
{
  char *dirname, *dirp;
  int call_hook;
  int found = 0;
  struct fat_iterate_context ctxt;
  err_t err;

  if (! (data->attr & FAT_ATTR_DIRECTORY))
    {
      error (ERR_BAD_FILE_TYPE, N_("not a directory"));
      return 0;
    }

  /* Extract a directory name.  */
  while (*path == '/')
    path++;

  dirp = strchr (path, '/');
  if (dirp)
    {
      unsigned len = dirp - path;

      dirname = malloc (len + 1);
      if (! dirname)
	goto fail;

      memcpy (dirname, path, len);
      dirname[len] = '\0';
    }
  else
    /* This is actually a file.  */
    dirname = strdup (path);

  call_hook = (! dirp && hook);

  err = fat_iterate_init (&ctxt);
  if (err)
    {
      free (dirname);
      return 0;
    }

  while (!(err = fat_iterate_dir_next (disk, data, &ctxt)))
    {
      struct dirhook_info info;
      memset (&info, 0, sizeof (info));

      info.dir = !! (ctxt.dir.attr & FAT_ATTR_DIRECTORY);
      info.case_insensitive = 1;

#ifdef MODE_EXFAT
      if (!ctxt.dir.have_stream)
	continue;
#else
      if (ctxt.dir.attr & FAT_ATTR_VOLUME_ID)
	continue;
#endif
      if (*dirname == '\0' && call_hook)
	{
	  if (hook (ctxt.filename, &info, hook_data))
	    break;
	  else
	    continue;
	}

      if (strcasecmp (dirname, ctxt.filename) == 0)
	{
	  found = 1;
	  data->attr = ctxt.dir.attr;
#ifdef MODE_EXFAT
	  data->file_size = ctxt.dir.file_size;
	  data->file_cluster = ctxt.dir.first_cluster;
#else
	  data->file_size = le_to_cpu32 (ctxt.dir.file_size);
	  data->file_cluster = ((le_to_cpu16 (ctxt.dir.first_cluster_high) << 16)
				| le_to_cpu16 (ctxt.dir.first_cluster_low));
#endif
	  data->cur_cluster_num = ~0U;

	  if (call_hook)
	    hook (ctxt.filename, &info, hook_data);

	  break;
	}
    }

  fat_iterate_fini (&ctxt);
  if (err == ERR_EOF)
    err = 0;

  if (errno == ERR_NONE && ! found && !call_hook)
    error (ERR_FILE_NOT_FOUND, N_("file `%s' not found"), origpath);

 fail:
  free (dirname);

  return found ? dirp : 0;
}

static err_t
fat_dir (device_t device, const char *path, fs_dir_hook_t hook,
	      void *hook_data)
{
  struct fat_data *data = 0;
  disk_t disk = device->disk;
  size_t len;
  char *dirname = 0;
  char *p;

  dl_ref (my_mod);

  data = fat_mount (disk);
  if (! data)
    goto fail;

  /* Make sure that DIRNAME terminates with '/'.  */
  len = strlen (path);
  dirname = malloc (len + 1 + 1);
  if (! dirname)
    goto fail;
  memcpy (dirname, path, len);
  p = dirname + len;
  if (path[len - 1] != '/')
    *p++ = '/';
  *p = '\0';
  p = dirname;

  do
    {
      p = fat_find_dir (disk, data, p, path, hook, hook_data);
    }
  while (p && errno == ERR_NONE);

 fail:

  free (dirname);
  free (data);

  dl_unref (my_mod);

  return errno;
}

static err_t
fat_open (file_t file, const char *name)
{
  struct fat_data *data = 0;
  char *p = (char *) name;

  dl_ref (my_mod);

  data = fat_mount (file->device->disk);
  if (! data)
    goto fail;

  do
    {
      p = fat_find_dir (file->device->disk, data, p, name, 0, 0);
      if (errno != ERR_NONE)
	goto fail;
    }
  while (p);

  if (data->attr & FAT_ATTR_DIRECTORY)
    {
      error (ERR_BAD_FILE_TYPE, N_("not a regular file"));
      goto fail;
    }

  file->data = data;
  file->size = data->file_size;

  return ERR_NONE;

 fail:

  free (data);

  dl_unref (my_mod);

  return errno;
}

static ssize_t
fat_read (file_t file, char *buf, size_t len)
{
  return fat_read_data (file->device->disk, file->data,
			     file->read_hook, file->read_hook_data,
			     file->offset, len, buf);
}

static err_t
fat_close (file_t file)
{
  free (file->data);

  dl_unref (my_mod);

  return errno;
}

#ifdef MODE_EXFAT
static err_t
fat_label (device_t device, char **label)
{
  struct fat_dir_entry dir;
  ssize_t offset = -sizeof(dir);
  struct fat_data *data;
  disk_t disk = device->disk;

  data = fat_mount (disk);
  if (! data)
    return errno;

  *label = NULL;

  while (1)
    {
      offset += sizeof (dir);

      if (fat_read_data (disk, data, 0, 0,
			       offset, sizeof (dir), (char *) &dir)
	   != sizeof (dir))
	break;

      if (dir.entry_type == 0)
	break;
      if (!(dir.entry_type & 0x80))
	continue;

      /* Volume label. */
      if (dir.entry_type == 0x83)
	{
	  size_t chc;
	  uint16_t t[ARRAY_SIZE (dir.type_specific.volume_label.str)];
	  size_t i;
	  *label = malloc (ARRAY_SIZE (dir.type_specific.volume_label.str)
				* MAX_UTF8_PER_UTF16 + 1);
	  if (!*label)
	    {
	      free (data);
	      return errno;
	    }
	  chc = dir.type_specific.volume_label.character_count;
	  if (chc > ARRAY_SIZE (dir.type_specific.volume_label.str))
	    chc = ARRAY_SIZE (dir.type_specific.volume_label.str);
	  for (i = 0; i < chc; i++)
	    t[i] = le_to_cpu16 (dir.type_specific.volume_label.str[i]);
	  *utf16_to_utf8 ((uint8_t *) *label, t, chc) = '\0';
	}
    }

  free (data);
  return errno;
}

#else

static err_t
fat_label (device_t device, char **label)
{
  struct fat_data *data;
  disk_t disk = device->disk;
  err_t err;
  struct fat_iterate_context ctxt;

  *label = 0;

  dl_ref (my_mod);

  data = fat_mount (disk);
  if (! data)
    goto fail;

  if (! (data->attr & FAT_ATTR_DIRECTORY))
    {
      error (ERR_BAD_FILE_TYPE, N_("not a directory"));
      return 0;
    }

  err = fat_iterate_init (&ctxt);
  if (err)
    goto fail;

  while (!(err = fat_iterate_dir_next (disk, data, &ctxt)))
    if ((ctxt.dir.attr & ~FAT_ATTR_ARCHIVE) == FAT_ATTR_VOLUME_ID)
      {
	*label = strdup (ctxt.filename);
	break;
      }

  fat_iterate_fini (&ctxt);

 fail:

  dl_unref (my_mod);

  free (data);

  return errno;
}

#endif

static err_t
fat_uuid (device_t device, char **uuid)
{
  struct fat_data *data;
  disk_t disk = device->disk;

  dl_ref (my_mod);

  data = fat_mount (disk);
  if (data)
    {
      char *ptr;
      *uuid = xasprintf ("%04x-%04x",
			     (uint16_t) (data->uuid >> 16),
			     (uint16_t) data->uuid);
      for (ptr = *uuid; ptr && *ptr; ptr++)
	*ptr = toupper (*ptr);
    }
  else
    *uuid = NULL;

  dl_unref (my_mod);

  free (data);

  return errno;
}

static struct fs fat_fs =
  {
#ifdef MODE_EXFAT
    .name = "exfat",
#else
    .name = "fat",
#endif
    .dir = fat_dir,
    .open = fat_open,
    .read = fat_read,
    .close = fat_close,
    .label = fat_label,
    .uuid = fat_uuid,
#ifdef UTIL
#ifdef MODE_EXFAT
    /* ExFAT BPB is 30 larger than FAT32 one.  */
    .reserved_first_sector = 0,
#else
    .reserved_first_sector = 1,
#endif
    .blocklist_install = 1,
#endif
    .next = 0
  };

#ifdef MODE_EXFAT
MOD_INIT(exfat)
#else
MOD_INIT(fat)
#endif
{
  COMPILE_TIME_ASSERT (sizeof (struct fat_dir_entry) == 32);
  fs_register (&fat_fs);
  my_mod = mod;
}
#ifdef MODE_EXFAT
MOD_FINI(exfat)
#else
MOD_FINI(fat)
#endif
{
  fs_unregister (&fat_fs);
}
