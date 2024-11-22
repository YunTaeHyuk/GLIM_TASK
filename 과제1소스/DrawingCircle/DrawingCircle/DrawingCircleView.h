
// DrawingCircleView.h: CDrawingCircleView 클래스의 인터페이스
//

#pragma once


class CDrawingCircleView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CDrawingCircleView() noexcept;
	DECLARE_DYNCREATE(CDrawingCircleView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_DRAWINGCIRCLE_FORM };
#endif

// 특성입니다.
public:
	CDrawingCircleDoc* GetDocument() const;

// 작업입니다.
public:
	int			m_nImageWidth;
	int			m_nImageHeight;
	int			m_nImageBpp;

	int			m_nStartX;
	int			m_nStartY;
	int			m_nEndX;
	int			m_nEndY;

	CxStatic	m_lblCenterMarker;
	CxStatic	m_lblCenterPos;
private:
	CImage		m_Image;

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CDrawingCircleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDraw();
	afx_msg void OnBnClickedBtnAction();
	afx_msg void OnBnClickedBtnOpen();

	void UpdateDisplay();
	void MoveRect();
	BOOL ValidImagePos(int nXpos, int nYpos);
	void DrawCircle(unsigned char* fm, int nXpos, int nYpos, int nR, int nColor);
	BOOL IsinCircle(int nXpos, int nYpos, int nCenterXpos, int nCenterYpos, int nR);
	BOOL CheckInput();
	int ChangePos(int nStartPos, int nEndPos);
	void FindCenter();
	afx_msg void OnEnChangeEditStartX();
	afx_msg void OnEnChangeEditStartY();
	afx_msg void OnEnChangeEditEndX();
	afx_msg void OnEnChangeEditEndY();
};

#ifndef _DEBUG  // DrawingCircleView.cpp의 디버그 버전
inline CDrawingCircleDoc* CDrawingCircleView::GetDocument() const
   { return reinterpret_cast<CDrawingCircleDoc*>(m_pDocument); }
#endif

