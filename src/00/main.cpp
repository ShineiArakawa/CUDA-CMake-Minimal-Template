#include <string>

#include <00/functional.cuh>
#include <image.hpp>

int main(int argc, char **argv) {
  utils::deviceInfo();

  std::string filePath =
      "/home/shinaraka/Projects/CUDA_Training/data/ShineiArakawa_cropped.JPG";
  std::string filePathOut =
      "/home/shinaraka/Projects/CUDA_Training/data/output.jpg";

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