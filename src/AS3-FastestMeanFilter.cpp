#include <opencv2/opencv.hpp>

using namespace std;

void myFastestMeanFilter(IplImage* src, IplImage* dest, int K) {
	printf("mean filter\n");
	// 1. generate SAT
	CvScalar** SAT = new CvScalar * [src->height];

	printf("1. generate SAT\n");
	for (int y = 0; y < src->height; y++) {
		SAT[y] = new CvScalar[src->width];
		for (int x = 0; x < src->width; x++) {
			SAT[y][x] = cvGet2D(src, y, x);

			if (x > 0) {
				SAT[y][x].val[0] += SAT[y][x - 1].val[0];
				SAT[y][x].val[1] += SAT[y][x - 1].val[1];
				SAT[y][x].val[2] += SAT[y][x - 1].val[2];
			}

			if (y > 0) {
				SAT[y][x].val[0] += SAT[y - 1][x].val[0];
				SAT[y][x].val[1] += SAT[y - 1][x].val[1];
				SAT[y][x].val[2] += SAT[y - 1][x].val[2];
			}

			if (x > 0 && y > 0) {
				SAT[y][x].val[0] -= SAT[y - 1][x - 1].val[0];
				SAT[y][x].val[1] -= SAT[y - 1][x - 1].val[1];
				SAT[y][x].val[2] -= SAT[y - 1][x - 1].val[2];
			}
		}
	}


	// 2. apply mean filter
	printf("2. apply mean filter\n");
	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			int upperX = max(x - K, 0);
			int upperY = max(y - K, 0);
			int lowerX = min(x + K, src->width - 1);
			int lowerY = min(y + K, src->height - 1);

			int width = lowerX - upperX + 1;
			int height = lowerY - upperY + 1;

			CvScalar LR = SAT[lowerY][lowerX];
			CvScalar UR = upperY > 0 ? SAT[upperY - 1][lowerX] : cvScalar(0, 0, 0);
			CvScalar LL = upperX > 0 ? SAT[lowerY][upperX - 1] : cvScalar(0, 0, 0);
			CvScalar UL = upperX > 0 && upperY > 0 ? SAT[upperY - 1][upperX - 1] : cvScalar(0, 0, 0);
			CvScalar g;

			for (int k = 0; k < 3; k++) {
				g.val[k] = (LR.val[k] - UR.val[k] - LL.val[k] + UL.val[k]) / (width * height);
			}

			cvSet2D(dest, y, x, g);
		}
	}
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\lena.jpg");
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	const int K = 50;

	myFastestMeanFilter(src, dest, K);

	cvShowImage("src", src);
	cvShowImage("dest", dest);

	cvWaitKey();

	return 0;
}