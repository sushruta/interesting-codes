#include <iostream>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


// There is no error checking at the moment
// One should ideally check if the dst is
// valid and of the same type and size as src
Mat imsmooth(Mat& src, int rad) {
  Mat dst;
  float p = 12.0f/rad/(rad + 2.0) - 2.0f;

  vector<float> kernelXY(2*rad + 1);
  float nrml = (rad + 1.0f) * (rad + 1.0f);
  for (int i=0; i<=rad; i++) {
    kernelXY[i] = (i + 1.0f) / nrml;
    kernelXY[2*rad - i] = (i + 1.0f) / nrml;
  }

  sepFilter2D(src, dst, -1, kernelXY, kernelXY);

  return dst;
}

void gradientHist(Mat& src, Mat& magnitude, Mat& histogram,
                  int nbins, int shrink, int rad) {
  Mat phase, Dx, Dy;

  magnitude.create(src.size(), DataType<float>::type);
  phase.create(src.size(), DataType<float>::type);

  int hist_mat_width = cvCeil(((float) src.size().width) / ((float) shrink));
  int hist_mat_height = cvCeil(((float) src.size().height) / ((float) shrink));

  histogram.create(Size(hist_mat_width, hist_mat_height),
                   CV_MAKE_TYPE(DataType<float>::type, nbins));
  histogram.setTo(0);

  Sobel(src, Dx, DataType<float>::type,
        1, 0, 1, 1.0, 0.0, BORDER_REFLECT);
  Sobel(src, Dy, DataType<float>::type,
        0, 1, 1, 1.0, 0.0, BORDER_REFLECT);

  int nchannels = src.channels();

  for (int row=0; row<src.rows; row++) {
    const float *pDx = Dx.ptr<float>(row);
    const float *pDy = Dy.ptr<float>(row);

    float *pMagnitude = magnitude.ptr<float>(row);
    float *pPhase = phase.ptr<float>(row);

    for (int col=0; col<src.cols*nchannels; col+=nchannels) {
      float maxMagn = -1.0f;
      float fdx, fdy;

      for (int channel=0; channel<nchannels; channel++) {
        float magn = pDx[col + channel]*pDx[col + channel] +
                     pDy[col + channel]*pDy[col + channel];
        if (magn > maxMagn) {
          maxMagn = magn;
          fdx = pDx[col + channel];
          fdy = pDy[col + channel];
        }
      }

      pMagnitude[col/nchannels] = maxMagn;

      // two neat tricks copied from the original author..!
      float angle = fastAtan2(fdy, fdx) / 180.0f - 1.0f * (fdy < 0);
      // fastATan2 gives 360 if fdx and fdy are close to zero. Correct that.
      if (fabs(fdx) + fabs(fdy) < 1e-5) {
        angle = 0.5f;
      }
      pPhase[col/nchannels] = angle;
    }
  }

  /* Now compute the histogram from 
   * the phase and magnitudes
  */

  // first normalize it w.r.t to its smoothed version
  magnitude /= imsmooth(magnitude, rad);

  for (int row=0; row<phase.rows; row++) {
    const float *pMagnitude = magnitude.ptr<float>(row);
    const float *pPhase = phase.ptr<float>(row);

    float *pHist = histogram.ptr<float>(row);

    for (int col=0; col<phase.cols; col++) {
      int idx = cvRound(col/shrink + pPhase[col]) * nbins;
      pHist[idx] += pMagnitude[col] / (shrink * shrink);
    }
  }

  return;
}

int main() {
  Mat img = imread("pic1.jpg");
  Mat dst, dst2;

  Mat histogram;
  histogram.create(Size(100, 100), CV_MAKE_TYPE(DataType<float>::type, 20));
  cout << histogram.size().width << ", " << histogram.size().height << ", " << histogram.channels() << endl;

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
