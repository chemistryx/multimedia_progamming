#include <opencv2/opencv.hpp>

IplImage* src = nullptr;
IplImage* dst = nullptr;

void applySwirl(IplImage* src, IplImage* dst, float strength) {
	cvSet(dst, cvScalar(0, 0, 0));

	int cx = src->width / 2;
	int cy = src->height / 2;

	for (int y2 = 0; y2 < dst->height; y2++) {
		for (int x2 = 0; x2 < dst->width; x2++) {
			float dx = x2 - cx;
			float dy = y2 - cy;

			float radius = sqrt(dx * dx + dy * dy);
			float theta = atan2(dy, dx);

			float max_radius = sqrt(cx * cx + cy * cy);
			float factor = 1.0f - radius / max_radius;
			float theta_prime = theta + strength * factor;

			float x1 = cx + radius * cos(theta_prime);
			float y1 = cy + radius * sin(theta_prime);

			if (x1 < 0 || x1 > src->width - 1) continue;
			if (y1 < 0 || y1 > src->height - 1) continue;

			CvScalar c = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, c);
		}
	}
}

int main() {
	src = cvLoadImage("C:\\temp\\lena.jpg");
	dst = cvCreateImage(cvGetSize(src), 8, 3);

	cvCopy(src, dst);

	float t = 0.0f;

	while (true) {
		float strength = 5.0f * sin(t);
		applySwirl(src, dst, strength);

		cvShowImage("dst", dst);
		cvWaitKey(30);

		t += 0.1;
	}

	cvShowImage("dst", dst);
	//cvSetMouseCallback("dst", myMouse);
	cvWaitKey();

	return 0;
}
