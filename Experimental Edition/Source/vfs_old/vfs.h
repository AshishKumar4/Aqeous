#ifndef VFS_H
#define VFS_H

#define FS_FILE       0
#define FS_DIRECTORY  1
#define FS_INVALID    2

typedef struct _FILE
{

	char        name[32];
	uint32_t    flags;
	uint32_t    fileLength;
	uint32_t    id;
	uint32_t    eof;
	uint32_t    position;
	uint32_t    currentCluster;
	uint32_t    deviceID;

}FILE, *PFILE;

typedef struct _FILE_SYSTEM
{

	char Name [8];
	FILE               (*Directory)  (const char* DirectoryName);
	void	           (*Mount)      ();
	void               (*Read)       (PFILE file, unsigned char* Buffer, unsigned int Length);
	void	           (*Close)      (PFILE);
	FILE               (*Open)       (const char* FileName);

}FILESYSTEM, *PFILESYSTEM;


extern FILE volOpenFile (const char* fname);
extern void volReadFile (PFILE file, unsigned char* Buffer, unsigned int Length);
extern void volCloseFile (PFILE file);
extern void volRegisterFileSystem (PFILESYSTEM, unsigned int deviceID);
extern void volUnregisterFileSystem (PFILESYSTEM);
extern void volUnregisterFileSystemByID (unsigned int deviceID);

#define DEVICE_MAX 26

//! File system list
PFILESYSTEM _FileSystems[DEVICE_MAX];


#endif // VFS_H
