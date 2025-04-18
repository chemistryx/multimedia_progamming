#include <opencv2/opencv.hpp>

void prob10(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g = cvScalar(0, 0, 0);

			if (y < src->height / 3) {
				g.val[2] = f.val[2];
			}
			else if (y < src->height * 2 / 3) {
				g.val[0] = f.val[0];
			}
			else {
				g.val[1] = f.val[1];
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob10", dest);
}

void prob11(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g = cvScalar(0, 0, 0);

			if (x < src->width / 2) {
				if (y < src->height / 2) {
					g.val[2] = f.val[2];
				}
				else {
					g.val[0] = f.val[0];
				}
			}
			else {
				if (y < src->height / 2) {
					g.val[1] = f.val[1];
				}
				else {
					double brightness = (f.val[0] + f.val[1] + f.val[2]) / 3.0f;
					g = cvScalar(brightness, brightness, brightness);
				}
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob11", dest);
}

void prob12(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			CvScalar g;

			for (int k = 0; k < 3; k++) {
				g.val[k] = f.val[k] > 128 ? -2 * f.val[k] + 512 : 2 * f.val[k];
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob12", dest);
}

void prob13(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			CvScalar g;
			double alpha = x / double(src->width - 1);

			for (int k = 0; k < 3; k++) {
				g.val[k] = (1 - alpha) * f.val[k] + alpha * (f.val[k] + 255);
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob13", dest);
}

void prob14(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			CvScalar g;

			for (int k = 0; k < 3; k++) {
				g.val[k] = abs(2 * (f.val[k] - 128));
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob14", dest);
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\lena.jpg");

	prob10(src);
	prob11(src);
	prob12(src);
	prob13(src);
	prob14(src);

	cvWaitKey();

	return 0;
}