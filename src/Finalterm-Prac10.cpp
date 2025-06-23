#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
IplImage* buf = nullptr;
CvPoint points[1000];
int count = 0;

void myMouse(int event, int x, int y, int flags, void* param) {
	//cvCopy(src, buf);

	if (event == CV_EVENT_LBUTTONDOWN) {
		points[count] = cvPoint(x, y);
		cvCircle(buf, points[count], 5, cvScalar(0, 255, 0), -1);

		if (count > 0) cvLine(buf, points[count - 1], points[count], cvScalar(0, 255, 0), 2);

		count += 1;
	}

	cvShowImage("src", buf);
}

int main() {
	src = cvLoadImage("C:\\temp\\lena.jpg");
	buf = cvCreateImage(cvGetSize(src), 8, 3);

	cvShowImage("src", src);
	cvSetMouseCallback("src", myMouse);

	cvCopy(src, buf);

	cvWaitKey();

	for (int i = 0; i < count - 1; i++) {
		CvPoint from = points[i];
		CvPoint to = points[i + 1];

		int steps = 100;

		for (int s = 0; s <= steps; s++) {
			float t = s / (float)steps;

			int x = (1 - t) * from.x + t * to.x;
			int y = (1 - t) * from.y + t * to.y;

			cvCopy(src, buf);
			cvCircle(buf, cvPoint(x, y), 10, cvScalar(0, 255, 0), -1);

			cvShowImage("src", buf);
			cvWaitKey(30);
		}
	}

	cvWaitKey();

	return 0;
}
