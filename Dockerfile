FROM nvidia/cuda:11.3.1-cudnn8-devel-ubuntu20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Tokyo
ENV PATH=$PATH:/usr/local/cuda/bin
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib64

ARG OPENCV_VERSION=4.5.4
ARG NPROC=4

# OpenCV
# Dependencies
RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y --no-install-recommends \
    build-essential cmake git pkg-config wget unzip \
    python-dev python3-dev python-numpy libtbb2 \
    qtbase5-dev \
    qtdeclarative5-dev \
    freeglut3 freeglut3-dev libtbb-dev \
    libjpeg8-dev libpng-dev libtiff5-dev libdc1394-22-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev \
    libgtk-3-dev \
    libatlas-base-dev gfortran
RUN apt-get -y clean
RUN rm -rf /var/lib/apt/lists/*

# Build OpenCV
RUN mkdir -p /installs
WORKDIR /installs
RUN wget https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.zip -O opencv-${OPENCV_VERSION}.zip \
    && wget https://github.com/opencv/opencv_contrib/archive/${OPENCV_VERSION}.zip -O opencv_contrib-${OPENCV_VERSION}.zip
RUN unzip opencv-${OPENCV_VERSION}.zip \
    && unzip opencv_contrib-${OPENCV_VERSION}.zip
RUN mkdir /installs/opencv-${OPENCV_VERSION}/build
WORKDIR /installs/opencv-${OPENCV_VERSION}/build

RUN cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-${OPENCV_VERSION}/modules \
    -D INSTALL_C_EXAMPLES=ON \
    -D INSTALL_PYTHON_EXAMPLES=ON \
    -D INSTALL_TESTS=ON \
    -D WITH_PYTHON=ON \
    -D BUILD_opencv_python2=OFF \
    -D BUILD_opencv_python3=ON \
    -D PYTHON_DEFAULT_EXECUTABLE=python3 \
    -D WITH_MKL=ON \
    -D WITH_CUDA=ON \
    -D CUDA_FAST_MATH=ON \
    -D WITH_CUBLAS=ON \
    -D WITH_CUDNN=ON \
    -D WITH_NVCUVID=OFF \
    -D OPENCV_DNN_CUDA=OFF \
    -D BUILD_opencv_cudaimgproc=OFF \
    -D BUILD_EXAMPLES=ON \
    -D WITH_TBB=ON \
    -D WITH_V4L=ON \
    -D WITH_QT=ON \
    -D WITH_OPENGL=ON ..

RUN make -j${NPROC} && make install

# The others
RUN apt-get update -y
RUN apt-get install -y libboost-all-dev

WORKDIR /
