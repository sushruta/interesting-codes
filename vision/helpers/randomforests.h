#include <opencv2/core.hpp>

using namespace cv;

class RFFeatureGetter : public Algorithm {
  public:
    virtual void getFeatures(const Mat& src,
                             const Mat& features,
                             const int radius,
                             const int gnrmRad,
                             const int gsmoothRad,
                             const int shrink,
                             const int outNum,
                             const int gradNum) const = 0;
};

class StructuredEdgeDetection : public Algorithm {
  public:
    virtual void detectEdges() const = 0
};

class RFFeatureGetterImpl : public RFFeatureGetter {
  public:
    RFFeatureGetterImpl() {
      name = "RFFeatureGetter";
    }

    void getFeatures(const Mat& src,
                     const Mat& features,
                     const int radius,
                     const int gnrmRad,
                     const int gsmoothRad,
                     const int shrink,
                     const int outNum,
                     const int gradNum) const {

      Mat luvImg = rgb2luv(src);

      vector<Mat> features;
      Size nsize = src.size() / float(shrink);

      // split the image into several channels
      split(imresize(luvImg, nsize), features);


    }

  protected:
    string name;
};
