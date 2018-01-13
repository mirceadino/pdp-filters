#ifndef PDP_PROJECT_UTILS_
#define PDP_PROJECT_UTILS_

#include <bits/stdc++.h>
#include <Magick++.h>

#include "matrix.h"

template<typename T>
struct RgbMatrix {
  RgbMatrix(int h, int w):
    height(h), width(w), red(h, w), green(h, w), blue(h, w) {}

  int height, width;
  Matrix<T> red;
  Matrix<T> green;
  Matrix<T> blue;
};


RgbMatrix<int> ImageToRgbMatrix(const Magick::Image& image) {
  const int h = image.rows();
  const int w = image.columns();

  RgbMatrix<int> matrix(h, w);

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      Magick::Color color = image.pixelColor(j, i);
      const int rvalue = color.quantumRed() * 1LL * 255 / QuantumRange;
      const int gvalue = color.quantumGreen() * 1LL * 255 / QuantumRange;
      const int bvalue = color.quantumBlue() * 1LL * 255 / QuantumRange;
      matrix.red[i][j] = rvalue;
      matrix.green[i][j] = gvalue;
      matrix.blue[i][j] = bvalue;
    }
  }

  return matrix;
}


Magick::Image RgbMatrixToImage(const RgbMatrix<int>& matrix) {
  const int h = matrix.height;
  const int w = matrix.width;

  Magick::Image image(Magick::Geometry(w, h), "white");

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      const int rvalue = matrix.red[i][j] * 1LL * QuantumRange / 255;
      const int gvalue = matrix.green[i][j] * 1LL * QuantumRange / 255;
      const int bvalue = matrix.blue[i][j] * 1LL * QuantumRange / 255;
      Magick::Color color(rvalue, gvalue, bvalue);
      image.pixelColor(j, i, color);
    }
  }
  return image;
}

#endif  // PDP_PROJECT_UTILS_
