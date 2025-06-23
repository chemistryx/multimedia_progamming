#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

// 선에 대한 정보를 저장할 구조체 선언
typedef struct _Stroke {
    CvPoint* points; // 선을 구성하는 점들을 저장할 배열 포인터
    int pointsCount; // 점 배열의 크기
    CvScalar color; // 선의 색상
    int radius; // 선의 반지름
} Stroke;

/**
 * Drawing Mode에 따라 그리기를 수행하는 함수
 * @param IplImage* src - 원본 이미지
 * @param int R[] - 붓 크기를 담은 배열
 * @param int brushCount - 붓 개수
 * @param drawingMode - 그리기 모드 (0: circle, 1: stroke)
 *
 * @return IplImage* - 그려진 이미지
 */
IplImage* paint(IplImage* src, int R[], int brushCount, int drawingMode);

/**
 * 특정 붓 크기로 그리기를 수행하는 함수
 * @param IplImage* canvas - 그리기를 수행할 대상 이미지
 * @param IplImage* referenceImage - 그리기를 수행할 때 참조할 이미지
 * @param int R - 붓 크기
 * @param drawingMode - 그리기 모드 (0: circle, 1: stroke)
 */
void paintLayer(IplImage* canvas, IplImage* referenceImage, int R, int drawingMode);

/**
 * 단일 원 형태의 선을 생성하는 함수
 * @param int x0 - 시작 x 좌표
 * @param int y0 - 시작 y 좌표
 * @param int R - 붓 크기
 * @param IplImage* referenceImage - 참조 이미지
 *
 * @return Stroke - 생성된 선 구조체
 */
Stroke makeStroke(int x0, int y0, int R, IplImage* referenceImage);

/**
 * 곡선 형태의 Spline 기반 선을 생성하는 함수
 * @param int x0 - 시작 x좌표
 * @param int y0 - 시작 y좌표
 * @param int R - 붓 크기
 * @param IplImage* referenceImage - 참조 이미지
 * @param IplImage* canvas - 현재 캔버스 이미지
 *
 * @return Stroke - 생성된 선 구조체
 */
Stroke makeSplineStroke(int x0, int y0, int R, IplImage* referenceImage, IplImage* canvas);

/**
 * 생성된 단일 원 형태의 선들을 캔버스에 그리는 함수
 * @param IplImage* canvas - 그리기 대상 이미지
 * @param Stroke* S - Stroke 구조체 배열
 * @param int strokesCount - 선 개수
 */
void paintStroke(IplImage* canvas, Stroke* S, int strokesCount);

/**
 * 생성된 곡선 형태의 Spline 기반 선들을 캔버스에 그리는 함수
 * @param IplImage* canvas - 그리기 대상 이미지
 * @param Stroke* S - Stroke 구조체 배열
 * @param int strokesCount - 선 개수
 */
void paintSplineStroke(IplImage* canvas, Stroke* S, int strokesCount);

/**
 * 두 픽셀 간 색상 차이를 구하는 함수
 * @param CvScalar a - 비교 대상 픽셀 1
 * @param CvScalar b - 비교 대상 픽셀 2
 *
 * @return double - 색상 차이 값
 */
inline double getDiff(CvScalar a, CvScalar b);

/**
 * Stroke 구조체 배열을 무작위로 섞는 함수
 * @param Stroke* strokes - Stroke 구조체 배열
 * @param int strokesCount - 선 개수
 */
void shuffle(Stroke* strokes, int strokesCount);

int main() {
    string srcPath;
    int drawingMode = -1;
    IplImage* src = nullptr;
    IplImage* canvas = nullptr;

    // 붓 개수 정의
    const int BRUSH_COUNT = 5;
    // 붓 크기를 원소로 가지는 배열 정의
    int R[BRUSH_COUNT] = { 16, 8, 4, 2, 1 };

    do {
        cout << "Input File Path: ";
        // 이미지 파일 경로 입력
        getline(cin, srcPath);

        // 이미지 불러오기
        src = cvLoadImage(srcPath.c_str());

        // 이미지를 불러올 수 없다면 메시지 출력
        if (!src) cout << "File Not Found!\n";
    } while (!src); // 이미지를 불러올 때 까지 반복

    do {
        int input;
        cout << "Select Drawing Mode (0=circle, 1=stroke):";
        // Drawing Mode 입력
        cin >> input;

        // 만약 입력 값이 0, 1이 아니라면 메시지 출력
        if (input != 0 && input != 1) cout << "Wrong Drawing Mode!\n";
        else drawingMode = input;
    } while (drawingMode == -1); // 올바른 drawingMode(0, 1)가 입력될 때까지 반복

    // 원본 이미지 출력
    cvShowImage("src", src);

    // 그리기 함수 호출
    canvas = paint(src, R, BRUSH_COUNT, drawingMode);

    // 결과 이미지 출력
    cvShowImage("canvas", canvas);

    // 키 입력 대기
    cvWaitKey();

    // 원본, 결과 이미지 창 닫기
    cvDestroyWindow("src");
    cvDestroyWindow("canvas");

    // 생성한 이미지에 대한 메모리 해제
    cvReleaseImage(&src);
    cvReleaseImage(&canvas);

    return 0;
}

IplImage* paint(IplImage* src, int R[], int brushCount, int drawingMode) {
    // 캔버스 이미지 생성
    IplImage* canvas = cvCreateImage(cvGetSize(src), 8, 3);
    // 흰색으로 이미지 전체 채우기
    cvSet(canvas, cvScalar(255, 255, 255));

    // 붓 개수만큼 반복
    for (int i = 0; i < brushCount; i++) {
        cvShowImage("canvas", canvas); // 현재 캔버스 이미지 출력
        IplImage* referenceImage = cvCreateImage(cvGetSize(src), 8, 3); // 참조 이미지 생성
        cvSmooth(src, referenceImage, CV_GAUSSIAN, R[i] * 4 + 1); // 붓 크기에 비례한 Gaussian Blur 커널 크기 설정 & 참조 이미지에 적용

        paintLayer(canvas, referenceImage, R[i], drawingMode); // 현재 붓 크기로 캔버스에 그리기
        cvReleaseImage(&referenceImage); // 생성한 이미지에 대한 메모리 해제

        cvWaitKey(1000); // 1초 대기
    }

    // 완성된 캔버스 이미지 반환
    return canvas;
}

void paintLayer(IplImage* canvas, IplImage* referenceImage, int R, int drawingMode) {
    int width = referenceImage->width; // 참조 이미지 가로 크기
    int height = referenceImage->height; // 참조 이미지 세로 크기
    int fg = 2; // 영역 크기를 조정하는 변수
    int step = fg * R; // 영역 크기를 나타내는 step 정의

    // Stroke 구조체 배열 동적 할당
    Stroke* strokes = new Stroke[((width / step + 1) * (height / step + 1))];
    int strokesCount = 0; // 선 개수 저장할 변수 초기화

    // Approximation threshold - 붓질 여부를 결정할 색상 차이 임계값, 값이 클수록 디테일 감소
    double T = 30.0f;

    // 이미지 전체를 step 간격으로 순회
    for (int y = 0; y < height; y += step) {
        for (int x = 0; x < width; x += step) {
            double areaError = 0.0f; // 현재 영역의 누적 색상 차이 값
            int cnt = 0; // 누적 횟수 저장할 변수 선언
            double maxDiff = -1; // 최대 색상 차이 값
            int x1 = x; // 최대 색상 차이를 가진 픽셀의 x 좌표
            int y1 = y; // 최대 색상 차이를 가진 픽셀의 y 좌표

            // 주변 step / 2 픽셀 영역 탐색
            for (int v = -step / 2; v <= step / 2; v++) {
                for (int u = -step / 2; u <= step / 2; u++) {
                    // 이미지 범위를 벗어나면 continue
                    if (x + u < 0 || x + u >= width || y + v < 0 || y + v >= height) continue;

                    CvScalar a = cvGet2D(referenceImage, y + v, x + u); // 참조 이미지 색상 값 가져오기
                    CvScalar b = cvGet2D(canvas, y + v, x + u); // 현재 캔버스의 색상 값 가져오기
                    double diff = getDiff(a, b); // 두 색상 차이 계산

                    // 최대 색상 차이가 갱신되면 최댓값, 좌표 갱신
                    if (diff > maxDiff) {
                        maxDiff = diff;
                        x1 = x + u;
                        y1 = y + v;
                    }

                    areaError += diff; // 누적 색상 차이 값 갱신
                    cnt += 1; // 누적 횟수 1 증가
                }
            }

            // 평균 색상 차이 값 구하기
            areaError /= cnt;

            // 평균 색상 차이 값이 임계값 T를 넘으면 해당 위치에 선 그리기
            if (areaError > T) {
                if (drawingMode == 0) { // drawingMode가 0일 경우 한개의 원으로 이루어진 선 그리기
                    strokes[strokesCount++] = makeStroke(x1, y1, step, referenceImage); // 원의 경우 영역 크기에 다 차게 그리기 위해 R 값으로 step을 넘겨줌
                } else if (drawingMode == 1) { // drawingMode가 1일 경우 Spline 기반 선 그리기
                    strokes[strokesCount++] = makeSplineStroke(x1, y1, R, referenceImage, canvas);
                }
            }
        }
    }

    // 생성된 선들을 캔버스에 그리기
    if (drawingMode == 0) paintStroke(canvas, strokes, strokesCount);
    else if (drawingMode == 1) paintSplineStroke(canvas, strokes, strokesCount);

    // 각 Stroke 구조체의 points 배열 메모리 해제
    for (int i = 0; i < strokesCount; i++) delete[] strokes[i].points;

    // strokes 배열 메모리 해제
    delete[] strokes;
}

Stroke makeStroke(int x0, int y0, int R, IplImage* referenceImage) {
    Stroke S; // Stroke 구조체 변수 생성
    S.color = cvGet2D(referenceImage, y0, x0); // (x0, y0)에서의 참조 이미지 색상 값 가져와 선의 색상으로 설정
    S.points = new CvPoint[1]; // 점 하나만 저장할 수 있는 배열 동적 할당
    S.points[0] = cvPoint(x0, y0); // (x0, y0) 위치를 점 배열에 저장
    S.pointsCount = 1; // 점 개수 1개로 설정
    S.radius = R; // 붓 크기(R)를 선의 반지름으로 설정

    // 완성된 Stroke 구조체 반환
    return S;
}

Stroke makeSplineStroke(int x0, int y0, int R, IplImage* referenceImage, IplImage* canvas) {
    CvScalar strokeColor = cvGet2D(referenceImage, y0, x0); // (x0, y0)에서의 참조 이미지 색상 값 가져와 선의 색상으로 설정

    double x = x0; // 현재 x 좌표 초기화
    double y = y0; // 현재 y 좌표 초기화

    double lastDx = 0; // 이전 x 방향 초기화
    double lastDy = 0; // 이전 y 방향 초기화

    int minStrokeLength = 4; // 선의 최소 길이
    int maxStrokeLength = 16; // 선의 최대 길이

    double fc = 1; // Curative Filter - 선의 곡률을 조정하는 변수

    int pointsCount = 0; // 점 개수 초기화
    CvPoint* points = new CvPoint[maxStrokeLength + 1]; // 가능한 최대 점 개수만큼 점 배열 동적할당
    points[pointsCount++] = cvPoint(x0, y0); // 시작 점 추가

    for (int i = 1; i <= maxStrokeLength; i++) { // 최대 길이까지 반복
        if (i < minStrokeLength) continue; // 최소 길이 미만인 경우 continue
        CvScalar refColor = cvGet2D(referenceImage, y, x); // 현재 위치의 참조 이미지 색상 값 가져오기

        // 만약 캔버스 색상이 참조 색상에 더 가까우면 중단
        if (getDiff(refColor, cvGet2D(canvas, y, x)) < getDiff(refColor, strokeColor)) break;

        int x1 = x - 1;
        int x2 = x + 1;
        int y1 = y - 1;
        int y2 = y + 1;

        // 이미지 범위를 벗어나면 중단
        if (x1 < 0 || x2 >= canvas->width || y1 < 0 || y2 >= canvas->height) break;

        // 주변 네 방향의 색상 가져오기
        CvScalar c1 = cvGet2D(referenceImage, y, x1); // 왼쪽 색상
        CvScalar c2 = cvGet2D(referenceImage, y, x2); // 오른쪽 색상
        CvScalar c3 = cvGet2D(referenceImage, y1, x); // 위쪽 색상
        CvScalar c4 = cvGet2D(referenceImage, y2, x); // 아래쪽 색상

        // 평균 밝기 계산: L(r, g, b) = 0.30 * r + 0.59 * g + 0.11 * b
        double i1 = c1.val[0] * 0.11 + c1.val[1] * 0.59 + c1.val[2] * 0.30; // 왼쪽 밝기
        double i2 = c2.val[0] * 0.11 + c2.val[1] * 0.59 + c2.val[2] * 0.30; // 오른쪽 밝기
        double i3 = c3.val[0] * 0.11 + c3.val[1] * 0.59 + c3.val[2] * 0.30; // 위쪽 밝기
        double i4 = c4.val[0] * 0.11 + c4.val[1] * 0.59 + c4.val[2] * 0.30; // 아래쪽 밝기

        double gx = (i2 - i1) / 2.0f; // x 방향 그래디언트
        double gy = (i4 - i3) / 2.0f; // y 방향 그래디언트

        double gradientMag = sqrt(gx * gx + gy * gy); // 그래디언트 크기

        // 그래디언트가 사라지면 중단
        if (gradientMag == 0) break;

        // 그래디언트 정규화
        double gxNorm = gx / gradientMag;
        double gyNorm = gy / gradientMag;

        // 수직 방향 계산
        double dx = -gyNorm;
        double dy = gxNorm;

        // 이전 방향과 반대인 경우 방향 반전
        if (lastDx * dx + lastDy * dy < 0) {
            dx = -dx;
            dy = -dy;
        }

        // Curative Filter 적용
        dx = fc * dx + (1 - fc) * lastDx;
        dy = fc * dy + (1 - fc) * lastDy;

        // (dx, dy) 크기 계산
        double mag = sqrt(dx * dx + dy * dy);

        if (mag == 0) break; // Division by zero 방지

        // 좌표 정규화
        dx = dx / mag;
        dy = dy / mag;

        // 다음 점의 좌표 계산
        x = x + R * dx;
        y = y + R * dy;

        // 이미지 범위를 벗어나면 중단
        if (x < 0 || x >= canvas->width || y < 0 || y >= canvas->height) break;

        // 이전 방향 갱신
        lastDx = dx;
        lastDy = dy;

        // 점 배열에 새 점 추가
        points[pointsCount++] = cvPoint(x, y);
    }

    Stroke stroke; // Stroke 구조체 변수 생성
    stroke.color = strokeColor; // 선 색상 설정
    stroke.points = points; // 점 배열 설정
    stroke.pointsCount = pointsCount; // 점 개수 설정
    stroke.radius = R; // 선 반지름 설정

    // 완성된 Stroke 구조체 반환
    return stroke;
}

void paintStroke(IplImage* canvas, Stroke* S, int strokesCount) {
    shuffle(S, strokesCount); // 선 배열을 무작위로 섞기

    for (int i = 0; i < strokesCount; i++) { // 선 개수만큼 반복
        cvCircle(canvas, S[i].points[0], S[i].radius, S[i].color, -1); // 각 선의 첫번째 점을 중심으로 하여 원 그리기
    }
}

void paintSplineStroke(IplImage* canvas, Stroke* S, int strokesCount) {
    shuffle(S, strokesCount); // 선 배열을 무작위로 섞기

    for (int i = 0; i < strokesCount; i++) { // 선 개수만큼 반복
        for (int j = 0; j < S[i].pointsCount - 1; j++) { // 각 선의 (점 개수 - 1)만큼 반복
            cvLine(canvas, S[i].points[j], S[i].points[j + 1], S[i].color, S[i].radius * 2); // 점을 연결하여 직선 그리기
        }
    }
}

inline double getDiff(CvScalar a, CvScalar b) {
    // 두 색상 값 a, b의 차이를 계산하여 반환
    return sqrt((a.val[0] - b.val[0]) * (a.val[0] - b.val[0]) + (a.val[1] - b.val[1]) * (a.val[1] - b.val[1]) + (a.val[2] - b.val[2]) * (a.val[2] - b.val[2]));
}

void shuffle(Stroke* strokes, int strokesCount) {
    for (int i = strokesCount - 1; i > 0; i--) {
        int r = rand() % strokesCount; // 0 ~ strokesCount - 1 사이의 무작위 정수 선택

        // 현재 인덱스의 값과 r 인덱스의 값 교환
        Stroke temp = strokes[i];
        strokes[i] = strokes[r];
        strokes[r] = temp;
    }
}
