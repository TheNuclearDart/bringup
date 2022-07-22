import sys
import os

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

excluded_sections = ['.ARM.attributes', '.comment']

parser = argparse.ArgumentParser(description="Build segment headers and full image")
parser.add_argument("--elf-path", dest='elf_path', type=str, help='Path of .elf to create image from.')
parser.add_argument("--sections-path", dest='sections_path', type=str, help='Path to .txt file containing list of sections')
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
os.mkdir('binaries')
print("Parsing %d sections") % num_segments
for segment in section_dict:
   if segment['name'] not in excluded_sections and segment['flags'] != "ALLOC":
      print("Parsing segment: " + segment['name'])
      os.system('arm-none-eabi-objcopy -j %s -O binary %s ./binaries/%s_raw.bin' % (segment['name'], args.elf_path, segment['name']))
   
      with open('image.bin', "ab+") as image_binary, open('./binaries/%s_raw.bin' % segment['name']) as raw_segment:
         segment_header = SegmentHeader()
         segment_header.size = segment['size']
         segment_header.dest_addr = segment['lma']

         # We don't want to lead
         if segment['name'] != ".fw_header":
            image_binary.write(segment_header)

         # Now append the raw binary of the segment
         image_binary.write(raw_segment.read())
   else:
      print("Excluding segment: %s" % segment['name'])
