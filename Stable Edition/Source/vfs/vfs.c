#include <vfs.h>
#include <fs.h>

FILE volOpenFile (const char* fname) {

	if (fname) {

		//! default to device 'a'
		unsigned char device = 'a';

		//! filename
		char* filename = (char*) fname;

		//! in all cases, if fname[1]==':' then the first character must be device letter
		if (fname[1]==':') {

			device = fname[0];
			filename += 2; //strip it from pathname
		}

		//! call filesystem
		if (_FileSystems [device - 'a']) {

			//! set volume specific information and return file
			FILE file = _FileSystems[device - 'a']->Open (filename);
			file.deviceID = device;
			return file;
		}
	}

	FILE file;
	file.flags = FS_INVALID;
	return file;
}


/**
*	Reads file
*/
void volReadFile (PFILE file, unsigned char* Buffer, unsigned int Length) {

	if (file)
		if (_FileSystems [file->deviceID - 'a'])
			_FileSystems[file->deviceID - 'a']->Read (file,Buffer,Length);
}

/**
*	Close file
*/
void volCloseFile (PFILE file) {

	if (file)
		if (_FileSystems [file->deviceID - 'a'])
			_FileSystems[file->deviceID - 'a']->Close (file);
}


/**
*	Registers a filesystem
*/
void volRegisterFileSystem (PFILESYSTEM fsys, unsigned int deviceID) {

	static int i=0;

	if (i < DEVICE_MAX)
		if (fsys) {

			_FileSystems[ deviceID ] = fsys;
			i++;
		}
}

/**
*	Unregister file system
*/
void volUnregisterFileSystem (PFILESYSTEM fsys) {

	for (int i=0;i < DEVICE_MAX; i++)
		if (_FileSystems[i]==fsys)
			_FileSystems[i]=0;
}

/**
*	Unregister file system
*/
void volUnregisterFileSystemByID (unsigned int deviceID) {

	if (deviceID < DEVICE_MAX)
		_FileSystems [deviceID] = 0;
}
