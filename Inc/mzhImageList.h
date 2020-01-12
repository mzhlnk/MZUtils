#pragma once

#ifndef _MZH_IMAGELIST
#define _MZH_IMAGELIST

#include <CommCtrl.h>
//#pragma comment(lib, "Comctl32.lib")
#include <mzhBase.h>

_MZH_BEGIN

template<bool t_bManaged>
class CImageListT : public CHObjT<HIMAGELIST, t_bManaged>
{
public:
	CImageListT(HIMAGELIST h = NULL)
		: CHObjT<HIMAGELIST, t_bManaged>(h)
	{ }
	virtual ~CImageListT()
	{
		if(t_bManaged)
			Release();
	}

public:
	virtual BOOL Close()
	{
		if(!::ImageList_Destroy(_hObj))
			return FALSE;
		_hObj = NULL;
		return TRUE;
	}
public:
	BOOL Create(int cx, int cy, UINT flags, int cInitial, int cGrow)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(ImageList_Create(cx, cy, flags, cInitial, cGrow));
	}

	BOOL SetImageCount(UINT nNewCount)
	{ return ImageList_SetImageCount(*this, nNewCount); }
	int GetImageCount(void)
	{ return ImageList_GetImageCount(*this); }
		
	COLORREF SetBkColor(COLORREF clrBk)
	{ return ImageList_SetBkColor(*this, clrBk); }
	COLORREF GetBkColor(void)
	{ return ImageList_GetBkColor(*this); }

	int Add(HBITMAP hbmImage, HBITMAP hbmMask)
	{ return ImageList_Add(*this, hbmImage, hbmMask); }
	int Add(HBITMAP hbmImage, COLORREF clrMask)
	{ return ImageList_AddMasked(*this, hbmImage, clrMask); }

	BOOL Draw(int nIndex, HDC hDCDst, int x, int y, UINT nStyle = ILD_NORMAL)
	{ return ImageList_Draw(*this, nIndex, hDCDst, x, y, nStyle); }
};

typedef CImageListT<true> CImageList;
typedef CImageListT<false> CImageListHandle;

_MZH_END
#endif

