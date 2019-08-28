///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

//#define _FINAL

#include <stdint.h>
#include <assert.h>

/////////////////////////////WIN32
#if defined _WIN32
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif
#include <windows.h>
#define __ENABLE_SUSPEND_RECOVER__ 1

/////////////////////////////IOS
#elif defined __APPLE_CC__
#include <stdlib.h>

/////////////////////////////Android
#elif defined __ANDROID__
#include <stdlib.h>
#define __ENABLE_SUSPEND_RECOVER__ 1
#endif

#if defined _WIN32
#	define DeviceHandle HDC
#else
#	define DeviceHandle void*
#endif

constexpr double  MATH_E = 2.7182818284590452354;			/**< e            */
constexpr double  MATH_LOG2E = 1.4426950408889634074;		/**< log2(e)      */
constexpr double  MATH_LOG10E = 0.43429448190325182765;		/**< log10(e)     */
constexpr double  MATH_LN2 = 0.69314718055994530942;		/**< ln(2)        */
constexpr double  MATH_LN10 = 2.30258509299404568402;		/**< ln(10)       */
constexpr double  MATH_PI = 3.14159265358979323846;			/**< pi           */
constexpr double  MATH_TWOPI = (MATH_PI * 2.0);				/**< 2 * pi       */
constexpr double  MATH_PI_2 = 1.57079632679489661923;		/**< pi / 2       */
constexpr double  MATH_PI_4 = 0.78539816339744830962;		/**< pi / 4       */
constexpr double  MATH_3PI_4 = 2.3561944901923448370E0;		/**< 3 * pi / 2   */
constexpr double  MATH_SQRTPI = 1.77245385090551602792981;	/**< sqrt(pi)     */
constexpr double  MATH_1_PI = 0.31830988618379067154;		/**< 1 / pi       */
constexpr double  MATH_2_PI = 0.63661977236758134308;		/**< 2 / pi       */
constexpr double  MATH_2_SQRTPI = 1.12837916709551257390;	/**< 2 / sqrt(pi) */
constexpr double  MATH_SQRT2 = 1.41421356237309504880;		/**< sqrt(2)      */
constexpr double  MATH_SQRT1_2 = 0.70710678118654752440;	/**< 1 / sqrt(2)  */
constexpr double  MATH_SQRT3 = 1.73205080756887729353;		/**< sqrt(3)      */
constexpr double  MATH_IVLN10 = 0.43429448190325182765;		/**< 1 / log(10)  */
constexpr double  MATH_INVLN2 = 1.4426950408889633870E0;	/**< 1 / log(2)   */
constexpr double  MATH_LOG10TWO = 0.30102999566398119521;	/**< log10(2)     */
constexpr double  MATH_LOG2TEN = 3.32192809488736234787;	/**< log2(10)     */
constexpr double  MATH_SQRT3_2 = 0.86602540378443864676;	/**< sqrt(3) / 2  */

constexpr float	FLOAT_MAX = 3.402823466e+38F;
constexpr float	FLOAT_MIN = 1.175494351e-38F;

#ifndef PI
constexpr float	PI = 3.14159f;
#endif

#ifndef MAX_PATH
#define MAX_PATH          260
#endif

enum Axis
{
	AxisX,
	AxisY,
	AxisZ
};

enum TargetPlatform {
	PC_Platform =0,
	IOS_Platform =1,
	ANDROID_Platform =2
};

#if defined _WIN32              //WIN32
#define CURRENT_PLATFORM PC_Platform
#elif defined __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE            //iOS
#define CURRENT_PLATFORM IOS_Platform
#else                           //OSX
#define CURRENT_PLATFORM PC_Platform
#endif
#elif defined __ANDROID__       //Android
#define CURRENT_PLATFORM ANDROID_Platform
#endif



typedef uint32_t RESOURCETYPE;
constexpr RESOURCETYPE UNKNOWNTYPE =		((RESOURCETYPE)('_xyp'));
constexpr RESOURCETYPE SKELETONTYPE =		((RESOURCETYPE)('bxyp'));
constexpr RESOURCETYPE TEXTURETYPE =		((RESOURCETYPE)('ixyp'));
constexpr RESOURCETYPE SHADERTYPE =			((RESOURCETYPE)('sxyp'));
constexpr RESOURCETYPE FIGURETYPE =			((RESOURCETYPE)('fxyp'));
constexpr RESOURCETYPE ANIMATORTYPE =		((RESOURCETYPE)('axyp'));
constexpr RESOURCETYPE CAMERATYPE =			((RESOURCETYPE)('cxyp'));
constexpr RESOURCETYPE RENDERTARGETTYPE =	((RESOURCETYPE)('rxyp'));
constexpr RESOURCETYPE ENVIRONMENTSETTYPE =	((RESOURCETYPE)('exyp'));
constexpr RESOURCETYPE DATABASETYPE =		((RESOURCETYPE)('dxyp'));
constexpr RESOURCETYPE SHOWCASETYPE =		((RESOURCETYPE)('hxyp'));

enum ShaderParameterDataType
{
	ParamTypeSampler = 0,
	ParamTypeFloat = 1,
	ParamTypeFloat2 = 2,
	ParamTypeFloat3 = 3,
	ParamTypeFloat4 = 4,
	ParamTypeMatrix = 16,
	ParamTypeInt = 50,
	ParamTypeBool = 51,
	ParamTypeString = 52,
	ParamTypeSurface = 53,
	ParamTypeState = 100,
	ParamTypeUnknown
};

#if INTPTR_MAX == INT64_MAX
#define _ptrCast(type) (type)(uint64_t)
#else
#define _ptrCast(type) (type)
#endif


#ifdef PYXIE_SHARED
	#ifdef _WIN32
	#define PYXIE_EXPORT __declspec(dllexport)
	#else
	#define PYXIE_EXPORT
	#endif
#else
	#define PYXIE_EXPORT
#endif
