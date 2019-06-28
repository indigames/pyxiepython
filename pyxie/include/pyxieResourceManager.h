///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieObject.h"
#include "pyxieDebug.h"
#include <map>
#include <vector>
#include <string>
#ifndef _WIN32
#include <sys/stat.h>
#endif

namespace pyxie
{
	class pyxieResource;
	class pyxieDatabase;
	class PYXIE_EXPORT pyxieResourceManager : public pyxieObject
	{
		std::vector<pyxieResource*> deleteList;
		std::map<uint32_t, pyxieResource*> dictionary;
		std::map<std::string, pyxieDatabase*> dbs;
		pyxieResourceManager();
	public:
		~pyxieResourceManager();
		static void New();
		static void Delete();
		static pyxieResourceManager& Instance();

		pyxieResource* GetResource(const char* path, RESOURCETYPE type, uint32_t instanceID = 1);
		bool AddResource(pyxieResource* res);
		void BuildResource(pyxieResource* res);
		bool DeleteUnused();
		bool DeleteDaemon();
		ErrorCode ReadFile(const char* inputPath, void*& outbuffer, int& outsize);
		pyxieDatabase* GetDB(const char* inputPath, int& indexOfEmbedFileTop);
	};
}
