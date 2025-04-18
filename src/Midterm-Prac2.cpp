#include <opencv2/opencv.hpp>

void prob6(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			double nx = x / double(src->width) * 2 - 1;
			double ny = y / double(src->height) * 2 - 1;

			double brightness = (f.val[0] + f.val[1] + f.val[2]) / 3.0f;

			if (abs(nx) + abs(ny) > 1) {
				f = cvScalar(brightness, brightness, brightness);
			}

			cvSet2D(dest, y, x, f);
		}
	}

	cvShowImage("prob6", dest);
}

void prob7(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			double nx = x / double(src->width) * 2;
			double ny = y / double(src->height) * 2;

			double brightness = (f.val[0] + f.val[1] + f.val[2]) / 3.0f;
			double distance = 0.0f;

			if (x > src->width / 2 && y > src->height / 2) {
				distance = sqrt((2 - nx) * (2 - nx) + (2 - ny) * (2 - ny));
			}
			else if (x > src->width / 2 && y < src->height / 2) {
				distance = sqrt((2 - nx) * (2 - nx) + ny * ny);
			}
			else if (x < src->width / 2 && y > src->height / 2) {
				distance = sqrt(nx * nx + (2 - ny) * (2 - ny));
			}
			else {
				distance = sqrt(nx * nx + ny * ny);
			}

			if (distance > 1) {
				f = cvScalar(brightness, brightness, brightness);
			}

			cvSet2D(dest, y, x, f);
		}
	}

	cvShowImage("prob7", dest);
}

void prob8(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			double nx = x / double(src->width) * 2 - 1;
			double ny = y / double(src->height) * 2 - 1;

			double brightness = (f.val[0] + f.val[1] + f.val[2]) / 3.0f;
			double distanceFromOrigin = sqrt(nx * nx + ny * ny);

			if (distanceFromOrigin > 1) {
				f = cvScalar(brightness, brightness, brightness);
			}

			cvSet2D(dest, y, x, f);
		}
	}

	cvShowImage("prob8", dest);
}

void prob9(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			double brightness = (f.val[0] + f.val[1] + f.val[2]) / 3.0f;
			CvScalar g = cvScalar(brightness, brightness, brightness);

			double alpha = x / double(src->width);
			CvScalar c;
			for (int k = 0; k < 3; k++) {
				c.val[k] = (1 - alpha) * f.val[k] + alpha * g.val[k];
			}

			cvSet2D(dest, y, x, c);
		}
	}

	cvShowImage("prob9", dest);
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\lena.jpg");

	prob6(src);
	prob7(src);
	prob8(src);
	prob9(src);

	cvWaitKey();

	return 0;
}