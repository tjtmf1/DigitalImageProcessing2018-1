
// DigitalImageProcessingTermProjectView.h: CDigitalImageProcessingTermProjectView 클래스의 인터페이스
//

#pragma once
#include "opencv2/opencv.hpp"
using namespace cv;

class CDigitalImageProcessingTermProjectView : public CView
{
protected: // serialization에서만 만들어집니다.
	CDigitalImageProcessingTermProjectView();
	DECLARE_DYNCREATE(CDigitalImageProcessingTermProjectView)

// 특성입니다.
public:
	CDigitalImageProcessingTermProjectDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	int videoWidth, videoHeight;								// 입력받을 영상의 사이즈 크기
	Mat background1, background2;								// 각 영상의 추출한 배경
	String filename1, filename2;								// 입력받은 영상의 경로
	VideoCapture capture1, capture2;							// 입력받은 영상의 포인터
	double MatCompare(Mat &m1, Mat &m2);						// 두 장면의 픽셀을 비교하여 유사도 계산하는 함수
	Mat ExtractSkin(Mat &input);								// input 장면의 살색영역을 추출하여 반환하는 함수
	double MotionCompare(Mat &m1, Mat &mp1, Mat &m2, Mat &mp2);	// mp1->m1 의 모션과 mp2->m2 의 모션을 비교하여 유사도를 계산 하는 함수(코사인유사도 이용)
	Point tssSearch(Mat &m1, Mat &m2, int x, int y, int windowSize, int blockSize);	// 모션 추출 기법 3ss이용
	int blockMatching(Mat &m1, Mat &m2, int x1, int y1, int x2, int y2, int blockSize);	//모션 추출 시 각 블럭의 유사도 계산
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CDigitalImageProcessingTermProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVideoLoad();
	afx_msg void OnAddedVideoLoad();
	afx_msg void OnCompare();
};

#ifndef _DEBUG  // DigitalImageProcessingTermProjectView.cpp의 디버그 버전
inline CDigitalImageProcessingTermProjectDoc* CDigitalImageProcessingTermProjectView::GetDocument() const
   { return reinterpret_cast<CDigitalImageProcessingTermProjectDoc*>(m_pDocument); }
#endif

