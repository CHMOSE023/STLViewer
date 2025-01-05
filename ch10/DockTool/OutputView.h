#pragma once

// OutputView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutputView view
#define UMSG_ADDSTRING			WM_USER+1
#define UMSG_REMOVEALLSTRING	WM_USER+2

class COutputView : public CScrollView
{
protected:
	COutputView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COutputView)

// Attributes
protected:
	CStringList		m_strList;
	CFont			m_Font;
	int				m_FontHeight;

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputView)
	public:
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COutputView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnAddString(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnRemoveAllString(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.