#ifndef LOADER_h
#define LOADER_h 1

typedef struct binHeader
{
  char bin_name[32];
  char prog_name[64];
  uint32_t f_size;        // File Size
  uint32_t prog_size;     // Binary Program size
  uint32_t main_start;    // _start function location, Relative
  uint32_t relative_first; // First byte address relative
}BinHeader_t;

#endif
