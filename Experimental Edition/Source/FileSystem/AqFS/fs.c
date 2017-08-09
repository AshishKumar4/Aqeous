/*** AqFS File System, Aqeous OS's default FS ***/

#include "fs.h"
#include "fs_alloc.h"
#include "shell.h"
#include "string.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "ahci.h"
#include "stdio.h"

#include "NeuralNetwork/Neuron/neuron.h"

extern volatile int multitasking_ON;

uint64_t root_location;

void Setup_fs()
{
	uint32_t buf = (uint32_t)kmalloc(512);
	read(curr_port, root_location / 512, 1, (uint32_t)buf);
	Directory_t* root = (Directory_t*)(buf + (uint32_t)(root_location % 512));
	strcpy(root->name, "root");
	root->perm = 0;
	root->files = 0;
	root->folders = 0;
	root->location = root_location;
	root->Next_Friend = 0;
	root->last_child = 0;
	root->First_child = 0;
	root->First_file = 0;
	root->last_file = 0;
	root->magic = DIR_MAGIC;
	//Next_available+=sizeof(Directory_t);
	curr_dir.dir = root;
	curr_dir.type = 1;

	root_dir.dir = root;
	root_dir.type = 2;
	strcpy(curr_dir.full_name, "root");
	write(curr_port, root_location / 512, 1, (uint32_t)buf);
	////kfree((uint32_t*)(uint32_t*)buf);
}

void create_directory(char *name, uint16_t perm, char* destination)
{
	Directory_t* dest;
	if (destination)
	{
		dest = (Directory_t*)search_folderOGP(destination);
		if (!dest)
		{
			printf("\nDestination Folder %s Not found", name);
			return;
		}
	}
	else
	{
		dest = get_special_dirs(CURR_DIR);
	}
	uint64_t parent = dest->location;
	uint32_t buff = (uint32_t)kmalloc(512);
	uint64_t Next_available = sec_alloc(1);
	read(curr_port, Next_available / 512, 1, (uint32_t)buff);
	Directory_t* dir = (Directory_t*)((buff)+(uint32_t)(Next_available % 512));
	dir->files = 0;
	dir->location = Next_available;
	dir->folders = 0;
	strcpy(dir->name, name);
	dir->parent = parent;
	dir->perm = perm;
	dir->Next_Friend = 0;
	dir->last_child = 0;
	dir->magic = DIR_MAGIC;
	dir->First_child = 0;//*/
	dir->last_file = 0;
	dir->First_file = 0;
	uint32_t buff2 = (uint32_t)kmalloc(512);
	read(curr_port, parent / 512, 1, (uint32_t)buff2);
	Directory_t* Parent = (Directory_t*)(uint32_t)(buff2 + (uint32_t)(parent % 512));
	uint64_t tm;
	//printf("\n%s %x",dir->name,dir->location);
	if (Parent->last_child)
	{
		tm = Parent->last_child;
		dir->Prev_Friend = tm;
		Parent->last_child = dir->location;
		Parent->folders++;
		write(curr_port, parent / 512, 1, (uint32_t)buff2);
		uint32_t buf = (uint32_t)kmalloc(512);
		read(curr_port, tm / 512, 1, (uint32_t)buf);
		Directory_t* dir2 = (Directory_t*)(buf + (uint32_t)(tm % 512));
		dir2->Next_Friend = dir->location;
		write(curr_port, tm / 512, 1, (uint32_t)buf);
		////kfree((uint32_t*)(uint32_t*)buf);
	}
	else
	{
		Parent->First_child = dir->location;
		dir->Prev_Friend = 0;
		Parent->last_child = dir->location;
		Parent->folders++;
		write(curr_port, parent / 512, 1, (uint32_t)buff2);
	}

	write(curr_port, Next_available / 512, 1, (uint32_t)buff);

	printf("\nFolder \"%s\" Created Successfully\n", name);
	name = 0;
	destination = 0;
	////kfree((uint32_t*)(uint32_t*)dest);
	////kfree((uint32_t*)(uint32_t*)buff);
	////kfree((uint32_t*)(uint32_t*)buff2);
}

void create_file(char *name, uint16_t perm, char* destination)
{
	Directory_t* dest;
	if (destination)
	{
		dest = search_folderOGP(destination);
		if (!dest)
		{
			printf("\nDestination Folder %s Not found", name);
			return;
		}
	}
	else
	{
		dest = get_special_dirs(CURR_DIR);
	}
	uint64_t parent = dest->location;
	uint32_t buff = (uint32_t)kmalloc(512);
	memset((void*)buff, 0, 512);
	uint64_t Next_available = sec_alloc(1);
	read(curr_port, Next_available / 512, 1, (uint32_t)buff);
	File_t* file = (File_t*)(buff + (uint32_t)(Next_available % 512));
	file->location = Next_available;

	file->perm = perm;
	strcpy(file->name, name);
	file->parent = parent;
	file->sz = 0;
	file->Next_Friend = 0;
	file->first_header = 0;
	file->last_header = 0;
	file->headers = 0;
	file->magic = FIL_MAGIC;

	uint32_t buff2 = (uint32_t)kmalloc(512);
	read(curr_port, parent / 512, 1, (uint32_t)buff2);
	Directory_t* Parent = (Directory_t*)(uint32_t)(buff2 + (uint32_t)(parent % 512));
	uint64_t tm;
	//printf("\n%s %x",dir->name,dir->location);
	if (Parent->last_file)
	{
		tm = Parent->last_file;
		file->Prev_Friend = tm;
		Parent->last_file = file->location;
		Parent->files++;
		write(curr_port, parent / 512, 1, (uint32_t)buff2);
		uint32_t buf = (uint32_t)kmalloc(512);
		read(curr_port, tm / 512, 1, (uint32_t)buf);
		File_t* file2 = (File_t*)(buf + (uint32_t)(tm % 512));
		file2->Next_Friend = file->location;
		write(curr_port, tm / 512, 1, (uint32_t)buf);
		////kfree((uint32_t*)(uint32_t*)buf);
	}
	else
	{
		Parent->First_file = file->location;
		Parent->last_file = file->location;
		file->Prev_Friend = 0;
		Parent->files++;
		write(curr_port, parent / 512, 1, (uint32_t)buff2);
	}

	write(curr_port, file->location / 512, 1, (uint32_t)buff);
	printf("\nFile \"%s\" Created Successfully\n", name);
	name = 0;
	destination = 0;
	////kfree((uint32_t*)(uint32_t*)dest);
	////kfree((uint32_t*)(uint32_t*)buff);
	////kfree((uint32_t*)(uint32_t*)buff2);
}

void find_friendDirs(char* name)
{
	Directory_t* dest;
	if (name)
	{
		dest = search_folderOGP(name);
		if (!dest)
		{
			printf("\nSearched Folder %s Not found", name);
			return;
		}
	}
	else
	{
		dest = get_special_dirs(CURR_DIR);
	}
	name = 0;
	uint64_t parent = dest->location;
	uint32_t buff = (uint32_t)kmalloc(512);
	read(curr_port, parent / 512, 1, (uint32_t)buff);
	Directory_t* Parent = (Directory_t*)(buff);
	uint64_t tmp = Parent->First_child;
	printf("\nShowing %x folders of %s\\", Parent->folders, Parent->name);
	uint32_t buf = (uint32_t)kmalloc(512);
	Directory_t* temp;
	for (uint32_t i = 0; i<Parent->folders; i++)
	{
		read(curr_port, tmp / 512, 1, (uint32_t)buf);
		temp = (Directory_t*)(buf);
		printf("\n\t /%s", temp->name);
		tmp = temp->Next_Friend;
	}
	////kfree((uint32_t*)(uint32_t*)buf);
	printf("\n");
}

void find_childFiles(char* name)
{
	Directory_t* dest;
	if (name)
	{
		dest = search_folderOGP(name);
		if (!dest)
		{
			printf("\nSearched Folder %s Not found", name);
			return;
		}
	}
	else
	{
		dest = get_special_dirs(CURR_DIR);
	}

	uint64_t parent = dest->location;
	uint32_t buff = (uint32_t)kmalloc(512);
	read(curr_port, parent / 512, 1, (uint32_t)buff);
	Directory_t* Parent = (Directory_t*)(buff);

	uint32_t buf = (uint32_t)kmalloc(512);
	read(curr_port, Parent->First_file / 512, 1, buf);
	File_t* file = (File_t*)(buf);

	printf("\nShowing %x files of %s\\", Parent->files, Parent->name);
	for (uint32_t i = 0; i<Parent->files; i++)
	{
		printf("\n\t\t /%s", file->name);
		read(curr_port, file->Next_Friend / 512, 1, buf);
		file = (File_t*)(buf);
	}
	printf("\n");
}

Directory_t* search_folderOGP(char* path) //Search Folder ON GIVEN PATH
{
	if (!path)
	{
		return get_special_dirs(CURR_DIR);
	}
	uint32_t buf = (uint32_t)kmalloc(512);
	uint32_t tbuf = (uint32_t)kmalloc(512);

	char* tmpath = (char*)kmalloc(strlen(path));
	memset(tmpath, 0, strlen(path));
	strcpy(tmpath, path);

	char* tmpd = strtok(tmpath, "/");

	if (!tmpd) tmpd = path;
	Directory_t* td = (Directory_t*)buf;
	Directory_t* temp = (Directory_t*)tbuf;

	Directory_t* pdir;
	if (path[0] != '/')  pdir = get_special_dirs(CURR_DIR);
	else pdir = get_special_dirs(ROOT);

	read(curr_port, pdir->location / 512, 1, (uint32_t)buf);
	for (; tmpd != NULL;)
	{
		if (!td->First_child) return 0;
		read(curr_port, td->First_child / 512, 1, tbuf);
		for (; strcmp(temp->name, tmpd) != 0;)
		{
			if (!temp->Next_Friend) return 0;
			read(curr_port, temp->Next_Friend / 512, 1, tbuf);
		}
		//printf(">>%s",tmpd);
		memcpy(td, temp, sizeof(Directory_t));
		tmpd = strtok(NULL, "/");
	}
	//printf("\n-->%s",td->name);
	return td;
}

Directory_t* search_folderOGP_T(char* path) //Search Folder ON GIVEN PATH; temp
{
	if (!path)
	{
		return get_special_dirs(CURR_DIR);
	}
	uint32_t buf = (uint32_t)kmalloc(512);
	uint32_t tbuf = (uint32_t)kmalloc(512);

	char* tmpd = strtok(path, "/");
	if (!tmpd) tmpd = path;

	Directory_t* td = (Directory_t*)buf;
	Directory_t* temp = (Directory_t*)tbuf;

	Directory_t* pdir;
	if (path[0] != '/')  pdir = get_special_dirs(CURR_DIR);
	else pdir = get_special_dirs(ROOT);

	read(curr_port, pdir->location / 512, 1, (uint32_t)buf);
	for (; tmpd != NULL;)
	{
		if (!td->First_child) return 0;
		read(curr_port, td->First_child / 512, 1, tbuf);
		for (; strcmp(temp->name, tmpd) != 0;)
		{
			if (!temp->Next_Friend) return 0;
			read(curr_port, temp->Next_Friend / 512, 1, tbuf);
		}
		memcpy(td, temp, sizeof(Directory_t));
		tmpd = strtok(NULL, "/");
	}
	return td;
}

Directory_t* search_folder(char* name)
{
	uint32_t buf = (uint32_t)kmalloc(512);
	read(curr_port, curr_dir.dir->location / 512, 1, (uint32_t)buf);
	Directory_t* dir = (Directory_t*)(buf + (uint32_t)(curr_dir.dir->location % 512));
	uint64_t cdir = dir->First_child;
	if (!cdir)
	{
		//printf("\nFolder not found1\n");
		return 0;
	}
	read(curr_port, cdir / 512, 1, (uint32_t)buf);
	Directory_t* temp = (Directory_t*)(buf + (uint32_t)(cdir % 512));
	for (int i = 0;; i++)
	{
		if (!strcmp(temp->name, name))
			break;
		cdir = temp->Next_Friend;
		if (!cdir)
		{
			//printf("\nFolder not found\n");
			return 0;
		}
		read(curr_port, cdir / 512, 1, (uint32_t)buf);
		temp = (Directory_t*)(buf + (uint32_t)(cdir % 512));
	}
	return temp;
}

File_handle_t* file_loaderOGP(char* path)
{
	uint32_t buf = (uint32_t)kmalloc(512);

	char* tmpath = (char*)kmalloc(strlen(path) + 1);
	strcpy(tmpath, path);

	char* tmpstr = tmpath;
	char* fname;
	int i = 0;
	for (i = strlen(tmpath) - 1; tmpstr[i] != '/' && i >= 0; i--);

	fname = &tmpstr[i + 1];
	if (i>0)
		tmpath[i] = '\0';
	else tmpath = 0;

	char* full_path = (char*)kmalloc(strlen(path) + 1);
	strcpy(full_path, path);
	full_path[strlen(path)] = '\0';

	Directory_t* dir = (Directory_t*)search_folderOGP_T(tmpath);
	if (!dir) return 0;

	uint64_t cdir = dir->First_file;
	if (!cdir)
	{
		//printf("\nThe folder is empty!!!\n");
		return 0;
	}
	read(curr_port, cdir / 512, 1, (uint32_t)buf);
	File_t* temp = (File_t*)(buf);
	for (int i = 0;; i++)
	{
		if (!strcmp(temp->name, fname))
			break;
		cdir = temp->Next_Friend;
		if (!cdir)
		{
			//printf("\nFile not found\n");
			return 0;
		}
		read(curr_port, cdir / 512, 1, (uint32_t)buf);
		temp = (File_t*)(buf);
	}
	printf("\n>%s", temp->name);
	File_handle_t* handle = (File_handle_t*)kmalloc(sizeof(File_handle_t));
	handle->file = (uint32_t)temp;
	handle->name = temp->name;
	handle->full_path = full_path;

	////kfree((uint32_t*)(uint32_t*)buf);
	return handle;
}

File_handle_t* file_loader(char* name)
{
	uint32_t buf = (uint32_t)kmalloc(512);
	//printf("\nfile location 1 %x",buf);
	read(curr_port, curr_dir.dir->location / 512, 1, (uint32_t)buf);
	Directory_t* dir = (Directory_t*)(buf + (uint32_t)(curr_dir.dir->location % 512));
	uint64_t cdir = dir->First_file;
	if (!cdir)
	{
		//printf("\nThe folder is empty!!!\n");
		return 0;
	}
	read(curr_port, cdir / 512, 1, (uint32_t)buf);
	File_t* temp = (File_t*)(buf + (uint32_t)(cdir % 512));
	for (int i = 0;; i++)
	{
		if (!strcmp(temp->name, name))
			break;
		cdir = temp->Next_Friend;
		if (!cdir)
		{
			//printf("\nFile not found\n");
			return 0;
		}
		read(curr_port, cdir / 512, 1, (uint32_t)buf);
		temp = (File_t*)(buf + (uint32_t)(cdir % 512));
	}
	File_handle_t* handle = (File_handle_t*)kmalloc(sizeof(File_handle_t));
	File_t* tb = (File_t*)kmalloc(sizeof(File_t));
	memcpy((void*)tb, (void*)temp, sizeof(File_t));
	handle->file = (uint32_t)tb;
	handle->name = tb->name;
	handle->full_path = tb->name;
	////kfree((uint32_t*)(uint32_t*)buf);
	return handle;
}

int file_loadOGP(char *path)
{
	File_handle_t* handle = file_loaderOGP(path);
	if (!handle)
	{
		//printf("\nCant load the file requested\n");
		return 0;
	}
	if (!start_handle)
	{
		start_handle = handle;
	}
	else
		current->next = handle;
	current = handle;
	current->next = 0;

	return 1;
	//printf("\nFile %s Loaded \n",name);
}

int file_load(char *name)
{
	File_handle_t* handle = file_loader(name);
	if (!handle)
	{
		//printf("\nCant load the file requested\n");
		return 0;
	}
	if (!start_handle)
	{
		start_handle = handle;
	}
	else
		current->next = handle;
	current = handle;
	current->next = 0;

	return 1;
	//printf("\nFile %s Loaded \n",name);
}

void file_closeOGP(char *path)
{
	char* tmpath = (char*)kmalloc(strlen(path));
	strcpy(tmpath, path);

	char* tmpstr = tmpath;
	//		char* fname;
	int i = 0;
	for (i = strlen(tmpath) - 1; tmpstr[i] != '/' && i >= 0; i--);

	//		fname = &tmpstr[i+1];
	if (i>0)
		tmpath[i] = '\0';
	else tmpath = 0;

	File_handle_t* temp = start_handle, *temp2 = 0;
	for (int i = 0; temp; i++)
	{
		if (!strcmp(temp->full_path, path))
		{
			File_handle_t* handle = temp;

			uint32_t buff = (uint32_t)kmalloc(512);
			File_t* file = (File_t*)handle->file;
			read(curr_port, file->location / 512, 1, (uint32_t)buff);
			File_t* ftmp = (File_t*)(buff);
			memcpy((void*)ftmp, (void*)file, sizeof(File_t));
			write(curr_port, file->location / 512, 1, (uint32_t)buff);

			if (!i)
				start_handle = temp->next;
			else if (temp2) temp2->next = temp->next;
			if (temp == current)
			{
				current = temp2;
			}
			////kfree((uint32_t*)(uint32_t*)temp);
			//goto out;
			return;
		}
		//if(!temp->next) break;
		temp2 = temp;
		temp = temp->next;
	}
	//return;
	//out:

	return;
}

void file_close(char *name)
{
	file_flush(name);
	File_handle_t* temp = start_handle, *temp2 = 0;
	for (int i = 0; temp; i++)
	{
		if (!strcmp(temp->name, name))
		{
			if (!i)
				start_handle = temp->next;
			else if (temp2) temp2->next = temp->next;
			if (temp == current)
			{
				current = temp2;
			}
			////kfree((uint32_t*)(uint32_t*)temp);
			//goto out;
			return;
		}
		//if(!temp->next) break;
		temp2 = temp;
		temp = temp->next;
	}
	//return;
	//out:

	return;
}

File_handle_t* file_searchOGP(char* path)
{
	File_handle_t* temp = start_handle;
	for (int i = 0; temp; i++)
	{
		//printf("\nname: \n%s\n%s\n", temp->full_path, path);
		if (!strcmp(temp->full_path, path))
		{
			return temp;
			//goto out;
		}

		//if(!temp->next) break;
		temp = temp->next;
	}
	//printf("\nFile %s not loaded yet!\n",name);
	return 0;
	//out:
	return temp;
}

File_handle_t* file_search(char* name)
{
	File_handle_t* temp = start_handle;
	for (int i = 0; temp; i++)
	{
		if (!strcmp(temp->name, name))
		{
			return temp;
			//goto out;
		}
		//if(!temp->next) break;
		temp = temp->next;
	}
	//printf("\nFile %s not loaded yet!\n",name);
	return 0;
	//out:
	return temp;
}

void set_curr_dir(uint64_t location, char* path)
{
	uint32_t buf = (uint32_t)kmalloc(4096);
	read(curr_port, location / 512, 1, (uint32_t)buf);
	Directory_t* dir = (Directory_t*)(buf);
	curr_dir.dir = dir;
	char* name = (char*)(buf + 512);
	strcpy(name, curr_dir.full_name);
	if (!strcmp(path, ".."))
	{
		int i;
		for (i = strlen(name); name[i] != '/'; --i);
		name[i] = '\0';
	}
	else
	{
		if (path[0] != '/')
			strcat(name, "/");
		strcat(name, path);
	}
	strcpy(curr_dir.full_name, name);
	printf("\ncurr dir: %s", name);
}

Directory_t* get_special_dirs(uint32_t type)
{
	Directory_t* tmp;
	//	uint32_t buf;
	if (type == CURR_DIR)
	{
		tmp = curr_dir.dir;
		read(curr_port, tmp->location / 512, 1, (uint32_t)tmp);
		return tmp;

	}
	else if (type == ROOT)
	{
		tmp = root_dir.dir;
		read(curr_port, tmp->location / 512, 1, (uint32_t)tmp);
		return tmp;
	}
	return curr_dir.dir;
}

Directory_t* get_dir(uint64_t location)
{
	uint32_t buf = (uint32_t)kmalloc(512);
	if (!location) return 0;
	read(curr_port, location / 512, 1, buf);
	if (((Directory_t*)buf)->magic == DIR_MAGIC)
		return (Directory_t*)buf;
	return 0;
}

File_t* get_file(uint64_t location)
{
	uint32_t buf = (uint32_t)kmalloc(512);
	if (!location) return 0;
	read(curr_port, location / 512, 1, buf);
	if (((File_t*)buf)->magic == FIL_MAGIC)
		return (File_t*)buf;
	return 0;
}

int flush_dir(Directory_t* dir)
{
	if (dir && dir->magic == DIR_MAGIC)
		write(curr_port, dir->location / 512, 1, (uint32_t)dir);
	//else return 0;
	return 1;
}

int flush_file(File_t* file)
{
	if (file && file->magic == FIL_MAGIC)
		write(curr_port, file->location / 512, 1, (uint32_t)file);
	//else return 0;
	return 1;
}

int delete_dir(Directory_t* dir)
{
	Directory_t* pdir = get_dir(dir->Prev_Friend);
	printf("\n%x", dir->Next_Friend);
	Directory_t* ndir = get_dir(dir->Next_Friend);
	printf("\n%x", ndir);

	if (pdir) pdir->Next_Friend = dir->Next_Friend;
	if (ndir) ndir->Prev_Friend = dir->Prev_Friend;
	//Delete all its files!

	Directory_t* parent = get_dir(dir->parent);
	--parent->folders;

	if (dir->location == parent->last_child)
		parent->last_child = dir->Prev_Friend;

	flush_dir(ndir);
	flush_dir(pdir);
	flush_dir(parent);

	write(curr_port, dir->location / 512, 1, clear_buf512);

	return 1;
}

int delete_file(File_t* file)
{
	File_t* pfile = get_file(file->Prev_Friend);
	File_t* nfile = get_file(file->Next_Friend);

	if (pfile) pfile->Next_Friend = file->Next_Friend;
	if (nfile) nfile->Prev_Friend = file->Prev_Friend;

	Directory_t* parent = get_dir(file->parent);
	--parent->files;

	if (file->location == parent->last_file)
		parent->last_file = file->Prev_Friend;

	file_close(file->name);

	flush_file(nfile);
	flush_file(pfile);
	flush_dir(parent);

	return 1;
}

File_Header_t* nx_header(File_Header_t* prev_header)
{
	if (!prev_header || !prev_header->Next_Header) return 0;
	File_Header_t* tmp = (File_Header_t*)kmalloc(512);
	read(curr_port, (prev_header->Next_Header / 512), 1, (uint32_t)tmp);
	if (tmp->magic == FHR_MAGIC)
		return tmp;
	//kfree((uint32_t*)tmp);
	return 0;
}

File_Header_t* get_header(uint64_t location)
{
	if (!location) return 0;
	File_Header_t* tmp = (File_Header_t*)kmalloc(512);
	read(curr_port, location / 512, 1, (uint32_t)tmp);
	if (tmp->magic == FHR_MAGIC)
		return tmp;
	//kfree((uint32_t*)tmp);
	return 0;
}

int del_header(File_Header_t* header)
{
	File_Header_t* pheader = get_header(header->Previous_Header);
	File_Header_t* nheader = get_header(header->Next_Header);

	pheader->Next_Header = nheader->location;
	nheader->Previous_Header = pheader->location;
	flush_header(nheader);
	flush_header(pheader);
	//kfree((uint32_t*)nheader);
	//kfree((uint32_t*)pheader);
	return 0;
}

File_Header_t* File_Header_Creator(File_t* file, uint16_t blocks) //Creates a header at the last of the file.
{
	//printf("\nAdding more available data space for the file %s \n",curr_file.name);
	fsbuf = (uint32_t)kmalloc(512);
	File_Header_t* header = (File_Header_t*)fsbuf;
	header->File_location = file->location;
	header->Next_Header = 0;
	header->used = 0;
	header->location = sec_alloc(blocks + 2);
	header->spread = blocks * 512;
	header->magic = FHR_MAGIC;
	header->Previous_Header = file->last_header;
	if (file->last_header)
	{
		uint32_t buf = (uint32_t)kmalloc(512);
		read(curr_port, file->last_header / 512, 1, (uint32_t)buf);
		File_Header_t* header2 = (File_Header_t*)buf;
		header2->Next_Header = header->location;
		write(curr_port, file->last_header / 512, 1, (uint32_t)buf);
		//kfree((uint32_t*)buf);
	}
	else
	{
		file->first_header = header->location;
	}
	write(curr_port, header->location / 512, 1, (uint32_t)fsbuf);

	file->last_header = header->location;
	file->headers++;
	file->sz += 512;

	return header;
}

File_Header_t* File_Header_Creator_sdw(File_t* file, File_Header_t* left_header, uint16_t blocks) //Sandwiched Header creator (in between two existing headers)
{
	//printf("\nAdding more available data space for the file %s \n",curr_file.name);
	fsbuf = (uint32_t)kmalloc(512);
	File_Header_t* header = (File_Header_t*)fsbuf;
	header->File_location = file->location;
	header->Next_Header = left_header->Next_Header;
	header->used = 0;
	header->location = sec_alloc(blocks + 2); //Extra for extra assurance
	header->spread = blocks * 512;
	header->magic = FHR_MAGIC;
	header->Previous_Header = left_header->location;
	uint32_t buf = (uint32_t)kmalloc(512);
	left_header->Next_Header = header->location;
	write(curr_port, left_header->location / 512, 1, (uint32_t)buf);

	header->Previous_Header = left_header->location;

	write(curr_port, header->location / 512, 1, (uint32_t)fsbuf);

	if (!file->first_header)
	{
		file->first_header = header->location;
	}
	file->last_header = header->location;
	file->headers++;
	file->sz += 512;

	return header;
}

void file_flushOGP(char* path)
{
	File_handle_t* handle = file_searchOGP(path);
	if (!handle) return;
	uint32_t buff = (uint32_t)kmalloc(512);
	File_t* file = (File_t*)handle->file;
	read(curr_port, file->location / 512, 1, (uint32_t)buff);
	File_t* ftmp = (File_t*)(buff + (uint32_t)(file->location % 512));
	memcpy((void*)ftmp, (void*)file, sizeof(File_t));
	write(curr_port, file->location / 512, 1, (uint32_t)buff);
	////kfree((uint32_t*)(uint32_t*)buff);
}

void file_flush(char* name)
{
	File_handle_t* handle = file_search(name);
	if (!handle) return;
	uint32_t buff = (uint32_t)kmalloc(512);
	File_t* file = (File_t*)handle->file;
	read(curr_port, file->location / 512, 1, (uint32_t)buff);
	File_t* ftmp = (File_t*)(buff + (uint32_t)(file->location % 512));
	memcpy((void*)ftmp, (void*)file, sizeof(File_t));
	write(curr_port, file->location / 512, 1, (uint32_t)buff);
	////kfree((uint32_t*)(uint32_t*)buff);
}

void file_truncate(File_handle_t* handle)
{
	File_t* file = (File_t*)handle->file;
	uint32_t buf = (uint32_t)kmalloc(512);
	File_Header_t* header = (File_Header_t*)buf;
	uint32_t buff = (uint32_t)kmalloc(512);
	memset((void*)buff, 0, 1024);
	uint64_t temp = file->first_header;
	for (uint32_t i = 0; i<file->headers; i++)
	{
		read(curr_port, temp / 512, 1, (uint32_t)buf);
		write(curr_port, temp / 512, 1, (uint32_t)buff);
		temp = header->Next_Header;
	}
	file->sz = 0;
	file->first_header = 0;
	file->headers = 0;
}

File_Header_t* file_header_search(uint32_t foffset, File_t* file) //Finds the header of a file which contains the memory regions of the offset
{
	uint32_t tm = (uint32_t)kmalloc(512);
	memset(tm, 0, 512);
	//printf("\n-->%d", file->first_header);
	read(curr_port, (file->first_header / 512), 1, tm);
	File_Header_t* tmp = (File_Header_t*)(tm);
	if(tmp->location != file->first_header)
	{
		printf("\nError file header location MisMatch!");
		tmp->location = file->first_header;
		//return 2;
	}

	uint32_t ts = 0;
	uint64_t tmp2 = 0;
	for (int i = file->headers; i > 0; --i)
	{
		ts += tmp->used;
		//printf("\ntmp->used : %d, spread: %d, location: %d, loc: %d", tmp->used, tmp->spread, tmp->location, file->first_header);
		if (ts > foffset)
		{
			tmp->reserved = ts - foffset; //Offset into header
			return tmp;
		}
		if (!(i - 1)) break;
		tmp2 = tmp->Next_Header;
		read(curr_port, tmp2 / 512, 1, (uint32_t)tm);
		tmp = (File_Header_t*)(tm + (uint32_t)(file->first_header % 512));
		if(tmp->location != tmp2)
		{
			printf("\nError file header location MisMatch!");
			return 3;
		}
	}
	return 0;
}

inline void flush_header(File_Header_t* header)
{
	write(curr_port, header->location / 512, 1, (uint32_t)header);
}

int file_readTM(uint32_t* buffer, uint32_t offset, uint32_t size, char* path) //Read file content and //write to memory.
{
	File_handle_t* handle = file_searchOGP(path);
	if (!handle) return 1; //File not loaded yet.
	char* name = handle->name;

	File_t* file_st = (File_t*)handle->file;
	File_Header_t* header = file_header_search(offset, file_st); //find which header has the offset memory.

	if (!size) size = file_st->sz - (512 * file_st->headers);

	if (!header) return 2; //Some error!!!!

	/*printf("\n{size: %d offset: %d}", size, offset);
  Shell_Dbuff_sync();*/

	if (header->magic != FHR_MAGIC)
	{
		//kfree((uint32_t*)header);
		//printf("\nHeader Wrong!");
		return 3; //Not a valid Header.
	}
	//printf("\nheader->size: %x header->previous %x header->Next %x", header->used, header->Previous_Header, header->Next_Header);
  //Shell_Dbuff_sync();

	uint32_t a1 = MIN(header->reserved, size);
	uint32_t b1 = header->used - header->reserved; //Local offset
	//printf("\na1: %d, b1: %d, header->reserved: %d", a1, b1, header->reserved);
	uint32_t tbuff = (uint32_t)kmalloc(ROUNDUP(a1, 1024));
	memset(tbuff, 0, ROUNDUP(a1, 1024));
	read(curr_port, (1 + ((header->location + b1) / 512)), (ROUNDUP(a1, 1024) / 512) + 1, tbuff); //Read the first part of buffer.

	//printf("\na1: %d ROUNDUP(a1,1024)/512: %d, ROUNDUP(a1,1024): %d %d\n", a1, ROUNDUP(a1,1024)/512, ROUNDUP(a1,1024), size);

	uint32_t cpdone = 0;

	uint32_t bb = (uint32_t)buffer;
	memcpy((void*)bb, (void*)(tbuff + (b1 % 512)), a1); //Get to the local offset and copy the data which has to be copied.

										//kfree((uint32_t*)tbuff);
	cpdone += a1;

	uint32_t left = size - cpdone;

	//printf("\nLeft: %d: %d; size: %d, cpdone: %d", left, buffer[(a1/4)-1], size, cpdone);
	  return 1;


	while (left)   //Keep extracting data from file until we extract all the required data successfully.
	{
		if (!header->Next_Header) return 4; //The Size requested from offset is more then the size of the file/Invalid header.
		read(curr_port, header->Next_Header / 512, 1, (uint32_t)header);
		if (header->magic != FHR_MAGIC) return 5;

		if (left < header->used) //Only 1 header left to be //read.
		{
			tbuff = (uint32_t)kmalloc(ROUNDUP(left, 512));
			read(curr_port, 1 + (header->location / 512), 1 + (left / 512), tbuff);

			memcpy((void*)(bb + cpdone), (void*)tbuff, left);

			//kfree((uint32_t*)tbuff);
			//kfree((uint32_t*)header);
			return 0; //Everything went fine.
		}
		tbuff = (uint32_t)kmalloc(ROUNDUP(header->used, 512));
		read(curr_port, 1 + (header->location / 512), 1 + (header->used / 512), tbuff);

		memcpy((void*)(bb + cpdone), (void*)tbuff, header->used);

		cpdone += header->used;
		//kfree((uint32_t*)tbuff);

		left -= header->used;
	}

	//kfree((uint32_t*)header);
	return 0; //Everything went fine.
}

uint32_t file_size(char* path)
{
	File_handle_t* handle = file_searchOGP(path);
	if (!handle) return 0; //File not loaded yet.

	File_t* file_st = (File_t*)handle->file;

	//  printf("\nfile size = %x headers = %x\nfile_st->magic %x %x\nfile parent: %x",file_st->sz - (512*file_st->headers),file_st->headers,file_st->magic, FIL_MAGIC, file_st->parent);

	return file_st->sz - (512 * file_st->headers);
}

int file_writeAppend(uint32_t* buffer, uint32_t size, char* path) //Write to a file from memory.
{
	File_handle_t* handle = file_searchOGP(path);
	if (!handle) return 4; //File not loaded yet.
	char* name = handle->name;

	uint32_t bb = (uint32_t)buffer;
	File_t* file_st = (File_t*)handle->file;
	File_Header_t* header;
	file_st->sz += size;
	//printf("\nfile size = %d headers = %d\nfile_st->magic %d %d\nfile parent: %d",file_st->sz - (512*file_st->headers),file_st->headers,file_st->magic, FIL_MAGIC, file_st->parent);


	uint32_t tbuff;

	if (file_st->last_header)
	{
		uint32_t buf = (uint32_t)kmalloc(512);
		read(curr_port, file_st->last_header / 512, 1, buf);
		header = (File_Header_t*)(buf);
		//printf("\nheader->spread: %x %x %x\n", header->spread, header->used, file_st->headers);

		if (header->magic != FHR_MAGIC) return 3;
		int t = header->spread - header->used;
		if (t>0) //If there is some space in the last header, fill it.
		{
			tbuff = (uint32_t)kmalloc(header->spread);
			//memset(tbuff,0,header->spread);

			read(curr_port, 1 + (header->location / 512), 1 + (header->used / 512), tbuff);
			memcpy((void*)(tbuff + header->used), (void*)bb, MIN((int)size, t));
			write(curr_port, 1 + (header->location / 512), header->spread / 512, tbuff);
			header->used += MIN((int)size, t);
			bb += MIN((int)size, t);
			size -= MIN((int)size, t);

			flush_header(header);
			//kfree((uint32_t*)tbuff);
			//kfree((uint32_t*)header);
			if (!size) return 0;
		}
	}
	if (size)
	{
		uint32_t blks = ROUNDUP(size, 512) / 512;

		header = File_Header_Creator(file_st, blks);
		if (!header) return 1;
		/*
		tbuff = (uint32_t)kmalloc(blks*512);
		memcpy(tbuff, bb, size);
		*/

		write(curr_port, 1 + (header->location / 512), blks, bb);
		if (size % 512)
		{
			tbuff = (uint32_t)kmalloc(512);
			memset((void*)tbuff, 0, 512);
			memcpy((void*)tbuff, (void*)(bb + (size - (size % 512))), size % 512);
		//	printf("\n\t\tABCD\n");
			write(curr_port, 1 + (header->location / 512) + blks, 1, tbuff);
		}
		header->used = size;
		flush_header(header);
		//printf("blks: %d header->location: %d \n", blks, (uint32_t)header->location);
	}
	//kfree((uint32_t*)tbuff);
	//kfree((uint32_t*)header);

	return 0;
}

int file_editFM(uint32_t offset, uint32_t osize, uint32_t *buffer, uint32_t fsize, char* path)
{
	File_handle_t* handle = file_searchOGP(path);
	if (!handle) return 0; //File not loaded yet.
	char* name = handle->name;

	File_t* file_st = (File_t*)handle->file;
	File_Header_t* header = file_header_search(offset, file_st); //find which header has the offset memory.

	if (!header) return -1; //Some error!!!!

	if (header->magic != FHR_MAGIC)
	{
		//kfree((uint32_t*)header);
		return -2; //Not a valid Header.
	}

	uint32_t left_end = header->used - header->reserved;

	uint32_t sz = header->spread - left_end;
	uint32_t bb = (uint32_t)buffer;

	uint32_t tl = header->spread - left_end;
	uint32_t cpdone = 0;

	//Several cases are possible->
	// [********------], [******------    ], [********-----****   ], [*****------][-----******]

	if (left_end + osize <= header->used)  // [********------], [******------    ], [********-----****], [*******----***   ]
	{
		//Local header offset [********----*****_____]
		//                            ^    ^   ^
		//                            1    2    3
		//  1 = left_end, 2 = a1, 3 - 2 = b1.

		uint32_t a1 = left_end + osize;
		uint32_t b1 = header->used - a1;



		uint32_t buff = (uint32_t)kmalloc(ROUNDUP(fsize + b1, 1024));
		read(curr_port, 1 + ((header->location + left_end) / 512), 1, buff);

		memcpy((void*)(buff + (left_end % 512)), (void*)bb, fsize);

		if (b1)
		{
			uint32_t tbuff = (uint32_t)kmalloc(ROUNDUP(b1, 1024));
			read(curr_port, 1 + ((header->location + a1) / 512), 2 + (b1 / 512), tbuff);

			memcpy((void*)(buff + (left_end % 512) + fsize), (void*)(tbuff + (a1 % 512)), b1);
			//kfree((uint32_t*)tbuff);
		}

		fsize += b1;

		write(curr_port, 1 + ((header->location + left_end) / 512), ROUNDUP(tl, 512) / 512, buff);
		cpdone += tl;

		if (fsize > tl)
		{
			uint32_t tdone = fsize - tl;
			File_Header_t* nxHeader = File_Header_Creator_sdw(file_st, header, (tdone / 512) + 1);
			nxHeader->used = tdone;
			write(curr_port, 1 + (nxHeader->location / 512), 1 + (tdone / 512), buff + cpdone);
			flush_header(nxHeader);
			//kfree((uint32_t*)nxHeader);
		}

		flush_header(header);
		//kfree((uint32_t*)buff);
		//kfree((uint32_t*)header);
		return 1;
	}
	else
	{
		// cases:        [*****------][-----******], [*******-----][-------][-----*****], [*******-----][-------][**********],
		//               [*******-----__][-------__][-----*****], [*****----][-----][-----**    ]


		//    [******-------][--------][------*****____]
		//           ^                       ^    ^
		//           1                       2    3
		// 1 = left_end, 2 = a1, 3 - 2 = b1, 2-1 = osize

		uint32_t cp = header->used - left_end;

		File_Header_t* theader = nx_header(header);
		File_Header_t* tmp = nx_header(theader);

		while (1)
		{
			if (!theader) return -1;

			cp += theader->used;

			if (cp >= osize) break;

			del_header(theader);
			theader = tmp;
			tmp = nx_header(tmp);
		}

		uint32_t b1 = cp - osize;
		uint32_t a1 = header->used - b1;

		uint32_t bb = (uint32_t)kmalloc(ROUNDUP(fsize + b1, 1024));

		read(curr_port, 1 + (header->location / 512), 1, bb);
		memcpy((void*)(bb + (left_end % 512)), buffer, fsize);

		if (b1)
		{
			uint32_t tbuff = (uint32_t)kmalloc(ROUNDUP(b1, 1024));
			read(curr_port, 1 + ((theader->location + a1) / 512), 2 + (b1 / 512), tbuff);
			memcpy((void*)(bb + (left_end % 512) + fsize), (void*)(tbuff + (a1 % 512)), b1);

			//kfree((uint32_t*)tbuff);
			fsize += b1;
		}
		del_header(theader);


		write(curr_port, 1 + ((header->location + left_end) / 512), ROUNDUP(tl, 512) / 512, bb);
		cpdone += tl;

		if (fsize > tl)
		{
			uint32_t tdone = fsize - tl;
			File_Header_t* nxHeader = File_Header_Creator_sdw(file_st, header, (tdone / 512) + 1);
			nxHeader->used = tdone;
			write(curr_port, 1 + (nxHeader->location / 512), 1 + (tdone / 512), bb + cpdone);
			flush_header(nxHeader);
			//kfree((uint32_t*)nxHeader);
		}

		flush_header(header);
		//kfree((uint32_t*)bb);
		//kfree((uint32_t*)header);
		return 1;
	}

}

void make_boot_sector()
{
	uint32_t buf = (uint32_t)kmalloc(1024);
	memset((void*)buf, 0, 1024);
	read(curr_port, 0, 2, (uint32_t)buf);
	Identity_Sectors_t* identity = (Identity_Sectors_t*)(buf + 436);
	strcpy(identity->name, "AqFS472");
	identity->active_partition = 446; //Partition 1

	uint8_t* boot_ptr = (uint8_t*)buf;
	boot_ptr += 510;
	*boot_ptr = 0x55;
	++boot_ptr;
	*boot_ptr = 0xAA;
	write(curr_port, 0, 2, (uint32_t)buf); //2nd sector.
	memset((void*)buf, 0, 1024);
	uint32_t* tmp = (uint32_t*)buf;
	*tmp = root_location;
	write(curr_port, start_off - 2, 2, (uint32_t)buf);
}

int AqFS_init()
{
	SATA_ident_t* info=(SATA_ident_t*)curr_disk->info;
	sectors=info->lba_capacity;
	start_off=10240;
	bytes=sectors;
	off=(bytes/512)/512;
	off += 16;
	++off;
	bytemap_off = off+start_off;
	bytemap_end = start_off + (bytes/512);
	uint32_t buf=fsalloc(1024);

	read(curr_port, 0,2,(DWORD)buf);

	Identity_Sectors_t* identity=(Identity_Sectors_t*)(buf + 436);
	if(strncmp(identity->name, "AqFS472",7))
	{
		printf("Filesystem Not supported/Disk not partitioned Correctly\n");
		return 1;
	}
	read(curr_port, start_off - 2,2,(DWORD)buf);

	uint32_t* tmp = (uint32_t*)buf;

	root_location = (*tmp);
	//Partition_struct_t* act_part = (Partition_struct_t*)(identity->active_partition + buf);

	//memcpy((void*)&root_location, (void*)&act_part->relative_sector, 6);

	printf("\n %s %x",identity->name,root_location);

	if(!root_location)  return 2;
	//while(1);
	set_curr_dir(root_location, "root");
	find_friendDirs(0);
	find_childFiles(0);
	aqfs_root = curr_dir.dir;
	return 0;
}

void AqFS_burn()
{
	curr_ahci=ahci_start;
	curr_disk=&curr_ahci->Disk[1];
	curr_port=&abar->ports[1];
	SATA_ident_t* info=(SATA_ident_t*)curr_disk->info;
	sectors=info->lba_capacity;
	printf("\nTotal Sectors: %x\n",sectors);
	start_off=10240;
	bytes=sectors;
	off=(bytes/512)/512;
	off += 16;
	++off;
	bytemap_off = off+start_off;
	bytemap_end = start_off + (bytes/512);
	start_handle=0;
	printf("\nFormating and Partitioning the Disk, May take a few minutes...\n");
	fs_alloc_init();
	printf("\nFile System formated successfully");
	root_location=sec_alloc(1);
	make_boot_sector();

	printf("\nMaster Partition made successfully");
	Setup_fs();
	printf("\nLoaded the root directory");
	create_directory("Aqeous",1,0);
	create_directory("Programs",1,0);
	//return;
	create_directory("user",1,0);
	create_directory("System",1,0);
	create_directory("System",1,"Aqeous");
	printf("\n");
	create_file("test.txt",1,0);

	find_friendDirs(0);
	find_childFiles(0);
	printf("\nHello");
}
