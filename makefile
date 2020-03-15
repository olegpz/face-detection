all: 	detect-image libfacedetect.so

SRC = \
src/facedetectcnn.cpp \
src/facedetectcnn-int8data.cpp \
src/facedetectcnn-model.cpp \

OBJ = $(SRC:.cpp=.o)

CFLAGS = \
-O3 \
-D_ENABLE_AVX2 \
-mavx2 \
-mfma \
-fopenmp \
-std=c++11 \
-I./src \
-Wall \
-g \


LFLAGS = \
-lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_datasets -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_video -lopencv_plot -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_viz -lopencv_surface_matching -lopencv_text -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core \


%.o: %.cpp $(MAKEFILES)
	g++ $(CFLAGS) -c $< -o $@

$(OBJ): %.o: %.cpp $(MAKEFILES)
	g++ $(CFLAGS) -fPIC -O0 -c $< -o $@

libfacedetect.so: $(OBJ)
	g++ $(CFLAGS) -fPIC -shared -o $@ $(OBJ) 

detect-image: $(OBJ) $(MAKEFILES)
	g++ $(OBJ) detect-image.cpp $(CFLAGS) $(LFLAGS) -Og -o detect-image

test:	$(TST_SRC) $(MAKEFILES)
	g++ $(TST_SRC) -ITest_Environment/include $(CFLAGS) -o test_grio

clean:
	-@rm -rf detect-image $(OBJ)

