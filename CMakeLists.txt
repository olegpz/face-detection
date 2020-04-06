# CMakeLists for libfacedetect

project(libfacedetect)

cmake_minimum_required(VERSION 2.8)

option(ENABLE_AVX2 "use avx2" ON)
option(USE_OPENMP "Use OpenMP" ON)
option(BUILD_SHARED_LIBS "" ON)

add_definitions(-DBUILD_SHARED_LIBS)
if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
       # Just setting CMAKE_POSITION_INDEPENDENT_CODE should be enough to set
       # -fPIC for GCC but sometimes it still doesn't get set, so make sure it
       # does.
       add_definitions("-fPIC")
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

SET(BUILD_VERSION "v0.1")
message("BUILD_VERSION:${BUILD_VERSION}")

SET(fdt_base_dir   ${PROJECT_SOURCE_DIR})
SET(fdt_src_dir    ${fdt_base_dir}/src)
SET(fdt_inc_dir    ${fdt_base_dir}/src)

SET(fdt_lib_name   facedetect)

FILE(GLOB_RECURSE fdt_source_files ${fdt_src_dir}/*.cpp)
LIST(SORT         fdt_source_files)

SET(INSTALLHEADER_FILES ${fdt_inc_dir}/facedetectcnn.h)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

IF(MSVC)
    # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
    OPTION(WIN32_USE_MP "Set to ON to build with the /MP option (Visual Studio 2005 and above)." ON)
    IF(WIN32_USE_MP)
        #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        add_compile_options(/MP)
    ENDIF(WIN32_USE_MP)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF(MSVC)

IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
    OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    #use -O3 to speedup
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF()

if(ENABLE_AVX2)
	add_definitions(-D_ENABLE_AVX2)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mfma")
endif()

if(USE_OPENMP)
    FIND_PACKAGE(OpenMP)
    if(OPENMP_FOUND)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    endif()
endif()

INCLUDE_DIRECTORIES(${fdt_inc_dir})

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(GNUInstallDirs)

# Create a library
ADD_LIBRARY(${fdt_lib_name} ${fdt_source_files} ${INSTALLHEADER_FILES})

# Generate export header
GENERATE_EXPORT_HEADER(${fdt_lib_name})
string(TOLOWER ${fdt_lib_name} LOWER_PROJECT_NAME)
set(INSTALLHEADER_FILES ${INSTALLHEADER_FILES} 
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${fdt_lib_name} ${CMAKE_BINARY_DIR})

set_target_properties(${fdt_lib_name} PROPERTIES
    PUBLIC_HEADER "${INSTALLHEADER_FILES}" # Install head files
    VERSION ${BUILD_VERSION}
   )

# Install target
INSTALL(TARGETS ${fdt_lib_name}
    EXPORT ${fdt_lib_name}Config
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/facedetection
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
export(TARGETS ${fdt_lib_name}
       APPEND FILE ${CMAKE_BINARY_DIR}/${fdt_lib_name}Config.cmake
)
# Install cmake configure files
install(EXPORT ${fdt_lib_name}Config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
        )
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${BUILD_VERSION}
    COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake")

# Create application. OpenCV is requred.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost COMPONENTS filesystem  REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(detect-image ${fdt_base_dir}/detect-image.cpp ${fdt_base_dir}/face-detection.cpp)
target_link_libraries(detect-image ${fdt_lib_name} ${OpenCV_LIBS} ${Boost_LIBRARIES})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(unittest ${fdt_base_dir}/unittest.cpp)
target_link_libraries(unittest ${fdt_lib_name} ${OpenCV_LIBS} ${GTEST_LIBRARIES} pthread)

message("AVX2 = ${ENABLE_AVX2}")
message("OpenMP = ${OPENMP_FOUND}")
