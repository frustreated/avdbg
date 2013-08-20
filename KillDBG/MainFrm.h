﻿// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "ChildView.h"
#include "OutputWindow.h"
#include "AsmView.h"
#include "ModuleList.h"

#define		WM_USER_DEBUGSTOP	WM_USER+1

class debug_kernel;

class CMainFrame : public CXTPFrameWnd
{

public:
	CMainFrame();
	virtual ~CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	CXTPOfficeBorder<CMFCPropertyGridCtrl> m_wndPEStruct;
	CXTPOfficeBorder<COutputWindow> m_wndOutputWnd;
	CXTPOfficeBorder<CAsmView> m_wndAsmView;
	CXTPOfficeBorder<CModuleList> m_wndModuleList;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;
	CChildView    m_wndView;
	CXTPDockingPaneManager m_paneManager;

	HACCEL m_hAcc;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtondiswnd();
	afx_msg void OnViewRegister();
	afx_msg void OnViewMemory();
	afx_msg void OnViewStack();
	afx_msg void OnViewOutput();
	afx_msg void OnViewPEStruct();
	afx_msg void OnViewMemoryMap();

	
	afx_msg void OnStepIn();
	afx_msg void OnStepOver();
	afx_msg void OnDebugRun();
	afx_msg void OnFollowAddr();
	afx_msg void OnSetBreakPoint();

	
	//	afx_msg void OnDestroy();
	afx_msg void OnFileOpen();
	// 创建可停靠窗口
	BOOL SetupDockPane(void);
	afx_msg void OnFileAttach();
	afx_msg LRESULT OnDebugStop(WPARAM wParam, LPARAM lParam);
	// 调试模块
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileAttach(CCmdUI *pCmdUI);
	afx_msg void OnFileStop();
	afx_msg void OnUpdateFileStop(CCmdUI *pCmdUI);

private:
	std::shared_ptr<debug_kernel> debug_kernel_ptr_;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};