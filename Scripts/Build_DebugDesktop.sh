BUILD_DIR=Distributions/Desktop/Debug
BUILD_PROGRAM=application

[ ! -d $BUILD_DIR ] && mkdir -p $BUILD_DIR
cd $BUILD_DIR && cmake ../../.. && make && cd $BUILD_PROGRAM && "./$BUILD_PROGRAM" && cd ../../../..
