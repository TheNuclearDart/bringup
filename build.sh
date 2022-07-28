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
   hash=$(git rev-parse --short HEAD)
   python3 ../build_image.py --hash 0x$hash --elf-path ./blink_stripped.elf --sections-path ./sections.txt
   # Concatenate the .bins into one larger binary.
   cat ./core/bootloader/bootloader.bin ./image.bin > full_image.bin
   echo "Full image built at ./build/full_image.bin"
fi