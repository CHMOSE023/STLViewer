#pragma once

// DlgEntityName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEntityName dialog

class CDlgEntityName : public CDialog
{
// Construction
public:
	CDlgEntityName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEntityName)
	enum { IDD = IDD_ENTITY_NAME };
	CString	m_EntityName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEntityName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEntityName)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.