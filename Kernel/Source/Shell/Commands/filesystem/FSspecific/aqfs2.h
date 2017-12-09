#ifndef AQFS2_GEN_H
#define AQFS2_GEN_H

void aqfs2_cd(char* dir);

void aqfs2_ls();

void aqfs2_cp(char* dpath, char* spath, char* nname);

void aqfs2_del(char* path);

void aqfs2_mkdir(char* path);

void aqfs2_mkfl(char *path, char* dir_name);

void aqfs2_editfl(char* path, uint32_t* data, uint32_t* type, uint32_t off, uint32_t osz);

void* aqfs2_rfl(char* path, uint32_t off, uint32_t sz);

void aqfs2_mv(char* path, char* spath);

#endif
