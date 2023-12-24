#define _USE_MATH_DEFINES
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <00/functional.cuh>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>

namespace utils {
int divRoundUp(int value, int radix) { return (value + radix - 1) / radix; };

void deviceInfo(int deviceID) {
  cudaDeviceProp dProp;
  cudaGetDeviceProperties(&dProp, deviceID);
  std::cout << "########################## CUDA Device Info "
               "##########################"
            << std::endl;
  std::cout << "Device: " << dProp.name << std::endl;
  std::cout << "Maximum number of threads per block:"
            << dProp.maxThreadsPerBlock << std::endl;
  std::cout << "Max dimension size of a thread block (x,y,z): ("
            << dProp.maxThreadsDim[0] << "," << dProp.maxThreadsDim[1] << ","
            << dProp.maxThreadsDim[2] << ")" << std::endl;
  std::cout << "Max dimension size of a grid size    (x,y,z): ("
            << dProp.maxGridSize[0] << "," << dProp.maxGridSize[1] << ","
            << dProp.maxGridSize[2] << ")" << std::endl;
  std::cout << "###############################################################"
               "#######"
            << std::endl;
}

Matrix::Matrix() {
  a = 1;
  b = 0;
  c = 0;
  d = 1;
  tx = 0;
  ty = 0;
}

Matrix::~Matrix() {}

Matrix Matrix::getRotMat(const float degree) {
  Matrix rotMat;

  const float rad = degree / 180.0f * M_PI;

  rotMat.a = std::cos(rad);
  rotMat.b = -std::sin(rad);
  rotMat.c = std::sin(rad);
  rotMat.d = std::cos(rad);

  return rotMat;
}

Matrix Matrix::inv() {
  Matrix inverted;

  float det = a * d - b * c;

  assert(det > 0.0f);

  inverted.a = d / det;
  inverted.b = -b / det;
  inverted.c = -c / det;
  inverted.d = a / det;
  inverted.tx = (b * ty - d * tx) / det;
  inverted.ty = (c * tx - a * ty) / det;

  return inverted;
}

std::string Matrix::to_string() {
  std::string buffer = "";
  buffer += std::to_string(a) + " " + std::to_string(b) + " " +
            std::to_string(tx) + "\n";
  buffer += std::to_string(c) + " " + std::to_string(d) + " " +
            std::to_string(ty) + "\n";
  buffer += std::to_string(0) + " " + std::to_string(0) + " " +
            std::to_string(1) + "\n";
  return buffer;
}
}  // namespace utils

namespace cKernel {
__global__ void convertToGrayKernel(uchar3 *inPixel,
                                    unsigned char *outinPixel) {
  int index = blockIdx.x * blockDim.x + threadIdx.x;

  outinPixel[index] = (unsigned char)(0.299f * inPixel[index].x +
                                      0.587f * (float)inPixel[index].y +
                                      0.114f * (float)inPixel[index].z);
}

__global__ void invertKernel(uchar3 *inPixel, uchar3 *outinPixel) {
  int index = blockIdx.x * blockDim.x + threadIdx.x;

  outinPixel[index].x = 255 - inPixel[index].x;
  outinPixel[index].y = 255 - inPixel[index].y;
  outinPixel[index].z = 255 - inPixel[index].z;
}

__global__ void warpKernel(const utils::Matrix transMat, const uchar4 *input,
                           uchar4 *output, const int width, const int height,
                           const int pitch) {
  const int x = blockDim.x * blockIdx.x + threadIdx.x;
  const int y = blockDim.y * blockIdx.y + threadIdx.y;

  if ((x < width) && (y < height)) {
    const float xOffset = 0.5f - width * 0.5f;
    const float yOffset = 0.5f - height * 0.5f;

    const float xOut = x + xOffset;
    const float yOut = y + yOffset;

    const float xIn =
        transMat.a * xOut + transMat.b * yOut + transMat.tx - xOffset;
    const float yIn =
        transMat.c * xOut + transMat.d * yOut + transMat.tx - yOffset;

    uchar4 value;
    if ((0.0f <= xIn) && (xIn < width) && (0.0f <= yIn) && (yIn < height)) {
      const int inIndex = (int)xIn + pitch * (int)yIn;
      value = input[inIndex];
    } else {
      value = make_uchar4(0, 0, 0, 0);
    }

    const int outIndex = x + pitch * y;
    output[outIndex] = value;
  }
}

}  // namespace cKernel

namespace cLaunch {
common::Image cudaWarpTransform(common::Image image, utils::Matrix &transMat) {
  const int height = image.getHeight();
  const int width = image.getWidth();
  const int channels = image.getChannles();
  assert(channels == 3);

  // Data format convertion
  uchar4 *hostImageIn = new uchar4[width * height];
  uchar4 *hostImageOut = new uchar4[width * height];

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      hostImageIn[w + h * width].x = image.getPixel(w, h, 0);
      hostImageIn[w + h * width].y = image.getPixel(w, h, 1);
      hostImageIn[w + h * width].z = image.getPixel(w, h, 2);
      hostImageIn[w + h * width].w = 255;
    }
  }

  uchar4 *deviceImageIn;
  uchar4 *deviceImageOut;
  utils::Matrix invTransMat = transMat.inv();

  {
    // Allocate device memory
    size_t hostPitch = sizeof(uchar4) * width;
    size_t devicePitch;
    cudaMallocPitch(&deviceImageIn, &devicePitch, sizeof(uchar4) * width,
                    height);
    cudaMallocPitch(&deviceImageOut, &devicePitch, sizeof(uchar4) * width,
                    height);
    const size_t pitchesInPixel = devicePitch / sizeof(uchar4);

    // Transfer (CPU → GPU)
    cudaMemcpy2D(deviceImageIn, devicePitch, hostImageIn, hostPitch, hostPitch,
                 height, cudaMemcpyHostToDevice);

    std::vector<int> nBlocksPower = {6, 7, 8, 9, 10};
    std::vector<std::vector<int>> blockPiars;
    for (int power : nBlocksPower) {
      for (int i = 0; i < power + 1; i++) {
        blockPiars.push_back(
            {(int)std::pow(2, i), (int)std::pow(2, power - i)});
      }
    }

    for (auto blockPiar : blockPiars) {
      dim3 blockDim(blockPiar[0], blockPiar[1]);
      dim3 gridDim(utils::divRoundUp(width, blockDim.x),
                   utils::divRoundUp(height, blockDim.y));

      // Launch
      std::chrono::system_clock::time_point start, end;
      start = std::chrono::system_clock::now();
      {
        int nTimes = std::pow(2, 17);
        for (int i = 0; i < nTimes; i++) {
          cKernel::warpKernel<<<gridDim, blockDim>>>(invTransMat, deviceImageIn,
                                                     deviceImageOut, width,
                                                     height, pitchesInPixel);
        }
      }
      end = std::chrono::system_clock::now();
      double elapsed =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
              .count();
      std::cout << "blockDim: (" << std::to_string(blockDim.x) << ", "
                << std::to_string(blockDim.y) << "), Kernel Execution Time: "
                << std::to_string(elapsed / 1000.0) << " [sec]" << std::endl;
    }

    // Transfer (GPU → CPU)
    cudaMemcpy2D(hostImageOut, hostPitch, deviceImageOut, devicePitch,
                 hostPitch, height, cudaMemcpyDeviceToHost);

    // Error check
    cudaError_t cudaError = cudaGetLastError();
    if (cudaError != 0) {
      std::cout << "Raised CUDA Error !" << std::endl;
    }
  }

  common::Image outImage = common::Image();
  outImage.newImage(width, height, channels);

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      outImage.setPixel(w, h, 0, hostImageOut[w + h * width].x);
      outImage.setPixel(w, h, 1, hostImageOut[w + h * width].y);
      outImage.setPixel(w, h, 2, hostImageOut[w + h * width].z);
    }
  }

  cudaFree(deviceImageIn);
  cudaFree(deviceImageOut);
  free(hostImageIn);
  free(hostImageOut);

  return outImage;
}

common::Image cudaGrayScaleTransform(common::Image image) {
  int height = image.getHeight();
  int width = image.getWidth();
  int channels = image.getChannles();

  assert(channels == 3);

  uchar3 *hostImageRGB = new uchar3[width * height];
  unsigned char *hostImageGray = new unsigned char[width * height];

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      hostImageRGB[w + h * width].x = image.getPixel(w, h, 0);
      hostImageRGB[w + h * width].y = image.getPixel(w, h, 1);
      hostImageRGB[w + h * width].z = image.getPixel(w, h, 2);
    }
  }

  uchar3 *deviceImageRGB;
  unsigned char *deviceImageGray;
  int dataSizeRGB = sizeof(uchar3) * width * height;
  int dataSizeGray = sizeof(unsigned char) * width * height;
  cudaMalloc((void **)&deviceImageRGB, dataSizeRGB);
  cudaMalloc((void **)&deviceImageGray, dataSizeGray);

  // Transfer (CPU → GPU)
  cudaMemcpy(deviceImageRGB, hostImageRGB, dataSizeRGB, cudaMemcpyHostToDevice);

  cKernel::convertToGrayKernel<<<width * height, 1>>>(deviceImageRGB,
                                                      deviceImageGray);

  // Transfer(GPU → CPU)
  cudaMemcpy(hostImageGray, deviceImageGray, dataSizeGray,
             cudaMemcpyDeviceToHost);

  common::Image grayImage = common::Image();
  grayImage.newImage(width, height, channels);

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      grayImage.setPixel(w, h, 0, hostImageGray[w + h * width]);
      grayImage.setPixel(w, h, 1, hostImageGray[w + h * width]);
      grayImage.setPixel(w, h, 2, hostImageGray[w + h * width]);
    }
  }

  cudaFree(deviceImageRGB);
  cudaFree(deviceImageGray);
  free(hostImageRGB);
  free(hostImageGray);

  return grayImage;
}

common::Image cudaInvertTransform(common::Image image) {
  int height = image.getHeight();
  int width = image.getWidth();
  int channels = image.getChannles();

  assert(channels == 3);

  uchar3 *hostImageIn = new uchar3[width * height];
  uchar3 *hostImageOut = new uchar3[width * height];

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      hostImageIn[w + h * width].x = image.getPixel(w, h, 0);
      hostImageIn[w + h * width].y = image.getPixel(w, h, 1);
      hostImageIn[w + h * width].z = image.getPixel(w, h, 2);
    }
  }

  uchar3 *deviceImageIn;
  uchar3 *deviceImageOut;
  int dataSizeIn = sizeof(uchar3) * width * height;
  int dataSizeOut = sizeof(uchar3) * width * height;
  cudaMalloc((void **)&deviceImageIn, dataSizeIn);
  cudaMalloc((void **)&deviceImageOut, dataSizeOut);

  // Transfer (CPU → GPU)
  cudaMemcpy(deviceImageIn, hostImageIn, dataSizeIn, cudaMemcpyHostToDevice);

  cKernel::invertKernel<<<width * height, 1>>>(deviceImageIn, deviceImageOut);

  // Transfer (GPU → CPU)
  cudaMemcpy(hostImageOut, deviceImageOut, dataSizeOut, cudaMemcpyDeviceToHost);

  common::Image outImage = common::Image();
  outImage.newImage(width, height, channels);

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      outImage.setPixel(w, h, 0, hostImageOut[w + h * width].x);
      outImage.setPixel(w, h, 1, hostImageOut[w + h * width].y);
      outImage.setPixel(w, h, 2, hostImageOut[w + h * width].z);
    }
  }

  cudaFree(deviceImageIn);
  cudaFree(deviceImageOut);
  free(hostImageIn);
  free(hostImageOut);

  cudaError_t cudaError = cudaGetLastError();
  std::cout << "cudaError=" << cudaError << std::endl;

  return outImage;
}
}  // namespace cLaunch