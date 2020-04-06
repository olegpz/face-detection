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

// A sample program demonstrating using Google C++ testing framework.

// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include <opencv2/opencv.hpp>
#include "facedetectcnn.h"
#include "face-detection.h"
#include "gtest/gtest.h"

using namespace std;
using namespace cv;

// namespace {

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>

// Tests function objectdetect_cnn.

// Tests face detection.
#define FACETEST(NAME,FILE,NFACES) \
TEST(FaceDetectionTest,NAME) { \
  Mat               in_image; \
  vector<FaceRect>  face; \
  /* load an image */ \
  try { in_image = imread(FILE); } catch (exception& e) { cerr << e.what() << endl; } \
  /* call a face detect function */ \
  face = objectdetect_cnn((unsigned char*)(in_image.ptr(0)), in_image.cols, in_image.rows, in_image.step); \
  /* Tests face detection of image with faces. */ \
  EXPECT_EQ(NFACES, face.size()); \
}

FACETEST(2007_007763,"img/faces/2007_007763.jpg",7)
FACETEST(2008_001009,"img/faces/2008_001009.jpg",2)
FACETEST(2008_001322,"img/faces/2008_001322.jpg",3)
FACETEST(2008_002079,"img/faces/2008_002079.jpg",6)
FACETEST(2008_002470,"img/faces/2008_002470.jpg",6)
FACETEST(2008_002506,"img/faces/2008_002506.jpg",3)
FACETEST(2008_004176,"img/faces/2008_004176.jpg",7)
FACETEST(2008_007676,"img/faces/2008_007676.jpg",7)
FACETEST(2009_004587,"img/faces/2009_004587.jpg",2)
FACETEST(bald_guys,"img/faces/bald_guys.jpg",24)
FACETEST(dogs,"img/faces/dogs.jpg",0)
FACETEST(Tom_Cruise_avp_2014_4,"img/faces/Tom_Cruise_avp_2014_4.jpg",1)

// }  // namespace

// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
