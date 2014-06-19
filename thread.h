/*-------------------------------------------------------------------------
 File    : $Archive: $
 Author  : $Author: $
 Version : $Revision: $
 Orginal : 2006-07-26, 15:50
 Descr   : Thread wrappers for Win32 and Posix
 
 
 Modified: $Date: $ by $Author: $
 ---------------------------------------------------------------------------
 TODO: [ -:Not done, +:In progress, !:Completed]
 <pre>
 </pre>
 
 
 \History
 - 30.04.09, FKling, Moved defines to enums
 - 16.04.09, FKling, Implementation
 
 ---------------------------------------------------------------------------*/
#ifndef __GOAT_THREAD_H__
#define __GOAT_THREAD_H__

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef Yield
#else
#include <pthread.h>
#endif

#include "event.h"

namespace gnilk
{
	
#define GOAT_THREAD_ERR_NOERROR 0
#define GOAT_THREAD_ERR_UNKNOWN -1
#define GOAT_THREAD_ERR_INVALIDTHREAD -2
#define GOAT_THREAD_ERR_CREATIONFAILED -3
#define GOAT_THREAD_ERR_WRONGSTATE -4
#define GOAT_THREAD_ERR_NOTSUPPORTED -5

	
	typedef int(*GOATTHREADFUNC)(void *param);
	
	class Thread
	{
	public:
		typedef enum 
		{
			kThreadStateUnknown = 0,
			kThreadStateRunning = 1,
			kThreadStateSuspended = 2,
			kThreadStateStopped = 3
		} kThreadStates;
		typedef enum
		{
			kThreadCreateDefault = 0,
			kThreadCreateSuspended = 1,
		} kThreadCreateFlags;
		
		// NOT YET USED
		typedef enum
		{
			kThreadErrorNotSupported = -5,
			kThreadErrorWrongState = -4,
			kThreadErrorCreateFailed = -3,
			kThreadErrorInvalidThread = -2,
			kThreadErrorUnknown = -1,
			kThreadErrorNoError = 0,
		} kThreadErrors;
		
	protected:
	#ifdef WIN32
		HANDLE hThread;
		DWORD dwThreadID;
	#else
		pthread_attr_t attr;
		pthread_t hThread;
		Event *pSuspendEvent;
	#endif
		GOATTHREADFUNC pFunc;
		void *pArg;
		int iExit;
		kThreadStates iState;
		int flags;

		Thread(GOATTHREADFUNC pFunc, void *pArg, kThreadCreateFlags flags);
	public:
		
		static void Sleep(unsigned int tMsec);
		static Thread *Create(GOATTHREADFUNC pFunc, void *pArg, kThreadCreateFlags flags);
		//int Dispose(SqThread *pThread);
		virtual ~Thread();
		int State();
		int Start();
		int Stop();
		int GetExitCode();
		int Abort();
		int Join();
		int Yield();

		void InternalExecute();	// how to hide this one??
	};
}



#endif