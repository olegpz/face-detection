all: 	libfacedetect.so detect-image

MAKEFILES = makefile

SRC = \
src/face-detection.cpp \
src/facedetectcnn.cpp \
src/facedetectcnn-int8data.cpp \
src/facedetectcnn-model.cpp \

OBJ = $(SRC:.cpp=.o)

CFLAGS = \
-fopenmp \
-mavx2 \
-mfma \
-std=c++11 \
-I./src \
-Wall \
-g \


LFLAGS = \
-lopencv_shape \
-lopencv_stitching \
-lopencv_superres \
-lopencv_videostab \
-lopencv_aruco \
-lopencv_bgsegm \
-lopencv_bioinspired \
-lopencv_ccalib \
-lopencv_datasets \
-lopencv_dpm \
-lopencv_face \
-lopencv_freetype \
-lopencv_fuzzy \
-lopencv_hdf \
-lopencv_line_descriptor \
-lopencv_optflow \
-lopencv_video \
-lopencv_plot \
-lopencv_reg \
-lopencv_saliency \
-lopencv_stereo \
-lopencv_structured_light \
-lopencv_phase_unwrapping \
-lopencv_rgbd \
-lopencv_viz \
-lopencv_surface_matching \
-lopencv_text \
-lopencv_ximgproc \
-lopencv_calib3d \
-lopencv_features2d \
-lopencv_flann \
-lopencv_xobjdetect \
-lopencv_objdetect \
-lopencv_ml \
-lopencv_xphoto \
-lopencv_highgui \
-lopencv_videoio \
-lopencv_imgcodecs \
-lopencv_photo \
-lopencv_imgproc \
-lopencv_core \
-lboost_filesystem -lboost_system \


%.o: %.cpp $(MAKEFILES)
	g++ $(CFLAGS) $(LFLAGS) -c $< -o $@

$(OBJ): %.o: %.cpp $(MAKEFILES)
	g++ $(CFLAGS) $(LFLAGS) -fPIC -O0 -c $< -o $@

libfacedetect.so: $(OBJ)
	g++ $(CFLAGS) $(LFLAGS) -fPIC -shared -o $@ $(OBJ) `pkg-config --libs --cflags opencv`

detect-image: $(OBJ) $(MAKEFILES) detect-image.cpp
	g++ detect-image.cpp $(CFLAGS)  $(LFLAGS) -O0 -o detect-image -lfacedetect -L. -lboost_filesystem -lboost_system

test:	$(TST_SRC) $(MAKEFILES)
	export LD_LIBRARY_PATH=.
	./detect-image ./img/ ./out/

clean:
	-@rm -rf detect-image $(OBJ) libfacedetect.so

