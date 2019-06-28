///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#include "pyxieFile.h"

#include "pyxie.h"
#include "pyxieResourceManager.h"
#include "pyxieFios.h"
#include "pyxieDatabase.h"
#include <set>
#include <string>
#include <vector>
#include <mutex>
#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

	struct DBFileDescripter {
		pyxie::pyxieDatabase* db;
		int index;
		int pos;
	};
	std::mutex DBFileDescripter_mtx;
	static const int maxDBFileDescripter = 16;
	DBFileDescripter DBFileDescripterTable[maxDBFileDescripter];


	struct FileType{
		std::string file;
		bool dir;
		size_t size;
	};
	struct DBScanDirDescripter {
		std::vector<FileType>* fileList;
		int pos;
	};
	static const int maxDBScanDirDescripter = 16;
	DBScanDirDescripter DBScanDirDescripterTable[maxDBScanDirDescripter];
	std::mutex DBScanDir_mtx;


	bool ready = false;
	void Initialize() {
		for (int i = 0; i < maxDBFileDescripter; i++) {
			DBFileDescripterTable[i].db = nullptr;
			DBFileDescripterTable[i].index = 0;
			DBFileDescripterTable[i].pos = 0;
		}
		for (int i = 0; i < maxDBScanDirDescripter; i++) {
			DBScanDirDescripterTable[i].fileList = nullptr;
			DBScanDirDescripterTable[i].pos = 0;
		}
		ready = true;
	}

	void CorrectPath(const char* path, char* correctPath) {
		pyxie_strncpy(correctPath, path, MAX_PATH);
		for (char* cp = correctPath; *cp; cp++)
			if (*cp == '\\') *cp = '/';
		const char* root = pyxie::pyxieFios::Instance().GetRoot();
		if (strstr(correctPath, root) == correctPath) {
			auto rootl = pyxie::pyxieFios::Instance().RootLength();
			auto l = strlen(correctPath);
            if(correctPath[l-1]=='/') l--;
			pyxie_strncpy(correctPath, correctPath + rootl, l - rootl + 1);
		}
	}

	int pyxieOpenW(const PATHCHAR* path, int flags, uint16_t mod) {
		if (!ready) Initialize();
		char charPath[MAX_PATH];
#ifdef _WIN32
		size_t num;
		wcstombs_s(&num, charPath, MAX_PATH, path, MAX_PATH);
#else
		pyxie_strncpy(charPath, path, MAX_PATH);
#endif
		return pyxieOpen(charPath, flags, mod);
	}


	int pyxieOpen(const char* path, int flags, uint16_t mod) {
		if (!ready) Initialize();

		char charPath[MAX_PATH];
		CorrectPath(path, charPath);


		int index;
		auto db = pyxie::pyxieResourceManager::Instance().GetDB(charPath, index);
		if (db) {
			auto fileIndex = db->FileIndex(charPath + index);
			if (db && (fileIndex != -1)) {
				std::unique_lock<std::mutex> uniq_lk(DBFileDescripter_mtx);
				for (int i = 0; i < maxDBFileDescripter; i++) {
					if (DBFileDescripterTable[i].db == nullptr) {
						DBFileDescripterTable[i].db = db;
						DBFileDescripterTable[i].index = fileIndex;
						DBFileDescripterTable[i].pos = 0;
						return i + DBDescripter;
					}
				}
				pyxie_errstr(errOpenFileOver);
			}
		}
		return -1;
	}

	int pyxieRead(int fd, void* buff, uint32_t count) {

		if (fd >= DBDescripter) {
			DBFileDescripter& dbd = DBFileDescripterTable[fd - DBDescripter];
			int readsize = 0;
			auto outsize = dbd.db->ExpandSize(dbd.index);
			auto insize = dbd.db->CompressSize(dbd.index);
			if (outsize == 0 || insize == 0) return 0;
			if (dbd.pos >= outsize) return 0;
			void* inbuff = pyxie::PYXIE_MALLOC(insize);
			if (!inbuff) return 0;
			if (dbd.pos == 0 && count >= (uint32_t)outsize) {
				auto err = dbd.db->ReadFile(dbd.index, inbuff, buff);
				if (err == errNoError) {
					dbd.pos += outsize;
					readsize = outsize;
				}
			}
			else {
				void* outbuff = pyxie::PYXIE_MALLOC(outsize);
				if (!outbuff) {
					pyxie::PYXIE_FREE(inbuff);
					return 0;
				}
				auto err = dbd.db->ReadFile(dbd.index, inbuff, outbuff);
				if (err == errNoError) {
					int cpysize = count;
					if (dbd.pos + (int)count > outsize) cpysize -= outsize - dbd.pos;
					memcpy(buff, ((char*)outbuff) + dbd.pos, cpysize);
					dbd.pos += cpysize;
					readsize = cpysize;
				}
				pyxie::PYXIE_FREE(outbuff);
			}
			pyxie::PYXIE_FREE(inbuff);
			return readsize;
		}
		return 0;
	}

	size_t pyxieSeek(int fd, size_t ofs, int orign) {
		if (fd >= DBDescripter) {
			DBFileDescripter& dbd = DBFileDescripterTable[fd - DBDescripter];
			switch (orign) {
			case SEEK_SET:
				dbd.pos = (int)ofs;
				break;
			case SEEK_CUR:
				dbd.pos += (int)ofs;
				break;
			case SEEK_END:
				dbd.pos -= dbd.db->ExpandSize(dbd.index) - (int)ofs;
			}
			return 0;
		}
		return -1;
	}

	int pyxieStat(int fd, struct stat* statbuf) {
		if (fd >= DBDescripter) {
			statbuf->st_dev = 0;
			statbuf->st_ino = 0;
			statbuf->st_mode = S_IFREG;
			statbuf->st_nlink = 0;
			statbuf->st_uid = 0;
			statbuf->st_gid = 0;
			statbuf->st_rdev = 0;
			statbuf->st_size = DBFileDescripterTable[fd - DBDescripter].db->ExpandSize(DBFileDescripterTable[fd - DBDescripter].index);
			statbuf->st_atime = 0;
			statbuf->st_mtime = 0;
			statbuf->st_ctime = 0;
			return 0;
		}
		return -1;
	}

	int pyxieStatFromPathW(const PATHCHAR* path, struct stat* statbuf) {
		if (!ready) Initialize();
		char charPath[MAX_PATH];
#ifdef _WIN32
		size_t num;
		wcstombs_s(&num, charPath, MAX_PATH, path, MAX_PATH);
#else
		pyxie_strncpy(charPath, path, MAX_PATH);
#endif
		char correctPath[MAX_PATH];
		CorrectPath(charPath, correctPath);

		int index;
		auto db = pyxie::pyxieResourceManager::Instance().GetDB(correctPath, index);
		if(db){
			memset(statbuf, 0, sizeof(struct stat));
			if (strlen(correctPath) == index) {
				statbuf->st_mode = S_IFDIR | S_IREAD;
				return 0;
			}
			auto fileIndex = db->FileIndex(correctPath + index);
			if (fileIndex != -1){
				statbuf->st_mode = S_IFREG| S_IREAD;
				statbuf->st_size = db->ExpandSize(index);
				return 0;
			}
			else{
				statbuf->st_mode = S_IFDIR | S_IREAD;
				return 0;
			}
		}
		return -1;
	}


	int pyxieClose(int fd) {
		if (fd >= DBDescripter) {
			std::unique_lock<std::mutex> uniq_lk(DBFileDescripter_mtx);
			DBFileDescripterTable[fd- DBDescripter].db = nullptr;
			DBFileDescripterTable[fd- DBDescripter].index = 0;
			return 0;
		}
		return -1;
	}





	int pyxieScanDirw(const PATHCHAR* path) {
		if (!ready) Initialize();

		int fd = -1;
		{
			std::unique_lock<std::mutex> uniq_lk(DBScanDir_mtx);
			for (int i = 1; i < maxDBScanDirDescripter; i++) {
				if (DBScanDirDescripterTable[i].fileList == nullptr) {
					DBScanDirDescripterTable[i].fileList = new std::vector<FileType>;
					DBScanDirDescripterTable[i].pos = 0;
					fd = i;
					break;
				}
			}
		}
		if (fd == -1) return fd;

		char charPath[MAX_PATH];
#ifdef _WIN32
		size_t num;
		wcstombs_s(&num, charPath, MAX_PATH, path, MAX_PATH);
#else
		pyxie_strncpy(charPath, path, MAX_PATH);
#endif
		char correctPath[MAX_PATH];
		CorrectPath(charPath, correctPath);

        int index;
		auto db = pyxie::pyxieResourceManager::Instance().GetDB(correctPath, index);

        std::set<std::string> dirs;

		uint32_t offset = 1;
		while (true) {
			auto name = db->EnumFileName(offset);
			if (!name) break;

			char dir[MAX_PATH];
			pyxie::GetPartOfFilePath(name, pyxie::DirectoryToGetFromPath, dir);
			if (strcmp(dir, correctPath + index) == 0) {
                int dirLen = (int)strlen(dir);
                if(dirLen!=0) dirLen++;
                FileType ft;
				ft.file = name + dirLen;
				ft.dir = false;
				auto idx = db->FileIndex(name);
				ft.size = db->ExpandSize(idx);
				DBScanDirDescripterTable[fd].fileList->push_back(ft);
			}
			else if (strstr(dir, correctPath + index) == dir) {
				char* endp = strchr(dir, '/');
				if (endp) *endp = 0;
				dirs.insert(std::string(dir));
			}
		}
		for (auto it = dirs.begin(); it != dirs.end(); it++) {
			FileType ft;
			ft.file = *it;
			ft.dir = true;
			ft.size = 0;
			DBScanDirDescripterTable[fd].fileList->push_back(ft);
		}
		return fd;
	}

	int pyxieScanDirClose(int fd) {
		if (fd < 16) {
			std::unique_lock<std::mutex> uniq_lk(DBScanDir_mtx);
			delete(DBScanDirDescripterTable[fd].fileList);
			DBScanDirDescripterTable[fd].fileList = nullptr;
			return 0;
		}
		return -1;
	}


	PyObject* makeStrObject(const char* str) {
#ifdef _WIN32
		size_t num;
        wchar_t wname[MAX_PATH];
		mbstowcs_s(&num, wname, MAX_PATH, str, strlen(str));
		return PyUnicode_FromWideChar(wname, wcslen(wname));
#else
        return PyUnicode_DecodeFSDefaultAndSize(str, strlen(str));
        //return PyBytes_FromStringAndSize(str, strlen(str));
#endif
    }

	int pyxieScanDirItrNext(int fd, PyObject **name, PyObject **path, struct stat* statbuf) {
		if (fd < 16) {
			DBScanDirDescripter& desc = DBScanDirDescripterTable[fd];
			if (desc.pos >= desc.fileList->size()) return -1;
            FileType& ft = desc.fileList->at(desc.pos);

            if(name){
                *name = makeStrObject(ft.file.c_str());
                if (!name) return -1;
            }

            if(path){
                std::string strPath = pyxie::pyxieFios::Instance().GetRoot();
                strPath += ft.file;
                *path = makeStrObject(strPath.c_str());
                if (!path) return -1;
            }

            if(statbuf){
                memset(statbuf, 0, sizeof(struct stat));
                if (ft.dir) {
                    statbuf->st_mode = S_IFDIR | S_IREAD;
                }
                else {
                    statbuf->st_mode = S_IFREG | S_IREAD;
                    statbuf->st_size = ft.size;
                }
            }
            desc.pos++;
			return 0;
		}
		return -1;
	}

	PyObject * pyxieListDirW(const PATHCHAR* path, PyObject *list) {
        auto fd = pyxieScanDirw(path);
        if(fd == -1) return NULL;
        if ((list = PyList_New(0)) == NULL) return NULL;

        while(true){
            PyObject* name;
            if(pyxieScanDirItrNext(fd, &name, nullptr, nullptr)==-1) break;
            if (PyList_Append(list, name) != 0) {
                Py_DECREF(name);
                Py_DECREF(list);
                list = NULL;
                break;
            }
            Py_DECREF(name);
        }
        pyxieScanDirClose(fd);
		return list;
	}


#ifdef __cplusplus
}
#endif
