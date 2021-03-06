
// DigitalImageProcessingTermProjectView.cpp: CDigitalImageProcessingTermProjectView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "DigitalImageProcessingTermProject.h"
#endif

#include "DigitalImageProcessingTermProjectDoc.h"
#include "DigitalImageProcessingTermProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigitalImageProcessingTermProjectView

IMPLEMENT_DYNCREATE(CDigitalImageProcessingTermProjectView, CView)

BEGIN_MESSAGE_MAP(CDigitalImageProcessingTermProjectView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_VIDEO_LOAD, &CDigitalImageProcessingTermProjectView::OnVideoLoad)
	ON_COMMAND(ID_ADDED_VIDEO_LOAD, &CDigitalImageProcessingTermProjectView::OnAddedVideoLoad)
	ON_COMMAND(ID_Compare, &CDigitalImageProcessingTermProjectView::OnCompare)
END_MESSAGE_MAP()

// CDigitalImageProcessingTermProjectView 생성/소멸

CDigitalImageProcessingTermProjectView::CDigitalImageProcessingTermProjectView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CDigitalImageProcessingTermProjectView::~CDigitalImageProcessingTermProjectView()
{
}

BOOL CDigitalImageProcessingTermProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CDigitalImageProcessingTermProjectView 그리기

void CDigitalImageProcessingTermProjectView::OnDraw(CDC* pDC)
{
	CDigitalImageProcessingTermProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CDigitalImageProcessingTermProjectView 인쇄

BOOL CDigitalImageProcessingTermProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CDigitalImageProcessingTermProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CDigitalImageProcessingTermProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CDigitalImageProcessingTermProjectView 진단

#ifdef _DEBUG
void CDigitalImageProcessingTermProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CDigitalImageProcessingTermProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDigitalImageProcessingTermProjectDoc* CDigitalImageProcessingTermProjectView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitalImageProcessingTermProjectDoc)));
	return (CDigitalImageProcessingTermProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CDigitalImageProcessingTermProjectView 메시지 처리기


void CDigitalImageProcessingTermProjectView::OnVideoLoad()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, ".avi", NULL, NULL, "AVI File (*.avi)|*.avi||");
	if (IDOK != dlg.DoModal())
		return;
	//파일 경로 입력
	CString cfilename = dlg.GetPathName();
	CT2CA strAtl(cfilename);
	filename1 = cfilename;

	//AVI 파일 로드
	capture1.open(filename1);
	//파일에 문제가 있는 경우, 에러 메시지 표시 후 종료
	if (!capture1.isOpened())
	{
		AfxMessageBox("Error Video");
		return;
	}

	//opencv MOG2를 이용하여 500~1500프레임의 배경을 추출한다.
	//추출한 배경은 background1에 저장
	Ptr<BackgroundSubtractor> MOG2 = createBackgroundSubtractorMOG2();
	MOG2.dynamicCast<BackgroundSubtractorMOG2>()->setDetectShadows(false);
	MOG2.dynamicCast<BackgroundSubtractorMOG2>()->setHistory(1000);
	MOG2.dynamicCast<BackgroundSubtractorMOG2>()->setBackgroundRatio(1);
	int frames = 0;
	while (true)
	{
		Mat back;
		Mat frame;
		capture1 >> frame;
		if (frame.data == nullptr)
			break;
		frames++;
		if (frames <= 500)
			continue;
		MOG2->apply(frame, back);
		if (frames == 1500)
			break;
	}
	capture1.release();
	capture1.open(filename1);
	MOG2->getBackgroundImage(background1);

}


void CDigitalImageProcessingTermProjectView::OnAddedVideoLoad()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, ".avi", NULL, NULL, "AVI File (*.avi)|*.avi||");
	if (IDOK != dlg.DoModal())
		return;
	//파일 경로 입력
	CString cfilename = dlg.GetPathName();
	CT2CA strAtl(cfilename);
	filename2 = cfilename;

	//AVI 파일 로드
	capture2.open(filename2);
	//파일에 문제가 있는 경우, 에러 메시지 표시 후 종료
	if (!capture2.isOpened())
	{
		AfxMessageBox("Error Video");
		return;
	}


	//opencv MOG2를 이용하여 500~1500프레임의 배경을 추출한다.
	//추출한 배경은 background2에 저장
	Ptr<BackgroundSubtractor> MOG2 = createBackgroundSubtractorMOG2();
	MOG2.dynamicCast<BackgroundSubtractorMOG2>()->setDetectShadows(false);
	MOG2.dynamicCast<BackgroundSubtractorMOG2>()->setHistory(1000);
	MOG2.dynamicCast<BackgroundSubtractorMOG2>()->setBackgroundRatio(1);
	int frames = 0;
	while (true)
	{
		Mat back;
		Mat frame;
		capture2 >> frame;
		if (frame.data == nullptr)
			break;
		frames++;
		if (frames <= 500)
			continue;
		MOG2->apply(frame, back);
		if (frames == 1500)
			break;
	}
	capture2.release();
	capture2.open(filename2);
	MOG2->getBackgroundImage(background2);
	cvNamedWindow("Original Video");
	cvNamedWindow("Compared Video");
	cvResizeWindow("Original Video", 640, 360);
	cvResizeWindow("Compared Video", 640, 360);
}


void CDigitalImageProcessingTermProjectView::OnCompare()
{
	boolean check = false;

	Mat lastFrame1, lastFrame2;
	
	
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	while (true)
	{
		Mat temp1, temp2;

		Mat frame1;
		capture1 >> frame1;
		if (frame1.data == nullptr)
			break;
		Mat frame2;
		capture2 >> frame2;
		if (frame2.data == nullptr)
			break;

		//img1, img2에 원본-배경을 저장
		Mat img1 = background1 - frame1;
		Mat img2 = background2 - frame2;
		dilate(img1, img1, cv::Mat(), cv::Point(-1, -1), 2);
		dilate(img2, img2, cv::Mat(), cv::Point(-1, -1), 2);

		//연산횟수를 줄이기 위해 그레이스케일로 변환하여 계산
		cvtColor(img1, img1, CV_RGB2GRAY);
		cvtColor(img2, img2, CV_RGB2GRAY);
		temp1 = img1;
		temp2 = img2;

		//쓰레스홀드 적용
		threshold(img1, img1, 20, 255, CV_THRESH_BINARY);
		threshold(img2, img2, 20, 255, CV_THRESH_BINARY);

		//살색 추출한 이미지를 더해줌.
		add(img1, ExtractSkin(frame1), img1);
		//add(img2, ExtractSkin(frame2), img2);

		//Max filter 적용 후 Connected Components Labeling 진행
		dilate(img1, img1, cv::Mat(), cv::Point(-1, -1), 3);
		dilate(img2, img2, cv::Mat(), cv::Point(-1, -1), 3);
		Mat img_labels1, stats1, centroids1;
		Mat img_labels2, stats2, centroids2;
		int numOfLables1 = connectedComponentsWithStats(img1, img_labels1, stats1, centroids1, 8, CV_32S);
		int numOfLables2 = connectedComponentsWithStats(img2, img_labels2, stats2, centroids2, 8, CV_32S);

		Mat sub1, sub2;
		for (int j = 1; j < numOfLables1; j++) {
			int area = stats1.at<int>(j, CC_STAT_AREA);
			int left = stats1.at<int>(j, CC_STAT_LEFT);
			int top = stats1.at<int>(j, CC_STAT_TOP);
			int width = stats1.at<int>(j, CC_STAT_WIDTH);
			int height = stats1.at<int>(j, CC_STAT_HEIGHT);

			//라벨링된 영역의 크기, 높이, 폭을 보고 사람이 아니라고 판단되면 패스함.
			if (area < 3000 || height < 80 || width > 400)
				continue;
			rectangle(frame1, Point(left, top), Point(left + width, top + height), Scalar(0, 0, 0), 1);
			sub1 = img1(Rect(left, top, width, height));
			temp1 = temp1(Rect(left, top, width, height));
			break;
		}
		for (int j = 1; j < numOfLables2; j++) {
			int area = stats2.at<int>(j, CC_STAT_AREA);
			int left = stats2.at<int>(j, CC_STAT_LEFT);
			int top = stats2.at<int>(j, CC_STAT_TOP);
			int width = stats2.at<int>(j, CC_STAT_WIDTH);
			int height = stats2.at<int>(j, CC_STAT_HEIGHT);

			if (area < 3000 || height < 80 || width > 400)
				continue;
			rectangle(frame2, Point(left, top), Point(left + width, top + height), Scalar(0, 0, 0), 1);
			sub2 = img2(Rect(left, top, width, height));
			temp2 = temp2(Rect(left, top, width, height));
			break;
		}
		if (sub1.data == nullptr)
			sub1 = Mat::zeros(Size(129, 129), img1.type());
		if (sub2.data == nullptr)
			sub2 = Mat::zeros(Size(129, 129), img2.type());
		
		//이전프레임과 현재프레임을 scaling을 통해 크기를 맞춰줌
		resize(sub1, sub1, Size(129, 129), 0, 0, INTER_CUBIC);
		resize(sub2, sub2, Size(129, 129), 0, 0, INTER_CUBIC);
		resize(temp1, temp1, Size(129, 129), 0, 0, INTER_CUBIC);
		resize(temp2, temp2, Size(129, 129), 0, 0, INTER_CUBIC);

		double motionSimilar = 0.0;

		//check는 첫프레임인지 확인하기 위해 사용됨.
		//이전프레임과 현재프레임을 비교 후 motionSimilarity를 계산
		if (check)
			motionSimilar = MotionCompare(temp1, lastFrame1, temp2, lastFrame2);

		lastFrame1 = temp1;
		lastFrame2 = temp2;

		if (!check)
			check = true;

		//현재프레임을 비교하여 pixel similarity를 계산
		double similar = MatCompare(sub1, sub2);
		CString str;

		//Motion Similarity와 Pixel Similarity에 적절한 가중치를 곱하여 영상의 유사도 계산
		str.Format(_T("Similar Rate : %f"), similar * 0.9 + motionSimilar * 0.1);
		String st(str);
		putText(frame2, st, Point(10, 40), 2, 1, Scalar::all(255));
		imshow("Original Video", frame1);
		imshow("Compared Video", frame2);
		if (waitKey(1) >= 0)
			break;
	}
}
double CDigitalImageProcessingTermProjectView::MatCompare(Mat &m1, Mat &m2)
{
	int matchCount = 0;
	int totalCount = 0;
	for (int i = 0; i < 129; i++)
	{
		for (int j = 0; j < 129; j++)
		{
			//얼굴~몸통으로 추정되는 영역은 무시
			if (i >= 43 && i < 86)
				continue;
			if (j >= 0 && j < 86)
				continue;

			//totalCount = m1의 픽셀중 값이 255인 픽셀의 개수
			//matchCOunt = m1와 m2모두 픽셀 값이 255인 픽셀의 개수
			if (m1.at<uchar>(i, j) == 255)
			{
				totalCount += 1;
				if (m2.at<uchar>(i, j) == 255)
					matchCount += 1;
			}
		}
	}
	return (double)matchCount / (double)totalCount * 100.0;
}

Mat CDigitalImageProcessingTermProjectView::ExtractSkin(Mat &input)
{
	float lowR = 190;
	float lowG = 178;
	float lowB = 156;

	int highR = 227;
	int highG = 220;
	int highB = 201;

	float lowY = 0.257*lowR + 0.504*lowG + 0.098*lowB + 16;
	float lowCb = -0.148*lowR - 0.291*lowG + 0.439*lowB + 128;
	float lowCr = 0.439*lowR - 0.368*lowG - 0.071*lowB + 128;

	float highY = 0.257*highR + 0.504*highG + 0.098*highB + 16;
	float highCb = -0.148*highR - 0.291*highG + 0.439*highB + 128;
	float highCr = 0.439*highR - 0.368*highG - 0.071*highB + 128;

	Mat output;

	cvtColor(input, output, CV_BGR2YCrCb);
	//255, 173, 127
	inRange(output, Scalar(0, lowCr, lowCb), Scalar(255, 173, 127), output);
	//morphological opening 작은 점들을 제거 


	//output = (output.size(), CV_8UC3, cv::Scalar(0));

	erode(output, output, cv::Mat());
	//add(input, Scalar(0), output, output);
	threshold(output, output, 135, 255, CV_THRESH_BINARY);

	return output;
}
double CDigitalImageProcessingTermProjectView::MotionCompare(Mat &m1, Mat &mp1, Mat &m2, Mat &mp2)
{
	static const int BLOCK_SIZE = 16;
	static const int WINDOW_SIZE = 8;
	int diffSum = 0;
	int sum=0;

	for (int i = WINDOW_SIZE + BLOCK_SIZE / 2; i < 128 - (BLOCK_SIZE / 2 + WINDOW_SIZE); i += BLOCK_SIZE)
	{
		for (int j = BLOCK_SIZE + WINDOW_SIZE / 2; j < 128 - (BLOCK_SIZE / 2 + WINDOW_SIZE); j += BLOCK_SIZE)
		{
			//블럭에서 각 영상의 모션벡터 계산
			Point original = tssSearch(m1, mp1, j, i, WINDOW_SIZE, BLOCK_SIZE) - Point(j, i);
			Point compared = tssSearch(m2, mp2, j, i, WINDOW_SIZE, BLOCK_SIZE) - Point(j, i);
			
			//두 영상에서 한 블럭의 모션벡터가 둘다 0이면
			if (original == Point(0, 0) || compared == Point(0, 0))
			{
				sum += 1;
				continue;
			}

			//내적과 벡터의 크기를 이용하는 코사인유사도를 사용하여 diffSum 계산 (0~1값으로 정규화)
			diffSum += ((original.x * compared.x + original.y * compared.y) / sqrt(original.x * original.x + original.y * original.y)
				/ sqrt(compared.x * compared.x + compared.y * compared.y) + 1) / 2;
			sum += 1;
		}
	}
	return (diffSum / (double)sum) * 100.0;
}

Point CDigitalImageProcessingTermProjectView::tssSearch(Mat &m1, Mat &m2, int x, int y, int windowSize, int blockSize)
{
	windowSize /= 2;
	if (windowSize == 0)
		return Point(x, y);

	Point match(x, y);
	int min = 99999999;
	for (int i = -1 * windowSize; i <= windowSize; i += windowSize)
	{
		for (int j = -1 * windowSize; j <= windowSize; j += windowSize)
		{
			int value = blockMatching(m1, m2, x, y, x + j, y + i, blockSize);
			if (value < min)
			{
				min = value;
				match.x = x + j;
				match.y = y + i;
			}
		}
	}
	return tssSearch(m1, m2, match.x, match.y, windowSize, blockSize);
}

int CDigitalImageProcessingTermProjectView::blockMatching(Mat &m1, Mat &m2, int x1, int y1, int x2, int y2, int blockSize)
{
	int sum = 0;
	for (int i = -blockSize / 2; i < blockSize / 2; i++)
	{
		for (int j = -blockSize / 2; j < blockSize / 2; j++)
		{
			sum += m1.at<uchar>(y1 + i, x1 + j);
			sum -= m2.at<uchar>(y2 + i, x2 + j);
		}
	}

	return (sum > 0) ? sum : -sum;
}