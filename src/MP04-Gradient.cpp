#include <opencv2/opencv.hpp>

void linearGradient(IplImage* src) {
	IplImage* img = cvCreateImage(cvGetSize(src), 8, 3);

	cvSet(img, cvScalar(0, 0, 255));

	for (int y = 0; y < img->height; y++) {
		for (int x = 0; x < img->width; x++) {
			CvScalar f = cvGet2D(img, y, x);
			CvScalar blue = cvScalar(255, 0, 0);
			CvScalar g;
			double alpha = x / double(img->width - 1);

			for (int k = 0; k < 3; k++) {
				g.val[k] = (1 - alpha) * f.val[k] + alpha * blue.val[k];
			}

			cvSet2D(img, y, x, g);
		}
	}

	cvShowImage("linear", img);
}

void radialGradient(IplImage* src) {
	IplImage* img = cvCreateImage(cvGetSize(src), 8, 3);

	cvSet(img, cvScalar(0, 0, 255));

	for (int y = 0; y < img->height; y++) {
		for (int x = 0; x < img->width; x++) {
			CvScalar f = cvGet2D(img, y, x);
			CvScalar blue = cvScalar(255, 0, 0);

			// norm coords
			double nx = x / double(img->width - 1) * 2 - 1;
			double ny = y / double(img->height - 1) * 2 - 1;

			double distanceFromOrigin = sqrt(nx * nx + ny * ny);

			// sqrt(2): max distance from origin in [-1, 1] coords
			double alpha = distanceFromOrigin / sqrt(2);

			CvScalar g;

			for (int k = 0; k < 3; k++) {
				g.val[k] = (1 - alpha) * f.val[k] + alpha * blue.val[k];
			}

			cvSet2D(img, y, x, g);
		}
	}

	cvShowImage("radial", img);
}

void diagonalGradient(IplImage* src) {
	IplImage* img = cvCreateImage(cvGetSize(src), 8, 3);

	cvSet(img, cvScalar(0, 0, 255));

	for (int y = 0; y < img->height; y++) {
		for (int x = 0; x < img->width; x++) {
			CvScalar f = cvGet2D(img, y, x);
			CvScalar blue = cvScalar(255, 0, 0);

			double alpha = (x + y) / double(img->width + img->height - 2);

			CvScalar g;
			for (int k = 0; k < 3; k++) {
				g.val[k] = (1 - alpha) * f.val[k] + alpha * blue.val[k];
			}

			cvSet2D(img, y, x, g);
		}
	}

	cvShowImage("diagonal", img);
}

int main() {
	IplImage* src = cvCreateImage(cvSize(600, 400), 8, 3);

	linearGradient(src);
	radialGradient(src);
	diagonalGradient(src);

	cvWaitKey();

	return 0;
}