// STLViewerDoc.cpp : implementation of the CSTLViewerDoc class
//

#include "stdafx.h"
#include "STLViewer.h"

#include "STLViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc

IMPLEMENT_DYNCREATE(CSTLViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSTLViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CSTLViewerDoc)
	ON_COMMAND(ID_STL_FILEIN, OnStlFilein)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc construction/destruction

CSTLViewerDoc::CSTLViewerDoc()
{
	// TODO: add one-time construction code here
}

CSTLViewerDoc::~CSTLViewerDoc()
{
}

BOOL CSTLViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_Part.RemoveAllEntity();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc serialization

void CSTLViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}

	m_Part.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc diagnostics

#ifdef _DEBUG
void CSTLViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSTLViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc commands

void CSTLViewerDoc::OnStlFilein() 
{
		CFileDialog dlg(TRUE,_T("stl"),NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Stereo Lithograpic File(*.stl)|*.stl"), NULL );

	if(dlg.DoModal()==IDOK){

		CSTLModel* pSTLModel = new CSTLModel();

		CString strName = dlg.GetPathName();
		pSTLModel->LoadSTLFile(strName);

		if(pSTLModel->IsEmpty())
			delete pSTLModel;
		else
			m_Part.AddEntity(pSTLModel);

		UpdateAllViews(NULL);
	}
}
