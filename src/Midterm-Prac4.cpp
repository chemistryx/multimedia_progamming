#include <opencv2/opencv.hpp>

void prob15(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);
	const int SIZE = 32; // 512 / 32 == 16

	for (int y = 0; y < src->height; y += SIZE) {
		for (int x = 0; x < src->width; x += SIZE) {
			CvScalar c[SIZE * SIZE];

			for (int v = 0; v < SIZE; v++) {
				for (int u = 0; u < SIZE; u++) {
					int nx = x + u;
					int ny = y + v;

					CvScalar f = cvGet2D(src, ny, nx);

					c[v * SIZE + u] = f;
				}
			}

			double b = 0.0f;
			double g = 0.0f;
			double r = 0.0f;

			for (int i = 0; i < SIZE * SIZE; i++) {
				b += c[i].val[0];
				g += c[i].val[1];
				r += c[i].val[2];
			}

			b /= SIZE * SIZE;
			g /= SIZE * SIZE;
			r /= SIZE * SIZE;

			for (int v = 0; v < SIZE; v++) {
				for (int u = 0; u < SIZE; u++) {
					int nx = x + u;
					int ny = y + v;

					cvSet2D(dest, ny, nx, cvScalar(b, g, r));
				}
			}
		}
	}

	cvShowImage("prob15", dest);
}

void prob16(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar c = cvGet2D(src, y, x);

			int ny = y / double(src->height) * 8; // 0 ~ 8

			double alpha = ny / 8.0f; // 0 ~ 1
			CvScalar g;

			for (int k = 0; k < 3; k++) {
				g.val[k] = (1 - alpha) * c.val[k] + alpha * (c.val[k] + 255);
			}

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob16", dest);
}

void prob17(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			int nx = x / double(src->width) * 8;
			int ny = y / double(src->height) * 8;

			CvScalar g = f;

			if ((nx + ny) % 2 != 0) g = cvScalar(0, 0, 0);

			cvSet2D(dest, y, x, g);
		}
	}

	cvShowImage("prob17", dest);
}

void drawPattern(IplImage* src, IplImage* dest, int width, int height, int dx, int dy) {

}

void prob18(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	int count = 4;
	int width = src->width / count;
	int height = src->height / count;

	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count; j++) {
			int dx = width * j;
			int dy = height * i;
			// draw small pattern
			for (int y = dy; y < dy + height; y++) {
				for (int x = dx; x < dx + width; x++) {
					CvScalar f = cvGet2D(src, y, x);
					double nx = (x - dx) / double(width) * 2 - 1;
					double ny = (y - dy) / double(height) * 2 - 1;

					CvScalar g = f;

					if (abs(nx) + abs(ny) > 1) {
						for (int k = 0; k < 3; k++) g.val[k] = 0.5 * f.val[k];
					}

					cvSet2D(dest, y, x, g);
				}
			}
		}
	}

	cvShowImage("prob18", dest);
}

void prob19(IplImage* src) {
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	int count = 4;
	int width = src->width / count;
	int height = src->height / count;

	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count; j++) {
			int dx = width * j;
			int dy = height * i;

			for (int y = dy; y < height + dy; y++) {
				for (int x = dx; x < width + dx; x++) {
					CvScalar f = cvGet2D(src, y, x);
					double nx = (x - dx) / double(width) * 2 - 1;
					double ny = (y - dy) / double(height) * 2 - 1;

					double distanceFromOrigin = sqrt(nx * nx + ny * ny);

					CvScalar g = f;

					if (distanceFromOrigin > 1) {
						for (int k = 0; k < 3; k++) g.val[k] = 0.5 * f.val[k];
					}

					cvSet2D(dest, y, x, g);
				}
			}
		}
	}

	cvShowImage("prob19", dest);
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\lena.jpg");

	prob15(src);
	prob16(src);
	prob17(src);
	prob18(src);
	prob19(src);

	cvWaitKey();

	return 0;
}