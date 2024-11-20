
// DrawingCircleView.cpp: CDrawingCircleView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "DrawingCircle.h"
#endif

#include "DrawingCircleDoc.h"
#include "DrawingCircleView.h"
#include "MainFrm.h"
#include <random>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawingCircleView

IMPLEMENT_DYNCREATE(CDrawingCircleView, CFormView)

BEGIN_MESSAGE_MAP(CDrawingCircleView, CFormView)

	ON_BN_CLICKED(IDC_BTN_DRAW, &CDrawingCircleView::OnBnClickedBtnDraw)
	ON_BN_CLICKED(IDC_BTN_ACTION, &CDrawingCircleView::OnBnClickedBtnAction)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CDrawingCircleView::OnBnClickedBtnOpen)
	ON_EN_CHANGE(IDC_EDIT_START_X, &CDrawingCircleView::OnEnChangeEditStartX)
	ON_EN_CHANGE(IDC_EDIT_START_Y, &CDrawingCircleView::OnEnChangeEditStartY)
	ON_EN_CHANGE(IDC_EDIT_END_X, &CDrawingCircleView::OnEnChangeEditEndX)
	ON_EN_CHANGE(IDC_EDIT_END_Y, &CDrawingCircleView::OnEnChangeEditEndY)
END_MESSAGE_MAP()

// CDrawingCircleView 생성/소멸

CDrawingCircleView::CDrawingCircleView() noexcept
	: CFormView(IDD_DRAWINGCIRCLE_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_nImageWidth = 800;
	m_nImageHeight = 600;
	m_nImageBpp = 8;
	m_nStartX = -1;
	m_nStartY = -1;
	m_nEndX = -1;
	m_nEndY = -1;
}

CDrawingCircleView::~CDrawingCircleView()
{
}

void CDrawingCircleView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CDrawingCircleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CDrawingCircleView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();


}

// CDrawingCircleView 진단

#ifdef _DEBUG
void CDrawingCircleView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDrawingCircleView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDrawingCircleDoc* CDrawingCircleView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawingCircleDoc)));
	return (CDrawingCircleDoc*)m_pDocument;
}
#endif //_DEBUG


// CDrawingCircleView 메시지 처리기


void CDrawingCircleView::OnBnClickedBtnDraw()
{
	int nRad;

	if (!CheckInput())
		return;

	if (m_Image != NULL)
	{
		m_Image.Destroy();

	}

	m_Image.Create(m_nImageWidth, -m_nImageHeight, m_nImageBpp);

	if (m_nImageBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
		{
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_Image.SetColorTable(0, 256, rgb);
	}

	unsigned char* fm = (unsigned char*)m_Image.GetBits();

	memset(fm, COLOR_WHITE, m_nImageWidth * m_nImageHeight);

	static std::mt19937 mt(1729);
	std::uniform_int_distribution<int> dist(5, 30);
	nRad = dist(mt);

	DrawCircle(fm, m_nStartX, m_nStartY, nRad, COLOR_GRAY);

	UpdateDisplay();
}

void CDrawingCircleView::OnBnClickedBtnAction()
{
	CMainFrame* pFrame = ((CMainFrame*)AfxGetApp()->m_pMainWnd);

	CString sSavePath;
	CString sSaveFullPath;
	CTime t = t.GetCurrentTime();
	int n = 0;

	if (!CheckInput())
		return;

	while((m_nStartX != m_nEndX) || (m_nStartY != m_nEndY))
	{
		if(m_nStartX != m_nEndX)
		{
			m_nStartX = ChangePos(m_nStartX, m_nEndX);
		}

		if (m_nStartY != m_nEndY)
		{
			m_nStartY = ChangePos(m_nStartY, m_nEndY);
		}
		MoveRect();


		sSavePath.Format("%s\\image", pFrame->m_sProgramFolderPath);
		pFrame->CreateFolder(sSavePath);
		sSaveFullPath.Format("%s\\save_image%d_%04d%02d%02d-%02d%02d%02d.bmp", sSavePath, ++n, t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		m_Image.Save(sSaveFullPath);

		Sleep(100);
	}
}

void CDrawingCircleView::OnBnClickedBtnOpen()
{
	CString strExt = "";
	CString strFullPath = "", strFolderPath = "";
	CString strFile = "";
	CString strFolder = "";
	CString sStr;

	CFileDialog dlg(TRUE, "Image File", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files (*.*)|*.*||", NULL);

	if (dlg.DoModal() == IDOK)
	{
		strFullPath = dlg.GetPathName();
		strExt = dlg.GetFileExt();
		strFile = dlg.GetFileName();
		strFolder.Format("%s", strFullPath.Left(strFullPath.Find("\\" + strFile)));

		if (m_Image != NULL)
		{
			m_Image.Destroy();
		}
		m_Image.Load(strFullPath);

		UpdateDisplay();
	}
}

void CDrawingCircleView::UpdateDisplay()
{
	CClientDC dc(this);
	m_Image.Draw(dc, 0, 0);
}

void CDrawingCircleView::MoveRect()
{
	int nPitch = m_Image.GetPitch();
	unsigned char* fm = (unsigned char*)m_Image.GetBits();
	int nRad;
	static std::mt19937 mt(1729);
	std::uniform_int_distribution<int> dist(5, 30);
	nRad = dist(mt);

	memset(fm, COLOR_WHITE, m_nImageWidth * m_nImageHeight);

	DrawCircle(fm, m_nStartX, m_nStartY, nRad, COLOR_GRAY);

	UpdateDisplay();
}

BOOL CDrawingCircleView::ValidImagePos(int nXpos, int nYpos)
{
	CRect rect(0, 0, m_nImageWidth, m_nImageHeight);

	return rect.PtInRect(CPoint(nXpos, nYpos));
}

void CDrawingCircleView::DrawCircle(unsigned char* fm, int nXpos, int nYpos, int nR, int nColor)
{
	int nCenterX = nXpos;
	int nCenterY = nYpos;
	int nPitch = m_Image.GetPitch();

	for (int i = nXpos - nR; i < nXpos + nR; i++)
	{
		for (int j = nYpos - nR; j < nYpos + nR; j++)
		{
			if (ValidImagePos(i, j))
			{
				if(IsinCircle(i, j, nCenterX, nCenterY, nR))
				{
					fm[j * nPitch + i] = nColor;
				}
			}

		}
	}
}

BOOL CDrawingCircleView::IsinCircle(int nXpos, int nYpos, int nCenterXpos, int nCenterYpos, int nR)
{
	double dbX = nXpos - nCenterXpos;
	double dbY = nYpos - nCenterYpos;
	double dbDist = (dbX * dbX) + (dbY * dbY);

	if ((dbDist >= nR * nR))
	{
		return FALSE;
	}

	return TRUE;
}

void CDrawingCircleView::OnEnChangeEditStartX()
{
	CString sStr;

	UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT_START_X, sStr);

	m_nStartX = atoi(sStr);
}

void CDrawingCircleView::OnEnChangeEditStartY()
{
	CString sStr;

	UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT_START_Y, sStr);

	m_nStartY = atoi(sStr);
}

void CDrawingCircleView::OnEnChangeEditEndX()
{
	CString sStr;

	UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT_END_X, sStr);

	m_nEndX = atoi(sStr);
}

void CDrawingCircleView::OnEnChangeEditEndY()
{
	CString sStr;

	UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT_END_Y, sStr);

	m_nEndY = atoi(sStr);
}

BOOL CDrawingCircleView::CheckInput()
{
	CString sStr;

	if (m_nStartX == -1)
	{
		AfxMessageBox("시작 좌표 X를 입력해주세요.");
		return FALSE;
	}

	if (m_nStartY == -1)
	{
		AfxMessageBox("시작 좌표 Y를 입력해주세요.");
		return FALSE;
	}

	if (m_nEndX == -1)
	{
		AfxMessageBox("종료 좌표 X를 입력해주세요.");
		return FALSE;
	}

	if (m_nEndY == -1)
	{
		AfxMessageBox("종료 좌표 Y를 입력해주세요.");
		return FALSE;
	}

	if (m_nStartX < 0 || m_nStartX > m_nImageWidth)
	{
		AfxMessageBox("시작 좌표 X가 범위를 벗어났습니다.(0 ~ 800)");
		SetDlgItemText(IDC_EDIT_START_X, "");
		m_nStartX = -1;
		return FALSE;
	}

	if (m_nStartY < 0 || m_nStartY > m_nImageHeight)
	{
		AfxMessageBox("시작 좌표 Y가 범위를 벗어났습니다.(0 ~ 640)");
		SetDlgItemText(IDC_EDIT_START_Y, "");
		m_nStartY = -1;
		return FALSE;
	}

	if (m_nEndX < 0 || m_nEndX > m_nImageWidth)
	{
		AfxMessageBox("종료 좌표 X가 범위를 벗어났습니다.(0 ~ 800)");
		SetDlgItemText(IDC_EDIT_END_X, "");
		m_nEndX = -1;
		return FALSE;
	}

	if (m_nEndY < 0 || m_nEndY > m_nImageHeight)
	{
		AfxMessageBox("종료 좌표 Y가 범위를 벗어났습니다.(0 ~ 640)");
		SetDlgItemText(IDC_EDIT_END_Y, "");
		m_nEndY = -1;
		return FALSE;
	}

	GetDlgItemText(IDC_EDIT_START_X, sStr);
	m_nStartX = atoi(sStr);
	GetDlgItemText(IDC_EDIT_START_Y, sStr);
	m_nStartY = atoi(sStr);

	return TRUE;
}

int CDrawingCircleView::ChangePos(int nStartPos, int nEndPos)
{
	int nResult;

	if (nStartPos < nEndPos)
	{
		if ((nEndPos - nStartPos) >= 100)
		{
			nStartPos += 80;
		}
		else if ((nEndPos - nStartPos) >= 50 && (nEndPos - nStartPos) < 100)
		{
			nStartPos += 30;
		}
		else if ((nEndPos - nStartPos) >= 30 && (nEndPos - nStartPos) < 50)
		{
			nStartPos += 10;
		}
		else if ((nEndPos - nStartPos) >= 10 && (nEndPos - nStartPos) < 30)
		{
			nStartPos += 8;
		}
		else if ((nEndPos - nStartPos) >= 3 && (nEndPos - nStartPos) < 10)
		{
			nStartPos += 3;
		}
		else
		{
			nStartPos++;
		}
	}
	else
	{
		if ((nStartPos - nEndPos) >= 100)
		{
			nStartPos -= 80;
		}
		else if ((nStartPos - nEndPos) >= 50 && (nStartPos - nEndPos) < 100)
		{
			nStartPos -= 30;
		}
		else if ((nStartPos - nEndPos) >= 30 && (nStartPos - nEndPos) < 50)
		{
			nStartPos -= 10;
		}
		else if ((nStartPos - nEndPos) >= 10 && (nStartPos - nEndPos) < 30)
		{
			nStartPos -= 8;
		}
		else if ((nStartPos - nEndPos) >= 3 && (nStartPos - nEndPos) < 10)
		{
			nStartPos -= 3;
		}
		else
		{
			nStartPos--;
		}
	}
	
	return nStartPos;
}