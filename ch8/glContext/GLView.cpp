// VRView.cpp : implementation of the CGLView class
//

#include "stdafx.h"
#include "glContext\OpenGLDC.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLView

IMPLEMENT_DYNCREATE(CGLView, CView)

BEGIN_MESSAGE_MAP(CGLView, CView)
	//{{AFX_MSG_MAP(CGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLView construction/destruction

CGLView::CGLView()
{
	m_pGLDC = NULL;
	m_bDragMove = FALSE;
}

CGLView::~CGLView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CGLView drawing

void CGLView::OnDraw(CDC* pDC)
{
	if(m_pGLDC){
		m_pGLDC->Ready();
		RenderScene(m_pGLDC);
		m_pGLDC->Finish();
	}
}

BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs)
{

	//  Add Window style required for OpenGL before window is created
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGLView message handlers
void CGLView::RenderScene(COpenGLDC* pDC)
{
	pDC->DrawCoord();
}

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pGLDC = new COpenGLDC(this->GetSafeHwnd());
	m_pGLDC->InitDC();
	return 0;
}

void CGLView::OnDestroy() 
{
	CView::OnDestroy();
	if(m_pGLDC)	delete m_pGLDC;	
}

void CGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if(m_pGLDC)	
		m_pGLDC->GLResize(cx,cy);
}

BOOL CGLView::OnEraseBkgnd(CDC* pDC) 
{
	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

void CGLView::OnViewType(UINT type)
{
	ASSERT(type >= VIEW_FRONT && type <= VIEW_NW_ISOMETRIC);
	m_pGLDC->m_Camera.set_view_type(type);
	Invalidate();
}

BOOL CGLView::GetBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1)
{
	return FALSE;
}

void CGLView::ZoomAll()
{
	double x0,y0,z0,x1,y1,z1;
	if(GetBox(x0,y0,z0,x1,y1,z1)){
		m_pGLDC->m_Camera.zoom_all(x0,y0,z0,x1,y1,z1);
		Invalidate();
	}
}

void CGLView::Zoom(double dScale)
{
	m_pGLDC->m_Camera.zoom(dScale);
	Invalidate();
}

void CGLView::MoveView(double dpx,double dpy)
{
	m_pGLDC->m_Camera.move_view_by_percent(dpx,dpy);
	Invalidate();
}

void CGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar){
	case VK_UP:
		m_pGLDC->m_Camera.move_view_by_percent(0.0,0.02);
		Invalidate();
		break;
	case VK_DOWN:
		m_pGLDC->m_Camera.move_view_by_percent(0.0,-0.02);
		Invalidate();
		break;
	case VK_RIGHT:
		m_pGLDC->m_Camera.move_view_by_percent(0.02,0);
		Invalidate();
		break;
	case VK_LEFT:
		m_pGLDC->m_Camera.move_view_by_percent(-0.02,0);
		Invalidate();
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGLView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar){
	case 'x':
		m_pGLDC->m_Camera.turnUp();
		Invalidate();
		break;
	case 'X':
		m_pGLDC->m_Camera.turnDown();
		Invalidate();
		break;
	case 'Z':
		m_pGLDC->m_Camera.turnRight();
		Invalidate();
		break;
	case 'z':
		m_pGLDC->m_Camera.turnLeft();
		Invalidate();
		break;
	}
	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bDragMove) //or for any other operations
		m_lastPoint = point;

	CView::OnLButtonDown(nFlags, point);
}

void CGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(	nFlags & MK_LBUTTON &&	m_bDragMove)
	{
		m_pGLDC->m_Camera.move_screen(point.x-m_lastPoint.x,m_lastPoint.y-point.y);
		m_lastPoint = point;
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}

void CGLView::setDragMove(BOOL bOn)
{
	m_bDragMove = bOn;
}