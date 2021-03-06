
// FactoryScriptEngineDoc.cpp : implementation of the CFactoryScriptEngineDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "FactoryScriptEngine.h"
#endif

#include "FactoryScriptEngineDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFactoryScriptEngineDoc

IMPLEMENT_DYNCREATE(CFactoryScriptEngineDoc, CDocument)

BEGIN_MESSAGE_MAP(CFactoryScriptEngineDoc, CDocument)
END_MESSAGE_MAP()


// CFactoryScriptEngineDoc construction/destruction

CFactoryScriptEngineDoc::CFactoryScriptEngineDoc()
{
	// TODO: add one-time construction code here

}

CFactoryScriptEngineDoc::~CFactoryScriptEngineDoc()
{
  // delete all entries
  RemoveAllEntries();
}


// CFactoryScriptEngineDoc serialization

void CFactoryScriptEngineDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CFactoryScriptEngineDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CFactoryScriptEngineDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CFactoryScriptEngineDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CFactoryScriptEngineDoc diagnostics

#ifdef _DEBUG
void CFactoryScriptEngineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFactoryScriptEngineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFactoryScriptEngineDoc commands
BOOL CFactoryScriptEngineDoc::IsScriptLoaded(void)
{
  return(m_listScript.GetCount() != 0);
}


BOOL CFactoryScriptEngineDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
  CString       strScript;
  CString       strArg;
  CStringArray  straryArgs;
  CScriptEntry* pscriptEntry;

  // remove all entries
  RemoveAllEntries();

  // now open file
  CStdioFile fileScript;
  if (fileScript.Open(lpszPathName, CFile::modeRead | CFile::typeText))
  {
    // now read a line and parse
    while (fileScript.ReadString(strScript) != NULL)
    {
      // clear the array
      straryArgs.RemoveAll();

      // tokenize the string
      int nTokenPos = 0;
      strArg = strScript.Tokenize(_T("\t"), nTokenPos);
      while (!strArg.IsEmpty())
      {
        // add it
        straryArgs.Add(strArg);

        // find the next token
        strArg = strScript.Tokenize(_T("\t"), nTokenPos);
      }

      // test for header
      strArg = straryArgs.GetAt(0);
      if (strArg.Compare(_T("Step")) != 0)
      {
        // now create a new script entry
        pscriptEntry = new CScriptEntry;
        pscriptEntry->m_strDescription = straryArgs.GetAt(1);
        pscriptEntry->m_strAction = straryArgs.GetAt(2);
        pscriptEntry->m_strCommand = straryArgs.GetAt(3);
        pscriptEntry->m_strResult = straryArgs.GetAt(4);
        strArg = straryArgs.GetAt(5);
        pscriptEntry->m_fAllowContinue = (strArg.Compare(_T("Yes")) == 0) ? true : false;
        strArg = straryArgs.GetAt(6);
        pscriptEntry->m_fAllowContinue = (strArg.Compare(_T("Yes")) == 0) ? true : false;
        pscriptEntry->m_iStorageLocation = atoi(straryArgs.GetAt(7));

        // add it to the script list
        m_listScript.AddTail(pscriptEntry);
      }
    }

    TRACE1("Number of Steps,%d\n", m_listScript.GetCount());
  }
  else
  {
    // report error on opening file
    AfxMessageBox("Error on opening script file!", MB_OK | MB_ICONERROR);
  }

  return TRUE;
}

void CFactoryScriptEngineDoc::RemoveAllEntries(void)
{
  POSITION posList;

  // get head position
  posList = m_listScript.GetHeadPosition();

  // now iterate and delete
  while (posList != NULL)
  {
    delete m_listScript.GetNext(posList);
  }
}