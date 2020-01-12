#pragma once
#ifndef _MZH_BITMAP
#define _MZH_BITMAP

#include <wingdi.h>
#include <mzhBase.h>

_MZH_BEGIN

template<typename T, bool t_bManaged>
class CGdiObjT : public CHObjT<T, t_bManaged>
{
public:
	explicit CGdiObjT(T h = NULL)
		: CHObjT<T, t_bManaged>(h)
	{ }
	virtual ~CGdiObjT()
	{
		if(t_bManaged)
			Release();
	}

public:
	operator HGDIOBJ() const	{ return (HGDIOBJ)_hObj; }

public:
	virtual BOOL Close()
	{
		if (_hObj)
		{
			if(!::DeleteObject((HGDIOBJ)_hObj))
				return FALSE;
			_hObj = NULL;
		}
		return TRUE;
	}

public:
	BOOL Unrealize()
	{
		ASSERT(IsValid());
		return ::UnrealizeObject(*this);
	}
	int GetObject(LPVOID lpvObject, int cbBuffer)
	{
		ASSERT(IsValid());
		return ::GetObject(*this, cbBuffer, lpvObject);
	}
	DWORD GetObjectType()
	{
		ASSERT(IsValid());
		return ::GetObjectType(*this);
	}
};

template<bool t_bManaged>
class CBitmapT : public CGdiObjT<HBITMAP, t_bManaged>
{
public:
	explicit CBitmapT(HBITMAP h = NULL)
		: CGdiObjT<HBITMAP, t_bManaged>(h)
	{ }
	explicit CBitmapT(HDC hDC, int nWidth, int nHeight)
	{ _hObj = ::CreateCompatibleBitmap(hDC, nWidth, nHeight); }

public:
	BOOL CreateBitmap(int nWidth, int nHeight, UINT cPlanes, UINT cBitsPerPel, LPCVOID lpvBits = NULL)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateBitmap(nWidth, nHeight, cPlanes, cBitsPerPel, lpvBits));
	}
	BOOL CreateBitmapIndirect(CONST BITMAP& bm)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateBitmapIndirect(&bm));
	}
	BOOL CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateCompatibleBitmap(hDC, nWidth, nHeight));
	}
	BOOL CreateDIBitmap(HDC hDC, CONST BITMAPINFOHEADER* pbmih, DWORD fdwInit, LPCVOID pbInit, CONST BITMAPINFO* pbmi, UINT iUsage)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateDIBitmap(hDC, pbmih, fdwInit, pbInit, pbmi, iUsage));
	}
	BOOL CreateDIBSection(HDC hDC, CONST BITMAPINFO* pbmi, UINT iUsage, LPVOID* ppvBits, HANDLE hSection = NULL, DWORD dwOffset = 0)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateDIBSection(hDC, pbmi, iUsage, ppvBits, hSection, dwOffset));
	}

public:
	int GetBitmap(BITMAP& bmp)
	{
		ASSERT(IsValid());
		return GetObject(&bmp, sizeof(bmp));
	}
	BOOL GetDimensionEx(SIZE& Dimension)
	{
		ASSERT(IsValid());
		return ::GetBitmapDimensionEx(*this, &Dimension);
	}
	BOOL SetDimensionEx(int nWidth, int nHeight, LPSIZE lpSize = NULL)
	{
		ASSERT(IsValid());
		return ::SetBitmapDimensionEx(*this, nWidth, nHeight, lpSize);
	}

public:
	LONG SetBitmapBits(DWORD cBytes, LPCVOID lpBits)
	{
		ASSERT(IsValid());
		return ::SetBitmapBits(*this, cBytes, lpBits);
	}
};
typedef CBitmapT<true> CBitmap;
typedef CBitmapT<false> CBitmapHandle;

template<bool t_bManaged>
class CBrushT : public CGdiObjT<HBRUSH, t_bManaged>
{
public:
	explicit CBrushT(HBRUSH h = NULL)
		: CGdiObjT<HBRUSH, t_bManaged>(h)
	{ }
	explicit CBrushT(COLORREF clr)
	{ _hObj = ::CreateSolidBrush(clr); }

public:
	BOOL CreateSolidBrush(COLORREF clr)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateSolidBrush(clr));
	}
	int GetLogBrush(LOGBRUSH& lbr)
	{
		ASSERT(!IsValid());
		return GetObject(&lbr, sizeof(lbr));
	}
};
typedef CBrushT<true> CBrush;
typedef CBrushT<false> CBrushHandle;

template<bool t_bManaged>
class CPenT : public CGdiObjT<HPEN, t_bManaged>
{
public:
	explicit CPenT(HPEN h = NULL)
		: CGdiObjT<HPEN, t_bManaged>(h)
	{ }
	explicit CPenT(int iStyle, int cWidth, COLORREF clr)
	{ _hObj = ::CreatePen(iStyle, cWidth, clr); }

public:
	BOOL CreatePen(int iStyle, int cWidth, COLORREF clr)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreatePen(iStyle, cWidth, clr));
	}
	int GetLogPen(LOGPEN& lp)
	{
		ASSERT(!IsValid());
		return GetObject(&lp, sizeof(lp));
	}
};
typedef CPenT<true> CPen;
typedef CPenT<false> CPenHandle;

template<bool t_bManaged>
class CRgnT : public CGdiObjT<HRGN, t_bManaged>
{
public:
	explicit CRgnT(HRGN h = NULL)
		: CGdiObjT<HRGN, t_bManaged>(h)
	{ }

public:
	BOOL CreateElliptic(int x1, int y1, int x2, int y2)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateEllipticRgn(x1, y1, x2, y2));
	}
	BOOL CreateEllipticIndirect(const RECT& rc)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateEllipticRgnIndirect(&rc));
	}
	BOOL CreatePolygon(const POINT* ppt, int cPoints, int iMode)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreatePolygonRgn(ppt, cPoints, iMode));
	}
	BOOL CreatePolyPolygon(const POINT* ppt, const INT* pc, int cPoly, int iMode)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreatePolyPolygonRgn(ppt, pc, cPoly, iMode));
	}
	BOOL CreateRect(int x1, int y1, int x2, int y2)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateRectRgn(x1, y1, x2, y2));
	}
	BOOL CreateRectIndirect(const RECT& rc)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateRectRgnIndirect(&rc));
	}
	BOOL CreateRoundRect(int x1, int y1, int x2, int y2, int w, int h)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateRoundRectRgn(x1, y1, x2, y2, w, h));
	}
	BOOL ExtCreate(const XFORM* lpx, DWORD nCount, const RGNDATA* lpData)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::ExtCreateRegion(lpx, nCount, lpData));
	}
public:
	BOOL Equal(HRGN h)
	{
		ASSERT(IsValid() && h);
		return ::EqualRgn(*this, h);
	}
	bool operator == (HRGN h) const	{ return Equal(h); }

public:
	int Combine(HRGN hSrc1, HRGN hSrc2, int iMode)
	{
		ASSERT(IsValid());
		return ::CombineRgn(*this, hSrc1, hSrc2, iMode);
	}
	int GetBox(RECT& rc)
	{
		ASSERT(IsValid());
		return ::GetRgnBox(*this, &rc);
	}
	DWORD GetData(LPRGNDATA lpData, DWORD dwCount)
	{
		ASSERT(IsValid());
		return ::GetRegionData(*this, dwCount, lpData);
	}
	int Offset(int x, int y)
	{
		ASSERT(IsValid());
		return ::OffsetRgn(*this, x, y);
	}
	BOOL PtIn(int x, int y)
	{
		ASSERT(IsValid());
		return ::PtInRegion(*this, x, y);
	}
	BOOL RectIn(const RECT& rc)
	{
		ASSERT(IsValid());
		return ::RectInRegion(*this, &rc);
	}
	BOOL SetRect(int x1, int y1, int x2, int y2)
	{
		ASSERT(IsValid());
		return ::SetRectRgn(*this, x1, y1, x2, y2);
	}
};
typedef CRgnT<true> CRgn;
typedef CRgnT<false> CRgnHandle;

template<bool t_bManaged>
class CPaletteT : public CGdiObjT<HPALETTE, t_bManaged>
{
public:
	explicit CPaletteT(HPALETTE h = NULL)
		: CGdiObjT<HPALETTE, t_bManaged>(h)
	{}

public:
	BOOL Create(const LOGPALETTE* lplgpl)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreatePalette(lplgpl));
	}
	HPALETTE CreateHalftonePalette(HDC hdc)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateHalftonePalette(hdc));
	}

public:
	BOOL Animate(UINT iStartIndex, UINT cEntries, const PALETTEENTRY* ppe)
	{
		ASSERT(IsValid());
		return ::AnimatePalette(*this, iStartIndex, cEntries, ppe);
	}
	UINT GetNearestIndex(COLORREF crColor)
	{
		ASSERT(IsValid());
		return ::GetNearestPaletteIndex(*this, crColor);
	}
	UINT GetEntries(UINT iStartIndex, UINT cEntries, LPPALETTEENTRY lppe)
	{
		ASSERT(IsValid());
		return ::GetPaletteEntries(*this, iStartIndex, cEntries, lppe);
	}
	UINT SetEntries(UINT iStart, UINT cEntries, const PALETTEENTRY* lppe)
	{
		ASSERT(IsValid());
		return ::SetPaletteEntries(*this, iStartIndex, cEntries, lppe);
	}
	BOOL ResizePalette(UINT nEntries)
	{
		ASSERT(IsValid());
		return ::ResizePalette(*this, nEntries);
	}
};
typedef CPaletteT<true> CPalette;
typedef CPaletteT<false> CPaletteHandle;

template<bool t_bManaged>
class CFontT : public CGdiObjT<HFONT, t_bManaged>
{
public:
	explicit CFontT(HFONT h = NULL)
		: CGdiObjT<HFONT, t_bManaged>(h)
	{ }

public:
	BOOL Create(int nHeight, int nWidth, int nEscapement, int nOrientation, int nWeight,
		BYTE bItalic			= FALSE,
		BYTE bUnderline			= FALSE,
		BYTE cStrikeOut			= 0,
		BYTE nCharSet			= DEFAULT_CHARSET,
		BYTE nOutPrecision		= OUT_DEFAULT_PRECIS,
		BYTE nClipPrecision		= CLIP_DEFAULT_PRECIS,
		BYTE nQuality			= DEFAULT_QUALITY,
		BYTE nPitchAndFamily	= DEFAULT_PITCH,
		LPCTSTR lpszFacename	= NULL)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateFont(nHeight, nWidth, nEscapement, nOrientation, nWeight,
			bItalic, bUnderline, cStrikeOut, nCharSet, nOutPrecision, nClipPrecision, nQuality,
			nPitchAndFamily, lpszFacename));
	}
	BOOL CreateIndirect(const LOGFONT& lf)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateFontIndirect(&lf));
	}
	BOOL CreateIndirectEx(const ENUMLOGFONTEXDV& enumlfex)
	{
		ASSERT(!IsValid());
		return VALID_ATTACH(::CreateFontIndirectEx(&enumlfex));
	}

public:
	int GetLogFont(LOGFONT& lf)
	{
		ASSERT(!IsValid());
		return GetObject(&lf, sizeof(lf));
	}
};
typedef CFontT<true> CFont;
typedef CFontT<false> CFontHandle;

_MZH_END
#endif
