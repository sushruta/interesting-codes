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

// it's actually BGR to LUV lol
Mat rgb2luv(Mat& src) {
  Mat dst(src.size(), src.type());

  float X, Y, Z;
  float L, u, v;
  float up, vp;
  float un = 0.19793943; float vn = 0.46831096;

  float mx[3] = {0.412453, 0.357580, 0.180423};
  float my[3] = {0.212671, 0.715160, 0.072169};
  float mz[3] = {0.019334, 0.119193, 0.950227};
  int nchannels = src.channels();
  for (int row=0; row<src.rows; row++) {
    const float *pSrc = src.ptr<float>(row);
    float *pDst = dst.ptr<float>(row);

    for (int col=0; col<src.cols*nchannels; col+=nchannels) {
      X = 0.0f; Y = 0.0f; Z = 0.0f;
      for (int c=0; c<nchannels; c++) {
        X += mx[c] * pSrc[col+c];
        Y += my[c] * pSrc[col+c];
        Z += mz[c] * pSrc[col+c];
      }

      if (Y > 0.008856) {
        L = cvRound(116 * cubeRoot(Y));
      } else {
        L = 903.3 * Y;
      }

      float denom = X + 15.0f * Y + 3.0f * Z;
      up = 4.0f * X / denom;
      vp = 9.0f * Y / denom;

      u = 13.0f * L * (up - un);
      v = 13.0f * L * (vp - vn);

      pDst[col + 0] = L;
      pDst[col + 1] = u;
      pDst[col + 2] = v;
    }
  }

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

