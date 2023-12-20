#pragma once

#include <cuda_runtime.h>
#include <image.hpp>
#include <string>

namespace utils {
int divRoundUp(int value, int radix);
void deviceInfo(int deviceID = 0);

class Matrix {
public:
  float a, b, c, d;
  float tx, ty;

  Matrix();
  ~Matrix();
  static Matrix getRotMat(const float degree);
  Matrix inv();
  std::string to_string();
};
} // namespace utils

namespace cKernel {
__global__ void convertToGrayKernel(uchar3 *color_pixel,
                                    unsigned char *gray_pixel);

__global__ void invertKernel(uchar3 *inPixel, uchar3 *outinPixel);

__global__ void warpKernel(const utils::Matrix transMat, const uchar4 *input,
                           uchar4 *output, const int width, const int height,
                           const int pitch);
} // namespace cKernel

namespace cLaunch {
common::Image cudaWarpTransform(common::Image image, utils::Matrix &transMat);
common::Image cudaGrayScaleTransform(common::Image image);
common::Image cudaInvertTransform(common::Image image);
} // namespace cLaunch