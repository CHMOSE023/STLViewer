// OpenGLDC.h: interface for the COpenGLDC class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _OPENGLDC_H
#define _OPNEGLDC_H

#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glaux.h"

#include "camera.h"

class COpenGLDC : public CObject  
{
public:
	COpenGLDC(HWND hWnd);
	virtual ~COpenGLDC();

private:
	HWND	m_hWnd;
	HGLRC	m_hRC;
	HDC		m_hDC;

public:
	GCamera		m_Camera;

public:
	//initialize
	BOOL InitDC();
	void GLResize(int cx,int cy);
	void GLSetupRC();

	void Ready();
	void Finish();
};

#endif
