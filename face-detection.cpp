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
*- 0.0.1    25/03/2020  Oleg Prosekov   First issue
*
******************************************************************************/

#include <stdio.h>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include "facedetectcnn.h"

#include <map>
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
  String json_file = res_folder + "/result.json";

  // parse folder for image files
  vector<String> files; 
  glob(folder, files, true);

  size_t Nfiles = files.size();
  vector<image_info> image(Nfiles);

  for (size_t k = 0; k < Nfiles; k++)
  {
      // load an image
      image[k].res_file_name = files[k];
      cout << "File : " << files[k] << endl;
      Mat in_image = imread(files[k]); 
      if(in_image.empty())
      {
          fprintf(stderr, "Can not load the image file %s.\n", files[k].c_str());
          image[0] = {0, 0};
          return image;
      }

      ///////////////////////////////////////////
      // CNN face detection 
      // Best detection rate
      //////////////////////////////////////////
      TickMeter cvtm;
      cvtm.start();

      image[k].face = objectdetect_cnn((unsigned char*)(in_image.ptr(0)), in_image.cols, in_image.rows, in_image.step);
      
      cvtm.stop();    
      printf("time = %gms\n", cvtm.getTimeMilli());

      //print the detection results
      
      image[k].Nfaces =(int)image[k].face.size();
      printf("%d faces detected.\n", image[k].Nfaces);
      Mat result_image = in_image.clone();

      for(int i = 0; i < image[k].Nfaces; i++)
      {
        int x = image[k].face[i].x;
        int y = image[k].face[i].y;
        int w = image[k].face[i].w;
        int h = image[k].face[i].h;

        blur(result_image(Rect(x, y, w, h)), result_image(Rect(x, y, w, h)), Size(w/4, h/4));

      } // end for detect faces

      // imshow("result", result_image);
      resize(result_image, result_image, Size(result_image.cols/2, result_image.rows/2));
      image[k].res_file_name = img2res(files[k],folder,res_folder);
      imwrite(image[k].res_file_name, result_image);

      // waitKey();
  } // end for files

  write2json(json_file, image);

  return image;
  
} /* face_detection */