BUILD_DIR=build
BUILD_PROGRAM=application

[ ! -d $BUILD_DIR ] && mkdir $BUILD_DIR
cd $BUILD_DIR && cmake .. && make && cd $BUILD_PROGRAM && "./$BUILD_PROGRAM" && cd ../..