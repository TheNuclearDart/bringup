echo $0
if [ "$1" == "clean" ]
then
   echo "Cleaning /build dir..."
   rm -rf build
else
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake ../ -Wno-dev
   make
fi