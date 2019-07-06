#ifndef ZLIB_FORMAT_H
#define ZLIB_FORMAT_H

#include "zlib.h"
#include <stdlib.h> //malloc、free
#include <string.h> //memcpy

#ifdef PYXIE_SHARED
#ifdef _WIN32
#define PYXIE_EXPORT __declspec(dllexport)
#else
#define PYXIE_EXPORT
#endif
#else
#define PYXIE_EXPORT
#endif

namespace pyxie
{
	/**@brief ZLIB(deflate)を使用して、データの圧縮及び展開 */

	static void* (*custom_malloc)(size_t);
	static void(*custom_free)(void*);
	/**@brief zlibで使用されているカスタムmalloc関数(zlib.hを参照して下さい) */
	static void* zlibcustommalloc_wrapper_func(voidpf, uInt items, uInt size) { return custom_malloc(items*size); }
	/**@brief zlibで使用されているカスタムfree関数(zlib.hを参照して下さい) */
	static void zlibcustomfree_wrapper_func(voidpf, voidpf ptr) { custom_free(ptr); }

	class PYXIE_EXPORT zlibFormat
	{
	public:
		zlibFormat();
		zlibFormat(void* (*yourmalloc)(size_t), void(*yourfree)(void*));
		~zlibFormat();
		bool Compress(void* in, void** out, unsigned long inlen, unsigned long* outlen);
		bool Extract(void* in, void* out, unsigned long inlen, unsigned long outlen);
	private:
		alloc_func zlibcustommalloc_wrapper;
		free_func zlibcustomfree_wrapper;
		void unused()
		{
			zlibcustommalloc_wrapper_func(NULL, 0, 0);
			zlibcustomfree_wrapper_func(NULL, NULL);
		}
	};
}

#endif
