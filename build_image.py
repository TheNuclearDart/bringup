import sys
import os
import shutil

import argparse
import ctypes
import zlib

# The purpose of this script is to take the collection of binary segments created
# from the original .elf, and append a header to them. Once they are creaated, they
# should be concatenated together.

# This definition _MUST_ match the segment_header_t in fw_header.h
class SegmentHeader(ctypes.Structure):
   _fields_ = [('dest_addr', ctypes.c_uint32),
               ('size'     , ctypes.c_uint32),
               ('rsvd'     , ctypes.c_uint8 * 8)]

class FwHeader(ctypes.Structure):
   _fields_ = [('active'    , ctypes.c_bool),
               ('fw_version', ctypes.c_uint32),
               ('image_size', ctypes.c_uint32),
               ('entry_addr', ctypes.c_uint32),
               ('crc32', ctypes.c_uint32)]

def auto_int(x):
   return int(x,0)

excluded_sections = ['.ARM.attributes', '.comment']
app_image_path    = 'image.bin'

parser = argparse.ArgumentParser(description="Build segment headers and full image")
parser.add_argument("--elf-path", dest='elf_path', type=str, help='Path of .elf to create image from.')
parser.add_argument("--sections-path", dest='sections_path', type=str, help='Path to .txt file containing list of sections')
parser.add_argument('--hash', dest='hash', type=auto_int, help='Git Hash to be written to header.')
args = parser.parse_args()

print("Using file " + args.elf_path)

segment_header_size  = ctypes.sizeof(SegmentHeader)

with open(args.sections_path) as sections:
   num_lines = (len(sections.readlines()) - 5)

with open(args.sections_path) as sections:
   section_dict = [{}] * (num_lines / 2)
   print('Number of sections is ' + str(num_lines))
   for x in range(5):
      next(sections)
   index = 0
   for count, line in enumerate(sections, start=0):
      if count % 2 == 0:
         if count != 0:
            index -= 1
         (idx, name, size, va, lma, off, algn) = line.split()
         section_dict[index] = {'idx':idx, 'name':name, 'size':int(size, 16), 'va':int(va, 16), 'lma':int(lma, 16), 'off':off}
      else:
         section_dict[index - 1]['flags'] = line.strip()
      index += 1

# Now that we have a list of sections and their sizes, we can create segment binaries
num_segments = len(section_dict)
try:
   # Clean up old images before rebuilding
   shutil.rmtree('binaries')
   os.remove(app_image_path)
except OSError:
   pass
os.mkdir('binaries')
print("Parsing %d sections") % num_segments

fw_header = FwHeader()
with open(app_image_path, "ab+") as image_binary:
   # write an empty header to the start of the binary
   image_binary.write(fw_header)
   
   entry_addr = 0
   for segment in section_dict:
      if segment['name'] not in excluded_sections and segment['flags'] != "ALLOC":
         print("Parsing segment: " + segment['name'])
         os.system('arm-none-eabi-objcopy -j %s -O binary %s ./binaries/%s_raw.bin' % (segment['name'], args.elf_path, segment['name']))
   
         with open('./binaries/%s_raw.bin' % segment['name']) as raw_segment:
            segment_header = SegmentHeader()
            segment_header.size = segment['size']
            segment_header.dest_addr = segment['lma']

            # We don't want to lead the fw_header with a segment header
            # Is this guard needed anymore?
            if segment['name'] != ".fw_header":
               image_binary.write(segment_header)

            # Now append the raw binary of the segment
            image_binary.write(raw_segment.read())

            if segment['name'] == ".isr_vector":
               entry_addr = segment['lma']
      else:
         print("Excluding segment: %s" % segment['name'])

print("Git Hash is: " + hex(args.hash))

fw_header_size = ctypes.sizeof(FwHeader)

bin_size = os.path.getsize(app_image_path)
print("Binary size: " + str(hex(bin_size)))

binary = open(app_image_path, "rb+") # make this more generic

binary.seek(fw_header_size)
binary_byte_array = binary.read()
crc32 = zlib.crc32(binary_byte_array)
crc32 = crc32 & 0xffffffff
print("CRC32 calculated as " + str(hex(crc32)))
print("Entry address is " + str(hex(entry_addr)))

#fw_header = FwHeader()

# Now populate the empty header at the start of the image
binary.readinto(fw_header)
fw_header.active = True # Default to true, fw update process will clear the active flag of an old image in flash.
fw_header.fw_version = args.hash
fw_header.image_size = bin_size
fw_header.entry_addr = entry_addr
fw_header.crc32 = crc32
binary.seek(0)
binary.write(fw_header)
binary.close()
