// OutputView.cpp : implementation file
//

#include "stdafx.h"
#include "OutputView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputView

IMPLEMENT_DYNCREATE(COutputView, CScrollView)

COutputView::COutputView()
{
	m_FontHeight = 10;
}

COutputView::~COutputView()
{
	m_strList.RemoveAll();
}


BEGIN_MESSAGE_MAP(COutputView, CScrollView)
	//{{AFX_MSG_MAP(COutputView)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UMSG_ADDSTRING,OnAddString)
	ON_MESSAGE(UMSG_REMOVEALLSTRING,OnRemoveAllString)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputView drawing

void COutputView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	CFont *oldFont = pDC->SelectObject(&m_Font);

	CString str;
	int y=0;
	POSITION pos = m_strList.GetHeadPosition();
	while(pos){
		str = m_strList.GetNext(pos);
		pDC->TextOut(0,y,str);
		y += m_FontHeight;
	}
	pDC->SelectObject(oldFont);
}

/////////////////////////////////////////////////////////////////////////////
// COutputView diagnostics

#ifdef _DEBUG
void COutputView::AssertValid() const
{
	CScrollView::AssertValid();
}

void COutputView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COutputView message handlers

void COutputView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	SetScrollSizes(MM_TEXT,CSize(500,200));
}

int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
		// Get the log font.
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), &ncm, 0));
	
	m_Font.CreateFontIndirect(&ncm.lfMessageFont);
	LOGFONT logFont;
	m_Font.GetLogFont(&logFont);
	m_FontHeight = -logFont.lfHeight;

	return 0;
}

LRESULT COutputView::OnAddString(WPARAM wParam,LPARAM lParam)
{
	const CString* str = (const CString*)wParam;
	m_strList.AddTail(*str);

	CSize size = GetTotalSize();
	int height = m_strList.GetCount()*m_FontHeight;
	if(height > size.cy){
		SetScrollSizes(MM_TEXT,CSize(500,height));
		POINT pt;
		pt.x = 0;
		pt.y = height;
		ScrollToPosition(pt);
	}

	Invalidate(FALSE);

	MSG msg;
	if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}
	return 0;
}

LRESULT COutputView::OnRemoveAllString(WPARAM wParam,LPARAM lParam)
{
	m_strList.RemoveAll();
	SetScrollSizes(MM_TEXT,CSize(500,200));
	Invalidate(FALSE);

	MSG msg;
	if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}
	return 0;
}
