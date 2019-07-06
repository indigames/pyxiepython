#pragma once
#include "pyxieTypes.h"

#if defined _WIN32			//WIN32
#include <process.h>
#include <windows.h>
typedef unsigned (__stdcall *ThreadFuncType)(void*);
typedef HANDLE THREADHANDLE;
typedef CRITICAL_SECTION CRITICALSECTION;
typedef CONDITION_VARIABLE CONDITIONVARIABLE;
typedef HANDLE SEMAPHORE;
#define THREADFUNC(func) unsigned __stdcall func(void* arg)
#define ExitThread() _endthreadex(0); return 0
#else						//IOS , OSX and Android
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
typedef pthread_mutex_t CRITICALSECTION;
typedef pthread_cond_t CONDITIONVARIABLE;
typedef sem_t SEMAPHORE;
typedef pthread_t THREADHANDLE;
typedef void* (*ThreadFuncType)(void *);
#define THREADFUNC(func) void* func(void * arg)
#define ExitThread() pthread_exit(0)
#endif

#define ArgPointer arg

namespace pyxie
{
	class CriticalSection
	{
		CRITICALSECTION criticalSection;
	public:
		CriticalSection();
		~CriticalSection();
		void Enter();
		void Leave();
	};
	class Semaphore
	{
		SEMAPHORE sem;
	public:
		Semaphore(int initval=1);
		~Semaphore();
		void Lock();
		void Unlock();
	};

	 THREADHANDLE PYXIE_EXPORT StartThread(ThreadFuncType func, void* arg, unsigned stackSize, const char* threadname);
	 void PYXIE_EXPORT JoinThread(THREADHANDLE handle);
	 void PYXIE_EXPORT SleepThread(int milliSecond, int microSecond=0);
}


/*	Sample 

	static THREADFUNC(sampleFunc){
		void* pArg = ArgPointer;
			:
		ExitThread();
	}
	static int data[4] = {1,2,3,4};
	THREADHANDLE h = StartThread(sampleFunc, data, 4096, "Sample thread");
	JoinThread(h);
*/