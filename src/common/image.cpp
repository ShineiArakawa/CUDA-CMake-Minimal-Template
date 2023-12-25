#include <common/image.hpp>

namespace common {
void Image::newImage(const int width, const int height, const int channels) {
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

int Image::getWidth() {
  int width = -1;
  if (_pixels != nullptr) {
    width = _pixels->size();
  }
  return width;
}

int Image::getHeight() {
  int height = -1;
  if (_pixels != nullptr) {
    height = (*_pixels)[0].size();
  }
  return height;
}

int Image::getChannles() {
  int channels = -1;
  if (_pixels != nullptr) {
    channels = (*_pixels)[0][0].size();
  }
  return channels;
}

int Image::getPixel(const int w, const int h, const int c) {
  int value = -1;
  if (_pixels != nullptr) {
    value = (*_pixels)[w][h][c];
  }
  return value;
}

Image::Pixels Image::getPixels() { return _pixels; }

void Image::setPixel(const int w, const int h, const int c, const int value) {
  if (_pixels != nullptr) {
    (*_pixels)[w][h][c] = value;
  }
}

void Image::setPixels(Pixels pixels) { _pixels = pixels; }

void Image::saveAsJpg(const std::string filePath) {
  if (_pixels != nullptr) {
    int imageWidth = getWidth();
    int imageHeight = getHeight();
    int nChannels = getChannles();

    auto outBytePixels = convertToBytePixels();

    stbi_write_jpg(filePath.c_str(), imageWidth, imageHeight, nChannels,
                   outBytePixels, 100);
  }
};

Image Image::fromFile(const std::string filePath) {
  int imageWidth, imageHeight, nChannels = 0;
  unsigned char *bytePixels =
      stbi_load(filePath.c_str(), &imageWidth, &imageHeight, &nChannels, 3);

  return Image::fromBytes(bytePixels, imageWidth, imageHeight, nChannels);
};

Image Image::fromBytes(const unsigned char *bytePixels, const int imageWidth,
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

uint8_t *Image::convertToBytePixels() {
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

}  // namespace common