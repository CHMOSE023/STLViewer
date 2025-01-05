// Entity.cpp: implementation of the CEntity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GeomKernel\Entity.h"
#include "glContext\openGLDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEntity::CEntity()
{
	m_bModified = FALSE;
	m_id = -1;
	m_pBox = NULL;
	m_color = RGB(128,128,128);
	m_name = _T("Unknow");
	m_bVisible = TRUE;
	m_bHighlight = FALSE;
}

CEntity::~CEntity()
{
	if(m_pBox)
		delete m_pBox;
}

void CEntity::Serialize(CArchive& ar)
{
	if(ar.IsStoring()){
		ar << m_id;
		ar << m_name;
		ar << m_color;
		ar << m_bVisible;
	}
	else{ //loading
		ar >> m_id;
		ar >> m_name;
		ar >> m_color;
		ar >> m_bVisible;
	}
}	

BOOL	CEntity::GetBox(CBox3D& box)
{
	if(m_bModified)
		UpdateBox();

	if( m_pBox){
		box = *m_pBox;
		return TRUE;
	}
	else
		return FALSE;
}

void	CEntity::SetID(UINT nID)
{
	m_id = nID;
}

UINT	CEntity::GetID()
{
	return m_id;
}

void	CEntity::SetName(LPCTSTR name)
{
	m_name = name;
}

CString	CEntity::GetName()
{
	return m_name;
}

void	CEntity::SetColor(COLORREF color)
{
	m_color = color;
}

COLORREF CEntity::GetColor()
{
	return m_color;
}

void CEntity::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}

BOOL CEntity::IsVisible()
{
	return m_bVisible;
}

void CEntity::SetHighlight(BOOL bHighlight)
{
	m_bHighlight = bHighlight;
}

BOOL CEntity::IsHighlight()
{
	return m_bHighlight;
}

///////////////////////////
//class tri chip 
IMPLEMENT_SERIAL(CTriChip,CObject,0)
CTriChip::CTriChip()
{
}

CTriChip::CTriChip(const CPoint3D& v0,const CPoint3D& v1,const CPoint3D& v2,const CVector3D& nor)
{
	vex[0] = v0;
	vex[1] = v1;
	vex[2] = v2;
	normal = nor;
}

CTriChip::~CTriChip()
{
}

const CTriChip& CTriChip::operator=(const CTriChip& tri)
{
	normal = tri.normal;
	for(int i=0;i<3;i++)
		vex[i] = tri.vex[i];
	return *this;
}

void CTriChip::Draw(COpenGLDC* pDC)
{
	pDC->DrawTriChip(normal.dx,normal.dy,normal.dz,
		vex[0].x,vex[0].y,vex[0].z,
		vex[1].x,vex[1].y,vex[1].z,
		vex[2].x,vex[2].y,vex[2].z);
}

void CTriChip::Serialize(CArchive& ar)
{
	if(ar.IsStoring()){
		ar << normal.dx << normal.dy << normal.dz;
		for(int i=0;i<3;i++)
			ar << vex[i].x << vex[i].y << vex[i].z;
	}
	else{
		ar >> normal.dx >> normal.dy >> normal.dz;
		for(int i=0;i<3;i++)
			ar >> vex[i].x >> vex[i].y >> vex[i].z;
	}
}

//////////////////////////////////////////
//class  CSTLModel

IMPLEMENT_SERIAL(CSTLModel,CObject,0)

CSTLModel::CSTLModel()
{
}

CSTLModel::~CSTLModel()
{
	Clear();
}

void CSTLModel::Add(CTriChip* tri)
{
	m_TriList.Add(tri);
}

void CSTLModel::UpdateBox()
{
	if(m_pBox){
		delete m_pBox;
		m_pBox = NULL;
	}

	if(m_TriList.GetSize()==0) 
		return;
	
	double x0,y0,z0,x1,y1,z1;
	x0=y0=z0=10000;
	x1=y1=z1=-10000;
	
	CTriChip* tri;
	for(int i=0;i<m_TriList.GetSize();i++){
		tri = m_TriList[i];
		for(int n=0;n<3;n++){
			if(tri->vex[n].x<x0)  x0 = tri->vex[n].x;
			if(tri->vex[n].x>x1)  x1 = tri->vex[n].x;
			if(tri->vex[n].y<y0)  y0 = tri->vex[n].y;
			if(tri->vex[n].y>y1)  y1 = tri->vex[n].y;
			if(tri->vex[n].z<z0)  z0 = tri->vex[n].z;
			if(tri->vex[n].z>z1)  z1 = tri->vex[n].z;
		}
	}

	m_pBox = new CBox3D(x0,y0,z0,x1,y1,z1);
	m_bModified = FALSE;
}

//load with STL File
BOOL CSTLModel::LoadSTLFile(LPCTSTR stlfile)
{
	FILE* file;
	errno_t err;
	if( (err  = _tfopen_s( &file, stlfile, _T("r"))) !=0 )
		return FALSE;

	char str[80];
	CTriChip* tri = NULL;
	while(fscanf_s(file,"%s",str,80)==1){
		if(strncmp(str,"normal",6)==0){
			tri = new CTriChip();
			fscanf_s(file,"%lf %lf %lf",&(tri->normal.dx),&(tri->normal.dy),&(tri->normal.dz));
			fscanf_s(file,"%*s %*s");
			fscanf_s(file,"%*s %lf %lf %lf",&(tri->vex[0].x),&(tri->vex[0].y),&(tri->vex[0].z));
			fscanf_s(file,"%*s %lf %lf %lf",&(tri->vex[1].x),&(tri->vex[1].y),&(tri->vex[1].z));
			fscanf_s(file,"%*s %lf %lf %lf",&(tri->vex[2].x),&(tri->vex[2].y),&(tri->vex[2].z));
			Add(tri);
		}
	}

	TCHAR title[80];
	if(GetFileTitle(stlfile,title,80)==0){
		SetName(title);
	}

	m_bModified = TRUE;
	return TRUE;
}

//Serialize
void	CSTLModel::Serialize(CArchive& ar)
{
	CEntity::Serialize(ar);
	
	if(ar.IsStoring()){
		m_TriList.Serialize(ar);
	}
	else{ //IsLoading()
		m_TriList.Serialize(ar);
		m_bModified = TRUE;
	}
}

void	CSTLModel::Draw(COpenGLDC* pDC)
{
	if(!m_bVisible)	return;
	
	if(m_bHighlight)
		pDC->Highlight(TRUE);
	else
		pDC->SetMaterialColor(m_color);

	if(pDC->IsSelectionMode()){
		pDC->LoadName((UINT)this);
	}

	for(int i=0;i<m_TriList.GetSize();i++){
		m_TriList[i]->Draw(pDC);
	}
}

void	CSTLModel::Clear()
{
	for(int i=0;i<m_TriList.GetSize();i++)
		delete m_TriList[i];
	m_TriList.RemoveAll();

	m_bModified = TRUE;
}

BOOL	CSTLModel::IsEmpty()
{
	return m_TriList.GetSize() == 0;
}

//get entity type
CEntity::Type CSTLModel::type() const
{
	return kSTLModel;
}

//////////////////////////////////////////

//static function - read definition file
BOOL CBezierCurve::ReadDefinitionFile(FILE* file,CEntity*& pEnt)
{
	CBezierCurve* pBezierCrv = new CBezierCurve();

	TCHAR name[80];
	_ftscanf_s(file,_T("%*s %s"),name,80);
	pBezierCrv->SetName(name);

	int uSize = 0;
	_ftscanf_s(file,_T("%*s %d"),&uSize);

	CPoint3D* ctrlPts = new CPoint3D[uSize];
	double x,y,z;
	for(int i=0;i<uSize;i++)	{
		_ftscanf_s(file,_T("%*s %lf %lf %lf"),&x,&y,&z);
		ctrlPts[i].x = x;
		ctrlPts[i].y = y;
		ctrlPts[i].z = z;
	}

	_ftscanf_s(file,_T("%s"),name,80);

	pBezierCrv->set(ctrlPts,uSize);
	delete [] ctrlPts;

	pEnt = pBezierCrv;
	return TRUE;
}

IMPLEMENT_SERIAL(CBezierCurve,CObject,0)
CBezierCurve::CBezierCurve()
:m_uSize(0),m_ctrlPts(NULL)
{
}

CBezierCurve::CBezierCurve(CPoint3D* ctrlPts,int uSize)
:m_uSize(0),m_ctrlPts(NULL)
{
	set(ctrlPts,uSize);
}

CBezierCurve::~CBezierCurve()
{
	clear();
}

void CBezierCurve::clear()
{
	m_uSize = 0;
	if(m_ctrlPts != NULL){
		delete [] m_ctrlPts;
		m_ctrlPts = NULL;
	}
	m_bModified = TRUE;
}

void CBezierCurve::set(CPoint3D* ctrlPts,int uSize)
{
	clear();
	m_uSize = uSize;
	m_ctrlPts = new CPoint3D[uSize];
	for(int i=0; i<uSize; i++)
		m_ctrlPts[i] = ctrlPts[i];

	m_bModified = TRUE;
}

void CBezierCurve::Draw(COpenGLDC* pDC)
{
	if(!m_bVisible)
		return;

	pDC->DrawBezierCurve(m_ctrlPts,m_uSize);
}

void CBezierCurve::Serialize(CArchive& ar)
{
	CEntity::Serialize(ar);

	if(ar.IsStoring()){
		ar << m_uSize;
		for(int i=0; i<m_uSize; i++)
			ar << m_ctrlPts[i].x << m_ctrlPts[i].y << m_ctrlPts[i].z;
	}
	else{
		if(m_ctrlPts){
			delete [] m_ctrlPts;
			m_ctrlPts = NULL;
		}
		
		CPoint3D pt;
		ar >> m_uSize;
		m_ctrlPts = new CPoint3D[m_uSize];
		
		for(int i=0; i<m_uSize; i++)
			ar >> m_ctrlPts[i].x >> m_ctrlPts[i].y >> m_ctrlPts[i].z;

		m_bModified = TRUE;
	}
}

BOOL	CBezierCurve::IsEmpty() const
{
	return m_uSize == 0;
}

//get entity type
CEntity::Type CBezierCurve::type() const
{
	return kBezierCurve;
}

void CBezierCurve::UpdateBox()
{
	if(m_pBox){
		delete m_pBox;
		m_pBox = NULL;
	}

	if(m_uSize==0) 
		return;

	double x0,y0,z0,x1,y1,z1;
	x0=y0=z0=10000;
	x1=y1=z1=-10000;

	for(int i=0;i<m_uSize;i++){
		if(m_ctrlPts[i].x<x0)  x0 = m_ctrlPts[i].x;
		if(m_ctrlPts[i].x>x1)  x1 = m_ctrlPts[i].x;
		if(m_ctrlPts[i].y<y0)  y0 = m_ctrlPts[i].y;
		if(m_ctrlPts[i].y>y1)  y1 = m_ctrlPts[i].y;
		if(m_ctrlPts[i].z<z0)  z0 = m_ctrlPts[i].z;
		if(m_ctrlPts[i].z>z1)  z1 = m_ctrlPts[i].z;
	}

	m_pBox = new CBox3D(x0,y0,z0,x1,y1,z1);
	m_bModified = FALSE;
}


//////////////////////////////////////////
//static function: read definition file
BOOL CBezierSurface::ReadDefinitionFile(FILE* file,CEntity*& pEnt)
{
	CBezierSurface* pBezierSurf = new CBezierSurface();

	TCHAR name[80];
	_ftscanf_s(file,_T("%*s %s"),name,80);
	pBezierSurf->SetName(name);

	int uSize = 0;
	int vSize = 0;
	_ftscanf_s(file,_T("%*s %d"),&uSize);
	_ftscanf_s(file,_T("%*s %d"),&vSize);

	int size = uSize*vSize;
	CPoint3D* ctrlPts = new CPoint3D[size];
	double x,y,z;
	for(int i=0;i<size;i++)	{
		_ftscanf_s(file,_T("%s %lf %lf %lf"),name,80,&x,&y,&z);
		ctrlPts[i].x = x;
		ctrlPts[i].y = y;
		ctrlPts[i].z = z;
	}

	_ftscanf_s(file,_T("%s"),name,80);

	pBezierSurf->set(ctrlPts,uSize,vSize);
	delete [] ctrlPts;
	pEnt = pBezierSurf;
	return TRUE;
}

IMPLEMENT_SERIAL(CBezierSurface,CObject,0)
CBezierSurface::CBezierSurface()
:m_uSize(0),m_vSize(0),m_ctrlPts(NULL)
{
}

CBezierSurface::CBezierSurface(CPoint3D* vex,int uSize,int vSize)
:m_uSize(0),m_vSize(0),m_ctrlPts(NULL)
{
	set(vex,uSize,vSize);
}

CBezierSurface::~CBezierSurface()
{
	clear();
}

void CBezierSurface::clear()
{
	m_uSize = 0;
	m_vSize = 0;
	if(m_ctrlPts != NULL){
		delete [] m_ctrlPts;
		m_ctrlPts = NULL;
	}
	m_bModified = TRUE;
}

void CBezierSurface::set(CPoint3D* vex,int uSize,int vSize)
{
	clear();
	m_uSize = uSize;
	m_vSize = vSize;
	
	int size = uSize * vSize;
	m_ctrlPts = new CPoint3D[size];
	
	for(int i=0; i<size; i++)
		m_ctrlPts[i] = vex[i];
	
	m_bModified = TRUE;
}

void CBezierSurface::Draw(COpenGLDC* pDC)
{
	if(!m_bVisible)	return;

	if(m_bHighlight)
		pDC->Highlight(TRUE);
	else
		pDC->SetMaterialColor(m_color);

	if(!pDC->IsSelectionMode())
		pDC->DrawBezierSurface(m_ctrlPts,m_uSize,m_vSize);
}

void CBezierSurface::Serialize(CArchive& ar)
{
	CEntity::Serialize(ar);

	if(ar.IsStoring()){
		int size = m_uSize*m_vSize;
		ar << m_uSize << m_vSize;
		for(int i=0;i<size;i++)
			ar << m_ctrlPts[i].x << m_ctrlPts[i].y << m_ctrlPts[i].z;
	}
	else{
		if(m_ctrlPts){
			delete [] m_ctrlPts;
			m_ctrlPts = NULL;
		}

		ar >> m_uSize >> m_vSize;
		int size = m_uSize*m_vSize;
		
		m_ctrlPts = new CPoint3D[size];
		for(int i=0;i<size;i++)
			ar >> m_ctrlPts[i].x >> m_ctrlPts[i].y >> m_ctrlPts[i].z;

		m_bModified = TRUE;
	}
}

BOOL	CBezierSurface::IsEmpty() const
{
	return (m_uSize*m_vSize) == 0;
}

//get entity type
CEntity::Type CBezierSurface::type() const
{
	return kBezierSurface;
}

void CBezierSurface::UpdateBox()
{
	if(m_pBox){
		delete m_pBox;
		m_pBox = NULL;
	}

	int size = m_uSize*m_vSize;
	if(size==0) 
		return;

	double x0,y0,z0,x1,y1,z1;
	x0=y0=z0=10000;
	x1=y1=z1=-10000;

	for(int i=0;i<size;i++){
		if(m_ctrlPts[i].x<x0)  x0 = m_ctrlPts[i].x;
		if(m_ctrlPts[i].x>x1)  x1 = m_ctrlPts[i].x;
		if(m_ctrlPts[i].y<y0)  y0 = m_ctrlPts[i].y;
		if(m_ctrlPts[i].y>y1)  y1 = m_ctrlPts[i].y;
		if(m_ctrlPts[i].z<z0)  z0 = m_ctrlPts[i].z;
		if(m_ctrlPts[i].z>z1)  z1 = m_ctrlPts[i].z;
	}

	m_pBox = new CBox3D(x0,y0,z0,x1,y1,z1);
	m_bModified = FALSE;
}



/////////////////////////////////////////////
//static function
BOOL CNurbsCurve::ReadDefinitionFile(FILE* file,CEntity*& pEnt)
{
	CNurbsCurve* pNurbsCrv = new CNurbsCurve();

	TCHAR name[80];
	_ftscanf_s(file,_T("%*s %s"),name,80);
	pNurbsCrv->SetName(name);

	int uSize = 0;
	_ftscanf_s(file,_T("%*s %d"),&uSize);

	CPoint3D* ctrlPts = new CPoint3D[uSize];
	double x,y,z;
	for(int i=0;i<uSize;i++)	{
		_ftscanf_s(file,_T("%*s %lf %lf %lf"),&x,&y,&z);
		ctrlPts[i].x = x;
		ctrlPts[i].y = y;
		ctrlPts[i].z = z;
	}

	int uOrder = 0;
	_ftscanf_s(file,_T("%*s %d"),&uOrder);
	
	int knotSize = uSize + uOrder;
	float* knots = new float[knotSize];
	float k;
	for(int i=0;i<knotSize;i++)	{
		_ftscanf_s(file,_T("%*s %f"),&k);
		knots[i] = k;
	}

	pNurbsCrv->set(ctrlPts,uSize,uOrder,knots);

	_ftscanf_s(file,_T("%s"),name,80);
	
	delete [] ctrlPts;
	delete [] knots;
	pEnt = pNurbsCrv;
	return TRUE;
}

//constructor && destructor
IMPLEMENT_SERIAL(CNurbsCurve,CObject,0)
CNurbsCurve::CNurbsCurve()
:m_uSize(0),m_ctrlPts(NULL),m_uOrder(4),m_uKnots(NULL)
{
}

CNurbsCurve::CNurbsCurve(CPoint3D* ctrlPts,int uSize, int uOrder,
						float* uKnots)
:m_uSize(0),m_ctrlPts(NULL),m_uOrder(4),m_uKnots(NULL)
{
	set(ctrlPts,uSize,uOrder,uKnots);
}

CNurbsCurve::~CNurbsCurve()
{
	clear();
}

void CNurbsCurve::clear()
{
	m_uSize = 0;
	if(m_ctrlPts != NULL){
		delete [] m_ctrlPts;
		m_ctrlPts = NULL;
	}
	
	m_uOrder = 0;
	if(m_uKnots != NULL){
		delete [] m_uKnots;
		m_uKnots = NULL;
	}

	m_bModified = TRUE;
}

void CNurbsCurve::set(CPoint3D* ctrlPts, int uSize, int uOrder, float* uKnots)
{
	clear();
	m_uSize = uSize;
	m_ctrlPts = new CPoint3D[uSize];
	for(int i=0; i<uSize; i++)
		m_ctrlPts[i] = ctrlPts[i];

	m_uOrder = uOrder;
	int knotSize = uSize+uOrder;
	m_uKnots = new float[knotSize];
	for(int i=0; i<knotSize; i++)
		m_uKnots[i] = uKnots[i];

	m_bModified = TRUE;
}

void CNurbsCurve::Draw(COpenGLDC* pDC)
{
	if(!m_bVisible)
		return;

	pDC->DrawNurbsCurve(m_ctrlPts,m_uSize,m_uOrder,m_uKnots);
}

void CNurbsCurve::Serialize(CArchive& ar)
{
	CEntity::Serialize(ar);

	if(ar.IsStoring()){
		ar << m_uSize;
		for(int i=0; i<m_uSize; i++)
			ar << m_ctrlPts[i].x << m_ctrlPts[i].y << m_ctrlPts[i].z;

		ar << m_uOrder;
		int knotSize = m_uSize + m_uOrder;
		for(int i=0; i<knotSize; i++)
			ar << m_uKnots[i];
	}
	else{
		if(m_ctrlPts){
			delete [] m_ctrlPts;
			m_ctrlPts = NULL;
		}

		if(m_uKnots){
			delete [] m_uKnots;
			m_uKnots = NULL;
		}

		ar >> m_uSize;
		m_ctrlPts = new CPoint3D[m_uSize];
		for(int i=0; i<m_uSize; i++)
			ar >> m_ctrlPts[i].x >> m_ctrlPts[i].y >> m_ctrlPts[i].z;

		ar >> m_uOrder;
		int knotSize = m_uSize + m_uOrder;
		m_uKnots = new float[knotSize];
		for(int i=0; i<knotSize; i++)
			ar >> m_uKnots[i];

		m_bModified = TRUE;
	}
}

BOOL CNurbsCurve::IsEmpty() const
{
	return m_uSize == 0;
}

CEntity::Type CNurbsCurve::type() const
{ 
	return kNurbsCurve;
}

void CNurbsCurve::UpdateBox()
{
	if(m_pBox){
		delete m_pBox;
		m_pBox = NULL;
	}

	if(m_uSize==0) 
		return;

	double x0,y0,z0,x1,y1,z1;
	x0=y0=z0=10000;
	x1=y1=z1=-10000;

	for(int i=0;i<m_uSize;i++){
		if(m_ctrlPts[i].x<x0)  x0 = m_ctrlPts[i].x;
		if(m_ctrlPts[i].x>x1)  x1 = m_ctrlPts[i].x;
		if(m_ctrlPts[i].y<y0)  y0 = m_ctrlPts[i].y;
		if(m_ctrlPts[i].y>y1)  y1 = m_ctrlPts[i].y;
		if(m_ctrlPts[i].z<z0)  z0 = m_ctrlPts[i].z;
		if(m_ctrlPts[i].z>z1)  z1 = m_ctrlPts[i].z;
	}

	m_pBox = new CBox3D(x0,y0,z0,x1,y1,z1);
	m_bModified = FALSE;
}

//////////////////////////////////////////
//////////////////////////////////////////
//static function: read definition file
BOOL CNurbsSurface::ReadDefinitionFile(FILE* file,CEntity*& pEnt)
{
	CNurbsSurface* pNurbsSurf = new CNurbsSurface();

	TCHAR name[80];
	_ftscanf_s(file,_T("%*s %s"),name,80);
	pNurbsSurf->SetName(name);

	int uSize = 0;
	int vSize = 0;
	_ftscanf_s(file,_T("%*s %d"),&uSize);
	_ftscanf_s(file,_T("%*s %d"),&vSize);

	int size = uSize*vSize;
	CPoint3D* ctrlPts = new CPoint3D[size];
	double x,y,z;
	for(int i=0;i<size;i++)	{
		_ftscanf_s(file,_T("%s %lf %lf %lf"),name,80,&x,&y,&z);
		ctrlPts[i].x = x;
		ctrlPts[i].y = y;
		ctrlPts[i].z = z;
	}

	int uOrder = 0;
	int vOrder = 0;
	_ftscanf_s(file,_T("%*s %d"),&uOrder);
	_ftscanf_s(file,_T("%*s %d"),&vOrder);

	int uKnotSize = uSize + uOrder;
	int vKnotSize = vSize + vOrder;
	float* uKnots = new float[uKnotSize];
	float* vKnots = new float[vKnotSize];
	float k;
	for(int i=0;i<uKnotSize;i++)	{
		_ftscanf_s(file,_T("%*s %f"),&k);
		uKnots[i] = k;
	}
	for(int i=0;i<vKnotSize;i++)	{
		_ftscanf_s(file,_T("%*s %f"),&k);
		vKnots[i] = k;
	}

	_ftscanf_s(file,_T("%s"),name,80);

	pNurbsSurf->set(ctrlPts,uSize,vSize,uOrder,vOrder,uKnots,vKnots);

	delete [] ctrlPts;
	delete [] uKnots;
	delete [] vKnots;
	pEnt = pNurbsSurf;
	return TRUE;
}

IMPLEMENT_SERIAL(CNurbsSurface,CObject,0)
CNurbsSurface::CNurbsSurface()
:m_uSize(0),m_vSize(0),m_ctrlPts(NULL),m_uOrder(0),m_vOrder(0),m_uKnots(NULL),m_vKnots(NULL)
{
}

CNurbsSurface::CNurbsSurface(CPoint3D* vex,int uSize,int vSize,int uOrder, int vOrder,float* uKnots,float* vKnots)
:m_uSize(0),m_vSize(0),m_ctrlPts(NULL),m_uOrder(0),m_vOrder(0),m_uKnots(NULL),m_vKnots(NULL)
{
	set(vex,uSize,vSize,uOrder,vOrder,uKnots,vKnots);
}

CNurbsSurface::~CNurbsSurface()
{
	clear();
}

void CNurbsSurface::clear()
{
	m_uSize = 0;
	m_vSize = 0;
	if(m_ctrlPts != NULL){
		delete [] m_ctrlPts;
		m_ctrlPts = NULL;
	}

	m_uOrder = 0;
	m_vOrder = 0;
	if(m_uKnots != NULL){
		delete [] m_uKnots;
		m_uKnots = NULL;
	}
	if(m_vKnots != NULL){
		delete [] m_vKnots;
		m_vKnots = NULL;
	}

	m_bModified = TRUE;
}

void CNurbsSurface::set(CPoint3D* vex,int uSize,int vSize,int uOrder, int vOrder,float* uKnots,float* vKnots)
{
	clear();
	m_uSize = uSize;
	m_vSize = vSize;

	int size = uSize * vSize;
	m_ctrlPts = new CPoint3D[size];

	for(int i=0; i<size; i++)
		m_ctrlPts[i] = vex[i];

	m_uOrder = uOrder;
	m_vOrder = vOrder;

	int uKnotSize = uSize + uOrder;
	int vKnotSize = vSize + vOrder;
	m_uKnots = new float[uKnotSize];
	m_vKnots = new float[vKnotSize];
	for(int i=0; i<uKnotSize; i++)
		m_uKnots[i] = uKnots[i];
	for(int i=0; i<vKnotSize; i++)
		m_vKnots[i] = vKnots[i];

	m_bModified = TRUE;
}

void CNurbsSurface::Draw(COpenGLDC* pDC)
{
	if(!m_bVisible)	return;

	if(m_bHighlight)
		pDC->Highlight(TRUE);
	else
		pDC->SetMaterialColor(m_color);

	if(!pDC->IsSelectionMode())
		pDC->DrawNurbsSurface(m_ctrlPts,m_uSize,m_vSize,m_uOrder,m_vOrder,m_uKnots,m_vKnots);
}

void CNurbsSurface::Serialize(CArchive& ar)
{
	CEntity::Serialize(ar);

	if(ar.IsStoring()){
		int size = m_uSize*m_vSize;
		ar << m_uSize << m_vSize;
		for(int i=0;i<size;i++)
			ar << m_ctrlPts[i].x << m_ctrlPts[i].y << m_ctrlPts[i].z;
		ar << m_uOrder << m_vOrder;
		for(int i=0;i<m_uSize + m_uOrder;i++)
			ar << m_uKnots[i];
		for(int i=0;i<m_vSize + m_vOrder;i++)
			ar << m_vKnots[i];
	}
	else{
		if(m_ctrlPts){
			delete [] m_ctrlPts;
			m_ctrlPts = NULL;
		}
		ar >> m_uSize >> m_vSize;
		int size = m_uSize*m_vSize;
		m_ctrlPts = new CPoint3D[size];
		for(int i=0;i<size;i++)
			ar >> m_ctrlPts[i].x >> m_ctrlPts[i].y >> m_ctrlPts[i].z;
		ar >> m_uOrder >> m_vOrder;
		int uKnotSize = m_uSize + m_uOrder;
		int vKnotSize = m_vSize + m_vOrder;
		m_uKnots = new float[uKnotSize];
		m_vKnots = new float[vKnotSize];
		for(int i=0;i<uKnotSize;i++)
			ar >> m_uKnots[i];
		for(int i=0;i<vKnotSize;i++)
			ar >> m_vKnots[i];

		m_bModified = TRUE;
	}
}

BOOL	CNurbsSurface::IsEmpty() const
{
	return (m_uSize*m_vSize) == 0;
}

//get entity type
CEntity::Type CNurbsSurface::type() const
{
	return kNurbsSurface;
}

void CNurbsSurface::UpdateBox()
{
	if(m_pBox){
		delete m_pBox;
		m_pBox = NULL;
	}

	int size = m_uSize*m_vSize;
	if(size==0) 
		return;

	double x0,y0,z0,x1,y1,z1;
	x0=y0=z0=10000;
	x1=y1=z1=-10000;

	for(int i=0;i<size;i++){
		if(m_ctrlPts[i].x<x0)  x0 = m_ctrlPts[i].x;
		if(m_ctrlPts[i].x>x1)  x1 = m_ctrlPts[i].x;
		if(m_ctrlPts[i].y<y0)  y0 = m_ctrlPts[i].y;
		if(m_ctrlPts[i].y>y1)  y1 = m_ctrlPts[i].y;
		if(m_ctrlPts[i].z<z0)  z0 = m_ctrlPts[i].z;
		if(m_ctrlPts[i].z>z1)  z1 = m_ctrlPts[i].z;
	}

	m_pBox = new CBox3D(x0,y0,z0,x1,y1,z1);
	m_bModified = FALSE;
}

//////////////////////////////////////////
CPart::CPart()
{
}

CPart::~CPart()
{
	RemoveAllEntity();
}

//draw
void CPart::Draw(COpenGLDC* pDC)
{
	CEntity *pEnt = NULL;
	for(int i=0;i<m_EntList.GetSize();i++){
		pEnt = m_EntList[i];
		pEnt->Draw(pDC);
	}
}

//serialize
void CPart::Serialize(CArchive& ar)
{
	CEntity::Serialize(ar);
	if(ar.IsStoring()){
		m_EntList.Serialize(ar);
	}
	else{
		m_EntList.Serialize(ar);
		m_bModified = TRUE;
	}
}

//operation
void CPart::AddEntity(CEntity* ent)
{
	m_EntList.Add(ent);
	m_bModified = TRUE;
}

void CPart::RemoveEntity(CEntity* ent)
{
	for(int i=0;i<m_EntList.GetSize();i++){
		if(ent == m_EntList[i]){
			m_EntList.RemoveAt(i);
			m_bModified = TRUE;
			break;
		}
	}
}

void CPart::RemoveAllEntity()
{
	for(int i=0;i<m_EntList.GetSize();i++)
		delete m_EntList[i];
	m_EntList.RemoveAll();
	m_bModified = TRUE;
}

//attrib accessing
BOOL CPart::IsEmpty()
{
	return m_EntList.GetSize() == 0 ;
}

//get entity type
CEntity::Type CPart::type() const
{
	return kPart;
}

UINT	CPart::GetEntitySize()
{
	return m_EntList.GetSize();
}

CEntity* CPart::GetEntity(UINT i)
{
	ASSERT(i<m_EntList.GetSize());
	return m_EntList[i];
}

void CPart::UpdateBox()
{
	if(m_pBox){
		delete m_pBox;
		m_pBox = NULL;
	}

	CBox3D box;
	for(int i=0;i<m_EntList.GetSize();i++){
		if(m_EntList[i]->GetBox(box)){
			if(m_pBox)
				*m_pBox += box;
			else{
				m_pBox = new CBox3D();
				*m_pBox = box;
			}
		}
	}
	m_bModified = FALSE;
}
