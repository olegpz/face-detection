# face-detection
A function that detects faces in the image and returns the number of faces and the coordinates of the rectangles describing the face.
This is an library for CNN-based face detection in images. The CNN model has been converted to static variables in C source files. What you need is just a C++ compiler. You can compile the source code under Windows, Linux and any platform with a C++ compiler.

# How to compile

You can use the library.  
Step:

- Complie it

```bash
mkdir build
cd build
cmake --build . --config Release
```

## How to use

You can use 'detect-image' application with two command line arguments: first source image folder, second output image folder.

You can run 'unittest' application with one command line arguments: source image folder which contains Dlib C++ Library images for Google Test.