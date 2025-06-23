#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
IplImage* dst = nullptr;

CvScalar myGet2D(IplImage* src, float y, float x) {
	int i1 = (int)x, j1 = (int)y;
	int i2 = i1 + 1, j2 = j1 + 1;

	if (i2 > src->width - 1) i2 = i1;
	if (j2 > src->height - 1) j2 = j1;

	float a = x - i1;
	float b = y - j1;

	CvScalar c[4];
	c[0] = cvGet2D(src, j1, i1); // left top
	c[1] = cvGet2D(src, j1, i2); // right top
	c[2] = cvGet2D(src, j2, i1); // left bottom
	c[3] = cvGet2D(src, j2, i2); // right bottom

	float w[4];

	w[0] = (1 - a) * (1 - b);
	w[1] = a * (1 - b);
	w[2] = (1 - a) * b;
	w[3] = a * b;

	CvScalar o = cvScalar(0, 0, 0);

	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 3; k++) {
			o.val[k] += c[i].val[k] * w[i];
		}
	}

	return o;
}

int main() {
	src = cvLoadImage("C:\\temp\\lena.jpg");
	dst = cvCreateImage(cvGetSize(src), 8, 3);

	float scale = 5.0f;
	float cx = src->width / 2;
	float cy = src->height / 2;

	for (int y2 = 0; y2 < src->height; y2++) {
		for (int x2 = 0; x2 < src->width; x2++) {
			float x1 = (x2 - cx) / scale + cx;
			float y1 = (y2 - cx) / scale + cx;

			if (x1 < 0 || x1 > src->width - 1) continue;
			if (y1 < 0 || y1 > src->height - 1) continue;

			CvScalar c = myGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, c);
		}
	}

	cvShowImage("dst", dst);
	cvWaitKey();

	return 0;
}
