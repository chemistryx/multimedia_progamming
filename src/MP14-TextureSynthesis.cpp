#include <opencv2/opencv.hpp>

double calculateSsd(IplImage* src, int sx, int sy, IplImage* dest, int tx, int ty, int K, int** mask) {
	double sum = 0.0f;
	int count = 0;

	for (int v = -K; v <= K; v++) {
		for (int u = -K; u <= K; u++) {
			int ssx = sx + u;
			int ssy = sy + v;
			int ttx = tx + u;
			int tty = ty + v;

			if (ttx < 0 || ttx >= dest->width || tty < 0 || tty >= dest->height) continue;
			if (mask[tty][ttx] == 0) continue;

			CvScalar f = cvGet2D(src, ssy, ssx);
			CvScalar g = cvGet2D(dest, tty, ttx);
			sum += (f.val[0] - g.val[0]) * (f.val[0] - g.val[0]) + (f.val[1] - g.val[1]) * (f.val[1] - g.val[1]) + (f.val[2] - g.val[2]) * (f.val[2] - g.val[2]);
			count += 1;
		}
	}

	return sum / count;
}

CvScalar findBestMatchColor(IplImage* src, IplImage* dest, int tx, int ty, int** mask) {
	const int K = 3;

	double minSsd = DBL_MAX;
	int minX = -1, minY = -1;

	for (int y = K; y < src->height - K; y++) {
		for (int x = K; x < src->width - K; x++) {
			double ssd = calculateSsd(src, x, y, dest, tx, ty, K, mask);
			if (ssd < minSsd) {
				minSsd = ssd;
				minX = x;
				minY = y;
			}
		}
	}

	return cvGet2D(src, minY, minX);
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\161.bmp");
	CvSize size = cvSize(src->width * 2, src->height * 2);
	IplImage* dest = cvCreateImage(size, 8, 3);

	cvShowImage("src", src);

	cvSet(dest, cvScalar(0, 0, 0));

	int** mask = new int* [size.height];

	for (int y = 0; y < size.height; y++) {
		mask[y] = new int[size.width];
		for (int x = 0; x < size.width; x++) {
			mask[y][x] = 0;
		}
	}

	// fil src part first
	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			cvSet2D(dest, y, x, f);
			mask[y][x] = 1;
		}
	}

	// right section
	for (int x = src->width; x < size.width; x++) {
		for (int y = 0; y < src->height; y++) {
			CvScalar c = findBestMatchColor(src, dest, x, y, mask);
			cvSet2D(dest, y, x, c);
			mask[y][x] = 1;

			cvShowImage("dest", dest);
			cvWaitKey(1);
		}
	}

	// bottom section
	for (int y = src->height; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar c = findBestMatchColor(src, dest, x, y, mask);
			cvSet2D(dest, y, x, c);
			mask[y][x] = 1;

			cvShowImage("dest", dest);
			cvWaitKey(1);
		}
	}

	cvShowImage("dest", dest);

	cvWaitKey();

	return 0;
}