echo $0
if [ "$1" == "clean" ]
then
   echo "Cleaning /build dir..."
   rm -rf build
else
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake ../ -Wno-dev
   make -j 8
   # Concatenate the .bins into one larger binary.
   python ../write_header.py
   cat ./core/bootloader/bootloader.bin ./blink.bin > full_image.bin
   echo "Full image built at ./build/full_image.bin"
fi