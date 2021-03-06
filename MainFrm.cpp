
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FactoryScriptEngine.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_PORT, OnUpdatePort)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_CONNSTS, OnUpdateConnectionStatus)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CONNSTS,
  ID_INDICATOR_PORT,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::UpdateCommPort(int iPort)
{
  CString strPort;
  strPort.Format(_T("Comm #%2d"), iPort);

  // update it
  int iIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_PORT);
  m_wndStatusBar.SetPaneText(iIndex, (LPCTSTR)strPort);
}

void CMainFrame::UpdateConnectionStatus(BOOL fState)
{
  CString strStatus;
  strStatus = (fState) ? "Connected" : "";

  // update it
  int iIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_CONNSTS);
  m_wndStatusBar.SetPaneText(iIndex, (LPCTSTR)strStatus);
}

// CMainFrame message handlers
void CMainFrame::OnUpdatePort(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
}

void CMainFrame::OnUpdateConnectionStatus(CCmdUI *pCmdUI)
{
  pCmdUI->Enable();
}
