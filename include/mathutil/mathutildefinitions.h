/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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