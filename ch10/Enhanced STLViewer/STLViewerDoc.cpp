// STLViewerDoc.cpp : implementation of the CSTLViewerDoc class
//

#include "stdafx.h"
#include "STLViewer.h"

#include "STLViewerDoc.h"

#include "GeomKernel\Entity.h"

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
	ON_COMMAND(ID_DEF_FILEIN, OnDefFilein)
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

	//CPoint3D pts[4*4];
	//pts[0] = CPoint3D(0,0,0);
	//pts[1] = CPoint3D(20,20,0);
	//pts[2] = CPoint3D(40,20,0);
	//pts[3] = CPoint3D(80,0,0);

	//pts[4] = CPoint3D(0,0,20);
	//pts[5] = CPoint3D(20,20,20);
	//pts[6] = CPoint3D(40,20,20);
	//pts[7] = CPoint3D(80,0,20);

	//pts[8] = CPoint3D(0,0,40);
	//pts[9] = CPoint3D(20,20,40);
	//pts[10] = CPoint3D(40,20,40);
	//pts[11] = CPoint3D(80,0,40);

	//pts[12] = CPoint3D(0,0,60);
	//pts[13] = CPoint3D(20,20,60);
	//pts[14] = CPoint3D(40,20,60);
	//pts[15] = CPoint3D(80,0,60);

	////CBezierCurve* crv = new CBezierCurve(pts,4);
	////m_Part.AddEntity(crv);

	//CBezierSurface* surf = new CBezierSurface(pts,4,4);
	//m_Part.AddEntity(surf);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc serialization

void CSTLViewerDoc::Serialize(CArchive& ar)
{
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

void CSTLViewerDoc::OnDefFilein() 
{
	CFileDialog dlg(TRUE,_T("def"),NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Geometry Defination File(*.def)|*.def"), NULL );

	if(dlg.DoModal()==IDOK){
		CString fileName = dlg.GetPathName();
		
		FILE* file;
		errno_t err;
		if( (err  = _tfopen_s( &file, fileName, _T("r"))) !=0 )
			return;


		//read file header
		TCHAR str1[80], str2[80];
		if(_ftscanf_s(file,_T("%s %s"),str1,80,str2,80)!=2)
			return;
		if(_tcsncmp(str1,_T("#ENTITY_TYPE"),12)!=0)
			return;
		
		CEntity* pEnt = NULL;	
		if(_tcsncmp(str2,_T("#BEZIER_CURVE"),13)==0)
			CBezierCurve::ReadDefinitionFile(file,pEnt);
		else if(_tcsncmp(str2,_T("#BEZIER_SURFACE"),15)==0)
			CBezierSurface::ReadDefinitionFile(file,pEnt);
		else if(_tcsncmp(str2,_T("#NURBS_CURVE"),12)==0)
			CNurbsCurve::ReadDefinitionFile(file,pEnt);
		else if(_tcsncmp(str2,_T("#NURBS_SURFACE"),14)==0)
			CNurbsSurface::ReadDefinitionFile(file,pEnt);
		else
			return;
	
		ASSERT(pEnt);

		fclose(file);
		m_Part.AddEntity(pEnt);
		UpdateAllViews(NULL);
	}
}

void CSTLViewerDoc::OnCloseDocument() 
{

	CDocument::OnCloseDocument();
}
