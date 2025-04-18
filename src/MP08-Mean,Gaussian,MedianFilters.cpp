#include <opencv2/opencv.hpp>

void meanFilter(IplImage* src, IplImage* dest) {
	const int K = 3;

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar g = cvScalar(0, 0, 0);

			for (int v = -K; v <= K; v++) {
				for (int u = -K; u <= K; u++) {
					int nx = x + u;
					int ny = y + v;

					if (nx < 0 || nx >= src->width || ny < 0 || ny >= src->height) continue;

					CvScalar f = cvGet2D(src, ny, nx);

					g.val[0] += f.val[0] / ((2 * K + 1) * (2 * K + 1));
					g.val[1] += f.val[1] / ((2 * K + 1) * (2 * K + 1));
					g.val[2] += f.val[2] / ((2 * K + 1) * (2 * K + 1));
				}
			}

			cvSet2D(dest, y, x, g);
		}
	}
}

void gaussianFilter(IplImage* src, IplImage* dest) {
	const int K = 1;
	double mask[3][3] = {
		{ 1 / 16.0f, 2 / 16.0f, 1 / 16.0f },
		{ 2 / 16.0f, 4 / 16.0f, 2 / 16.0f },
		{ 1 / 16.0f, 2 / 16.0f, 1 / 16.0f },
	};

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar g = cvScalar(0, 0, 0);

			for (int v = -K; v <= K; v++) {
				for (int u = -K; u <= K; u++) {
					int nx = x + u;
					int ny = y + v;

					if (nx < 0 || nx >= src->width || ny < 0 || ny >= src->height) continue;

					CvScalar f = cvGet2D(src, ny, nx);

					for (int k = 0; k < 3; k++) {
						g.val[k] += f.val[k] * mask[u + K][v + K];
					}
				}
			}

			cvSet2D(dest, y, x, g);
		}
	}
}

void medianFilter(IplImage* src, IplImage* dest) {
	const int K = 1;
	const int SIZE = (2 * K + 1) * (2 * K + 1);

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			int idx = 0;
			CvScalar colors[SIZE];

			for (int v = -K; v <= K; v++) {
				for (int u = -K; u <= K; u++) {
					int nx = x + u;
					int ny = y + v;

					if (nx < 0 || nx >= src->width || ny < 0 || ny >= src->height) continue;

					colors[idx++] = cvGet2D(src, ny, nx);
				}
			}

			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					int brightnessA = colors[i].val[0] + colors[i].val[1] + colors[i].val[2];
					int brightnessB = colors[j].val[0] + colors[j].val[1] + colors[j].val[2];
					if (brightnessA > brightnessB) {
						CvScalar temp = colors[i];
						colors[i] = colors[j];
						colors[j] = temp;
					}
				}
			}

			cvSet2D(dest, y, x, colors[SIZE / 2]);
		}
	}
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\saltpepper.jpg");
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	//meanFilter(src, dest);
	//gaussianFilter(src, dest);
	medianFilter(src, dest);

	cvShowImage("src", src);
	cvShowImage("dest", dest);

	cvWaitKey();

	return 0;
}