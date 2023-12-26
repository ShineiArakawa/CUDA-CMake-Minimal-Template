#include <00/functional.cuh>
#include <common/image.hpp>
#include <string>

int main(int argc, char **argv) {
  utils::deviceInfo();

  std::string filePath =
      "../data/image_1.jpg";
  std::string filePathOut =
      "../data/output.jpg";

  common::Image image = common::Image::fromFile(filePath);

  common::Image transformedImage;
  {
    // transformedImage = cLaunch::cudaGrayScaleTransform(image);
  }
  {
    // transformedImage = cLaunch::cudaInvertTransform(image);
  }
  {
    utils::Matrix tranMat = utils::Matrix::getRotMat(90.0);
    transformedImage = cLaunch::cudaWarpTransform(image, tranMat);
  }

  // Save
  transformedImage.saveAsJpg(filePathOut);
}