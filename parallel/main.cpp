#include <bits/stdc++.h>
#include <Magick++.h>

#include "kernels.h"
#include "matrix.h"
#include "utils.h"

using namespace Magick;
using namespace std;

RgbMatrix<int> Solve(const RgbMatrix<int>& input, const Matrix<int>& kernel,
                     int num_threads);

int main(int argc, char** argv) {
  if (argc <= 4) {
    fprintf(stderr,
            "usage: %s <input_image> <output_image> <kernel> <num_threads>\n",
            argv[0]);
    for (int i = 0; i < Kernels::num_kernels(); ++i) {
      fprintf(stderr, "kernel #%d: %s\n", i, Kernels::name(i).c_str());
    }
    return 1;
  }

  string input_filename(argv[1]);
  string output_filename(argv[2]);
  int kernel;
  sscanf(argv[3], "%d", &kernel);
  int num_threads;
  sscanf(argv[4], "%d", &num_threads);

  Image input_image, output_image;
  try {
    fprintf(stderr, "Attempting to read \"%s\"... ", input_filename.c_str());
    input_image.read(input_filename);
    fprintf(stderr, "Image was read.\n");

    fprintf(stderr, "Attempting to convert image to RGB matrix... ");
    RgbMatrix<int> rgb_matrix = ImageToRgbMatrix(input_image);
    fprintf(stderr, "Conversion to RGB matrix is done.\n");

    rgb_matrix = Solve(rgb_matrix, Kernels::kernel(kernel), num_threads);

    fprintf(stderr, "Attempting to convert RGB matrix to image... ");
    output_image = RgbMatrixToImage(rgb_matrix);
    fprintf(stderr, "Conversion to image is done.\n");

    fprintf(stderr, "Attempting to write to \"%s\"... ", output_filename.c_str());
    output_image.write(output_filename);
    fprintf(stderr, "Image was written.\n");

  } catch (Exception& err) {
    cout << err.what() << endl;
    fprintf(stderr, "Some error occured.\n");
    return 1;
  }

  return 0;
}

void ComputeLines(const RgbMatrix<int>& input, const Matrix<int>& kernel,
                  vector<int> lines, RgbMatrix<int>* output) {
  for (const int i : lines) {
    /* fprintf(stderr, "Computing line #%i... \n", i); */

    for (int j = 0; j < output->width; ++j) {
      const int x = i - kernel.h() / 2;
      const int y = j - kernel.w() / 2;
      output->red[i][j] = input.red.convolute(kernel, x, y);
      output->green[i][j] = input.green.convolute(kernel, x, y);
      output->blue[i][j] = input.blue.convolute(kernel, x, y);
    }

    /* fprintf(stderr, "Line #%i was computed.\n", i); */
  }
}

RgbMatrix<int> Solve(const RgbMatrix<int>& input, const Matrix<int>& kernel,
                     int num_threads) {
  RgbMatrix<int> output(input.height, input.width);

  vector<future<void>> futures;
  const int lines_per_thread = input.height / num_threads +
                               (input.height % num_threads != 0);

  for (int i = 0; i < num_threads; ++i) {
    vector<int> lines;
    for (int k = i * lines_per_thread;
         k < min(input.height, (i + 1)*lines_per_thread);
         ++k) {
      lines.push_back(k);
    }
    futures.push_back(async(launch::async, ComputeLines,
                            ref(input), ref(kernel), lines, &output));
  }

  for (future<void>& future : futures) {
    future.wait();
  }

  return output;
}
