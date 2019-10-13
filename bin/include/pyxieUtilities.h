///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include <stdio.h>
#include <assert.h>
#ifdef PYXIE_SHARED
#ifdef _WIN32
#define PYXIE_EXPORT __declspec(dllexport)
#else
#define PYXIE_EXPORT
#endif
#else
#define PYXIE_EXPORT
#endif

namespace pyxie {

	uint32_t PYXIE_EXPORT GenerateNameHash(const char* name);

	const char* GetPlatformName();

	inline bool IsPowerOf2(int x)
	{
		if (x == 0)return false;
		return (x & (x - 1)) ? false : true;
	}

	inline int next_power_of_two(int n) {
		if (n <= 1) return n;
		--n;
		n |= n >> 1;
		n |= n >> 2;
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;
		++n;
		return n;
	}

	inline float DEGREES_TO_RADIANS(float x) {
		return x * (PI / 180.f);
	}

	inline float RADIANS_TO_DEGREES(float x) {
		return x * (180.f / PI);
	}

	template<typename T>
	inline T Min(T x, T y) {
		return (x < y) ? x : y;
	}

	template<typename T>
	inline T Max(T x, T y) {
		return (x > y) ? x : y;
	}

	template<typename T>
	T LimitRange(T a, T l, T h) {
		return Min(Max(l, a), h);
	}

//	template<typename T1, typename T2>
//	T1 ALIGN(T1 value, T2 alignment) {
//		return (((uintptr_t)(value)) + (((uintptr_t)(alignment)) - 1U))& ~(((uintptr_t)(alignment)) - 1U);
//	}
#ifndef ALIGN
#define ALIGN(value, alignment)                                        \
	(                                                                       \
		(((uintptr_t) (value)) + (((uintptr_t) (alignment)) - 1U))          \
		& ~(((uintptr_t) (alignment)) - 1U)                                 \
	)
#endif


#ifdef _WIN32

#define pyxie_strcpy strcpy_s
#define pyxie_strncpy(d,s,t) strncpy_s(d,t,s,_TRUNCATE)
#define pyxie_snprintf sprintf_s
#define pyxie_strncat(d,s,t) strncat_s(d,t,s,_TRUNCATE)
#else
#define pyxie_strcpy strcpy
#define pyxie_strncpy strncpy
#define pyxie_snprintf snprintf
#define pyxie_strncat strncat
#endif

	template<typename T>
	inline void	Swap(T& v1, T& v2)
	{
		T tmp = v1;
		v1 = v2;
		v2 = tmp;
	}

	template<typename T>
	inline void	SetFlag(T& value, const T flag)
	{
		value |= flag;
	}

	template<typename T>
	inline void	ClearFlag(T& value, const T flag)
	{
		value &= (~flag);
	}

	template<typename T>
	inline bool	CheckFlag(const T value, const T flag)
	{
		return ((value & flag) != 0);
	}


	/////////////////////////////WIN32
#if defined _WIN32
	inline void* PYXIE_MALLOC_ALIGNED(size_t a, size_t s) {
		return _aligned_malloc(s, a);
	}
	inline void PYXIE_FREE_ALIGNED(void* a) {
		return _aligned_free(a);
	}
	/////////////////////////////IOS
#elif defined __APPLE_CC__

	inline void* PYXIE_MALLOC_ALIGNED(size_t a, size_t s)
	{
		if (a < sizeof(void*)) a = sizeof(void*);
		else a = next_power_of_two(a);
		void* p;
		posix_memalign(&p, a, s);
		assert(p != NULL);
		return p;
	}
	inline void PYXIE_FREE_ALIGNED(void* a) {
		return free(a);
	}

	/////////////////////////////Android
#elif defined __ANDROID__
	inline void* PYXIE_MALLOC_ALIGNED(size_t a, size_t s) {
		return memalign(a, s);
	}
	inline void PYXIE_FREE_ALIGNED(void* a) {
		return free(a);
	}
#endif

	inline void* PYXIE_MALLOC(size_t s) {
		return malloc(s);
	}

	inline void PYXIE_FREE(void* p) {
		return free(p);
	}

#define	PYXIE_SAFE_FREE(p) { if(p){pyxie::PYXIE_FREE(p); (p) = 0;} }

	inline void* PYXIE_REALLOC(void* p, size_t s) {
		return realloc(p, s);
	}


	enum PartToGetFromPath {
		NameToGetFromPath,
		ExtentionToGetFromPath,
		NameWithExtentionToGetFromPath,
		DirectoryToGetFromPath
	};
	void PYXIE_EXPORT GetPartOfFilePath(const char* path, PartToGetFromPath part, char* outfilename);
	void PYXIE_EXPORT ConvertLongTextureName(char* name);
	void PYXIE_EXPORT ReplaceSep(char* path);


	class MemoryCleaner {
		void* ptr;
	public:
		MemoryCleaner(void* _ptr) {
			ptr = _ptr;
		}
		~MemoryCleaner() {
			PYXIE_FREE(ptr);
		}
	};
}

