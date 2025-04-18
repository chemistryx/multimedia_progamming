#include <opencv2/opencv.hpp>

void prob1(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			cvSet2D(dest, src->height - y - 1, x, f);
		}
	}

	cvShowImage("prob1", dest);
}

void prob2(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			int nx = (src->width / 2 + x) % src->width;

			cvSet2D(dest, y, nx, f);
		}
	}

	cvShowImage("prob2", dest);
}

void prob3(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			CvScalar g = f;
			double brightness = (g.val[0] + g.val[1] + g.val[2]) / 3.0f;

			double nx = x / double(src->width - 1) * 2 - 1;
			double ny = y / double(src->height - 1) * 2 - 1;

			if (abs(nx) + abs(ny) > 1) {
				g = cvScalar(brightness, brightness, brightness);
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob3", dest);
}

void prob4(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			CvScalar g = f;
			double brightness = (g.val[0] + g.val[1] + g.val[2]) / 3.0f;

			double nx = x / double(src->width - 1) * 2 - 1;
			double ny = y / double(src->height - 1) * 2 - 1;

			double distanceFromOrigin = sqrt(nx * nx + ny * ny);

			if (distanceFromOrigin > 1) {
				g = cvScalar(brightness, brightness, brightness);
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob4", dest);
}

void prob5(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			CvScalar g = f;

			double nx = x / double(src->width - 1) * 20 - 10;
			double ny = y / double(src->height - 1) * 20 - 10;

			double distanceFromOrigin = sqrt(nx * nx + ny * ny);

			if ((int)distanceFromOrigin % 2 != 0) {
				g = cvScalar(0, 0, 0);
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob5", dest);
}

void prob6(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			CvScalar g = f;

			int nx = x / double(src->width) * 10;
			int ny = y / double(src->height) * 10;

			if ((nx + ny) % 2 == 0) {
				g = cvScalar(0, 0, 0);
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob6", dest);
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\sejong_small.jpg");

	prob1(src);
	prob2(src);
	prob3(src);
	prob4(src);
	prob5(src);
	prob6(src);

	cvWaitKey();

	return 0;
}