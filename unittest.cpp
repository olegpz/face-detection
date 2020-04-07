// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <limits.h>
#include <opencv2/opencv.hpp>
#include "facedetectcnn.h"
#include "face-detection.h"
#include "gtest/gtest.h"

using namespace std;
using namespace cv;

static vector<String> files;

// Tests function objectdetect_cnn.

// Tests face detection.
#define FACETEST(NAME,NFACES)                                                                               \
TEST(FaceDetectionTest,NAME) {                                                                              \
  String            file = #NAME;                                                                           \
  Mat               in_image;                                                                               \
  vector<FaceRect>  face;                                                                                   \
  /* load an image */                                                                                       \
  size_t k;                                                                                                 \
  for (k = 0; k < files.size(); k++) {                                                                      \
    size_t t = files[k].find(file);                                                                         \
    if (t < files[k].length()) break;                                                                       \
  }                                                                                                         \
  try { in_image = imread(files[k]); } catch (exception& e) { cerr << e.what() << endl; }                   \
  /* call a face detect function */                                                                         \
  face = objectdetect_cnn((unsigned char*)(in_image.ptr(0)), in_image.cols, in_image.rows, in_image.step);  \
  /* Tests face detection of image with faces. */                                                           \
  EXPECT_EQ(NFACES, face.size());                                                                           \
} /* FACETEST */

// Find, detect images and check result (number of faces)
// Images took (https://github.com/davisking/dlib) from Dlib C++ Library (http://dlib.net/)
FACETEST(2007_007763,7)
FACETEST(2008_001009,2)
FACETEST(2008_001322,3)
FACETEST(2008_002079,6)
FACETEST(2008_002470,6)
FACETEST(2008_002506,3)
FACETEST(2008_004176,7)
FACETEST(2008_007676,7)
FACETEST(2009_004587,2)
FACETEST(bald_guys,24)
FACETEST(dogs,0)
FACETEST(Tom_Cruise_avp_2014_4,1)

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if (argc != 2)
  {
      printf("Usage: %s <image_folder_name>\n", argv[0]);
      return -1;
  }

  // parse folder for image files
  glob(String(argv[1]), files, true);

  return RUN_ALL_TESTS();

} /* main */
