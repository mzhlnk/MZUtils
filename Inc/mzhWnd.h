#pragma once

#ifndef _MZH_WND
#define _MZH_WND

#include <atltypes.h>
#include <mzhBase.h>

_MZH_BEGIN

template <bool t_bManaged>
class CWndT
{
public:
	CWndT(HWND hWnd = NULL)
		: m_hWnd(hWnd)
	{ }
	virtual ~CWndT()
	{
		if(t_bManaged && m_hWnd)
			DestroyWindow();
	}

public:
	HWND m_hWnd;

public:
	operator HWND() const
	{ return m_hWnd; }
	HWND GetSafeHwnd(void) const
	{ return (this ? m_hWnd : NULL); }
	void Attach(HWND hWnd)
	{
		if(m_hWnd != hWnd)
		{
			if(t_bManaged && m_hWnd)
				DestroyWindow();
			m_hWnd = hWnd;
		}
	}
	HWND Dettach()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

public:
	virtual BOOL Create(LPCTSTR lpClassName,
						LPCTSTR lpszTitle,
						DWORD dwStyle,
						int x,
						int y,
						int nWidth,
						int nHeight,
						HWND hParent,
						HMENU hMenu,
						HINSTANCE hInstance,
						LPVOID lpParam = NULL)
	{
		HWND hWnd = CreateWindow(lpClassName, lpszTitle, dwStyle, x, y, nWidth, nHeight, hParent, hMenu, hInstance, lpParam);
		ASSERT(m_hWnd == NULL || m_hWnd == hWnd);
		VERIFY(m_hWnd = hWnd);
		return m_hWnd ? TRUE : FALSE;
	}
	HWND CreateEx(	DWORD dwExStyle,
					LPCTSTR lpClassName,
					LPCTSTR lpszTitle,
					DWORD dwStyle,
					int x,
					int y,
					int nWidth,
					int nHeight,
					HWND hParent,
					HMENU hMenu,
					HINSTANCE hInstance,
					LPVOID lpParam = NULL)
	{
		HWND hWnd = ::CreateWindowEx(dwExStyle, lpClassName, lpszTitle, dwStyle, x, y, nWidth, nHeight, hParent, hMenu, hInstance, lpParam);
		ASSERT(m_hWnd == NULL || m_hWnd == hWnd);
		VERIFY(m_hWnd = hWnd);
		return m_hWnd ? TRUE : FALSE;
	}

	virtual BOOL DestroyWindow()
	{ ASSERT(IsWindow()); return ::DestroyWindow(*this); }

public:
	LRESULT SendMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0)
	{ ASSERT(IsWindow()); return ::SendMessage(*this, nMsg, wParam, lParam); }
	LRESULT SendMessageTimeout(UINT Msg, WPARAM wParam, LPARAM lParam, UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult)
	{ ASSERT(IsWindow()); return ::SendMessageTimeout(*this, Msg, wParam, lParam, fuFlags, uTimeout, lpdwResult); }
	BOOL PostMessage(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0)
	{ ASSERT(IsWindow()); return ::PostMessage(*this, nMsg, wParam, lParam); }
	BOOL SendNotifyMessage(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0)
	{ ASSERT(IsWindow()); return ::SendNotifyMessage(*this, nMsg, wParam, lParam); }
	BOOL SendMessageCallback(UINT Msg, WPARAM wParam, LPARAM lParam, SENDASYNCPROC lpResultCallBack, ULONG_PTR dwData)
	{ ASSERT(IsWindow()); return ::SendMessageCallback(*this, Msg, wParam, lParam, lpResultCallBack, dwData); }

	BOOL SetWindowText(LPCTSTR lpszText)
	{ ASSERT(IsWindow()); return ::SetWindowText(*this, lpszText); }
	int GetWindowText(LPTSTR lpszText, int nMaxCount)
	{ ASSERT(IsWindow()); return ::GetWindowText(*this, lpszText, nMaxCount); }
	int GetWindowTextLength() const
	{ ASSERT(IsWindow()); return ::GetWindowTextLength(*this); }
	BOOL SetDlgItemInt(int nID, UINT nVal, BOOL bSigned = TRUE)
	{ ASSERT(IsWindow()); return ::SetDlgItemInt(*this, nID, nVal, bSigned); }
	BOOL SetDlgItemText(int nID, LPCTSTR lpszText)
	{ ASSERT(IsWindow()); return ::SetDlgItemText(*this, nID, lpszText); }

	virtual HWND SetFocus() const
	{ ASSERT(IsWindow()); return ::SetFocus(*this); }
	virtual HMENU GetMenu() const
	{ ASSERT(IsWindow()); return ::GetMenu(*this); }
	virtual BOOL SetMenu(HMENU hMenu)
	{ ASSERT(IsWindow()); return ::SetMenu(*this, hMenu); }
	virtual HMENU GetSystemMenu(BOOL bRevert) const
	{ ASSERT(IsWindow()); return ::GetSystemMenu(*this, bRevert); }

public:
	BOOL IsWindow() const
	{ return ::IsWindow(*this); }

	BOOL IsWindowEnabled() const
	{ ASSERT(IsWindow()); return ::IsWindowEnabled(*this); }
	BOOL EnableWindow(BOOL bEnable = TRUE)
	{ ASSERT(IsWindow()); return ::EnableWindow(*this, bEnable); }

	BOOL IsIconic() const
	{ ASSERT(IsWindow()); return ::IsIconic(*this); }
	BOOL IsZoomed() const
	{ ASSERT(IsWindow()); return ::IsZoomed(*this); }
	BOOL IsChild(HWND hWnd) const
	{ ASSERT(IsWindow()); return ::IsChild(*this, hWnd); }

	BOOL SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags)
	{ ASSERT(IsWindow()); return ::SetWindowPos(*this, hWndInsertAfter, x, y, cx, cy, uFlags); }
	BOOL MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE)
	{ ASSERT(IsWindow()); return ::MoveWindow(*this, x, y, nWidth, nHeight, bRepaint); }
	BOOL MoveWindow(CONST RECT& rc, BOOL bRepaint = TRUE)
	{ return MoveWindow(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, bRepaint); }
	BOOL GetWindowRect(RECT& rc) const
	{ ASSERT(IsWindow()); return ::GetWindowRect(*this, &rc); }
	BOOL GetClientRect(RECT& rc) const
	{ ASSERT(IsWindow()); return ::GetClientRect(*this, &rc); }

	int GetWindowRgn(HRGN hRgn) const
	{ ASSERT(IsWindow()); return ::GetWindowRgn(*this, hRgn); }
	int SetWindowRgn(HRGN hRgn, BOOL bRedraw = TRUE)
	{ ASSERT(IsWindow()); return ::SetWindowRgn(*this, hRgn, bRedraw); }
	int GetWindowRgnBox(RECT& rc)
	{ ASSERT(IsWindow()); return ::GetWindowRgnBox(*this, &rc); }

	BOOL GetWindowPlacement(WINDOWPLACEMENT& wndpl) const
	{ ASSERT(IsWindows() && lpwndpl); return ::GetWindowPlacement(*this, &wndpl); }
	BOOL SetWindowPlacement(CONST WINDOWPLACEMENT& wndpl)
	{ ASSERT(IsWndows() && lpwndpl); return ::SetWindowPlacement(*this, &wndpl); }

	BOOL GetWindowDisplayAffinity(DWORD& dwAffinity)
	{ ASSERT(IsWindow()); return ::GetWindowDisplayAffinity(*this, &dwAffinity); }
	BOOL SetWindowDisplayAffinity(DWORD dwAffinity)
	{ ASSERT(IsWindow()); return ::SetWindowDisplayAffinity(*this, dwAffinity); }

	BOOL ClientToScreen(POINT& point) const
	{ ASSERT(IsWindow()); return ::ClientToScreen(*this, &point); }
	BOOL ClientToScreen(RECT& rc) const
	{
		ASSERT(IsWindow());
		if( ::ClientToScreen(*this, (LPPOINT)&rc) && ::ClientToScreen(*this, ((LPPOINT)&rc) + 1) )
		{
			if(GetExStyle() & WS_EX_LAYOUTRTL)
				CRect::SwapLeftRight(&rc);
		}
		return FALSE;
	}
	BOOL ScreenToClient(POINT& point) const
	{ ASSERT(IsWindow()); return ::ScreenToClient(*this, &point); }

	DWORD GetStyle() const
	{ ASSERT(IsWindow()); return ::GetWindowLong(*this, GWL_STYLE); }
	DWORD GetExStyle() const
	{ ASSERT(IsWindow()); return ::GetWindowLong(*this, GWL_EXSTYLE); }

	HDC GetDC() const
	{ ASSERT(IsWindow()); return ::GetDC(*this); }
	HDC GetDCEx(HRGN hrgnClip, DWORD flags) const
	{ ASSERT(IsWindow()); return ::GetDCEx(*this, hrgnClip, flags); }
	int ReleaseDC(HDC hDC) const
	{ ASSERT(IsWindow()); return ::ReleaseDC(*this, hDC); }

	HDC BeginPaint(PAINTSTRUCT& ps)
	{ ASSERT(IsWindow()); return ::BeginPaint(*this, &ps); }
	BOOL EndPaint(CONST PAINTSTRUCT& ps)
	{ ASSERT(IsWindow()); return ::EndPaint(*this, &ps); }

	BOOL LockWindowUpdate()
	{ ASSERT(IsWindow()); return ::LockWindowUpdate(*this); }

	BOOL ShowWindow(int nCmdShow)
	{ ASSERT(IsWindow()); return ::ShowWindow(*this, nCmdShow); }
	BOOL ShowWindowAsync(int nCmdShow)
	{ ASSERT(IsWindow()); return ::ShowWindowAsync(*this, nCmdShow); }

	BOOL ShowOwnedPopups(BOOL fShow = TRUE)
	{ ASSERT(IsWindow()); return ::ShowOwnedPopups(*this, fShow); }
	BOOL OpenIcon()
	{ ASSERT(IsWindow()); return ::OpenIcon(*this); }
	BOOL CloseWindow()
	{ ASSERT(IsWindow()); return ::CloseWindow(*this); }

	BOOL UpdateWindow()
	{ ASSERT(IsWindow()); return ::UpdateWindow(*this); }
	BOOL FlashWindow(BOOL bInvert = TRUE)
	{ ASSERT(IsWindow()); return ::FlashWindow(*this, bInvert); }
	BOOL FlashWindowEx(DWORD dwFlags, UINT uCount,  DWORD dwTimeout)
	{
		ASSERT(IsWindow());
		FLASHWINFO fw = { sizeof(fw), *this, dwFlags, uCount, dwTimeout };
		return ::FlashWindowEx(*this, &fw);
	}
	BOOL AnimateWindow(DWORD dwTime, DWORD dwFlags)
	{ ASSERT(IsWindow()); return ::AnimateWindow(*this, dwTime, dwFlags); }
	BOOL DrawAnimatedRects(int idAni, CONST RECT& rcFrom, CONST RECT& rcTo)
	{ ASSERT(IsWindow()); return ::DrawAnimatedRects(*this, idAni, &rcFrom, &rcTo); }

	BOOL IsWindowVisible() const
	{ ASSERT(IsWindow()); return ::IsWindowVisible(*this); }
	BOOL Invalidate(BOOL bErase = TRUE)
	{ ASSERT(IsWindow()); return ::InvalidateRect(*this, NULL, bErase); }
	BOOL InvalidateRect(LPCRECT lpRect = NULL, BOOL bErase = TRUE)
	{ ASSERT(IsWindow()); return ::InvalidateRect(*this, lpRect, bErase); }
	BOOL InvalidateRgn(HRGN hRgn = NULL, BOOL bErase = TRUE)
	{ ASSERT(IsWindow()); return ::InvalidateRgn(*this, hRgn, bErase); }

	BOOL ValidateRect(CONST RECT& rc)
	{ ASSERT(IsWindow()); return ::ValidateRect(*this, &rc); }
	BOOL ValidateRgn(HRGN hRgn)
	{ ASSERT(IsWindow()); return ::ValidateRgn(*this, hRgn); }

	BOOL GetUpdateRect(RECT& rc, BOOL bErase)
	{ ASSERT(IsWindow()); return ::GetUpdateRect(*this, &rc, bErase); }
	int GetUpdateRgn(HRGN hRgn, BOOL bErase)
	{ ASSERT(IsWindow()); return ::GetUpdateRgn(*this, hRgn, bErase); }

	HDC GetWindowDC()
	{ ASSERT(IsWindow()); return ::GetWindowDC(*this); }
	BOOL RedrawWindow(CONST RECT& rcUpdate, HRGN hrgnUpdate, UINT flags)
	{ ASSERT(IsWindow()); return ::RedrawWindow(*this, &rcUpdate, hrgnUpdate, flags); }

public:
	// Timer Functions
	UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse, TIMERPROC lpfnTimer = NULL)
	{ ASSERT(IsWindow()); return ::SetTimer(*this, nIDEvent, nElapse, lpfnTimer); }
	BOOL KillTimer(UINT_PTR nIDEvent)
	{ ASSERT(IsWindow()); return ::KillTimer(*this, nIDEvent); }

	//static HWND GetActiveWindow()		{ return ::GetActiveWindow(); }
	HWND SetActiveWindow()
	{ ASSERT(IsWindow()); return ::SetActiveWindow(*this); }

	//static HWND GetForegroundWindow()	{ return ::GetForegroundWindow(); }
	BOOL SetForegroundWindow()
	{ ASSERT(IsWindow()); return ::SetForegroundWindow(*this); }

	//static HWND GetCapture()		{ return ::GetCapture(); }
	HWND SetCapture()
	{ ASSERT(IsWindow()); return ::SetCapture(*this); }

public:
	BOOL RegisterHotKey(int id, UINT fsModifiers, UINT vk)
	{ ASSERT(IsWindow()); return ::RegisterHotKey(*this, id, fsModifiers, vk); }
	BOOL UnregisterHotKey(int id)
	{ ASSERT(IsWindow()); return ::UnregisterHotKey(*this, id); }

public:
	BOOL GetLayeredWindowAttributes(LPCOLORREF pcrKey, PBYTE pbAlpha, PDWORD pdwFlags)
	{ ASSERT(IsWindow()); return ::GetLayeredWindowAttributes(*this, pcrKey, pbAlpha, pdwFlags); }
	BOOL SetLayeredWindowAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags = LWA_COLORKEY | LWA_ALPHA)
	{ ASSERT(IsWindow()); return ::SetLayeredWindowAttributes(*this, crKey, bAlpha, dwFlags); }
	BOOL UpdateLayeredWindow(HDC hdcDst, PPOINT pptDst, PSIZE psize, HDC hdcSrc, PPOINT pptSrc, COLORREF crKey, PBLENDFUNCTION pblend, DWORD dwFlags)
	{ ASSERT(IsWindow()); return ::UpdateLayeredWindow(*this, hdcDst, pptDst, psize, hdcSrc, pptSrc, crKey, pblend, dwFlags); }
	BOOL UpdateLayeredWindowIndirect(CONST UPDATELAYEREDWINDOWINFO* pULWInfo)
	{ ASSERT(IsWindow()); return ::UpdateLayeredWindowIndirect(*this, pULWInfo); }

public:
	BOOL PrintWindow(HDC hdcBlt, UINT nFlags)
	{ ASSERT(IsWindow()); return ::PrintWindow(*this, hdcBlt, nFlags); }
	BOOL DrawCaption(HDC hdc, CONST RECT& rc, UINT uFlags)
	{ ASSERT(IsWindow()); return ::DrawCaption(*this, hdc, &rc, uFlags); }

public:	
	BOOL IsDialogMessage(LPMSG lpMsg)
	{ ASSERT(IsWindow()); return ::IsDialogMessage(*this, lpMsg); }
	LRESULT SendDlgItemMessage(int nID, UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0)
	{ ASSERT(IsWindow()); return ::SendDlgItemMessage(*this, nID, nMsg, wParam, lParam); }
	BOOL EndDialog(INT_PTR nResult)
	{ ASSERT(IsWindow()); return ::EndDialog(*this, nResult); }

	int GetDlgCtrlID() const
	{ ASSERT(IsWindow()); return ::GetDlgCtrlID(*this); }
	HWND GetDlgItem(int nID) const
	{ ASSERT(IsWindow()); return ::GetDlgItem(*this, nID); }

public:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = 0L;
		if(OnWndMsg(uMsg, wParam, lParam, lResult))
			return lResult;
		return DefWndProc(uMsg, wParam, lParam);
	}
	virtual LRESULT DefWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{ return ::DefWindowProc(*this, uMsg, wParam, lParam); }
	virtual BOOL OnWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		switch (uMsg)
		{
		case WM_PAINT:
			OnPaint();
			return TRUE;
		case WM_COMMAND:
			return OnCommand(wParam, lParam, lResult);
		case WM_NOTIFY:
			return OnNotify(wParam, lParam, lResult);
		case WM_TIMER:
			OnTimer((UINT_PTR)wParam);
			return TRUE;
		case WM_CREATE:
			lResult = OnCreate((LPCREATESTRUCT)lParam);
			return TRUE;
		case WM_DESTROY:
			OnDestroy();
			//lResult = 0L;
			return TRUE;
		case WM_NCCREATE:
			lResult = OnNcCreate((LPCREATESTRUCT)lParam);
			return TRUE;
		case WM_NCDESTROY:
			OnNcDestroy();
			//lResult = 0L;
			return TRUE;
		default:
			break;
		}
		return FALSE;
	}
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{ return FALSE; }
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{ return FALSE; }
	virtual void OnPaint() { }
	virtual void OnTimer(UINT_PTR nIDEvent)	{ }
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct)	{ return DefWndProc(WM_CREATE, 0, (LPARAM)lpCreateStruct); }
	virtual void OnDestroy() { }
	virtual BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct)	{ return DefWndProc(WM_NCCREATE, 0, (LPARAM)lpCreateStruct); }
	virtual void OnNcDestroy()	{ m_hWnd = NULL; }
};

typedef CWndT<true> CWnd;
typedef CWndT<false> CWndHandle;

template<bool t_bAutoDelete>
class CAppWndT : public CWnd
{
public:
	CAppWndT(HWND hWnd = NULL)
		: CWnd(hWnd)
	{ }
	virtual ~CAppWndT()
	{
		if(m_hWnd)
			DestroyWindow();
		ASSERT(!IsWindow());
	}

public:
	virtual void OnNcDestroy()
	{
		__super::OnNcDestroy();
		::PostQuitMessage(0);
		if(t_bAutoDelete)
			delete this;
	}
};

typedef CAppWndT<false> CAppWnd;

_MZH_END

#endif
