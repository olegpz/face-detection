all: 	libfacedetect.so detect-image

MAKEFILES = makefile

SRC = \
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
-lboost_filesystem -lboost_system \
`pkg-config --libs --cflags opencv` \


%.o: %.cpp $(MAKEFILES)
	g++ $(CFLAGS) $(LFLAGS) -c $< -o $@

$(OBJ): %.o: %.cpp $(MAKEFILES)
	g++ $(CFLAGS) $(LFLAGS) -fPIC -O0 -c $< -o $@

libfacedetect.so: $(OBJ)
	g++ $(CFLAGS) -fPIC -shared -o $@ $(OBJ) $(LFLAGS)

detect-image: $(OBJ) $(MAKEFILES) detect-image.cpp face-detection.cpp face-detection.h libfacedetect.so
	g++ face-detection.cpp detect-image.cpp $(CFLAGS)  $(LFLAGS) -O0 -o detect-image -lfacedetect -L.

test:	$(TST_SRC) $(MAKEFILES)
	export LD_LIBRARY_PATH=.
	./detect-image ./img/ ./out/

clean:
	-@rm -rf detect-image $(OBJ) libfacedetect.so

