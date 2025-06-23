#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
IplImage* buf = nullptr;
CvPoint mousePos;

float scale = 2.0f;

void myMouse(int event, int x, int y, int flags, void*) {
	cvCopy(src, buf);

	if (event == CV_EVENT_MOUSEMOVE) {
		mousePos = cvPoint(x, y);

		CvPoint topLeft = cvPoint(mousePos.x - 50, mousePos.y - 50);
		CvPoint bottomRight = cvPoint(mousePos.x + 50, mousePos.y + 50);

		for (int y2 = topLeft.y; y2 < bottomRight.y; y2++) {
			for (int x2 = topLeft.x; x2 < bottomRight.x; x2++) {
				float x1 = (x2 - mousePos.x) / scale + mousePos.x;
				float y1 = (y2 - mousePos.y) / scale + mousePos.y;

				if (x1 < 0 || x1 > src->width - 1) continue;
				if (y1 < 0 || y1 > src->height - 1) continue;

				if (x2 < 0 || x2 > src->width - 1) continue;
				if (y2 < 0 || y2 > src->height - 1) continue;

				CvScalar c = cvGet2D(src, y1, x1);

				cvSet2D(buf, y2, x2, c);
			}
		}

		cvShowImage("src", buf);
	}
}

int main() {
	src = cvLoadImage("C:\\temp\\lena.jpg");
	buf = cvCreateImage(cvGetSize(src), 8, 3);

	cvShowImage("src", src);
	cvSetMouseCallback("src", myMouse);
	cvWaitKey();

	return 0;
}
