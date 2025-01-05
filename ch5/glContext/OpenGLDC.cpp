// OpenGLDC.cpp: implementation of the COpenGLDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenGLDC.h"

#define GL_PI 3.1415f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenGLDC::COpenGLDC(HWND hWnd):m_hWnd(hWnd)
{
	m_bSelectionMode = FALSE;
}

COpenGLDC::~COpenGLDC()
{
}

BOOL COpenGLDC::InitDC()
{
	if (m_hWnd == NULL) return FALSE;
	
	m_Camera.init();

	m_hDC = ::GetDC(m_hWnd);			// Get the Device context

	int pixelformat;
	PIXELFORMATDESCRIPTOR pfdWnd =
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

    if ( (pixelformat = ChoosePixelFormat(m_hDC, &pfdWnd)) == 0 )
	{
		AfxMessageBox(_T("ChoosePixelFormat to wnd failed"));
		return FALSE;
	}

    if (SetPixelFormat(m_hDC, pixelformat, &pfdWnd) == FALSE)
        AfxMessageBox(_T("SetPixelFormat failed"));

	m_hRC=wglCreateContext(m_hDC);

	VERIFY(wglMakeCurrent(m_hDC,m_hRC));
	GLSetupRC();
	wglMakeCurrent(NULL,NULL);
	return m_hRC!=0;
}


void COpenGLDC::GLResize(int w,int h)
{
	wglMakeCurrent(m_hDC,m_hRC);

	// Prevent a divide by zero
	if(h == 0) h = 1;
	if(w == 0) w = 1;
	m_Camera.set_screen(w,h);
}

void COpenGLDC::GLSetupRC()
{
	//initialize color and rendering
	m_bShading = TRUE;

	//bright white light - full intensity RGB values
	GLfloat lightAmbient[] = {0.75f,0.75f,0.75f,1.0f};
	GLfloat lightDiffuse[] = {1.0f,1.0f,1.0f,1.0f};


	glEnable(GL_DEPTH_TEST);		//Hidden surface removal
	glEnable(GL_CULL_FACE);			//calculate inside of object to support two side of surfaces
	glFrontFace(GL_CCW);				//counter clock-wise polygons face out

	glEnable(GL_LIGHTING);			//enable lighting

	//setup and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
	SetLightDirection(1,1,1);
	glEnable(GL_LIGHT0);

	//Initialize Material Color to Gray
	SetBkColor(RGB(0,0,0));				//black background
	SetMaterialColor(RGB(225,175,22));	//golden material color
	SetColor(RGB(255,255,255));			//white frame color
	SetHighlightColor(RGB(255,0,0));	//red highlight color	

	//Point Size
	glPointSize(3.0);
}


void COpenGLDC::Ready()
{
	wglMakeCurrent(m_hDC,m_hRC);
	ClearBkground();
	OnShading();
	m_Camera.projection();
}

void COpenGLDC::Finish()
{
	glFlush();
	SwapBuffers(m_hDC);
	wglMakeCurrent(m_hDC,NULL);
}


//////////////////LIGHT && MATERIALS SETTING//////////
void COpenGLDC::ClearBkground()
{
	GLclampf	r,g,b;
	r = (GLclampf)GetRValue(m_clrBk)/255.0;
	g = (GLclampf)GetGValue(m_clrBk)/255.0;
	b = (GLclampf)GetBValue(m_clrBk)/255.0;

	glClearColor(r,g,b,0.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//setting model
void COpenGLDC::OnShading()
{
	if(m_bShading){
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
	else{
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
}

void COpenGLDC::Shading(BOOL bShading)
{
	m_bShading = bShading;
}

BOOL COpenGLDC::IsShading()
{
	return m_bShading;
}

void COpenGLDC::Lighting(BOOL bLighting)
{
	if(bLighting)
		glEnable( GL_LIGHTING );
	else
		glDisable( GL_LIGHTING );
}

BOOL COpenGLDC::IsLighting()
{
	GLboolean bLighting;
	glGetBooleanv(GL_LIGHTING,&bLighting);   
	return bLighting;
}

void COpenGLDC::SetLightDirection(float dx,float dy,float dz)
{
	m_vecLight[0] = dx;
	m_vecLight[1] = dy;
	m_vecLight[2] = dz;
	GLfloat lightPos[] = { dx, dy, dz, 0.0f };
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
}

void COpenGLDC::GetLightDirection(float& dx,float& dy,float& dz)
{
	dx = m_vecLight[0];
	dy = m_vecLight[1];
	dz = m_vecLight[2];
}

//rendering color
void COpenGLDC::SetMaterialColor(COLORREF clr)
{
	m_clrMaterial = clr;
	BYTE r,g,b;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);
	GLfloat mat_amb_diff[] = {(GLfloat)r/255,(GLfloat)g/255,(GLfloat)b/255,1.0};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
}

void COpenGLDC::GetMaterialColor(COLORREF& clr)
{
	clr = m_clrMaterial;
}

void COpenGLDC::SetBkColor(COLORREF clr)
{
	m_clrBk = clr;
}

void COpenGLDC::GetBkColor(COLORREF& clr)
{
	clr = m_clrBk;
}

void COpenGLDC::SetColor(COLORREF clr)
{
	m_clr = clr;
	BYTE r,g,b;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);
	glColor3ub(r,g,b);
}

void COpenGLDC::GetColor(COLORREF& clr)
{
	clr =  m_clr;
}

void COpenGLDC::SetHighlightColor(COLORREF clr)
{
	m_clrHighlight = clr;
}

void COpenGLDC::GetHighlightColor(COLORREF& clr)
{
	clr = m_clrHighlight;
}

void COpenGLDC::Highlight(BOOL bHighlight)
{
	BYTE r,g,b;
	if(bHighlight){
		r = GetRValue(m_clrHighlight);
		g = GetGValue(m_clrHighlight);
		b = GetBValue(m_clrHighlight);
	}
	else{
		r = GetRValue(m_clrMaterial);
		g = GetGValue(m_clrMaterial);
		b = GetBValue(m_clrMaterial);
	}
	GLfloat mat_amb_diff[] = {(GLfloat)r/255,(GLfloat)g/255,(GLfloat)b/255,1.0};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
}


//////////////////////// drawing functions ////////////////////////
//draw point
void COpenGLDC::DrawPoint(const CPoint3D& pt)
{
	glBegin(GL_POINTS);
		glVertex3f(pt.x,pt.y,pt.z);
	glEnd();
}

void COpenGLDC::DrawLine(const CPoint3D& sp,const CPoint3D& ep)
{
	glBegin(GL_LINES);
		glVertex3f(sp.x,sp.y,sp.z);
		glVertex3f(ep.x,ep.y,ep.z);
	glEnd();
}

void COpenGLDC::DrawPolyline(const CPoint3D* pt,int size)
{
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<size;i++)
		glVertex3f(pt[i].x,pt[i].y,pt[i].z);
	glEnd();
}

void COpenGLDC::DrawBezierCurve(const CPoint3D* pts,int size)
{
	const GLdouble* pts0 = (const GLdouble*) pts;
	
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<size;i++)
		glVertex3dv(pts0+i*3);
	glEnd();

	//Sets up the bezier
	glMap1d(GL_MAP1_VERTEX_3, //Type of data generated
		0.0,		//Lower u range
		100.0,	//Upper u range
		3,			//Distance between points in the data
		size,		//Number of control points
		pts0);

	//Enable bezier evaluator
	glEnable(GL_MAP1_VERTEX_3);

	//Draw segments for the curve
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<=100;i++){
		glEvalCoord1d(i);
	}
	glEnd();
}

void COpenGLDC::DrawBezierSurface(const CPoint3D* pts,int uSize,int vSize)
{
	glDisable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE); 

	const GLdouble* pts0 = (const GLdouble*) pts;

	int size = uSize*vSize;
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<size;i++)
		glVertex3dv(pts0+i*3);
	glEnd();

	for(int i=0;i<uSize;i++){
		glBegin(GL_LINE_STRIP);
			for(int j=0;j<vSize;j++)
				glVertex3dv(pts0+(i*vSize+j)*3);
		glEnd();
	}

	for(int j=0;j<vSize;j++){
		glBegin(GL_LINE_STRIP);
		for(int i=0;i<uSize;i++)
			glVertex3dv(pts0+(i*vSize+j)*3);
		glEnd();
	}

	//Sets up the bezier
	glMap2d(GL_MAP2_VERTEX_3, //Type of data generated
		0.0,		//Lower u range
		10.0,		//Upper u range
		3,			//Distance between points in the data
		uSize,	//Order in u direction
		0.0,		//Lower v range
		10.0,		//Upper v range
		3*uSize, //Distance between points in the data
		vSize,	 //Order in v direction
		pts0);

	//Enable the 2D evaluator
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);

	// Map a grid of 100 points form 0 to 10
	glMapGrid2d(25,0.0,10.0,25,0.0,10.0);

	//Draw segements for for the curve
	glEvalMesh2(GL_FILL,0,25,0,25);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE); 
	glEnable(GL_CULL_FACE);
}

void COpenGLDC::DrawNurbsCurve(const CPoint3D* pts,int size,int order,float* knots)
{
	GLfloat* ctrlPts = new GLfloat[size*3];
	for(int i=0; i<size; i++)	{
		ctrlPts[i*3]	 = pts[i].x;
		ctrlPts[i*3+1] = pts[i].y;
		ctrlPts[i*3+2] = pts[i].z;
	}
		
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<size;i++)
		glVertex3fv(ctrlPts+i*3);
	glEnd();

	//create a NURBS object
	GLUnurbsObj *pNurbsObj = gluNewNurbsRenderer();

	// setting properties for NURBS
	gluNurbsProperty(pNurbsObj,GLU_SAMPLING_TOLERANCE,25.0);

	//drawing
	gluBeginCurve(pNurbsObj);
		gluNurbsCurve(pNurbsObj,
			size+order,
			knots,
			3,
			(float*) ctrlPts,
			order,
			GL_MAP1_VERTEX_3);
	gluEndCurve(pNurbsObj);

	if(pNurbsObj)
		gluDeleteNurbsRenderer(pNurbsObj);

	delete [] ctrlPts;
}

void COpenGLDC::DrawNurbsSurface(const CPoint3D* pts,int uSize,int vSize,int uOrder,int vOrder,
											float* uKnots, float* vKnots)
{
	glDisable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE); 

	int size = uSize*vSize;
	GLfloat* ctrlPts = new GLfloat[size*3];
	for(int i=0; i<size; i++)	{
		ctrlPts[i*3]	 = pts[i].x;
		ctrlPts[i*3+1] = pts[i].y;
		ctrlPts[i*3+2] = pts[i].z;
	}

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<size;i++)
		glVertex3fv(ctrlPts+i*3);
	glEnd();

	glBegin(GL_LINE_STRIP);
	for(int i=0;i<uSize;i++)
	for(int j=0;j<vSize;j++)
		glVertex3fv(ctrlPts+(i*vSize+j)*3);
	glEnd();

	glEnable(GL_AUTO_NORMAL);

	//create a NURBS object
	GLUnurbsObj *pNurbsObj = gluNewNurbsRenderer();

	// setting properties for NURBS
	gluNurbsProperty(pNurbsObj,GLU_SAMPLING_TOLERANCE,25.0);
	gluNurbsProperty(pNurbsObj,GLU_DISPLAY_MODE,(GLfloat) GLU_FILL);

	//drawing
	gluBeginSurface(pNurbsObj);
	gluNurbsSurface(pNurbsObj,
		uSize+uOrder, uKnots,
		vSize+vOrder, vKnots,
		vSize*3,
		3,
		ctrlPts,
		uOrder,
		vOrder,
		GL_MAP2_VERTEX_3);
	gluEndSurface(pNurbsObj);

	if(pNurbsObj)
		gluDeleteNurbsRenderer(pNurbsObj);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE); 
	glEnable(GL_CULL_FACE);

	delete [] ctrlPts;
}

void COpenGLDC::DrawTriChip(double n0,double n1,double n2,double v00,double v01,double v02,
					double v10,double v11,double v12,double v20,double v21,double v22)
{
	glBegin(GL_TRIANGLES);
		glNormal3d(n0,n1,n2);
		glVertex3d(v00,v01,v02);
		glVertex3d(v10,v11,v12);
		glVertex3d(v20,v21,v22);
	glEnd();
}

//Draw 3D Solid
void COpenGLDC::DrawSphere(const CPoint3D& cen,double r,const CVector3D& vec)
{
	glPushMatrix();
	glTranslatef(cen.x,cen.y,cen.z);

	CVector3D vecNY(0,-1,0);
	CVector3D axis = vecNY*vec;
	double ang = _AngleBetween(vecNY,vec);
	ang = ang*180/GL_PI;
	glRotatef(ang,axis.dx,axis.dy,axis.dz);

	auxSolidSphere(r);
	glPopMatrix();
}

void COpenGLDC::DrawCylinder(const CPoint3D& cen,double r,const CVector3D& h)
{
	glPushMatrix();

	glTranslatef(cen.x,cen.y,cen.z);

	CVector3D vecNY(0,-1,0);
	CVector3D axis = vecNY*h;
	double ang = _AngleBetween(vecNY,h);
	ang = ang*180/GL_PI;
	glRotatef(ang,axis.dx,axis.dy,axis.dz);


	auxSolidCylinder(r,h.GetLength());
	glPopMatrix();
}

void COpenGLDC::DrawTorus(const CPoint3D& cen,const CVector3D& ax,double r_in,double r_out)
{
	glPushMatrix();

	glTranslatef(cen.x,cen.y,cen.z);

	CVector3D vecNY(0,-1,0);
	CVector3D axis = vecNY*ax;
	double ang = _AngleBetween(vecNY,ax);
	ang = ang*180/GL_PI;
	glRotatef(ang,axis.dx,axis.dy,axis.dz);


	auxSolidTorus(r_in,r_out);
	glPopMatrix();
}

void COpenGLDC::DrawCone(const CPoint3D& cen,double r,const CVector3D& h)
{
	glPushMatrix();

	glTranslatef(cen.x,cen.y,cen.z);

	CVector3D vecNY(0,-1,0);
	CVector3D axis = vecNY*h;
	double ang = _AngleBetween(vecNY,h);
	ang = ang*180/GL_PI;

	glRotatef(ang,axis.dx,axis.dy,axis.dz);

	GLfloat angle,x,y;
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0,0,h.GetLength());
		for(angle =0.0f;angle<(2.0f*GL_PI);angle += (GL_PI/8.0f))
		{
			x = r*sin(angle);
			y = r*cos(angle);
			glVertex2f(x,y);
		}
	glEnd();

	// Begin a new triangle fan to cover the bottom
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.0f,0.0f);
		for(angle =0.0f;angle<(2.0f*GL_PI);angle += (GL_PI/8.0f))
		{
			x = r*sin(angle);
			y = r*cos(angle);
			glVertex2f(x,y);
		}
	glEnd();
	glPopMatrix();
}

////////////////////////////////////////

void COpenGLDC::DrawCoord()
{

	BOOL bLighting = IsLighting();
	Lighting(FALSE);

	double width,height;
	m_Camera.get_view_rect(width,height);
	double len = min(width,height);
	len *= 0.2;

	CPoint3D cPt,xPt,yPt,zPt;
	xPt.x = yPt.y = zPt.z = len;

	COLORREF old_clr;
	GetColor(old_clr);

	//axis-x: red
	SetColor(RGB(255,0,0));
	DrawLine(cPt,xPt);

	//axis-y: green
	SetColor(RGB(0,255,0));
	DrawLine(cPt,yPt);

	//axis-z: blue
	SetColor(RGB(0,0,255));
	DrawLine(cPt,zPt);

	Lighting(bLighting);
	SetColor(old_clr);
}

//////////////////////////////////////////////////
void COpenGLDC::BeginSelection(int xPos,int yPos)
{
	m_bSelectionMode = TRUE;
	wglMakeCurrent(m_hDC,m_hRC);
	
	//set up selection buffer	
	glSelectBuffer(BUFFER_LENGTH,m_selectBuff);
	
	//switch to projection and save the matrix
	m_Camera.selection(xPos,yPos);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	InitNames();
}

int	COpenGLDC::EndSelection(UINT* items)
{
	m_bSelectionMode = FALSE;
	int hits = glRenderMode(GL_RENDER);
	for(int i=0;i<hits;i++){
		items[i] = m_selectBuff[i*4+3];
	}
	wglMakeCurrent(m_hDC,NULL);
	return hits;
}

BOOL COpenGLDC::IsSelectionMode()
{
	return m_bSelectionMode;
}

void COpenGLDC::InitNames()
{
	glInitNames();
	glPushName(0);
}

void COpenGLDC::LoadName(UINT name)
{
	glLoadName(name);
}

void COpenGLDC::PushName(UINT name)
{
	glPushName(name);
}

void COpenGLDC::PopName()
{
	glPopName();
}

