// GLView.h : interface of the CGLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVIEW_H__B2996431_1A48_492F_87B8_D8E922845B95__INCLUDED_)
#define AFX_GLVIEW_H__B2996431_1A48_492F_87B8_D8E922845B95__INCLUDED_

class COpenGLDC;
class CGLView : public CView
{
protected:
	COpenGLDC*	m_pGLDC;

protected: // create from serialization only
	CGLView();
	DECLARE_DYNCREATE(CGLView)

// Attributes
public:
	virtual void RenderScene(COpenGLDC* pDC);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGLView();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGLView)
	afx_msg int	 OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLVIEW_H__B2996431_1A48_492F_87B8_D8E922845B95__INCLUDED_)
