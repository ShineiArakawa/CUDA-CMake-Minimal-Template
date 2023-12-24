#include <string>

#include <00/functional.cuh>
#include <common/image.hpp>

int main(int argc, char **argv) {
  utils::deviceInfo();

  std::string filePath =
      "D:/GitHub/CUDA-CMake-Minimal-Template/data/image_1.jpg";
  std::string filePathOut =
      "D:/GitHub/CUDA-CMake-Minimal-Template/data/output.jpg";

  common::Image image = common::Image::fromFile(filePath);

  common::Image transformedImage;
  {
    // transformedImage = cudaGrayScaleTransform(image);
  }
  {
    // transformedImage = cudaInvertTransform(image);
  }
  {
    utils::Matrix tranMat = utils::Matrix::getRotMat(90.0);
    transformedImage = cLaunch::cudaWarpTransform(image, tranMat);
  }

  // Save
  transformedImage.saveAsJpg(filePathOut);
}