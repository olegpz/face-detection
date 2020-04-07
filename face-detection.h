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
* @file:        face-detection.h
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
#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include "facedetectcnn.h"

struct face_rectangle
{
  int x,y;
  int w,h;
};

struct image_info
{
  std::string       res_file_name;
  int               Nfaces;
  vector<double>    confidence;
  vector<FaceRect>  face;
};

std::vector<image_info> face_detection(cv::String folder, cv::String res_folder);

#endif /* FACE_DETECTION_H */
