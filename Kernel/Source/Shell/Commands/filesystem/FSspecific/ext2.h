#ifndef EXT2_GEN_H
#define EXT2_GEN_H


void ext2_mkdir(char* arguments);

void ext2_ls();

void ext2_cd(char* arguments);

void ext2_cp(char* dpath, char* spath, char* nname);

void ext2_del(char* path);

void ext2_mkfl(char *path);

void ext2_editfl(char* path, uint32_t* data, uint32_t* type, uint32_t off, uint32_t osz);

void ext2_rfl(char* path, uint32_t off, uint32_t sz);

void ext2_mv(char* path, char* spath);

#endif
