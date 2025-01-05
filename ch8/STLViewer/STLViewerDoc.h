// STLViewerDoc.h : interface of the CSTLViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GeomKernel\Entity.h"

class CSTLViewerDoc : public CDocument
{
protected: // create from serialization only
	CSTLViewerDoc();
	DECLARE_DYNCREATE(CSTLViewerDoc)

// Attributes
public:
	CPart	m_Part;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTLViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTLViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSTLViewerDoc)
	afx_msg void OnStlFilein();
	afx_msg void OnDefFilein();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
