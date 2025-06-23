#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
IplImage* buf = nullptr;
CvPoint p1;
CvPoint p2;
int mode = 0; // 0: black, 1: white
CvScalar color = cvScalar(0, 0, 0);

void myMouse(int event, int x, int y, int flags, void*) {
	cvCopy(src, buf);

	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
		p2 = cvPoint(x, y);
	}

	if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) == CV_EVENT_FLAG_LBUTTON) {
		p2 = cvPoint(x, y);
		cvRectangle(buf, p1, p2, color, 3);
		cvShowImage("src", buf);
	}

	if (event == CV_EVENT_LBUTTONUP) {
		p2 = cvPoint(x, y);
		cvRectangle(src, p1, p2, color, 3);
	}

	if (event == CV_EVENT_RBUTTONDOWN) {
		if (mode == 0) {
			color = cvScalar(255, 255, 255);
			mode = 1;
		}
		else {
			color = cvScalar(0, 0, 0);
			mode = 0;
		}
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
