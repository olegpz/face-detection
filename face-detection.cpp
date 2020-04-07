/**************************************************************************
*
* ====================================================
* Copyright (C)
*
* Permission to use, copy, modify, and distribute this
* software is freely granted, provided that this notice
* is preserved.
* ====================================================
*
* A function that detects faces in the image and returns the number of faces
* and the coordinates of the rectangles describing the face
*
* @file:        face-detection.cpp
* $Author:$     Oleg Prosekov
* $Revision:$   1
* $Date:$       25/03/2020
*
* History:
*
* Version   Date        Author          Change Description
*
*- 1.0.0    07/04/2020  Oleg Prosekov   First release
*- 0.0.1    25/03/2020  Oleg Prosekov   First issue
*
******************************************************************************/

#include <stdio.h>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include "facedetectcnn.h"

#include <vector>
#include <iostream>
#include "face-detection.h"

using namespace std;
using namespace cv;

static void write2json(String json_file, vector<image_info> img_inf)
{
  ofstream js(json_file);
  if (js.is_open()) {
    js << "{" << endl << "    \"Images\": [" << endl;

    for (auto k = 0; k < (int)img_inf.size(); k++)
    {
      js << "        {" << endl;
      js << "            \"File Name\": \"" << img_inf[k].res_file_name << "\"," << endl;
      js << "            \"Faces\": ["  << endl;
      for (auto i = 0; i < img_inf[k].Nfaces; i++)
      {
        js << "                {" << endl;
        js << "                    \"confidence\": " << img_inf[k].confidence[i] << "," << endl;
        js << "                    \"x\": " << img_inf[k].face[i].x << ",  \"y\": " << img_inf[k].face[i].y << "," << endl;
        js << "                    \"width\": " << img_inf[k].face[i].w << ",  \"height\": " << img_inf[k].face[i].h << endl;
        js << "                }";
        if (i < img_inf[k].Nfaces-1) js << ",";
        js << endl;
      }
      js << "            ]" << endl;
      js << "        }";
      if (k < (int)img_inf.size()-1) js << ",";
      js << endl;
    }

    js << "    ]" << endl << "}" << endl;

    js.close();
  }

} /* write2json */

static String img2res(String img_file, String img_folder, String res_folder)
{
  // size_t first = img_file.find_last_of("/");
  size_t first = img_file.find(img_folder);
  String res_file = res_folder + img_file.substr(first+img_folder.length(),img_file.length());
  // cout << first << " : " << res_file << endl;
  boost::filesystem::path p(res_file);
  try
  {
    boost::filesystem::create_directories(p.parent_path());
  }
  catch(const boost::filesystem::filesystem_error& err)
  {
    cerr << err.what() << endl;
  }

  return res_file;

} /* img2res */

vector<image_info> face_detection(String folder, String res_folder)
{
  String json_file = folder + "/result.json";

  // parse folder for image files
  vector<String> files;
  glob(folder, files, true);

  size_t Nfiles = files.size();
  vector<image_info> images;

  // Process all files
  for (size_t k = 0; k < Nfiles; k++)
  {
      // load an image
      Mat         in_image;
      image_info  image;

      try
      {
        in_image = imread(files[k]);
      }
      catch (std::exception& e)
      {
        cout << e.what() << endl;
        continue;
      }

      if (in_image.empty())
      {
        // fprintf(stderr, "Can not load the image file %s.\n", files[k].c_str());
        continue;
      }
      else
      {
        image.res_file_name = files[k];
        cout << "File : " << files[k] << endl;
      }


      ///////////////////////////////////////////
      // CNN face detection
      //////////////////////////////////////////
      TickMeter cvtm;
      cvtm.start();

      image.face = objectdetect_cnn((unsigned char*)(in_image.ptr(0)), in_image.cols, in_image.rows, in_image.step);

      cvtm.stop();
      printf("time = %gms\n", cvtm.getTimeMilli());

      // store the detection results

      image.Nfaces =(int)image.face.size();
      printf("%d faces detected.\n", image.Nfaces);
      Mat res_image = in_image.clone();

      for(int i = 0; i < image.Nfaces; i++)
      {
        int x = image.face[i].x;
        int y = image.face[i].y;
        int w = image.face[i].w;
        int h = image.face[i].h;
        image.confidence.push_back(sqrt(image.face[i].score) * 100);

        // rectangle(res_image, Rect(x, y, w, h), Scalar(0, 255, 0), 2);

        blur(res_image(Rect(x, y, w, h)), res_image(Rect(x, y, w, h)), Size(w/4, h/4));

        // printf("face %d: confidence=%g, [%d, %d, %d, %d]\n",
        //         i, image[k].confidence[i], x, y, w, h);

      } // end for detect faces

      // imshow("result", res_image);

      resize(res_image, res_image, Size(res_image.cols/2, res_image.rows/2));
      image.res_file_name = img2res(files[k],folder,res_folder);
      // Write results to image files
      imwrite(image.res_file_name, res_image);
      images.push_back(image);

      // waitKey();
  } // end for files

  // Write JSON file which contains info of input images
  write2json(json_file, images);

  return images;

} /* face_detection */