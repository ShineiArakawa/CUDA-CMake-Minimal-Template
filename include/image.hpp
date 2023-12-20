#pragma once
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <cstdlib>
#include <memory>
#include <vector>

#include <stb_image.h>
#include <stb_image_write.h>

#include <DearImGUI.hpp>

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

  void newImage(const int width, const int height, const int channels) {
    _pixels = std::make_shared<std::vector<std::vector<std::vector<int>>>>();

    for (int w = 0; w < width; w++) {
      _pixels->push_back(std::vector<std::vector<int>>());
      for (int h = 0; h < height; h++) {
        (*_pixels)[w].push_back(std::vector<int>());
        for (int c = 0; c < channels; c++) {
          (*_pixels)[w][h].push_back(0);
        }
      }
    }
  }

  int getWidth() {
    int width = -1;
    if (_pixels != nullptr) {
      width = _pixels->size();
    }
    return width;
  }

  int getHeight() {
    int height = -1;
    if (_pixels != nullptr) {
      height = (*_pixels)[0].size();
    }
    return height;
  }

  int getChannles() {
    int channels = -1;
    if (_pixels != nullptr) {
      channels = (*_pixels)[0][0].size();
    }
    return channels;
  }

  int getPixel(const int w, const int h, const int c) {
    int value = -1;
    if (_pixels != nullptr) {
      value = (*_pixels)[w][h][c];
    }
    return value;
  }

  Pixels getPixels() { return _pixels; }

  void setPixel(const int w, const int h, const int c, const int value) {
    if (_pixels != nullptr) {
      (*_pixels)[w][h][c] = value;
    }
  }

  void setPixels(Pixels pixels) { _pixels = pixels; }

  void saveAsJpg(const std::string filePath) {
    if (_pixels != nullptr) {
      int imageWidth = getWidth();
      int imageHeight = getHeight();
      int nChannels = getChannles();

      auto outBytePixels = convertToBytePixels();

      stbi_write_jpg(filePath.c_str(), imageWidth, imageHeight, nChannels,
                     outBytePixels, 100);
    }
  };

  static Image fromFile(const std::string filePath) {
    int imageWidth, imageHeight, nChannels = 0;
    unsigned char *bytePixels =
        stbi_load(filePath.c_str(), &imageWidth, &imageHeight, &nChannels, 3);

    return Image::fromBytes(bytePixels, imageWidth, imageHeight, nChannels);
  };

  static Image fromBytes(const unsigned char *bytePixels, const int imageWidth,
                         const int imageHeight, const int nChannels) {
    // Alloc
    Pixels pixels =
        std::make_shared<std::vector<std::vector<std::vector<int>>>>();

    for (int w = 0; w < imageWidth; w++) {
      pixels->push_back(std::vector<std::vector<int>>());

      for (int h = 0; h < imageHeight; h++) {
        (*pixels)[w].push_back(std::vector<int>());
      }
    }

    // Convert
    for (int w = 0; w < imageWidth; w++) {
      for (int h = 0; h < imageHeight; h++) {
        const unsigned char *texel =
            bytePixels + (w + imageWidth * h) * nChannels;

        for (int channel = 0; channel < nChannels; channel++) {
          const unsigned char charValue = texel[channel];
          const int value = charValue;
          (*pixels)[w][h].push_back(value);
        }
      }
    }

    return Image(pixels);
  };

  uint8_t *convertToBytePixels() {
    uint8_t *bytePixels = nullptr;

    if (_pixels != nullptr) {
      int width = getWidth();
      int height = getHeight();
      int nChannels = getChannles();

      bytePixels = (uint8_t *)malloc(width * height * nChannels);

      int index = 0;
      for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
          for (int channel = 0; channel < nChannels; channel++) {
            const int value = getPixel(w, h, channel);
            bytePixels[index++] = value;
          }
        }
      }
    }

    return bytePixels;
  };
};

inline void loadTexture(const std::string &filePath, GLuint &texID) {
  int texWidth, texHeight, channels;
  // Texture
  // ============================================================================================
  unsigned char *bytesTexture = stbi_load(
      filePath.c_str(), &texWidth, &texHeight, &channels, STBI_rgb_alpha);
  if (!bytesTexture) {
    fprintf(stderr, "Failed to load image file: %s\n", filePath.c_str());
    exit(1);
  }

  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, bytesTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(bytesTexture);
}
} // namespace common
