#ifndef AQFS_GEN_H
#define AQFS_GEN_H

void aqfs_cd(char* dir);

void aqfs_ls();

void aqfs_cp(char* dpath, char* spath, char* nname);

void aqfs_del(char* path);

void aqfs_mkdir(char* path);

void aqfs_mkfl(char *path);

void aqfs_editfl(char* path, uint32_t* data, uint32_t* type, uint32_t off, uint32_t osz);

void* aqfs_rfl(char* path, uint32_t off, uint32_t sz);

void aqfs_mv(char* path, char* spath);

#endif
