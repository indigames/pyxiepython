///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#ifndef _pyxie_debug_
#define _pyxie_debug_

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum _ErrorCode
	{
		errNoError = 0x8000,
		errFileNotFound,
		errFailedToCompileShader,
		errFailedToAllocateMainMemory,
		errNoMatchFormatVersion,
		errFailedToLoadFile,
		errFailedToClone,
		errInvalidFileFormat,
		errShaderError,
		errOpenFileOver,
		errUnknown
	}ErrorCode;

#ifdef _FINAL
#define	pyxie_printf(...)			((void)0)
#define	pyxie_errstr(code)			((void)0)
#define	pyxie_gl_errstr(err)		((void)0)
#define	pyxie_assert(exp)			((void)0)
#define	pyxie_assert_msg(exp, ...)	((void)0)
#define	pyxie_ogl_assert			((void)0)
#define	pyxie_shader_ogl_assert		((void)0)
#else
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
	void	PYXIE_EXPORT pyxie_printf(const char* format, ...);
	const char PYXIE_EXPORT * pyxie_errstr(ErrorCode code);
	const char* pyxie_gl_errstr(unsigned int err);

//pyxie_printf("error:%s/%d\n", __FILE__, __LINE__);

#define pyxie_error_return(fmt, ...) pyxie_printf(fmt, ##__VA_ARGS__); return false;
#define pyxie_error_check_return(exp, fmt, ...)  if(!(exp)){pyxie_printf(fmt, ##__VA_ARGS__); return false;}
#define pyxie_warning(fmt, ...) pyxie_printf(fmt, ##__VA_ARGS__);

#define	pyxie_assert(exp)			assert(exp)
#define	pyxie_assert_msg(exp, ...)	assert(exp)
#define	pyxie_ogl_assert   \
            { \
                unsigned int _err_ = glGetError(); \
                if(_err_ != GL_NO_ERROR){ \
                    pyxie_printf("%s(0x%x):%s%d",pyxie_gl_errstr(_err_),_err_, __FILE__, __LINE__); \
				} \
            }
#define	pyxie_shader_ogl_assert   \
            { \
                unsigned int _err_ = glGetError(); \
                if(_err_ != GL_NO_ERROR) \
				{ \
					pyxie_printf("%s(0x%x):%s%d",pyxie_gl_errstr(_err_),_err_, __FILE__, __LINE__); \
					ShaderError(); \
				} \
            }
#endif


	void DebugSaveFile(const char* txt, const char* filename);


#ifdef __cplusplus
}
#endif


#endif
