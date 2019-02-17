#ifndef __MATHUTILDEFINITIONS_H__
#define __MATHUTILDEFINITIONS_H__

#ifdef MUTIL_STATIC
	#define DLLMUTIL
#elif MUTIL_DLL
	#ifdef __linux__
		#define DLLMUTIL __attribute__((visibility("default")))
	#else
		#define DLLMUTIL __declspec(dllexport)
	#endif
#else
	#ifdef __linux__
		#define DLLMUTIL
	#else
		#define DLLMUTIL __declspec(dllimport)
	#endif
#endif

#endif