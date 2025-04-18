#include <opencv2/opencv.hpp>

int main() {
	IplImage* src = cvLoadImage("C:\\temp\\lena.jpg");
	IplImage* dest = cvCreateImage(cvGetSize(src), 8, 3);

	double contrast = 1.0f;
	double brightness = 0.0f;
	int key = 0;

	while (key != 'q') {
		for (int y = 0; y < src->height; y++) {
			for (int x = 0; x < src->width; x++) {
				CvScalar f = cvGet2D(src, y, x);
				CvScalar g;

				for (int k = 0; k < 3; k++) {
					g.val[k] = contrast * f.val[k] + brightness;
				}

				cvSet2D(dest, y, x, g);
			}
		}

		key = cvWaitKey();

		switch (key) {
		case '1':
			brightness += 10;
			break;
		case '2':
			brightness -= 10;
			break;
		case '3':
			contrast += 0.1;
			break;
		case '4':
			contrast -= 0.1;
			break;
		}

		cvShowImage("dest", dest);
	}

	cvWaitKey();

	return 0;
}