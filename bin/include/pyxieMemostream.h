#pragma once

#include <stdlib.h>
#include <string>
namespace pyxie
{

	class pyxieMemostream
	{
	public:
		pyxieMemostream(size_t size) {
			latest = current = buffer = (char*)malloc(size);
			GK = buffer + size;
		}
		~pyxieMemostream() {
			close();
		}
		void close() {
			if (buffer) {
				free(buffer);
				buffer = nullptr;
			}
		}
		size_t getSize() {
			return (size_t)(latest - buffer);
		}
		const char* getPtr() {
			return buffer;
		}
		void seekp(long pos) {
			current = buffer + pos;
			if (latest < current) latest = current;
		}
		void clear() {
			latest = current = buffer;
		}

		template<typename T>
		void write(const T& t)
		{
			size_t s = sizeof(T);
			write(reinterpret_cast<const char*>(&t), s);
		}

		void write(const char* p, size_t size)
		{
			while (current + size >= GK) {
				auto newsize = (size_t)(GK - buffer) + (1024 * 1024);
				buffer = (char*)realloc(buffer, newsize);
				GK = buffer + newsize;
			}
			memcpy(current, p, size);
			current += size;
			if (latest < current) latest = current;
		}

	private:
		char* buffer;
		char* current;
		char* latest;
		char* GK;
	};

	template<typename T>
	inline pyxieMemostream& operator << (pyxieMemostream& ostm, const T& val)
	{
		ostm.write(val);
		return ostm;
	}

	template<>
	inline pyxieMemostream& operator << (pyxieMemostream& ostm, const std::string& val)
	{
		ostm.write(val.c_str(), val.size() + 1);
		return ostm;
	}

	inline pyxieMemostream& operator << (pyxieMemostream& ostm, const char* val)
	{
		auto size = std::strlen(val);
		ostm.write(val, size + 1);
		return ostm;
	}
}
