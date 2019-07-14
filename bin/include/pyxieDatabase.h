///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieTypes.h"
#include "pyxieDictionary.h"
#include "pyxieResource.h"
#include "pyxieResourceManager.h"
#include "pyxieFios.h"
//#include "Thread.h"
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

namespace pyxie
{
	//Database file format
	//	DBHeader			//16 bytes
	//	DBContainer			//12 bytes (x numFiles)
	//	PatriciaPackedNode	//12 bytes (x numFiles)
	//	PatriciaCharTable	//scalable
	//	Data				//

	struct DBContainer
	{
		uint32_t	dataOffset;
		uint32_t	expandSize;
		uint32_t	compressSize;
	};

	struct DBHeader {
		uint32_t hashcode;
		uint32_t totalsize;
		uint32_t numFiles;
		uint32_t keyTableSize;

		uint32_t HeaderSize() {return sizeof(DBHeader) + sizeof(DBContainer)*numFiles + sizeof(PatriciaPackedNode)*numFiles + keyTableSize;}
		DBContainer* DBContainerTop() {return (DBContainer*)(((char*)this) + sizeof(DBHeader));}
		PatriciaPackedNode* PatriciaPackedNodeTop() {return (PatriciaPackedNode*)(((char*)this) + sizeof(DBHeader) + sizeof(DBContainer)*numFiles);}
		char* PatriciaCharTableTop() {return ((char*)this) + sizeof(DBHeader) + sizeof(DBContainer)*numFiles + sizeof(PatriciaPackedNode)*numFiles;}
	};

	class PYXIE_EXPORT pyxieDatabase : public pyxieResource
	{
		DBHeader* header;
		pyxieStaticDictionary* dict;
		DBContainer* container;
		char* keys;
		uint32_t fileTop;
	public:
		pyxieDatabase(const char* dbpath);
		pyxieDatabase(pyxieDatabase* org);
		~pyxieDatabase();
		void Build();
		void Initialize() {}
		void Clone(bool afterFinishBuild);
		RESOURCETYPE ResourceType() { return DATABASETYPE; }

		int FileIndex(const char* path);
		int CompressSize(int index);
		int ExpandSize(int index);

		ErrorCode ReadFile(const char* path, void*& outbuffer, int& outsize, uint32_t aligh = 1);
		ErrorCode ReadFile(int index, void* inbuff, void* outbuff);

		uint32_t NumFiles() { return header ? header->numFiles : 0; }
		const char* EnumFileName(uint32_t& offset);
	};
}