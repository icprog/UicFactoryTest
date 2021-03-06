
// FactoryScriptEngineView.h : interface of the CFactoryScriptEngineView class
//

#pragma once

#include "CliProtocol.h"

class CFactoryScriptEngineView : public CFormView
{
protected: // create from serialization only
	CFactoryScriptEngineView();
	DECLARE_DYNCREATE(CFactoryScriptEngineView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_FACTORYSCRIPTENGINE_FORM };
#endif

// Attributes
public:
	CFactoryScriptEngineDoc* GetDocument() const;

protected:
  BOOL          m_fConnected;
  BOOL          m_fInitialized;
  BOOL          m_fScriptRunning;
  int           m_iCommPort;
  CCliProtocol  m_cliComm;
  int           m_iStep;
  CString       m_strDescription;
  CString       m_strResults;
  CListCtrl     m_listResults;
  CScriptEntry* m_pscriptCurrent;
  POSITION      m_posScriptList;

// Operations
public:

protected:
  void  ScriptExecute(void);
  void  AddEntryToList(BOOL fPassFail);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CFactoryScriptEngineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
  afx_msg LRESULT OnDeviceMessage(WPARAM wParam, LPARAM lParam);
  afx_msg void OnTargetConnect();
  afx_msg void OnTargetDisconnect();
  afx_msg void OnTargetSettings();
  afx_msg void OnBnClickedBtnPass();
  afx_msg void OnBnClickedBtnFail();
  afx_msg void OnScriptExecute();
  afx_msg void OnUpdateTargetConnect(CCmdUI *pCmdUI);
  afx_msg void OnUpdateTargetDisconnect(CCmdUI *pCmdUI);
  afx_msg void OnUpdateTargetSettings(CCmdUI *pCmdUI);
  afx_msg void OnUpdateScriptExecute(CCmdUI *pCmdUI);
public:
  CString m_strAction;
};

#ifndef _DEBUG  // debug version in FactoryScriptEngineView.cpp
inline CFactoryScriptEngineDoc* CFactoryScriptEngineView::GetDocument() const
   { return reinterpret_cast<CFactoryScriptEngineDoc*>(m_pDocument); }
#endif

