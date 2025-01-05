#include "stdafx.h"
#include "resource.h"
#include "..\inc\glContext\OpenGLDC.h"


CGLWnd::CGLWnd():CWnd()
{
	m_pGLDC = NULL;
}


BEGIN_MESSAGE_MAP(CGLWnd, CWnd)
	//{{AFX_MSG_MAP(CGLWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_GLWND_FRONT,OnViewType)
	ON_COMMAND(ID_GLWND_BACK,OnViewType)
	ON_COMMAND(ID_GLWND_TOP,OnViewType)
	ON_COMMAND(ID_GLWND_BOTTOM,OnViewType)
	ON_COMMAND(ID_GLWND_RIGHT,OnViewType)
	ON_COMMAND(ID_GLWND_LEFT,OnViewType)
	ON_COMMAND(ID_GLWND_ISOMETRIC,OnViewType)
	ON_COMMAND(ID_GLWND_ZOOMIN,OnViewType)
	ON_COMMAND(ID_GLWND_ZOOMOUT,OnViewType)
	ON_COMMAND(ID_GLWND_ZOOMALL,OnViewType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CGLWnd::Create(const RECT& rect,CWnd* pParentWnd,UINT nID,	CCreateContext* pContext)
{
	return CWnd::Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,pParentWnd,nID,pContext);
}

void CGLWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(m_pGLDC){
		m_pGLDC->Ready();
		RenderScene(m_pGLDC);
		m_pGLDC->Finish();
	}
}

void CGLWnd::RenderScene(COpenGLDC* pDC)
{
}

BOOL CGLWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CGLWnd::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pGLDC = new COpenGLDC(this);
	m_pGLDC->InitDC();
	return 0;
}

void CGLWnd::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize(nType,cx,cy);
	if(m_pGLDC)	
		m_pGLDC->GLResize(cx,cy);
}

BOOL CGLWnd::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CGLWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	if(m_pGLDC)	delete m_pGLDC;
}

void CGLWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu menubar;
	if (menubar.LoadMenu(IDR_GLWND_MENU)) {
		CMenu* pPopup = menubar.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CPoint pt = point;
		ClientToScreen(&pt);
		pPopup->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}
	CWnd::OnRButtonDown(nFlags,point);
}

void CGLWnd::OnViewType()
{
	const MSG* msg = GetCurrentMessage();
	switch(msg->wParam){
	case ID_GLWND_FRONT:
		m_pGLDC->m_Camera.setViewType(VIEW_FRONT);
		break;
	case ID_GLWND_BACK:
		m_pGLDC->m_Camera.setViewType(VIEW_BACK);
		break;
	case ID_GLWND_TOP:
		m_pGLDC->m_Camera.setViewType(VIEW_TOP);
		break;
	case ID_GLWND_BOTTOM:
		m_pGLDC->m_Camera.setViewType(VIEW_BOTTOM);
		break;
	case ID_GLWND_RIGHT:
		m_pGLDC->m_Camera.setViewType(VIEW_RIGHT);
		break;
	case ID_GLWND_LEFT:
		m_pGLDC->m_Camera.setViewType(VIEW_LEFT);
		break;
	case ID_GLWND_ISOMETRIC:
		m_pGLDC->m_Camera.setViewType(VIEW_ISOMETRIC);
		break;
	case ID_GLWND_ZOOMIN:
		m_pGLDC->m_Camera.zoom(0.9);
		break;
	case ID_GLWND_ZOOMOUT:
		m_pGLDC->m_Camera.zoom(1.1);
		break;
	case ID_GLWND_ZOOMALL:
		ZoomAll();
		break;
	}
	Invalidate();
}


void CGLWnd::ZoomAll()
{
}



/////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////
CGL2DWnd::CGL2DWnd():CWnd()
{
	m_pGLDC = NULL;
}


BEGIN_MESSAGE_MAP(CGL2DWnd, CWnd)
	//{{AFX_MSG_MAP(CGL2DWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CGL2DWnd::Create(const RECT& rect,CWnd* pParentWnd,UINT nID,	CCreateContext* pContext)
{
	return CWnd::Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,pParentWnd,nID,pContext);
}

void CGL2DWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(m_pGLDC){
		m_pGLDC->Ready();
		RenderScene(m_pGLDC);
		m_pGLDC->Finish();
	}
}

void CGL2DWnd::RenderScene(COpenGLDC* pDC)
{
	DrawGrid(5,10);
}

BOOL CGL2DWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CGL2DWnd::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pGLDC = new COpenGLDC(this);
	m_pGLDC->Init2DDC();
	return 0;
}

void CGL2DWnd::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize(nType,cx,cy);
	if(m_pGLDC)	
		m_pGLDC->GLResize(cx,cy);
}

BOOL CGL2DWnd::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CGL2DWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	if(m_pGLDC)	delete m_pGLDC;
}

void CGL2DWnd::SetViewport(double minX,double maxX,double minY,double maxY)
{
	ASSERT(m_pGLDC);
	m_pGLDC->m_Camera.set2Dviewport(minX,maxX,minY,maxY);
}

void CGL2DWnd::DrawGrid(int col,int row)
{
	ASSERT(m_pGLDC);
	double x0,x1,y0,y1;
	CPoint3D sp,ep;
	m_pGLDC->m_Camera.get2Dviewport(x0,x1,y0,y1);

	glDisable(GL_LIGHTING);
	COLORREF oldClr;
	m_pGLDC->GetLineColor(oldClr);
	m_pGLDC->SetLineColor(128,0,0);

	sp.x = x0;
	ep.x = x1;
	for(int i=0;i<row+1;i++){
		sp.y = ep.y = y0 +i*(y1-y0)/row;
		m_pGLDC->DrawLine(sp,ep);
	}

	sp.y = y0;
	ep.y = y1;
	for(i=0;i<col+1;i++){
		sp.x = ep.x = x0 +i*(x1-x0)/col;
		m_pGLDC->DrawLine(sp,ep);
	}
	
	m_pGLDC->SetLineColor(oldClr);
	glEnable(GL_LIGHTING);
}