#include <opencv2/opencv.hpp>

void prob1(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			cvSet2D(dest, src->height - 1 - y, x, f);
		}
	}

	cvShowImage("prob1", dest);
}

void prob2(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			cvSet2D(dest, y, src->width - 1 - x, f);
		}
	}

	cvShowImage("prob2", dest);
}

void prob3(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			cvSet2D(dest, (y + src->height / 2) % src->height, x, f);
		}
	}

	cvShowImage("prob3", dest);
}

void prob4(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			cvSet2D(dest, y, (x + src->width / 2) % src->width, f);
		}
	}

	cvShowImage("prob4", dest);
}

void prob5(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			cvSet2D(dest, ((src->height - 1 - y) + src->height / 2) % src->height, ((src->width - 1 - x) + src->width / 2) % src->width, f);
		}
	}

	cvShowImage("prob5", dest);
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\lena.jpg");

	prob1(src);
	prob2(src);
	prob3(src);
	prob4(src);
	prob5(src);

	cvWaitKey();

	return 0;
}