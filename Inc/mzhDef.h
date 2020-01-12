#pragma once

#ifndef _MZH_DEF
#define _MZH_DEF

#define _MZH_BEGIN	namespace MZH {
#define _MZH_END	}	namespace mzh = MZH;
#define _MZH		::MZH::

#define RELEASE_METHOD(obj, rm, nul)	\
	{ if(obj != nul) { rm (obj); obj = nul; } }

#define FREE_PTR(p)			RELEASE_METHOD(p, ::free, NULL)			//	{ if(p) { free(p); p = NULL; } }
#define FREE_GLB(h)			RELEASE_METHOD(h, ::GlobalFree, NULL)		//	{ if(h) { GlobalFree(h); h = NULL; } }
#define DELETE_OBJ(p)		RELEASE_METHOD(p, delete, NULL)			//	{ if(p) { delete p; p = NULL; } }
#define DELETE_OBJS(p)		RELEASE_METHOD(p, delete[], NULL)		//	{ if(p) { delete[] p; p = NULL; } }
#define DELETE_ARRAY		DELETE_OBJS
#define SET_HANDLE(h, iv)	RELEASE_METHOD(h, ::CloseHandle, iv)
#define NULL_HANDLE(h)		SET_HANDLE(h, NULL)
#define INVALID_HANDLE(h)	SET_HANDLE(h, INVALID_HANDLE_VALUE)

#endif
