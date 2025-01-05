// GLView.cpp : implementation of the CGLView class
//

#include "stdafx.h"
#include "GL.h"

#include "GLDoc.h"
#include "GLView.h"

#include "OpenGLDC.h"

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
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLView construction/destruction

CGLView::CGLView()
{
	m_pGLDC = NULL;	
}

CGLView::~CGLView()
{
}

BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs)
{

	//  Add Window style required for OpenGL before window is created
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
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

/////////////////////////////////////////////////////////////////////////////
// CGLView message handlers

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pGLDC = new COpenGLDC(GetSafeHwnd());
	m_pGLDC->InitDC();
	SetTimer(0,50,NULL);	
	return 0;
}

void CGLView::OnDestroy() 
{
	CView::OnDestroy();
	if(m_pGLDC)
		delete m_pGLDC;	
}

BOOL CGLView::OnEraseBkgnd(CDC* pDC) 
{
	//return CView::OnEraseBkgnd(pDC);
	return FALSE;
}

void CGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if(m_pGLDC)	
		m_pGLDC->GLResize(cx,cy);
}

void CGLView::RenderScene(COpenGLDC* pDC)
{
	// Rotate
	glRotatef(3.0f,0.0f, 0.0f, 1.0f); // Rock Z
	glRotatef(3.0f,1.0f, 0.0f, 0.0f); // Roll X

	//glPushMatrix();
	glColor3ub(255,0,0);//red
	auxSolidSphere(360); //0,0,0

	glColor3ub(0,0,255);//blue
	glTranslatef(600,0,200);//600,0,200
	auxSolidSphere(120);

	glColor3ub(0,255,0);//green
	glTranslatef(-1200,600,0);//-600,600,200
	auxSolidSphere(60);

	//glPopMatrix();
}

void CGLView::OnTimer(UINT nIDEvent) 
{
	Invalidate(FALSE);	
	CView::OnTimer(nIDEvent);
}