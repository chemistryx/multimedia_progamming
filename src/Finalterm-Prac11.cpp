#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
IplImage* buf = nullptr;
IplImage* dst = nullptr;

CvPoint p1;
CvPoint p2;

void drawImage(IplImage* src, IplImage* dst, CvPoint start, CvPoint end) {
	int startX = std::min(p1.x, p2.x);
	int startY = std::min(p1.y, p2.y);

	int w = abs(end.x - start.x);
	int h = abs(end.y - start.y);

	for (float y2 = 0; y2 < h; y2++) {
		for (float x2 = 0; x2 < w; x2++) {
			float nx = x2 / w;
			float ny = y2 / h;

			int x1 = nx * (src->width - 1);
			int y1 = ny * (src->height - 1);

			if (x1 < 0 || x1 > src->width - 1) continue;
			if (y1 < 0 || y1 > src->height - 1) continue;

			int drawX = startX + x2;
			int drawY = startY + y2;

			if (drawX > dst->width - 1) continue;
			if (drawY > dst->height - 1) continue;

			CvScalar c = cvGet2D(src, y1, x1);
			cvSet2D(dst, drawY, drawX, c);
		}
	}
}

void myMouse(int event, int x, int y, int flags, void* param) {

	if (event == CV_EVENT_LBUTTONDOWN) {
		cvCopy(dst, buf);
		p1 = cvPoint(x, y);
	}

	// drag
	if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) == CV_EVENT_FLAG_LBUTTON) {
		p2 = cvPoint(x, y);

		if (p2.x < 0 || p2.x > dst->width - 1) return;
		if (p2.y < 0 || p2.y > dst->height - 1) return;

		cvCopy(buf, dst);
		drawImage(src, dst, p1, p2);
		cvShowImage("dst", dst);
	}
}

int main() {
	src = cvLoadImage("C:\\temp\\lena.jpg");
	buf = cvCreateImage(cvGetSize(src), 8, 3);
	dst = cvCreateImage(cvGetSize(src), 8, 3);

	cvSet(dst, cvScalar(255, 255, 255));

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);

	cvWaitKey();

	return 0;
}
