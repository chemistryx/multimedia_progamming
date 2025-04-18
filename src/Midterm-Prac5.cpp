#include <opencv2/opencv.hpp>

void prob20(IplImage *src)
{
    IplImage *dest = cvCreateImage(cvGetSize(src), 8, 3);

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);
            double nx = x / double(src->width) * 20 - 10;
            double ny = y / double(src->height) * 20 - 10;

            double distanceFromOrigin = sqrt(nx * nx + ny * ny);

            CvScalar g = f;
            if ((int)distanceFromOrigin % 2 != 0)
            {
                g = cvScalar(0, 0, 0);
            }

            cvSet2D(dest, y, x, g);
        }
    }

    cvShowImage("prob20", dest);
}

void prob21(IplImage *src)
{
    IplImage *dest = cvCreateImage(cvGetSize(src), 8, 3);

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);
            double nx = x / double(src->width) * 20 - 10;
            double ny = y / double(src->height) * 20 - 10;

            double distanceFromOrigin = sqrt(nx * nx + ny * ny);

            CvScalar g = f;
            if (int(abs(nx) + abs(ny)) % 2 != 0)
            {
                g = cvScalar(0, 0, 0);
            }

            cvSet2D(dest, y, x, g);
        }
    }

    cvShowImage("prob21", dest);
}

void prob22(IplImage *src)
{
    IplImage *dest = cvCreateImage(cvGetSize(src), 8, 3);

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);

            double amplitude = (src->width / 4.0f);
            double freq = 2 * CV_PI / src->height;

            int shift = amplitude * sin(freq * y); // -(width / 4) ~ (width / 4)

            // printf("x: %d, y: %d, shift: %d\n", x, y, shift);

            if (x + shift < 0 || x + shift >= src->width)
                continue;

            cvSet2D(dest, y, x + shift, f);
        }
    }

    cvShowImage("prob22", dest);
}

void prob23(IplImage *src)
{
    IplImage *dest = cvCreateImage(cvGetSize(src), 8, 3);

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);

            double amplitude = (src->height / 4.0f);
            double freq = 2 * CV_PI / src->width;

            int shift = amplitude * sin(freq * x); // -(height / 4) ~ (height / 4)

            // printf("x: %d, y: %d, shift: %d\n", x, y, shift);

            if (y + shift < 0 || y + shift >= src->height)
                continue;

            cvSet2D(dest, y + shift, x, f);
        }
    }

    cvShowImage("prob23", dest);
}

void prob24(IplImage *src)
{
    IplImage *dest = cvCreateImage(cvGetSize(src), 8, 3);

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);

            double nx = x / double(src->width) * 10 - 5; // -5 ~ 5
            double ny = y / double(src->height) * 2;     // 0 ~ 2

            double sin1 = 2.5 * sin(CV_PI * ny);

            CvScalar g = f;

            // (sin1 - nx) 최솟값: nx: 5, ny: 1.5일때
            // sin1 = -2.5, nx = 5, sin1 - nx = -7.5
            // 안전하게 10 더해줌 - 양수 나머지 연산
            if (int(sin1 - nx + 10) % 2 != 0)
            {
                g = cvScalar(0, 0, 0);
            }

            cvSet2D(dest, y, x, g);
        }
    }

    cvShowImage("prob24", dest);
}

void prob25(IplImage *src)
{
    IplImage *dest = cvCreateImage(cvGetSize(src), 8, 3);

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);

            double nx = x / double(src->width) * 2;       // 0 ~ 2
            double ny = y / double(src->height) * 10 - 5; // -5 ~ 5

            double sin1 = 2.5 * sin(CV_PI * nx);

            CvScalar g = f;

            // (sin1 - ny) 최솟값: nx: 1.5, ny: 5일때
            // sin1 = -2.5, ny = 5, sin1 - ny = -7.5
            // 안전하게 10 더해줌 - 양수 나머지 연산
            if (int(sin1 - ny + 10) % 2 != 0)
            {
                g = cvScalar(0, 0, 0);
            }

            cvSet2D(dest, y, x, g);
        }
    }

    cvShowImage("prob25", dest);
}

int main()
{
    IplImage *src = cvLoadImage("C:\\temp\\lena.jpg");

    prob20(src);
    prob21(src);
    prob22(src);
    prob23(src);
    prob24(src);
    prob25(src);

    cvWaitKey();

    return 0;
}
