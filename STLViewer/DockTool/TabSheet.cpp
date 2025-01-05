// TabSheet.cpp : implementation file
//

#include "stdafx.h"
#include "docktool\DockTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSheet

CTabSheet::CTabSheet()
{
	m_curView = NULL;
	m_iMargin = 5;
}

CTabSheet::~CTabSheet()
{
//	for(int i=0;i<m_arrView.GetSize();i++)
//		delete m_arrView[i];
	m_arrView.RemoveAll();
}


BEGIN_MESSAGE_MAP(CTabSheet, CTabCtrl)
	//{{AFX_MSG_MAP(CTabSheet)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSheet message handlers

void CTabSheet::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nView = GetCurSel();
	SetActiveView(nView);
	*pResult = 0;
}

void CTabSheet::RemoveAllPage()
{
	m_arrView.RemoveAll();
}

///////////////////////////////////////////////////////////////////
//  virtual functions
///////////////////////////////////////////////////////////////////

void CTabSheet::OnSize(UINT nType,int cx,int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);
	m_rcView.left  = m_iMargin;
	m_rcView.right = cx-m_iMargin;
	m_rcView.top   = m_iMargin+23;
	m_rcView.bottom= cy-m_iMargin;
	if(m_curView){
		m_curView->SetWindowPos(NULL,m_rcView.left,m_rcView.top,
			m_rcView.Width(),m_rcView.Height(),SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
	}
}

BOOL CTabSheet::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	if(CTabCtrl::Create(dwStyle,rect,pParentWnd,nID)){
		InitFont();
		return TRUE;
	}
	else
		return FALSE;
}

//////////
//////////////////////////////////////////////////
// The remainder of this class was written by Dirk Clemens...

BOOL CTabSheet::AddView(LPTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext)
{	
	
#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif
	
	CCreateContext context;
	if (pContext == NULL)
	{
		// *** LiangYiBin.Donald
		context.m_pCurrentDoc = NULL;
		context.m_pCurrentFrame = GetParentFrame();
		context.m_pLastView = NULL;
		context.m_pNewDocTemplate = NULL;
		context.m_pNewViewClass = pViewClass;
		pContext = &context;
	}
	
	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL
		
	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created
	
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;

	// Create with the right size and position
	if (!pWnd->Create(NULL, NULL, dwStyle, m_rcView, this, 0, pContext))
	{
		TRACE0("Warning: couldn't create client pane for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	CView* pView = (CView*) pWnd;

	TC_ITEM ti;
	ti.mask=TCIF_TEXT;

	ti.pszText= lpszLabel;
	InsertItem(m_arrView.GetSize(),&ti);
	m_arrView.Add(pView);

/*	if(m_arrView.GetSize() != 1){
		pView->EnableWindow(FALSE);
		pView->ShowWindow(SW_HIDE);
	}
	else{
		SetActiveView(0);
	}
*/
	SetActiveView(m_arrView.GetSize()-1);
	return TRUE;
}

BOOL CTabSheet::AddView(LPTSTR lpszLabel, CWnd* pView)
{
	TC_ITEM ti;
	ti.mask=TCIF_TEXT;

	ti.pszText= lpszLabel;
	InsertItem(m_arrView.GetSize(),&ti);
	m_arrView.Add(pView);

	SetActiveView(m_arrView.GetSize()-1);

	CRect rect;
	this->GetClientRect(&rect);
	int cx = rect.Width();
	int cy = rect.Height();

	m_rcView.left  = m_iMargin;
	m_rcView.right = cx-m_iMargin;
	m_rcView.top   = m_iMargin+23;
	m_rcView.bottom= cy-m_iMargin;
	if(m_curView){
		m_curView->SetWindowPos(NULL,m_rcView.left,m_rcView.top,
			m_rcView.Width(),m_rcView.Height(),SWP_NOZORDER|SWP_SHOWWINDOW);
	}

	return TRUE;
}

void CTabSheet::SetMargin(int margin)
{
	m_iMargin = margin;
}

void CTabSheet::SetActiveView(int nView)
{
	SetCurSel(nView);
	ASSERT(nView>=0 && nView<m_arrView.GetSize());
	CWnd* pView = m_arrView[nView];
	if(m_curView && m_curView != pView){
		m_curView->EnableWindow(FALSE);
		m_curView->ShowWindow(SW_HIDE);
	}
	pView->EnableWindow(TRUE);
	pView->ShowWindow(SW_SHOW);
	pView->SetFocus();
	pView->SetWindowPos(NULL,m_rcView.left,m_rcView.top,
			m_rcView.Width(),m_rcView.Height(),SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
	m_curView = pView;
}


void CTabSheet::InitFont()
{
	// Get the log font.
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), &ncm, 0));
	
	m_TabFont.CreateFontIndirect(&ncm.lfMessageFont);
	this->SetFont(&m_TabFont);
}