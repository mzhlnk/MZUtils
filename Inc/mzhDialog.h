#pragma once

#ifndef _MZH_DIALOG
#define _MZH_DIALOG

#include <mzhWnd.h>
#include <mzhPaintDC.h>

_MZH_BEGIN

class CDialog : public CWnd
{
public:
	CDialog(void)
	{ }
	virtual ~CDialog(void)
	{ }

public:
	void EndDialog(int nResult)
	{ ::EndDialog(m_hWnd, nResult); }

protected:
	virtual void OnOK()
	{ VERIFY(UpdateData()); ::EndDialog(m_hWnd, IDOK); }
	virtual void OnCancel()
	{ ::EndDialog(m_hWnd, IDCANCEL); }
};

_MZH_END
#endif

