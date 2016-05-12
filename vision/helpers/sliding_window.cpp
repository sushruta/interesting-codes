#include <iostream>
#include <string>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const char *keys =
{
  "{i || input image name}"
  "{o || output image name}"
};

// compute the HCF (or GCD) of two numbers
// This is not a generalized method. It
// expects sane input ...
int gcd(int a, int b) {
  int tmp;
  if (a < b) {
    tmp = a;
    a = b;
    b = tmp;
  }

  while (b != 0) {
    tmp = a;
    a = b;
    b = tmp%b;
  }

  return a;
}

// Don't use this for scaling the image up LOL
cv::Mat imresize(const cv::Mat& src, const double scale) {
  cv::Mat dst;
  cv::resize(src, dst, cv::Size(0, 0), scale, scale, cv::INTER_AREA);
  return dst;
}

int main(int argc, char **argv) {
  cv::CommandLineParser parser(argc, argv, keys);
  if (!parser.check()) {
    parser.printErrors();
    return -1;
  }

  std::string inFilename = parser.get<std::string>("i");
  std::string outFilename = parser.get<std::string>("o");

  cv::Mat img = cv::imread(inFilename, 1);
  if (img.empty()) {
    cout << "Cannot read image file " << inFilename << endl;
    return -1;
  }

  std::vector<cv::Mat> pyramid;
  double scales[] = {1.0, 0.75, 0.5, 0.25};
  for (int i=0; i<4; i++) {
    pyramid.push_back(imresize(img, scales[i]));
  }

  // Ok, now let's slide a window across
  // in each of the images in the pyramid.
  for (int i=0; i<pyramid.size(); i++) {
    cv::Mat& py_img = pyramid[i];
    cout << "Dealing with pyramid of size " << py_img.size() << endl;
    int window_length = gcd(py_img.size().width, py_img.size().height);
    int sx = 0; int sy = 0;

    cout << "window length is " << window_length << endl;
    
    while (sx < py_img.size().width) {
      sy = 0;
      while (sy < py_img.size().height) {
        cout << "sx : " << sx << " and " << " sy : " << sy << endl;
        Mat window = py_img(cv::Rect(sx, sy, window_length, window_length));
        /****
         * Do something with the window
        *****/
        sy += window_length;
      }
      sx += window_length;
    }

    cout << "-- Done --" << endl;
  }

  return 0;
}
