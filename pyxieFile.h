///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#ifndef _pyxie_file_h_
#define _pyxie_file_h_

#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

	#ifndef PATHCHAR
	#ifdef _WIN32
	typedef wchar_t PATHCHAR;
	#else
	typedef char PATHCHAR;
	#endif
	#endif

	#ifndef DBDescripter
	#define DBDescripter 0x40000000
	#endif

	#ifndef _STAT
	#ifdef _WIN32
	#define _STAT _stat
	#else
	#define _STAT stat
	#endif
	#endif

	#define StatCpy(dst, src)\
	dst->st_atime = src.st_atime;\
	dst->st_dev = src.st_dev;\
	dst->st_ino = src.st_ino;\
	dst->st_mode = src.st_mode;\
	dst->st_nlink = src.st_nlink;\
	dst->st_uid = src.st_uid;\
	dst->st_gid = src.st_gid;\
	dst->st_rdev = src.st_rdev;\
	dst->st_size = src.st_size;\
	dst->st_atime = src.st_atime;\
	dst->st_mtime = src.st_mtime;\
	dst->st_ctime = src.st_ctime

	int pyxieOpenW(const PATHCHAR* path, int flags, uint16_t mod);
	int pyxieOpen(const char* path, int flags, uint16_t mod);
	int pyxieRead(int fd, void* buff, uint32_t count);
	int pyxieReadLine(int fd, void* buff, uint32_t count);
	size_t pyxieSeek(int fd, size_t ofs, int orign);
	int pyxieStat(int fd, struct stat* statbuf);
	int pyxieStatFromPathW(const PATHCHAR* path, struct stat* statbuf);
	int pyxieClose(int fd);
	PyObject * pyxieListDirW(const PATHCHAR* path, PyObject *list);

	int pyxieScanDirw(const PATHCHAR* path);
	int pyxieScanDirItrNext(int fd, PyObject **name, PyObject **path, struct stat* statbuf);
	int pyxieScanDirClose(int fd);

#ifdef __cplusplus
}
#endif


#endif
