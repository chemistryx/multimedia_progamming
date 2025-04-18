#include <opencv2/opencv.hpp>

void computeHistogram(IplImage* src, int histogram[][256]) {
	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			histogram[0][(int)f.val[0]] += 1;
			histogram[1][(int)f.val[1]] += 1;
			histogram[2][(int)f.val[2]] += 1;
		}
	}
}

void computeCumulativeHistogram(int cumulativeHistogram[][256], int histogram[][256]) {
	cumulativeHistogram[0][0] = histogram[0][0];
	cumulativeHistogram[1][0] = histogram[1][0];
	cumulativeHistogram[2][0] = histogram[2][0];

	for (int i = 0; i < 3; i++) {
		for (int j = 1; j < 256; j++) {
			cumulativeHistogram[i][j] = cumulativeHistogram[i][j - 1] + histogram[i][j];
		}
	}
}

void histogramEqualization(IplImage* src, IplImage* dest, int cumulativeHistogram[][256]) {
	int numPixels = src->width * src->height;

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;

			for (int k = 0; k < 3; k++) {
				g.val[k] = cumulativeHistogram[k][(int)f.val[k]] / double(numPixels) * 255;
			}

			cvSet2D(dest, y, x, g);
		}
	}
}

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\histogram1.jpg");
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);
	int histogram[3][256];
	int cumulativeHistogram[3][256];

	// init array
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 256; j++) {
			histogram[i][j] = 0;
			cumulativeHistogram[i][j] = 0;
		}
	}

	computeHistogram(src, histogram);
	computeCumulativeHistogram(cumulativeHistogram, histogram);
	histogramEqualization(src, dest, cumulativeHistogram);

	cvShowImage("dest", dest);

	cvWaitKey();

	return 0;
}