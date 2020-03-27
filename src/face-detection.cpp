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
  vector<image_info> img_inf(Nfiles);

  for (size_t k = 0; k < Nfiles; k++)
  {
      // load an image and convert it to gray (single-channel)
      img_inf[k].res_file_name = files[k];
      cout << files[k] << endl;
      Mat image = imread(files[k]); 
      if(image.empty())
      {
          fprintf(stderr, "Can not load the image file %s.\n", files[k].c_str());
          img_inf[0] = {0, 0};
          return img_inf;
      }

      int * pResults = NULL; 
      //pBuffer is used in the detection functions.
      //If you call functions in multiple threads, please create one buffer for each thread!
      unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
      if (!pBuffer)
      {
          fprintf(stderr, "Can not alloc buffer.\n");
          img_inf[0] = {0, 0};
          return img_inf;
      }

      ///////////////////////////////////////////
      // CNN face detection 
      // Best detection rate
      //////////////////////////////////////////
      //!!! The input image must be a BGR one (three-channel) instead of RGB
      //!!! DO NOT RELEASE pResults !!!
      TickMeter cvtm;
      cvtm.start();

      pResults = facedetect_cnn(pBuffer, (unsigned char*)(image.ptr(0)), image.cols, image.rows, (int)image.step);
      
      cvtm.stop();    
      printf("time = %gms\n", cvtm.getTimeMilli());
      
      printf("%d faces detected.\n", (pResults ? *pResults : 0));
      Mat result_image = image.clone();

      //print the detection results
      img_inf[k].Nfaces = (pResults ? *pResults : 0);
      img_inf[k].face = (pResults ? new face_rectangle[*pResults] : NULL);

      for(int i = 0; i < (pResults ? *pResults : 0); i++)
      {
        short * p = ((short*)(pResults+1))+142*i;
        int confidence = p[0];
        int x = p[1];
        int y = p[2];
        int w = p[3];
        int h = p[4];
        
        //show the score of the face. Its range is [0-100]
        char sScore[256];
        snprintf(sScore, 256, "%d", confidence);
        //draw face rectangle
        // rectangle(result_image, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
        blur(result_image(Rect(x, y, w, h)), result_image(Rect(x, y, w, h)), Size(w/4, h/4));
        
        img_inf[k].face[i] = {.x = x, .y =y, .w = w, .h = h};
        //print the result
        // printf("face %d: confidence=%d, [%d, %d, %d, %d]\n", i, confidence, x, y, w, h);
 
      } // end for detect faces

      // imshow("result", result_image);
      resize(result_image, result_image, Size(result_image.cols/2, result_image.rows/2));
      // imshow("res_img", result_image);
      img_inf[k].res_file_name = img2res(files[k],folder,res_folder);
      imwrite(img_inf[k].res_file_name, result_image);

      // waitKey();

      //release the buffer
      free(pBuffer);
  } // end for files

  // fjs << js;
  write2json(json_file, img_inf);

  return img_inf;
  
} /* face_detection */