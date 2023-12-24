#pragma once
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include <common/GUI.hpp>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <vector>

namespace common {
class Image {
 public:
  using Pixels = std::shared_ptr<std::vector<std::vector<std::vector<int>>>>;

 private:
  Pixels _pixels = nullptr;

 public:
  Image(){};
  Image(Pixels pixels) { _pixels = pixels; };
  ~Image(){};

  void newImage(const int width, const int height, const int channels);

  int getWidth();
  int getHeight();
  int getChannles();
  int getPixel(const int w, const int h, const int c);
  Pixels getPixels();
  void setPixel(const int w, const int h, const int c, const int value);
  void setPixels(Pixels pixels);

  void saveAsJpg(const std::string filePath);

  static Image fromFile(const std::string filePath);
  static Image fromBytes(const unsigned char *bytePixels, const int imageWidth,
                         const int imageHeight, const int nChannels);
  uint8_t *convertToBytePixels();
};

void loadTexture(const std::string &filePath, GLuint &texID);
}  // namespace common
