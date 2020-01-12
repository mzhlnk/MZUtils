#pragma once

#ifndef _MZH_DC
#define _MZH_DC

#include <mzhGdiObj.h>

_MZH_BEGIN

template<bool t_bManaged>
class CDCT : public CHObjT<HDC, t_bManaged>
{
public:
	explicit CDCT(HDC hDC = NULL)
		: CHObjT<HDC, t_bManaged>(hDC)
	{ }
	virtual ~CDCT(void)
	{
		if(t_bManaged && IsValid())
			DeleteDC();
	}

public:
	HDC GetSafeDC() const	{ return (this ? _hDC : NULL); }

protected:
	virtual BOOL Close()
	{
		ASSERT(_hDC);
		if(!::DeleteDC(_hDC))
			return FALSE;
		_hDC = NULL;
		return TRUE;
	}
public:
	virtual BOOL CreateCompatibleDC(HDC hDC)
	{
		ASSERT(!IsValid());
		HDC h = ::CreateCompatibleDC(hDC);
		if(h)
		{
			Attach(h);
			return TRUE;
		}
		return FALSE;
	}
	virtual int ReleaseDC(HWND hWnd)
	{ ASSERT(IsValid()); return ::ReleaseDC(hWnd, *this); }
	virtual BOOL DeleteDC()
	{ return Close(); }
	BOOL CancelDC()
	{ ASSERT(IsValid()); return ::CancelDC(*this); }

public:
	int GetDeviceCaps(int nIndex)
	{ ASSERT(IsValid()); return ::GetDeviceCaps(*this, nIndex); }
	BOOL GetDeviceGammaRamp(LPVOID lpRamp)
	{ ASSERT(IsValid()); return ::GetDeviceGammaRamp(*this, lpRamp); }
	BOOL SetDeviceGammaRamp(LPVOID lpRamp)
	{ ASSERT(IsValid()); return ::SetDeviceGammaRamp(*this, lpRamp); }
	int DrawEscape(int iEscape, int cbIn, LPCSTR lpIn)
	{ ASSERT(IsValid()); return ::DrawEscape(*this, iEscape, cbIn, lpIn); }
	HDC ResetDC(CONST DEVMODE* lpInitData)
	{ ASSERT(IsValid()); return ::ResetDC(*this, lpInitData); }
	int SaveDC()
	{ ASSERT(IsValid()); return ::SaveDC(*this); }
	BOOL RestoreDC(int nSavedDC)
	{ ASSERT(IsValid()); return ::RestoreDC(*this, nSavedDC); }

	int GetGraphicsMode()
	{ ASSERT(IsValid()); return ::GetGraphicsMode(*this); }
	int SetGraphicsMode(int iMode)
	{ ASSERT(IsValid()); return ::SetGraphicsMode(*this, iMode); }
	int GetMapMode()
	{ ASSERT(IsValid()); return GetMapMode(*this); }
	int SetMapMode(int mode)
	{ ASSERT(IsValid()); return ::SetMapMode(*this, mode); }

public:
	DWORD GetLayout()
	{ ASSERT(IsValid()); return ::GetLayout(*this); }
	DWORD SetLayout(DWORD dwLayout)
	{ ASSERT(IsValid()); return ::SetLayout(*this, dwLayout); }
	HGDIOBJ GetCurrentObject(UINT type)
	{ ASSERT(IsValid()); return ::GetCurrentObject(*this, type); }
	HGDIOBJ SelectObject(HGDIOBJ h)
	{ ASSERT(IsValid()); return ::SelectObject(*this, h); }
	COLORREF GetBrushColor()
	{ ASSERT(IsValid()); return ::GetDCBrushColor(*this); }
	COLORREF SetBrushColor(COLORREF color)
	{ ASSERT(IsValid()); return ::SetDCBrushColor(*this, color); }
	COLORREF GetPenColor()
	{ ASSERT(IsValid()); return ::GetDCPenColor(*this); }
	COLORREF SetPenColor(COLORREF color)
	{ ASSERT(IsValid()); return ::SetDCPenColor(*this, color); }

public:
	int GetBkMode()
	{ ASSERT(IsValid()); return ::GetBkMode(*this); }
	int SetBkMode(int mode)
	{ ASSERT(IsValid()); return ::SetBkMode(*this, mode); }
	UINT GetTextAlign()
	{ ASSERT(IsValid()); return ::GetTextAlign(*this); }
	UINT SetTextAlign(UINT align)
	{ ASSERT(IsValid()); return ::SetTextAlign(*this, align); }
	int GetTextCharacterExtra()
	{ ASSERT(IsValid()); return ::GetTextCharacterExtra(*this); }
	int SetTextCharacterExtra(int nCharExtra)
	{ ASSERT(IsValid()); return ::SetTextCharacterExtra(*this, nCharExtra); }
	COLORREF GetTextColor()
	{ ASSERT(IsValid()); return ::GetTextColor(*this); }
	COLORREF SetTextColor(COLORREF color)
	{ ASSERT(IsValid()); return ::SetTextColor(*this, color); }
	BOOL GetTextExtentExPoint(LPCTSTR lpszStr, int cchString, int nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize)
	{ ASSERT(IsValid()); return ::GetTextExtentExPoint(*this, lpszStr, cchString, nMaxExtent, lpnFit, alpDx, lpSize); }
	BOOL GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize)
	{ ASSERT(IsValid()); return ::GetTextExtentExPointI(*this, pgiIn, cgi, nMaxExtent, lpnFit, alpDx, lpSize); }
	BOOL GetTextExtentPoint(LPCTSTR lpString, int c, LPSIZE lpSize)
	{ ASSERT(IsValid()); return ::GetTextExtentPoint32(*this, lpString, c, lpSize); }
	BOOL GetTextExtentPointI(LPWORD pgiIn, int cgi, LPSIZE lpSize)
	{ ASSERT(IsValid()); return ::GetTextExtentPointI(*this, pgiIn, cgi, lpSize); }
	BOOL TextOut(int x, int y, LPCTSTR lpszText, int len)
	{ ASSERT(IsValid()); return ::TextOut(*this, x, y, lpszText, len); }
	BOOL ExtTextOut(int x, int y, UINT fuOptions, LPCRECT lprc, LPCTSTR lpString, UINT cbCount, CONST INT* lpDx)
	{ ASSERT(IsValid()); return ::ExtTextOut(*this, x, y, fuOptions, lprc, lpString, cbCount, lpDx); }
	int DrawText(LPCTSTR lpchText, int cchText, LPRECT lprc, UINT format = DT_LEFT | DT_TOP)
	{ ASSERT(IsValid()); return ::DrawText(*this, lpchText, cchText, lprc, format); }
	int DrawTextEx(LPTSTR lpchText, int cchText, LPRECT lprc, UINT uDTFormat = DT_LEFT | DT_TOP, LPDRAWTEXTPARAMS lpDTParams = NULL)
	{ ASSERT(IsValid()); return ::DrawTextEx(*this, lpchText, cchText, lprc, uDTFormat, lpDTParams); }
	BOOL PolyTextOut(CONST POLYTEXT* pptxt, int cStrings)
	{ ASSERT(IsValid()); return ::PolyTextOut(*this, pptxt, cStrings); }
	LONG TabbedTextOut(int x, int y, LPCTSTR lpString, int nCount, int nTabPositions, CONST INT* lpnTabStopPositions, int nTabOrigin)
	{ ASSERT(IsValid()); return ::TabbedTextOut(*this, x, y, lpString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin); }
	BOOL GetAspectRatioFilterEx(LPSIZE lpAspectRatio)
	{ ASSERT(IsValid()); return ::GetAspectRatioFilterEx(*this, lpAspectRatio); }
	BOOL GetCharABCWidths(UINT uFirstChar, UINT uLastChar, LPABC lpabc)
	{ ASSERT(IsValid()); return ::GetCharABCWidths(*this, uFirstChar, uLastChar, lpabc); }
	BOOL GetCharABCWidthsFloat(UINT iFirstChar, UINT iLastChar, LPABCFLOAT lpABCF)
	{ ASSERT(IsValid()); return ::GetCharABCWidthsFloat(*this, iFirstChar, iLastChar, lpABCF); }
	BOOL GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc)
	{ ASSERT(IsValid()); return ::GetCharABCWidthsI(*this, giFirst, cgi, pgi, lpabc); }
	DWORD GetCharacterPlacement(LPCTSTR lpString, int nCount, int nMaxExtent, LPGCP_RESULTS lpResults, DWORD dwFlags)
	{ ASSERT(IsValid()); return ::GetCharacterPlacement(*this, lpString, nCount, nMaxExtent, lpResults, dwFlags); }
	BOOL GetCharWidth(UINT iFirstChar, UINT iLastChar, LPINT lpBuffer)
	{ ASSERT(IsValid()); return ::GetCharWidth32(*this, iFirstChar, iLastChar, lpBuffer); }
	BOOL GetCharWidthFloat(UINT iFirstChar, UINT iLastChar, PFLOAT pxBuffer)
	{ ASSERT(IsValid()); return ::GetCharWidthFloat(*this, iFirstChar, iLastChar, pxBuffer); }
	BOOL GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT piWidths)
	{ ASSERT(IsValid()); return ::GetCharWidthI(*this, giFirst, cgi, pgi, piWidths); }
	DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpvBuffer, DWORD cbData)
	{ ASSERT(IsValid()); return ::GetFontData(*this, dwTable, dwOffset, lpvBuffer, cbData); }
	DWORD GetFontLanguageInfo()
	{ ASSERT(IsValid()); return ::GetFontLanguageInfo(*this); }
	DWORD GetFontUnicodeRanges(LPGLYPHSET lpgs)
	{ ASSERT(IsValid()); return ::GetFontUnicodeRanges(*this, lpgs); }
	DWORD GetGlyphIndices(LPCTSTR lpstr, int c, LPWORD pgi, DWORD fl = GGI_MARK_NONEXISTING_GLYPHS)
	{ ASSERT(IsValid()); return ::GetGlyphIndices(*this, lpstr, c, pgi, fl); }
	DWORD GetGlyphOutline(UINT uChar, UINT uFormat, LPGLYPHMETRICS lpgm, DWORD cbBuffer, LPVOID lpvBuffer, CONST MAT2* lpmat2)
	{ ASSERT(IsValid()); return ::GetGlyphOutline(*this, uChar, uFormat, lpgm, cbBuffer, lpvBuffer, lpmat2); }
	DWORD GetKerningPairs(DWORD nNumPairs, LPKERNINGPAIR lpkrnpair)
	{ ASSERT(IsValid()); return ::GetKerningPairs(*this, nNumPairs, lpkrnpair); }
	UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpOTM)
	{ ASSERT(IsValid()); return ::GetOutlineTextMetrics(*this, cbData, lpOTM); }
	DWORD GetTabbedTextExtent(LPCTSTR lpString, int nCount, int nTabPositions, CONST INT* lpnTabStopPositions)
	{ ASSERT(IsValid()); return ::GetTabbedTextExtent(*this, lpString, nCount, nTabPositions, lpnTabStopPositions); }
	int GetTextFace(int nCount, LPTSTR lpFaceName)
	{ ASSERT(IsValid()); return ::GetTextFace(*this, nCount, lpFaceName); }
	BOOL GetTextMetrics(LPTEXTMETRIC lptm)
	{ ASSERT(IsValid()); return ::GetTextMetrics(*this, lptm); }
	DWORD SetMapperFlags(DWORD dwFlag)
	{ ASSERT(IsValid()); return ::SetMapperFlags(*this, dwFlag); }
	BOOL SetTextJustification(int nBreakExtra, int nBreakCount)
	{ ASSERT(IsValid()); return ::SetTextJustification(*this, nBreakCount, nBreakCount); }

public:
	BOOL GetOrgEx(LPPOINT lpPoint)
	{ ASSERT(IsValid()); return ::GetDCOrgEx(*this, lpPoint); }
	BOOL DPtoLP(LPPOINT lppt, int nCount)
	{ ASSERT(IsValid()); return ::DPtoLP(*this, lppt, nCount); }
	BOOL LPtoDP(LPPOINT lppt, int nCount)
	{ ASSERT(IsValid()); return ::LPtoDP(*this, lppt, nCount); }
	BOOL GetCurrentPositionEx(POINT& pt)
	{ ASSERT(IsValid()); return ::GetCurrentPositionEx(*this, &pt); }
	BOOL GetViewportExtEx(SIZE& size)
	{ ASSERT(IsValid()); return ::GetViewportExtEx(*this, &size); }
	BOOL SetViewportExtEx(int x, int y, LPSIZE lpsz = NULL)
	{ ASSERT(IsValid()); return ::SetViewportExtEx(*this, x, y, lpsz); }
	BOOL GetViewportOrgEx(POINT& pt)
	{ ASSERT(IsValid()); return ::GetViewportOrgEx(*this, &pt); }
	BOOL SetViewportOrgEx(int x, int y, LPPOINT lppt = NULL)
	{ ASSERT(IsValid()); return ::SetViewportOrgEx(*this, x, y, lppt); }
	BOOL GetWindowExtEx(SIZE& size)
	{ ASSERT(IsValid()); return ::GetWindowExtEx(*this, &size); }
	BOOL SetWindowExtEx(int x, int y, LPSIZE lpsz = NULL)
	{ ASSERT(IsValid()); return ::SetWindowExtEx(*this, x, y, lpsz); }
	BOOL GetWindowOrgEx(POINT& pt)
	{ ASSERT(IsValid()); return ::GetWindowOrgEx(*this, &pt); }
	BOOL SetWindowOrgEx(int x, int y, LPPOINT lppt = NULL)
	{ ASSERT(IsValid()); return ::SetWindowOrgEx(*this, x, y, lppt); }
	BOOL OffsetViewportOrgEx(int x, int y, LPPOINT lppt = NULL)
	{ ASSERT(IsValid()); return ::OffsetViewportOrgEx(*this, x, y, lppt); }
	BOOL OffsetWindowOrgEx(int x, int y, LPPOINT lppt = NULL)
	{ ASSERT(IsValid()); return ::OffsetWindowOrgEx(*this, x, y, lppt); }
	BOOL ScaleViewportExtEx(int xn, int xd, int yn, int yd, LPSIZE lpsz = NULL)
	{ ASSERT(IsValid()); return ::ScaleViewportExtEx(xn, xd, yn, yd, lpsz); }
	BOOL ScaleWindowExtEx(int xn, int xd, int yn, int yd, LPSIZE lpsz = NULL)
	{ ASSERT(IsValid()); return ::ScaleWindowExtEx(xn, xd, yn, yd, lpsz); }
	BOOL GetWorldTransform(XFORM& xform)
	{ ASSERT(IsValid()); return ::GetWorldTransform(*this, &xform); }
	BOOL ModifyWorldTransform(CONST XFORM& xf, DWORD iMode)
	{ ASSERT(IsValid()); return ::ModifyWorldTransform(*this, &xf, iMode); }
	BOOL SetWorldTransform(CONST XFORM& xf)
	{ ASSERT(IsValid()); return ::SetWorldTransform(*this, &xf); }

public:
	CPoint MoveTo(CONST POINT pt)	{ return MoveTo(pt.x, pt.y); }
	CPoint MoveTo(int x, int y)
	{
		ASSERT(IsValid());
		CPoint pt;
		::MoveToEx(*this, x, y, &pt);
		return pt;
	}
	BOOL LineTo(CONST POINT pt) { return LineTo(pt.x, pt.y); }
	BOOL LineTo(int x, int y)
	{ ASSERT(IsValid()); return ::LineTo(*this, x, y); }
	BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{ ASSERT(IsValid()); return ::Chord(*this, x1, y1, x2, y2, x3, y3, x4, y4); }
	BOOL Ellipse(int x1, int y1, int x2, int y2)
	{ ASSERT(IsValid()); return ::Ellipse(*this, x1, y1, x2, y2); }
	void FillRect(CONST RECT& rc, HBRUSH hbr)
	{ ASSERT(IsValid()); ::FillRect(*this, &rc, hbr); }
	int FrameRect(CONST RECT& rc, HBRUSH hbr)
	{ ASSERT(IsValid()); return ::FrameRect(*this, &rc, hbr); }
	BOOL InvertRect(CONST RECT& rc)
	{ ASSERT(IsValid()); return ::InvertRect(*this, &rc); }
	BOOL Pie(int left, int top, int right, int bottom, int xr1, int yr1, int xr2, int yr2)
	{ ASSERT(IsValid()); return ::Pie(*this, left, top, right, bottom, xr1, yr1, xr2, yr2); }
	BOOL Polygon(CONST POINT* lpPoints, int nCount)
	{ ASSERT(IsValid()); return ::Polygon(*this, lpPoints, nCount); }
	BOOL PolyPolygon(CONST POINT* lpPoints, CONST INT* lpPolyCounts, int nCount)
	{ ASSERT(IsValid()); return ::PolyPolygon(*this, lpPoints, lpPolyCounts, nCount); }
	BOOL Rectangle(int left, int top, int right, int bottom)
	{ ASSERT(IsValid()); return ::Rectangle(*this, left, top, right, bottom); }
	BOOL RoundRect(int left, int top, int right, int bottom, int width, int height)
	{ ASSERT(IsValid()); return ::RoundRect(*this, left, top, right, bottom, width, height); }

public:
	BOOL DrawEdge(RECT& rc, UINT edge, UINT grfFlags)
	{ ASSERT(IsValid()); return ::DrawEdge(*this, &rc, edge, grfFlags); }
	BOOL DrawFocusRect(CONST RECT& rc)
	{ ASSERT(IsValid()); return ::DrawFocusRect(*this, &rc); }
	BOOL DrawFrameControl(RECT& rc, UINT uType, UINT uState)
	{ ASSERT(IsValid()); return ::DrawFrameControl(*this, &rc, uType, uState); }
	BOOL DrawState(HBRUSH hbr, DRAWSTATEPROC pfnOutputFunc, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT uFlags)
	{ ASSERT(IsValid()); return ::DrawState(*this, hbr, pfnOutputFunc, lData, wData, x, y, cx, cy, uFlags); }
	int ExcludeUpdateRgn(HWND hWnd)
	{ ASSERT(IsValid()); return ::ExcludeUpdateRgn(*this, hWnd); }
	UINT GetBoundsRect(RECT& rcBounds, UINT flags = DCB_RESET)
	{ ASSERT(IsValid()); return ::GetBoundsRect(*this, &rcBounds, flags); }
	UINT SetBoundsRect(CONST RECT& rcBounds, UINT flags = DCB_RESET)
	{ ASSERT(IsValid()); return ::SetBoundsRect(*this, &rcBounds, flags); }
	int GetROP2()
	{ ASSERT(IsValid()); return ::GetROP2(*this); }
	int SetROP2(int rop2)
	{ ASSERT(IsValid()); return ::SetROP2(*this, rop2); }
	BOOL GrayString(HBRUSH hBrush, GRAYSTRINGPROC pfnOutputFunc, LPARAM lpData, int nCount, int x, int y, int cx, int cy)
	{ ASSERT(IsValid()); return ::GrayString(*this, hBrush, pfnOutputFunc, lpData, nCount, x, y, cx, cy); }
	BOOL PaintDesktop()
	{ ASSERT(IsValid()); return ::PaintDesktop(*this); }
	HWND GetWindow()
	{ ASSERT(IsValid()); return ::WindowFromDC(*this); }

public:
	void FillSolidRect(LPCRECT lpRect, COLORREF clr)
	{
		ASSERT(IsValid());
		::SetBkColor(*this, clr);
		::ExtTextOut(*this, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
	}

public:
	BOOL AlphaBlend(int x, int y, int cx, int cy, HDC hDC_s, int x_s, int y_s, int cx_s, int cy_s, BLENDFUNCTION ftn)
	{ ASSERT(IsValid()); return ::AlphaBlend(*this, x, y, cx, cy, hDC_s, x_s, y_s, cx_s, cy_s, ftn); }
	BOOL BitBlt(int x, int y, int cx, int cy, HDC hDC_s, int x_s, int y_s, DWORD dwRop = SRCCOPY)
	{ ASSERT(IsValid()); return ::BitBlt(*this, x, y, cx, cy, hDC_s, x_s, y_s, dwRop); }
	BOOL FloodFill(int x, int y, COLORREF color)
	{ ASSERT(IsValid()); return ::FloodFill(*this, x, y, color); }
	BOOL ExtFloodFill(int x, int y, COLORREF color, UINT type)
	{ ASSERT(IsValid()); return ::ExtFloodFill(*this, x, y, color, type); }
	UINT GetDIBColorTable(UINT iStart, UINT cEntries, RGBQUAD* prgbq)
	{ ASSERT(IsValid()); return ::GetDIBColorTable(*this, iStart, cEntries, prgbq); }
	UINT SetDIBColorTable(UINT iStart, UINT cEntries, CONST RGBQUAD* prgbq)
	{ ASSERT(IsValid()); return ::SetDIBColorTable(*this, iStart, cEntries, prgbq); }
	int GetDIBits(HBITMAP hbm, UINT start, UINT cLines, LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT usage)
	{ ASSERT(IsValid()); return ::GetDIBits(*this, hbm, start, cLines, lpvBits, lpbmi, usage); }
	int SetDIBits(HBITMAP hbm, UINT start, UINT cLines, LPCVOID lpBits, CONST BITMAPINFO* lpbmi, UINT ColorUse)
	{ ASSERT(IsValid()); return ::SetDIBits(*this, hbm, start, cLines, lpBits, lpbmi, ColorUse); }
	int SetDIBitsToDevice(int xDest, int yDest, DWORD w, DWORD h, int xSrc, int ySrc, UINT StartScan, UINT cLines, CONST VOID * lpvBits, CONST BITMAPINFO* lpbmi, UINT ColorUse)
	{ ASSERT(IsValid()); return ::SetDIBitsToDevice(*this, xDest, yDest, w, h, xSrc, ySrc, StartScan, cLines, lpvBits, lpbmi, ColorUse); }
	COLORREF GetPixel(int x, int y)
	{ ASSERT(IsValid()); return ::GetPixel(*this, x, y); }
	COLORREF SetPixel(int x, int y, COLORREF color)
	{ ASSERT(IsValid()); return ::SetPixel(*this, x, y, color); }
	BOOL SetPixelV(int x, int y, COLORREF color)
	{ ASSERT(IsValid()); return ::SetPixelV(*this, x, y, color); }
	BOOL GradientFill(PTRIVERTEX pVertex, ULONG nVertex, PVOID pMesh, ULONG nMesh, ULONG uMode)
	{ ASSERT(IsValid()); return ::GradientFill(*this, pVertex, nVertex, pMesh, nMesh, uMode); }
	BOOL MaskBlt(int xDest, int yDest, int width, int height, HDC hdcSrc, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, int yMask, DWORD rop)
	{ ASSERT(IsValid()); return ::MaskBlt(this, xDest, yDest, width, height, hdcSrc, xSrc, ySrc, hbmMask, xMask, yMask, rop); }
	BOOL PlgBlt(CONST POINT* lpPoint, HDC hdcSrc, int xSrc, int ySrc, int width, int height, HBITMAP hbmMask, int xMask, int yMask)
	{ ASSERT(IsValid()); return ::PlgBlt(*this, lpPoint, hdcSrc, xSrc, ySrc, width, height, hbmMask, xMask, yMask); }
	int GetStretchBltMode()
	{ ASSERT(IsValid()); return ::GetStretchBltMode(*this); }
	int SetStretchBltMode(int mode)
	{ ASSERT(IsValid()); return ::SetStretchBltMode(*this, mode); }
	BOOL StretchBlt(int x, int y, int cx, int cy, HDC hDC_s, int x_s, int y_s, int cx_s, int cy_s, DWORD rop)
	{ ASSERT(IsValid()); return ::StretchBlt(x, y, cx, cy, hDC_s, x_s, y_s, cx_s, cy_s, rop); }
	int StretchDIBits(int x, int y, int cx, int cy, int x_s, int y_s, int cx_s, int cy_s, LPCVOID lpBits, CONST BITMAPINFO* lpbmi, UINT iUsage, DWORD rop)
	{ ASSERT(IsValid()); return ::StretchDIBits(*this, x, y, cx, cy, x_s, y_s, cx_s, cy_s, lpBits, lpbmi, iUsage, rop); }
	BOOL TransparentBlt(int x, int y, int cx, int cy, HDC hDC_s, int x_s, int y_s, int cx_s, int cy_s, UINT crTransparent)
	{ ASSERT(IsValid()); return ::TransparentBlt(*this, x, y, cx, cy, hDC_s, x_s, y_s, cx_s, cy_s, crTransparent); }

public:
	BOOL FillRgn(HRGN hRgn, HBRUSH hbr)
	{ ASSERT(IsValid()); return ::FillRgn(*this, hRgn, hbr); }
	BOOL FrameRgn(HRGN hRgn, HBRUSH hbr, int w, int h)
	{ ASSERT(IsValid()); return ::FrameRgn(*this, hRgn, hbr, w, h); }
	BOOL InvertRgn(HRGN hRgn)
	{ ASSERT(IsValid()); return ::InvertRgn(*this, hRgn); }
	BOOL PaintRgn(HRGN hRgn)
	{ ASSERT(IsValid()); return ::PaintRgn(*this, hRgn); }
	int SetPolyFillMode(int iMode)
	{ ASSERT(IsValid()); return ::SetPolyFillMode(*this, iMode); }

public:
	COLORREF GetNearestColor(COLORREF crColor)
	{ ASSERT(IsValid()); return ::GetNearestColor(*this, crColor); }
	UINT GetSystemPaletteEntries(UINT iStartIndex, UINT nEntries, LPPALETTEENTRY lppe)
	{ ASSERT(IsValid()); return ::GetSystemPaletteEntries(*this, iStartIndex, nEntries, lppe); }
	UINT GetSystemPaletteUse()
	{ ASSERT(IsValid()); return ::GetSystemPaletteUse(*this); }
	UINT SetSystemPaletteUse(UINT uUsage)
	{ ASSERT(IsValid()); return ::SetSystemPaletteUse(*this, uUsage); }
	UINT RealizePalette()
	{ ASSERT(IsValid()); return ::RealizePalette(*this); }
	HPALETTE SelectPalette(HPALETTE hpal, BOOL bForceBkgd)
	{ ASSERT(IsValid()); return ::SelectPalette(*this, hpal, bForceBkgd); }
	BOOL SetColorAdjustment(CONST COLORADJUSTMENT* lpca)
	{ ASSERT(IsValid()); return ::SetColorAdjustment(*this, lpca); }
	BOOL UpdateColors()
	{ ASSERT(IsValid()); return ::UpdateColors(*this); }

public:
	HBITMAP CreateCompatibleBitmap(int nWidth, int nHeight)
	{ ASSERT(IsValid()); return ::CreateCompatibleBitmap(*this, nWidth, nHeight); }
	HBITMAP CreateDIBitmap(CONST BITMAPINFOHEADER* pbmih, DWORD fdwInit, LPCVOID pbInit, CONST BITMAPINFO* pbmi, UINT iUsage)
	{ ASSERT(IsValid()); return ::CreateDIBitmap(*this, pbmih, fdwInit, pbInit, pbmi, iUsage); }
	HBITMAP CreateDIBSection(CONST BITMAPINFO* pbmi, UINT iUsage, LPVOID* ppvBits, HANDLE hSection = NULL, DWORD dwOffset = 0)
	{ ASSERT(IsValid()); return ::CreateDIBSection(*this, pbmi, iUsage, ppvBits, hSection, dwOffset); }
};

typedef CDCT<true> CDC;
typedef CDCT<false> CDCHandle;

class CMemDC : public CDC
{
public:
	explicit CMemDC()
	{ }
	~CMemDC()
	{ Close(); }

protected:
	HGDIOBJ m_hOld;
	CBitmap m_bmp;
	
protected:
	virtual BOOL Close()
	{
		if(m_hOld)
		{
			SelectObject(m_hOld);
			m_hOld = NULL;
		}
		return __super::Close();
	}
public:
	virtual BOOL CreateCompatibleDC(HDC hDC, int nWidth, int nHeight)
	{
		if(	__super::CreateCompatibleDC(hDC) &&
			m_bmp.CreateCompatibleBitmap(hDC, nWidth, nHeight) )
		{
			m_hOld = (HBITMAP)SelectObject(m_bmp);
			return TRUE;
		}
		return FALSE;
	}
};

class CPaintDC : public CDCHandle
{
public:
	explicit CPaintDC(HWND hWnd)
		: m_hWnd(hWnd)
		, CDCHandle(BeginPaint(hWnd, &m_ps))
	{ }
	explicit CPaintDC(CWnd* pWnd)
	{
		m_hWnd = pWnd->GetSafeHwnd();
		_hDC = BeginPaint(m_hWnd, &m_ps);
	}
	virtual ~CPaintDC()
	{ EndPaint(m_hWnd, &m_ps); }

protected:
	HWND m_hWnd;
	PAINTSTRUCT m_ps;
};


_MZH_END
#endif