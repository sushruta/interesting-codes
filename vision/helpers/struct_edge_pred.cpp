#include <iostream>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "util.h"

using namespace cv;
using namespace std;

int main() {
  Mat img = imread("pic1.jpg");
  Mat dst, dst2;

  Mat histogram;
  histogram.create(Size(100, 100), CV_MAKE_TYPE(DataType<float>::type, 20));

  dst = imsmooth(img, 2);
  dst2 = img / dst;
  dst2 *= 255.0;

  // now display the image
  namedWindow("My src Image", WINDOW_AUTOSIZE);
  namedWindow("My dst Image", WINDOW_AUTOSIZE);
  namedWindow("My dst2 Image", WINDOW_AUTOSIZE);
  imshow("My src Image", img);
  imshow("My dst Image", dst);
  imshow("My dst2 Image", dst2);

  waitKey();
  return 0;
}
