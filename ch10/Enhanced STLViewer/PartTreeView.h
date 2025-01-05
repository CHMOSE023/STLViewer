#pragma once

// PartTreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPartTreeView view
#include <afxcview.h>

class CSTLViewerDoc;
class CEntity;
class CPartTreeView : public CTreeView
{
protected:
	CPartTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPartTreeView)

// Attributes
public:

// Operations
public:
	CSTLViewerDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPartTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPartTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void	ChangeEntityName(CEntity* ent);
	void	ChangeEntityColor(CEntity* ent);
	void	ChangeEntityVisible(CEntity* ent);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPartTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CImageList	m_ImageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
