#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
IplImage* buf = nullptr;
CvPoint mousePos;
int radius = 0;

void myMouse(int event, int x, int y, int flags, void*) {
	cvCopy(src, buf);

	if (event == CV_EVENT_MOUSEMOVE) {
		mousePos = cvPoint(x, y);
	}
}

int main() {
	src = cvLoadImage("C:\\temp\\lena.jpg");
	buf = cvCreateImage(cvGetSize(src), 8, 3);

	cvShowImage("src", src);
	cvSetMouseCallback("src", myMouse);

	int t = 0;
	while (true) {
		radius = 20 + int(20 * sin(t * 0.1));

		cvCopy(src, buf);

		cvCircle(buf, mousePos, radius, cvScalar(255, 255, 255), -1);

		cvShowImage("src", buf);

		t += 1;
		cvWaitKey(30);
	}

	cvWaitKey();

	return 0;
}
