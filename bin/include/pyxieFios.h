///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieObject.h"
#include <string>
#include <vector>

namespace pyxie
{
	class PYXIE_EXPORT pyxieFios : public pyxieObject
	{
	public:
		void SetRoot(const char* path=nullptr);
		const char* GetRoot();
		int RootLength();

		FILE* Open(const char* filename, bool forWrite=false, bool create = true);
		void Close(FILE* file);
		bool Seek(FILE* file, uint32_t offset, int from); //SEEK_CUR SEEK_END SEEK_SET
		uint32_t Read(FILE* file, void* buffer, uint32_t size);
		uint32_t Write(FILE* file, const void* buffer, uint32_t size);

		int Size(const char* filename);
		bool Read(const char* filename, void* buffer, uint32_t size, uint32_t offset=0);
		bool Write(const char* filename, const void* buffer, uint32_t size, bool create = true);
		void SearchFiles(const char* topPath, const char* serachFile, std::vector<std::string> &directories);
		bool MakeDir(const char *path);
		void GetFullPath(const char* path, char* outbuff);

		static pyxieFios& Instance();
	private:
		pyxieFios() {}
	};

	class FileCloser
	{
		FILE*	m_F;
	public:
		FileCloser(FILE* F) : m_F(F) {}
		~FileCloser()
		{
			if (m_F) pyxieFios::Instance().Close(m_F);
		}
	};


}
