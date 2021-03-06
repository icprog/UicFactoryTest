
// FactoryScriptEngineView.cpp : implementation of the CFactoryScriptEngineView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "FactoryScriptEngine.h"
#endif

#include "FactoryScriptEngineDoc.h"
#include "FactoryScriptEngineView.h"

#include "MainFrm.h"
#include "CommPortDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFactoryScriptEngineView

IMPLEMENT_DYNCREATE(CFactoryScriptEngineView, CFormView)

BEGIN_MESSAGE_MAP(CFactoryScriptEngineView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
  ON_MESSAGE(IDU_DEVICE_MESSAGE, OnDeviceMessage)
  ON_COMMAND(ID_TARGET_CONNECT, &CFactoryScriptEngineView::OnTargetConnect)
  ON_COMMAND(ID_TARGET_DISCONNECT, &CFactoryScriptEngineView::OnTargetDisconnect)
  ON_COMMAND(ID_TARGET_SETTINGS, &CFactoryScriptEngineView::OnTargetSettings)
  ON_COMMAND(ID_SCRIPT_EXECUTE, &CFactoryScriptEngineView::OnScriptExecute)
  ON_BN_CLICKED(IDC_BTN_PASS, &CFactoryScriptEngineView::OnBnClickedBtnPass)
  ON_BN_CLICKED(IDC_BTN_FAIL, &CFactoryScriptEngineView::OnBnClickedBtnFail)
  ON_UPDATE_COMMAND_UI(ID_TARGET_CONNECT, &CFactoryScriptEngineView::OnUpdateTargetConnect)
  ON_UPDATE_COMMAND_UI(ID_TARGET_DISCONNECT, &CFactoryScriptEngineView::OnUpdateTargetDisconnect)
  ON_UPDATE_COMMAND_UI(ID_TARGET_SETTINGS, &CFactoryScriptEngineView::OnUpdateTargetSettings)
  ON_UPDATE_COMMAND_UI(ID_SCRIPT_EXECUTE, &CFactoryScriptEngineView::OnUpdateScriptExecute)
END_MESSAGE_MAP()

// CFactoryScriptEngineView construction/destruction

CFactoryScriptEngineView::CFactoryScriptEngineView()
	: CFormView(IDD_FACTORYSCRIPTENGINE_FORM)
  , m_fConnected(false)
  , m_fInitialized(false)
  , m_iStep(0)
  , m_strDescription(_T(""))
  , m_strResults(_T(""))
  , m_strAction(_T(""))
{
	// TODO: add construction code here

}

CFactoryScriptEngineView::~CFactoryScriptEngineView()
{
  // if still connected, close it
  if (m_fConnected)
  {
    // close the port
    m_cliComm.Close();
  }

  // save the comm port
  CWinApp* pApp = AfxGetApp();
  pApp->WriteProfileInt(_T("Settings"), _T("Comm Port"), m_iCommPort);
}

void CFactoryScriptEngineView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT_STEP, m_iStep);
  DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
  DDX_Text(pDX, IDC_EDIT_RESULTS, m_strResults);
  DDX_Control(pDX, IDC_LIST_RESULTS, m_listResults);
  DDX_Text(pDX, IDC_EDIT_ACTION, m_strAction);
}

BOOL CFactoryScriptEngineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CFactoryScriptEngineView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

  // check for initialization
  if (!m_fInitialized)
  {
    // get the comm port
    CWinApp* pApp = AfxGetApp();
    m_iCommPort = pApp->GetProfileInt(_T("Settings"), _T("Comm Port"), 1);

    CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
    pFrame->UpdateCommPort(m_iCommPort);

    // update connection status/update buttons
    pFrame->UpdateConnectionStatus(false);

    // set up the list control
    m_listResults.InsertColumn(0, "Step", LVCFMT_LEFT, 75);
    m_listResults.InsertColumn(1, "Description", LVCFMT_LEFT, 700);
    m_listResults.InsertColumn(2, "Results", LVCFMT_LEFT, 700);
    m_listResults.InsertColumn(3, "Pass/Fail", LVCFMT_LEFT, 150);

    // set initialized
    m_fInitialized = true;
  }
}


// CFactoryScriptEngineView printing

BOOL CFactoryScriptEngineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFactoryScriptEngineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFactoryScriptEngineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CFactoryScriptEngineView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}


// CFactoryScriptEngineView diagnostics

#ifdef _DEBUG
void CFactoryScriptEngineView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFactoryScriptEngineView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFactoryScriptEngineDoc* CFactoryScriptEngineView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFactoryScriptEngineDoc)));
	return (CFactoryScriptEngineDoc*)m_pDocument;
}
#endif //_DEBUG


// CFactoryScriptEngineView message handlers
//////////////////////////////////////////////////////////////////////
// CFactoryScriptEngineView device message handler
//////////////////////////////////////////////////////////////////////
LRESULT CFactoryScriptEngineView::OnDeviceMessage(WPARAM wParam, LPARAM lParam)
{
  // get the message
  CMsgBuffer	mbufLcl(false);

  // process the message
  while (m_cliComm.GetUnsMsg(&mbufLcl))
  {
  }

  // return ok
  return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
// CFactoryScriptEngineView message handlers
////////////////////////////////////////////////////////////////////////////////////////

void CFactoryScriptEngineView::OnTargetConnect()
{
  // now open the port
  if (!(m_fConnected = m_cliComm.Open(m_iCommPort, 115200, NOPARITY, 8, ONESTOPBIT, m_hWnd, IDU_DEVICE_MESSAGE)))
  {
    // report error
    AfxMessageBox("Unable to open comm port!", MB_ICONERROR | MB_OK);
  }

  // update connection status
  CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
  pFrame->UpdateConnectionStatus(m_fConnected);
}


void CFactoryScriptEngineView::OnTargetDisconnect()
{
  // close the port
  m_fConnected = m_cliComm.Close();
}


void CFactoryScriptEngineView::OnTargetSettings()
{
  // create the dialog, set the current port
  CCommPortDlg	dlgComm(false);
  dlgComm.m_iCommPort = m_iCommPort;

  // process the dialog
  if (dlgComm.DoIt() == IDOK)
  {
    // copy the comm port
    m_iCommPort = dlgComm.m_iCommPort;

    // update the status bar
    CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
    pFrame->UpdateCommPort(m_iCommPort);
  }
}

void CFactoryScriptEngineView::OnBnClickedBtnPass()
{
  // add entry with a pass
  AddEntryToList(true);
}


void CFactoryScriptEngineView::OnBnClickedBtnFail()
{
  // add entry with a fail
  AddEntryToList(false);
}


void CFactoryScriptEngineView::OnScriptExecute()
{
  // set the step index
  m_iStep = 1;

  // get the head position
  m_posScriptList = GetDocument()->m_listScript.GetHeadPosition();

  // execute it
  ScriptExecute();
}


////////////////////////////////////////////////////////////////////////////////////////
// CFactoryScriptEngineView update message handlers
////////////////////////////////////////////////////////////////////////////////////////

void CFactoryScriptEngineView::OnUpdateTargetConnect(CCmdUI *pCmdUI)
{
  // enable based on connection state
  pCmdUI->Enable(!m_fConnected);
}


void CFactoryScriptEngineView::OnUpdateTargetDisconnect(CCmdUI *pCmdUI)
{
  // enable based on connection state
  pCmdUI->Enable(m_fConnected);
}


void CFactoryScriptEngineView::OnUpdateTargetSettings(CCmdUI *pCmdUI)
{
  // enable based on connection state
  pCmdUI->Enable(!m_fConnected);
}

void CFactoryScriptEngineView::OnUpdateScriptExecute(CCmdUI *pCmdUI)
{
  // enable if connected and script is loaded
  // pCmdUI->Enable(!m_fConnected & GetDocument()->IsScriptLoaded());
  pCmdUI->Enable(GetDocument()->IsScriptLoaded());
}

void CFactoryScriptEngineView::ScriptExecute(void)
{
  CFactoryScriptEngineDoc* pDoc = GetDocument();

  // clear any stored values
  pDoc->m_straryRecords.RemoveAll();

  // get the current script entry
  m_pscriptCurrent = pDoc->m_listScript.GetNext(m_posScriptList);

  // check for done
  if (m_pscriptCurrent != NULL)
  {
    // populate the form
    m_strDescription = m_pscriptCurrent->m_strDescription;
    m_strAction = m_pscriptCurrent->m_strAction;
    m_strResults = m_pscriptCurrent->m_strResult;

    // check for a command
    if (m_pscriptCurrent->m_strCommand.Compare("NA") != 0)
    {
      // fill the message
      CMsgBuffer  lclMsg;
      lclMsg.PutMsgString(m_pscriptCurrent->m_strCommand);

      // set the timeout/send message
      int iTimeout = (m_pscriptCurrent->m_fResponseRequired) ? 100 : 0;
      m_cliComm.SendMessage(&lclMsg, iTimeout);

      // now test response if timeout is not 0
      if ((iTimeout != 0) && (lclMsg.GetMsgStatus() == MSG_DATA_VALID))
      {
        // good response
        m_strResults = lclMsg.GetMsgString(0);
        AddEntryToList(true);

        // check for storage
        if (m_pscriptCurrent->m_iStorageLocation != 0)
        {
          // store it
          pDoc->m_straryRecords.SetAtGrow(m_pscriptCurrent->m_iStorageLocation, m_strResults);
        }
      }
      else
      {
        // automatically fail test
        AddEntryToList(false);
      }
    }

    // update data
    UpdateData(false);
  }
  else
  {
    AfxMessageBox("Test is done!", MB_OK | MB_ICONINFORMATION);
  }
}

void CFactoryScriptEngineView::AddEntryToList(BOOL fPassFail)
{
  int     iListCount;
  CString strTemp;

  // get the count/add item to list
  iListCount = m_listResults.GetItemCount();
  strTemp.Format("%3d", m_iStep);
  m_listResults.InsertItem(iListCount, strTemp);
  m_listResults.SetItemText(iListCount, 1, m_pscriptCurrent->m_strDescription);
  m_listResults.SetItemText(iListCount, 2, m_pscriptCurrent->m_strResult);
  m_listResults.SetItemText(iListCount, 3, (fPassFail) ? _T("PASS") : _T("FAIL"));
  m_listResults.EnsureVisible(iListCount, false);

  // now determine next step
  if ((fPassFail) || (!fPassFail && m_pscriptCurrent->m_fAllowContinue))
  {
    // increment the step
    m_iStep++;
    ScriptExecute();
  }
}