/*
By downloading, copying, installing or using the software you agree to this license.
If you do not agree to this license, do not download, install,
copy or use the software.


                  License Agreement For libfacedetection
                     (3-clause BSD License)

Copyright (c) 2018-2020, Shiqi Yu, all rights reserved.
shiqi.yu@gmail.com

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the names of the copyright holders nor the names of the contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

This software is provided by the copyright holders and contributors "as is" and
any express or implied warranties, including, but not limited to, the implied
warranties of merchantability and fitness for a particular purpose are disclaimed.
In no event shall copyright holders or contributors be liable for any direct,
indirect, incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitute goods or services;
loss of use, data, or profits; or business interruption) however caused
and on any theory of liability, whether in contract, strict liability,
or tort (including negligence or otherwise) arising in any way out of
the use of this software, even if advised of the possibility of such damage.
*/

#include <stdio.h>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "face-detection.h"

// #include "json.hpp"
// using json = nlohmann::json;

int main(int argc, char* argv[]) 
{
  // std::ifstream i(".vscode/launch.json");
  // json j;
  // i >> j;
  // std::cout << std::setw(4) << j << std::endl;

  if (argc != 3)
  {
      printf("Usage: %s <image_folder_name>  <output_folder_name>\n", argv[0]);
      return -1;
  }

  std::vector<image_info> img_inf = face_detection(cv::String(argv[1]),cv::String(argv[2]));

	return 0;
}
