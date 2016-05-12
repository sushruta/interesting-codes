#include <opencv2/ximgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace cv;
using namespace cv::ximgproc;

const char* keys =
{
  "{i || input image name}"
  "{m || model name}"
  "{o || output image name}"
};

int main(int argc, char **argv) {
  cv::CommandLineParser parser(argc, argv, keys);
  if ( !parser.check() ) {
    parser.printErrors();
    return -1;
  }

  string modelFilename = parser.get<string>("m");
  string inFilename = parser.get<string>("i");
  string outFilename = parser.get<string>("o");

  cv::Mat image = imread(inFilename, 1);
  if (image.empty()) {
    cout << "Cannot read the image: " << inFilename << endl;
    return -1;
  }
  image.convertTo(image, cv::DataType<float>::type, 1/255.0);

  cv::Mat edges(image.size(), image.type());

  cv::Ptr<StructuredEdgeDetection> pDollar =
    createStructuredEdgeDetection(modelFilename);
  pDollar->detectEdges(image, edges);

  if (outFilename == "") {
    cv::namedWindow("edges", 1);
    cv::imshow("edges", edges);

    cv::waitKey(0);
  } else {
    cv::imwrite(outFilename, 255*edges);
  }

  return 0;
}
