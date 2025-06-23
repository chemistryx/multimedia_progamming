#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
IplImage* dst = nullptr;

CvPoint p1;
CvPoint p2;

void setIdentity(float M[][3]) {
	M[0][0] = 1.0f; M[0][1] = 0.0f; M[0][2] = 0.0f;
	M[1][0] = 0.0f; M[1][1] = 1.0f; M[1][2] = 0.0f;
	M[2][0] = 0.0f; M[2][1] = 0.0f; M[2][2] = 1.0f;

}

void setScale(float M[][3], float sx, float sy) {
	setIdentity(M);
	M[0][0] = sx;
	M[1][1] = sy;
}

void setRotation(float M[][3], float angle) {
	setIdentity(M);
	float rad = angle / 180.0f * CV_PI;

	M[0][0] = cos(rad); M[0][1] = -sin(rad);
	M[1][0] = sin(rad); M[1][1] = cos(rad);
}

void setTranslate(float M[][3], float tx, float ty) {
	setIdentity(M);
	M[0][2] = tx;
	M[1][2] = ty;
}

void setMultiply(float M[][3], float A[][3], float B[][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			M[i][j] = 0.0f;
			for (int k = 0; k < 3; k++) {
				M[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void setAffineInverse(float IM[][3], float sx, float sy, float angle, float tx, float ty) {
	float S[3][3], R[3][3], T[3][3], RT[3][3];

	setScale(S, 1 / sx, 1 / sy);
	setRotation(R, -angle);
	setTranslate(T, -tx, -ty);

	setMultiply(RT, R, S);
	setMultiply(IM, RT, T);
}

void setCenterRotateInverse(float IM[][3], float angle) {
	float R[3][3], T1[3][3], T2[3][3], RT[3][3];

	float cx = src->width / 2;
	float cy = src->height / 2;

	setTranslate(T1, -cx, -cy);
	setRotation(R, -angle);
	setTranslate(T2, cx, cy);

	setMultiply(RT, R, T1);
	setMultiply(IM, T2, RT);
}

void applyInverseTransform(float IM[][3], IplImage* src, IplImage* dst) {
	cvSet(dst, cvScalar(0, 0, 0));

	for (float y2 = 0; y2 < dst->height; y2++) {
		for (float x2 = 0; x2 < dst->width; x2++) {
			float w2 = 1.0f;
			float x1 = IM[0][0] * x2 + IM[0][1] * y2 + IM[0][2] * w2;
			float y1 = IM[1][0] * x2 + IM[1][1] * y2 + IM[1][2] * w2;
			float w1 = IM[2][0] * x2 + IM[2][1] * y2 + IM[2][2] * w2;

			x1 /= w1;
			y1 /= w1;

			if (x1 < 0 || x1 > src->width - 1) continue;
			if (y1 < 0 || y1 > src->height - 1) continue;

			CvScalar c = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, c);
		}
	}
}

float angle = 0.0f;
void myMouse(int event, int x, int y, int flags, void* param) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
	}

	/*if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) == CV_EVENT_FLAG_LBUTTON) {
		p2 = cvPoint(x, y);
	}*/

	if (event == CV_EVENT_LBUTTONUP) {
		p2 = cvPoint(x, y);

		float theta1 = atan2(p1.y, p1.x);
		float theta2 = atan2(p2.y, p2.x);

		angle += (theta2 - theta1) * 180.0f / CV_PI; // rad -> deg

		float IM[3][3];

		setCenterRotateInverse(IM, angle);
		//setAffineInverse(IM, 1, 1, angle, 0, 0);
		applyInverseTransform(IM, src, dst);
		cvShowImage("dst", dst);
	}
}

int main() {
	src = cvLoadImage("C:\\temp\\lena.jpg");
	dst = cvCreateImage(cvGetSize(src), 8, 3);

	cvCopy(src, dst);

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();

	return 0;
}
