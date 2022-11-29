BUILD_DIR=Distribution
BUILD_PROGRAM=Application

[ ! -d $BUILD_DIR ] && mkdir -p $BUILD_DIR
cd $BUILD_DIR && cmake .. && make && cd "./Projects/$BUILD_PROGRAM" && "./$BUILD_PROGRAM" && cd ../..
