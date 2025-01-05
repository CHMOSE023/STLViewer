// GLView.cpp : implementation of the CGLView class
//

#include "stdafx.h"
#include "GL.h"

#include "GLDoc.h"
#include "GLView.h"

#include "gl\gl.h"
#include "gl\glu.h"
#include "gl\glaux.h"

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
	m_hDC = NULL;
	m_hRC = NULL;
}

CGLView::~CGLView()
{
}

BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs)
{

	//  Add Window style required for OpenGL before window is created
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CS_OWNDC;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGLView drawing

void CGLView::OnDraw(CDC* pDC)
{
	wglMakeCurrent(m_hDC,m_hRC);

	RenderScene();

	SwapBuffers(m_hDC);

	wglMakeCurrent(m_hDC,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CGLView diagnostics

#ifdef _DEBUG

CGLDoc* CGLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLDoc)));
	return (CGLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLView message handlers

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	GLInit();
	SetTimer(0,50,NULL);	
	return 0;
}

void CGLView::OnDestroy() 
{
	GLRelease();
	CView::OnDestroy();
}

BOOL CGLView::OnEraseBkgnd(CDC* pDC) 
{
	//return CView::OnEraseBkgnd(pDC);
	return FALSE;
}

void CGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	VERIFY(wglMakeCurrent(m_hDC,m_hRC));
	GLResize(cx,cy);
	VERIFY(wglMakeCurrent(NULL,NULL));
}

void CGLView::GLInit()
{
	m_hDC = ::GetDC(m_hWnd); // Get the Device context

	ASSERT(m_hDC);

	static	PIXELFORMATDESCRIPTOR pfdWnd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Structure size.
		1,                             // Structure version number.
		PFD_DRAW_TO_WINDOW |           // Property flags.
		PFD_SUPPORT_OPENGL |
 		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,                            // 24-bit color.
		0, 0, 0, 0, 0, 0,              // Not concerned with these.
		0, 0, 0, 0, 0, 0, 0,           // No alpha or accum buffer.
		32,                            // 32-bit depth buffer.
		0, 0,                          // No stencil or aux buffer.
		PFD_MAIN_PLANE,                // Main layer type.
		0,                             // Reserved.
		0, 0, 0                        // Unsupported.
	};

	int pixelformat;
    pixelformat = ChoosePixelFormat(m_hDC, &pfdWnd);

    SetPixelFormat(m_hDC, pixelformat, &pfdWnd);

	m_hRC=wglCreateContext(m_hDC);

	//make the rending context current, perform initialization, the
	//deselect it
	VERIFY(wglMakeCurrent(m_hDC,m_hRC));
	GLSetupRC();
	VERIFY(wglMakeCurrent(NULL,NULL));
}

//clear up rendering context
void CGLView::GLRelease()
{
	wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd,m_hDC);
}

void CGLView::GLResize(int cx,int cy)
{
	double nRange = 1200.0;

	// Prevent a divide by zero
	if(cy == 0)
		cy = 1;

	// Set Viewport to window dimensions
  glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION);

	// Reset coordinate system
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if(cx <= cy)
		glOrtho(-nRange,nRange,-nRange*cy/cx,nRange*cy/cx,nRange,-nRange);
	else
		glOrtho(-nRange*cx/cy,nRange*cx/cy,-nRange,nRange,nRange,-nRange);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

}

void CGLView::GLSetupRC()
{
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glEnable(GL_COLOR_MATERIAL);

	// Lighting components
	GLfloat  ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat  diffuseLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat	 lightPos[] = { 1.0f, 1.0f, -1.0f, 0.0f };

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);


	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR,specular);
	glMateriali(GL_FRONT,GL_SHININESS,100);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f ); //background color
	// default color
	glColor3ub(0, 0, 255);
}

void CGLView::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rotate
	glRotatef(3.0f,0.0f, 0.0f, 1.0f); // Rock Z
	glRotatef(3.0f,1.0f, 0.0f, 0.0f); // Roll X

	glPushMatrix();
	glColor3ub(255,0,0);//red
	auxSolidSphere(360); //0,0,0

	glColor3ub(0,0,255);//blue
	glTranslatef(600,0,200);//600,0,0
	auxSolidSphere(120);

	glColor3ub(0,255,0);//green
	glTranslatef(-1200,600,0);//-600,0,0
	auxSolidSphere(60);
	glPopMatrix();

	glFlush();
}

void CGLView::OnTimer(UINT nIDEvent) 
{
	Invalidate(FALSE);	
	CView::OnTimer(nIDEvent);
}
