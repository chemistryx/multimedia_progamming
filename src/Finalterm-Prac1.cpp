#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
CvPoint p1;
CvPoint p2;

void myMouse(int event, int x, int y, int flags, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
		p2 = cvPoint(x, y);
	}

	if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) == CV_EVENT_FLAG_LBUTTON) {
		p1 = p2;
		p2 = cvPoint(x, y);
		cvLine(src, p1, p2, cvScalar(255, 255, 255), 3);
	}

	if (event == CV_EVENT_LBUTTONUP) {
		p1 = p2;
		p2 = cvPoint(x, y);
	}

	cvShowImage("src", src);
}

int main() {
	src = cvLoadImage("C:\\temp\\lena.jpg");

	cvShowImage("src", src);
	cvSetMouseCallback("src", myMouse);
	cvWaitKey();

	return 0;
}
