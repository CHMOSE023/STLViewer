// LeftDockBar.cpp: implementation of the CLeftDockBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "docktool\DockTool.h"
#include <afxpriv.h>
#include "OutputView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CTabSheetDockBar,CCoolDialogBar )
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTabSheetDockBar::CTabSheetDockBar()
{
}

CTabSheetDockBar::~CTabSheetDockBar()
{
}


BEGIN_MESSAGE_MAP(CTabSheetDockBar, CCoolDialogBar)
	//{{AFX_MSG_MAP(CCoolDialogDockBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//message handlers
BOOL CTabSheetDockBar::Create( CWnd* pParentWnd,LPCTSTR pTitle)
{

	ASSERT_VALID(pParentWnd);
	if(!pParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		return FALSE;

	UINT nStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_LEFT |
		CBRS_ALIGN_RIGHT| CBRS_FLYBY | CBRS_SIZE_DYNAMIC;

	return CCoolDialogBar::Create(pParentWnd, pTitle, nStyle);
}

//add in functions
void CTabSheetDockBar::SetFloatSize(const CSize& size)
{
	m_sizeFloat = size;
}

void CTabSheetDockBar::SetDockSize(const CSize& size)
{
	m_sizeVert = size;
}

int CTabSheetDockBar::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CCoolDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rect;
	GetClientRect(&rect);
	rect.left += 15;
	rect.top += 15;
	rect.bottom -= 15;
	rect.right -= 15;

	m_TabCtrl.Create(WS_CHILD|WS_VISIBLE,rect,this,10071);
	m_pCtrlWnd = &m_TabCtrl;
	return 0;
}

//////////////////////////////////////////////////////////////
//					CMessageViewDockBar
//////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMessageViewDockBar,CCoolDialogBar )


// Construction/Destruction
CMessageViewDockBar::CMessageViewDockBar():CCoolDialogBar()
{
	m_pOutputView = NULL;
}

CMessageViewDockBar::~CMessageViewDockBar()
{
}


BEGIN_MESSAGE_MAP(CMessageViewDockBar, CCoolDialogBar)
	//{{AFX_MSG_MAP(CMessageViewDockBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMessageViewDockBar::Create( CWnd* pParentWnd,LPCTSTR pTitle)
{
	ASSERT_VALID(pParentWnd);
	if(!pParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		return FALSE;

	UINT nStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM |
		CBRS_ALIGN_TOP| CBRS_FLYBY | CBRS_SIZE_DYNAMIC;

	return CCoolDialogBar::Create(pParentWnd, pTitle,nStyle);
}

int CMessageViewDockBar::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CCoolDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!AddView(RUNTIME_CLASS(COutputView),NULL)){
		TRACE0("fail to create output view\n");
		return -1;
	}

	m_pCtrlWnd = m_pOutputView;
	return 0;
}

BOOL CMessageViewDockBar::AddView(CRuntimeClass *pViewClass, CCreateContext *pContext)
{
	ASSERT(m_pOutputView == NULL);

#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif
	
	CCreateContext context;
	if (pContext == NULL)
	{
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

	RECT rcView;
	rcView.bottom = rcView.top = rcView.left = rcView.right = 0;

	// Create with the right size and position
	if (!pWnd->Create(NULL, NULL, dwStyle, rcView, this, 0, pContext))
	{
		TRACE0("Warning: couldn't create client pane for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}

	m_pOutputView = (CView*)pWnd;

	return TRUE;
}

void CMessageViewDockBar::AddMessageLine(LPCTSTR pStr)
{
	ASSERT(m_pOutputView);
	m_pOutputView->SendMessage(UMSG_ADDSTRING,(LONG)&pStr);
}

void CMessageViewDockBar::RemoveAllMessageLines()
{
	ASSERT(m_pOutputView);
	m_pOutputView->SendMessage(UMSG_REMOVEALLSTRING);
}

void CMessageViewDockBar::SetFloatSize(const CSize& size)
{
	m_sizeFloat = size;
}

void CMessageViewDockBar::SetDockSize(const CSize& size)
{
	m_sizeHorz = size;
}
