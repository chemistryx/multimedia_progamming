#include <opencv2/opencv.hpp>
#include "MatrixInverse.h"
#include "vec.h"
#include "mat.h"

using namespace std;

IplImage* src = nullptr; // 원본 이미지를 저장할 포인터
IplImage* dst = nullptr; // 결과 이미지를 저장할 포인터
int width = 500; // 결과 이미지 가로 크기
int height = 500; // 결과 이미지 세로 크기

// 육면체를 구성하는 8개의 꼭지점 3차원 좌표
vec3 pos[8] = {
	vec3(-0.5, -0.5, 0.5),
	vec3(-0.5, 0.5, 0.5),
	vec3(0.5, 0.5, 0.5),
	vec3(0.5, -0.5, 0.5),
	vec3(-0.5, -0.5, -0.5),
	vec3(-0.5, 0.5, -0.5),
	vec3(0.5, 0.5, -0.5),
	vec3(0.5, -0.5, -0.5)
};

// 사각형 한 면
struct rect {
	int ind[4];	// 꼭지점의 인덱스
	vec3 pos[4]; // 꼭지점의 화면 방향으로의 3차원 위치
	vec3 nor; // 법선(normal) 벡터 방향 (= 면이 향하는 방향)
};

// 사각형 정보를 채워주는 함수(바로 아래 cube정의에 사용)
rect setRect(int a, int b, int c, int d) {
	rect r;
	r.ind[0] = a;
	r.ind[1] = b;
	r.ind[2] = c;
	r.ind[3] = d;
	
	return r;
}

// 사각형 6개를 정의해 육면체를 구성
rect cube[6] = {
	setRect(1, 0, 3, 2),
	setRect(2, 3, 7, 6),
	setRect(3, 0, 4, 7),
	setRect(6, 5, 1, 2),
	setRect(6, 7, 4, 5),
	setRect(5, 4, 0, 1)
};

vec3 epos = vec3(1.5, 1.5, 1.5); // 카메라(시점의 3차원) 위치
mat4 ModelMat; // 모델에 변형을 주는 변형 행렬
mat4 ViewMat; // 카메라 시점을 맞춰주는 변형 행렬
mat4 ProjMat; // 화면상 위치로 투영해주는 변형 행렬

// 초기화 함수
void init()	{
	ModelMat = mat4(1.0f);
	ViewMat = LookAt(epos, vec3(0, 0, 0), vec3(0, 1, 0)); // 카메라 위치(epos)에서 (0,0,0)을 바라보는 카메라 설정
	ProjMat = Perspective(45, width / (float) height, 0.1, 100); // 45도의 시야각을 가진 투영 변환 (가시거리 0.1~100)
}

// 육면체 모델에 회전을 적용하는 함수
void rotateModel(float rx, float ry, float rz) {
	ModelMat = RotateX(rx) * RotateY(ry) * RotateZ(rz) * ModelMat;
}

// 3차원 좌표를 화면에 투영된 2차원 + 깊이값(z) 좌표로 변환하는 함수
vec3 convert3Dto2D(vec3 in) {
	vec4 p = ProjMat * ViewMat * ModelMat * vec4(in);
	p.x /= p.w;
	p.y /= p.w;
	p.z /= p.w;
	p.x = (p.x + 1) / 2.0f * width;
	p.y = (-p.y + 1) / 2.0f * height;
	
	return vec3(p.x, p.y, p.z);
}

// 육면체의 회전에 따른 각 면의 3차원 좌표 및 법선 벡터 방향을 업데이트하는 함수
void updatePosAndNormal(rect* r, vec3 p[]) {
	for (int i = 0; i < 4; i++) r->pos[i] = convert3Dto2D(p[r->ind[i]]);
	
	vec3 a = normalize(r->pos[0] - r->pos[1]);
	vec3 b = normalize(r->pos[2] - r->pos[1]);
	
	r->nor = cross(a, b);
}

// 점 배열 P Q에 대해 Homography 연산을 수행하는 함수
void calculateHomography(float H[][3], CvPoint P[], CvPoint Q[], int n) {
	float A[8][8], B[8][1], IA[8][8];
	float h[8];

	// 4개의 좌표쌍에 대한 연산 수행
	for (int k = 0; k < 4; k++) {
		int row = 2 * k; // x 좌표 인덱스
		int rowNext = 2 * k + 1; // y 좌표 인덱스

		// [x y 1 0 0 0 -x'x -x'y] 형태로 행 채우기
		A[row][0] = P[k].x; A[row][1] = P[k].y; A[row][2] = 1.0f; A[row][3] = 0;
		A[row][4] = 0; A[row][5] = 0; A[row][6] = -Q[k].x * P[k].x; A[row][7] = -Q[k].x * P[k].y;

		// [0 0 0 x y 1 -y'x -y'y] 형태로 행 채우기
		A[rowNext][0] = 0; A[rowNext][1] = 0; A[rowNext][2] = 0; A[rowNext][3] = P[k].x;
		A[rowNext][4] = P[k].y; A[rowNext][5] = 1.0f; A[rowNext][6] = -Q[k].y * P[k].x; A[rowNext][7] = -Q[k].y * P[k].y;

		// B 벡터를 목적지 좌표들로 채우기
		B[row][0] = Q[k].x;
		B[rowNext][0] = Q[k].y;
	}

	// A의 역행렬 A^-1 계산
	InverseMatrixGJ8(A, IA);

	// Ah = B, h = A^-1 B 연산을 통해 h 벡터 구하기
	for (int i = 0; i < 8; i++) {
		h[i] = 0.0f;
		for (int j = 0; j < 8; j++) {
			h[i] += IA[i][j] * B[j][0];
		}
	}

	// 앞서 구한 h 벡터의 값으로 H 행렬 구성
	H[0][0] = h[0]; H[0][1] = h[1]; H[0][2] = h[2];
	H[1][0] = h[3]; H[1][1] = h[4]; H[1][2] = h[5];
	H[2][0] = h[6]; H[2][1] = h[7]; H[2][2] = 1.0f;
}

// 역변형을 수행하는 함수
void applyInverseTransform(float M[][3], IplImage* src, IplImage* dest) {
	// 결과 이미지 전체 순회
	for (float y2 = 0; y2 < dest->height; y2++) {
		for (float x2 = 0; x2 < dest->width; x2++) {
			float w2 = 1.0f;

			// 원본 이미지 좌표 계산
			float x1 = M[0][0] * x2 + M[0][1] * y2 + M[0][2] * w2;
			float y1 = M[1][0] * x2 + M[1][1] * y2 + M[1][2] * w2;
			float w1 = M[2][0] * x2 + M[2][1] * y2 + M[2][2] * w2;

			x1 /= w1;
			y1 /= w1;

			// 원본 이미지 좌표 범위 체크
			if (x1 < 0 || x1 > src->width - 1 || y1 < 0 || y1 > src->height - 1) continue;

			// 포인터 직접 접근에 필요한 변수 할당
			int srcIdx = int(y1) * src->widthStep + int(x1) * 3;
			int destIdx = int(y2) * dest->widthStep + int(x2) * 3;

			// 원본 이미지의 데이터를 결과 이미지로 복사
			dest->imageData[destIdx] = src->imageData[srcIdx]; // B
			dest->imageData[destIdx + 1] = src->imageData[srcIdx + 1]; // G
			dest->imageData[destIdx + 2] = src->imageData[srcIdx + 2]; // R
		}
	}
}

// 그림을 그린다 (각 면의 테두리를 직선으로 그림)
void drawImage() {
	cvSet(dst, cvScalar(0, 0, 0)); // 그릴 때마다 이미지 검정으로 초기화

	// 각 면에 대해 반복
	for (int i = 0; i < 6; i++) {
		updatePosAndNormal(&cube[i], pos);
		
		// 깜빡임 방지 위해 임계값 0.05로 지정
		if (cube[i].nor.z < 0.05) continue; // 보이지 않는 사각형을 제외, 보이는 사각형만 그린다	
		
		vec3 p1 = cube[i].pos[0]; // 좌측 상단
		vec3 p2 = cube[i].pos[1]; // 좌측 하단
		vec3 p3 = cube[i].pos[2]; // 우측 하단
		vec3 p4 = cube[i].pos[3]; // 우측 상단

		// 변환 대상이 될 좌표 배열 선언 (반시계방향)
		CvPoint P[4] = { cvPoint(0, 0), cvPoint(0, src->height - 1), cvPoint(src->width - 1, src->height - 1), cvPoint(src->width - 1, 0) };
		CvPoint Q[4] = { cvPoint(p1.x, p1.y), cvPoint(p2.x, p2.y), cvPoint(p3.x, p3.y), cvPoint(p4.x, p4.y) };

		float H[3][3], IH[3][3];

		// Homography 연산하여 H에 저장
		calculateHomography(H, P, Q, 4);

		// 역변형을 위해 H의 역행렬 계산
		InverseMatrixGJ3(H, IH);
		// 역변형 수행
		applyInverseTransform(IH, src, dst);

		// 면 윤곽선 그리기
		cvLine(dst, cvPoint(p1.x, p1.y), cvPoint(p2.x, p2.y), cvScalar(255, 255, 255), 3);
		cvLine(dst, cvPoint(p2.x, p2.y), cvPoint(p3.x, p3.y), cvScalar(255, 255, 255), 3);
		cvLine(dst, cvPoint(p3.x, p3.y), cvPoint(p4.x, p4.y), cvScalar(255, 255, 255), 3);
		cvLine(dst, cvPoint(p4.x, p4.y), cvPoint(p1.x, p1.y), cvScalar(255, 255, 255), 3);
	}

	cvShowImage("3D view", dst);
}

void myMouse(int event, int x, int y, int flags, void*) {
	static CvPoint prev = cvPoint(0, 0); // 이전 좌표의 위치를 저장할 정적 변수 선언

	if (event == CV_EVENT_LBUTTONDOWN) prev = cvPoint(x, y); // 마우스 왼쪽 버튼 클릭 시 현재 위치 저장

	// 마우스 왼쪽 버튼을 누른 채 움직이는 경우
	if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) == CV_EVENT_FLAG_LBUTTON) {
		int dx = x - prev.x; // x 좌표 차이 구함
		int dy = y - prev.y; // y 좌표 차이 구함
		
		rotateModel(dy, dx, -dy); // 마우스 조작에 따라 모델을 회전함
		drawImage(); // 큐브 그리기
		prev = cvPoint(x, y); // 이전 좌표 갱신
	}
}

int main() {
	char srcPath[1000];
	dst = cvCreateImage(cvSize(width, height), 8, 3);

	do {
		cout << "Input File Path: ";
		cin.getline(srcPath, 1000); // 파일 경로 입력 받기

		src = cvLoadImage(srcPath); // 원본 이미지 로드

		if (!src) cout << "File not Found!\n"; // 이미지를 찾을 수 없는 경우 오류 출력
	} while (!src); // 이미지 로드될때까지 반복
	
	init(); // 초기화

	while (true) {
		rotateModel(0, 1, 0); // 모델 회전
		drawImage(); // 큐브 그리기
		cvSetMouseCallback("3D view", myMouse); // 마우스 콜백 등록
		
		int key = cvWaitKey(1);
		
		if (key == ' ') key = cvWaitKey(); // ' ' 입력되면 일시 정지
	}

	return 0;
}
