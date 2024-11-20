
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "DrawingCircle.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//실행파일 경로
	char	name[256];
	GetModuleFileName(NULL, name, 256);
	m_sProgramFullPath = (LPSTR)name;
	m_sProgramFolderPath = m_sProgramFullPath.Left(m_sProgramFullPath.ReverseFind('\\'));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	//프로그램 제목 설정
	CString strName;
	cs.style &= (~FWS_ADDTOTITLE & ~WS_THICKFRAME  & ~WS_MAXIMIZEBOX);
	strName.Format("%s Ver. %s (Release Date: %s)", APP_NAME, APP_VER, APP_DATE);
	SetTitle(strName); ;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::CreateFolder(CString sPath)
{
	if (GetFileAttributes(sPath) == INVALID_FILE_ATTRIBUTES)
		CreateDirectory(sPath, NULL);
}

