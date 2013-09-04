#pragma once
#include "x86Analysis.h"

extern std::shared_ptr<debug_kernel> debug_kernel_ptr;
// CAsmView

class CAsmWnd : public CWnd
{
	DECLARE_DYNAMIC(CAsmWnd)

public:
	CAsmWnd();
	virtual ~CAsmWnd();

	BOOL Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetEIP(DWORD eip);
	void SetTopAddr(DWORD addr)
	{
		m_AddrToShow = addr;
		UpdateScrollInfo();
	}
	DWORD GetSelAddrStart()
	{
		return m_dwSelAddrStart;
	}

private:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	void PreviousCode(DWORD dwTargetAddr,DWORD* pdwPreInsn);
	void UpdateScrollInfo();

	std::vector<DWORD> m_vecAddress;
	std::vector<std::string> m_vecAsm;
	CFont m_Font;
	int m_nLineHight;
	int m_nFontWidth;
	int m_nMargenWidth;

	DWORD m_AddrToShow;
	DWORD m_Eip;
	x86dis m_Decoder;
	DWORD m_dwSelAddrStart;
	DWORD m_dwSelAddrEnd;
	BOOL m_bLButtonDown;
	std::string m_strSelWord;

	BOOL ExtTextOutWithSelection(CDC& dc, int x, int y, LPCTSTR lpszString, UINT nCount);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL SetPaintFont(const LOGFONT& font)
	{
		m_Font.Detach();

		if (! m_Font.CreateFontIndirect(&font))
		{
			return FALSE;
		}

		CDC* pDC = GetDC();
		CFont* pFont = pDC->SelectObject(&m_Font);
		pFont->DeleteObject();
		TEXTMETRIC	text_metrit = {0};
		if (!pDC->GetTextMetrics(&text_metrit))
		{
			return FALSE;
		}
		;
		// 默认行高
		m_nLineHight = text_metrit.tmHeight + text_metrit.tmExternalLeading + 5 ;
		// 默认字体宽度
		m_nFontWidth = text_metrit.tmAveCharWidth;
		ReleaseDC(pDC);

		return TRUE;
	}
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


