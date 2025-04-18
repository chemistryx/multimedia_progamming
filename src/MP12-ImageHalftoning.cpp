#include <opencv2/opencv.hpp>

double getDiff(CvScalar a, CvScalar b) {
	return sqrt((a.val[0] - b.val[0]) * (a.val[0] - b.val[0]) + (a.val[1] - b.val[1]) * (a.val[1] - b.val[1]) + (a.val[2] - b.val[2]) * (a.val[2] - b.val[2]));
}

void thresholding(IplImage* src, IplImage* dest) {
	CvScalar palette[] = {
		cvScalar(0, 0, 0),
		cvScalar(255, 255, 255),
		cvScalar(255, 0, 0),
		cvScalar(0, 255, 255),
		cvScalar(0, 0, 255),
	};

	const int COLOR_SIZE = 5;

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;

			double minDiff = DBL_MAX;

			for (int i = 0; i < COLOR_SIZE; i++) {
				double diff = getDiff(f, palette[i]);

				if (diff < minDiff) {
					minDiff = diff;
					g = palette[i];
				}
			}

			cvSet2D(dest, y, x, g);
		}
	}
}

void patterning(IplImage* src, IplImage* dest) {
	IplImage* blur = cvCreateImage(cvGetSize(src), 8, 3);

	cvSmooth(src, blur, CV_BLUR, 3);
	cvSet(dest, cvScalar(0, 0, 0));

	//int pattern[] = { 2, 1, 3, 0 };
	//int step = 2;
	int pattern[] = { 1, 8, 0, 6, 2, 5, 7, 3, 4 };
	int step = 3;

	for (int y = 0; y < src->height; y += step) {
		for (int x = 0; x < src->width; x += step) {
			CvScalar f = cvGet2D(blur, y, x);

			for (int k = 0; k < 3; k++) {
				int brightness = f.val[k];
				double div = 255 / double(step * step + 1);
				int pointsToPaint = brightness / div;
				if (pointsToPaint > step * step) pointsToPaint = step * step;

				for (int i = 0; i < pointsToPaint; i++) {
					int u = pattern[i] % step;
					int v = pattern[i] / step;
					int x2 = x + u;
					int y2 = y + v;
					if (x2 >= src->width || y2 >= src->height) continue;
					CvScalar g = cvGet2D(dest, y2, x2);
					g.val[k] = 255;
					cvSet2D(dest, y2, x2, g);
				}
			}
		}
	}
}

void diterning(IplImage* src, IplImage* dest) {
	CvScalar palette[] = {
		cvScalar(0, 0, 0),
		cvScalar(255, 255, 255),
	};

	const int COLOR_SIZE = 2;

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;

			double minDiff = DBL_MAX;

			for (int i = 0; i < COLOR_SIZE; i++) {
				double diff = getDiff(f, palette[i]);

				if (diff < minDiff) {
					minDiff = diff;
					g = palette[i];
				}
			}

			cvSet2D(dest, y, x, g);

			CvScalar d;

			for (int k = 0; k < 3; k++) {
				d.val[k] = f.val[k] - g.val[k];
			}

			{
				int x2 = x + 1;
				int y2 = y;
				double weight = 7 / 16.0f;
				if (x2 >= 0 && x2 < src->width && y2 >= 0 && y2 < src->height) {
					CvScalar c = cvGet2D(src, y2, x2);
					for (int k = 0; k < 3; k++) c.val[k] += d.val[k] * weight;

					cvSet2D(src, y2, x2, c);
				}
			}
			{
				int x2 = x;
				int y2 = y + 1;
				double weight = 5 / 16.0f;
				if (x2 >= 0 && x2 < src->width && y2 >= 0 && y2 < src->height) {
					CvScalar c = cvGet2D(src, y2, x2);
					for (int k = 0; k < 3; k++) c.val[k] += d.val[k] * weight;

					cvSet2D(src, y2, x2, c);
				}
			}
			{
				int x2 = x + 1;
				int y2 = y + 1;
				double weight = 1 / 16.0f;
				if (x2 >= 0 && x2 < src->width && y2 >= 0 && y2 < src->height) {
					CvScalar c = cvGet2D(src, y2, x2);
					for (int k = 0; k < 3; k++) c.val[k] += d.val[k] * weight;

					cvSet2D(src, y2, x2, c);
				}
			}
			{
				int x2 = x - 1;
				int y2 = y + 1;
				double weight = 3 / 16.0f;
				if (x2 >= 0 && x2 < src->width && y2 >= 0 && y2 < src->height) {
					CvScalar c = cvGet2D(src, y2, x2);
					for (int k = 0; k < 3; k++) c.val[k] += d.val[k] * weight;

					cvSet2D(src, y2, x2, c);
				}
			}
		}
	}
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\lena.jpg");
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	//thresholding(src, dest);
	//patterning(src, dest);
	diterning(src, dest);

	cvShowImage("src", src);
	cvShowImage("dest", dest);

	cvWaitKey();

	return 0;
}