#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
IplImage* buf = nullptr;
CvPoint points[3];
int step = 0;

void myMouse(int event, int x, int y, int flags, void*) {
	cvCopy(src, buf);

	if (event == CV_EVENT_LBUTTONDOWN) {
		if (step == 0) {
			points[0] = cvPoint(x, y);
		}
		else if (step == 1) {
			points[1] = cvPoint(x, y);
		}
		else if (step == 2) {
			points[2] = cvPoint(x, y);
		}
	}

	if (event == CV_EVENT_MOUSEMOVE) {
		if (step == 1) {
			cvLine(buf, points[0], cvPoint(x, y), cvScalar(0, 0, 0), 3);
		}
		else if (step == 2) {
			cvLine(buf, points[1], cvPoint(x, y), cvScalar(0, 0, 0), 3);
		}

		cvShowImage("src", buf);
	}

	if (event == CV_EVENT_LBUTTONUP) {
		if (step == 1) {
			points[1] = cvPoint(x, y);
			cvLine(src, points[0], points[1], cvScalar(0, 0, 0), 3);

		}
		else if (step == 2) {
			points[2] = cvPoint(x, y);
			cvLine(src, points[1], points[2], cvScalar(0, 0, 0), 3);
			cvLine(src, points[2], points[0], cvScalar(0, 0, 0), 3);

			// check CCW
			// ad-bc (a, b) (c, d)
			CvPoint A = points[0], B = points[1], C = points[2];

			CvPoint vecAB = cvPoint(B.x - A.x, B.y - A.y);
			CvPoint vecAC = cvPoint(C.x - A.x, C.y - A.y);

			int cross = vecAB.x * vecAC.y - vecAB.y * vecAC.x;

			// OpenCV specific
			if (cross < 0) cvFillConvexPoly(src, points, 3, cvScalar(255, 255, 255));
		}

		cvShowImage("src", src);
		step += 1;
		if (step > 2) step = 0;
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
