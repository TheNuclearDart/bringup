import sys
import os
import ctypes
import zlib

# This definition _MUST_ match that in fw_header.h
class FwHeader(ctypes.Structure):
   _fields_ = [('active'    , ctypes.c_bool),
               ('fw_version', ctypes.c_uint32),
               ('image_size', ctypes.c_uint32),
               ('crc32', ctypes.c_uint32),
               ('rsvd', ctypes.c_uint8 * 112)]

bin_path = "./blink.bin"
fw_header_size = ctypes.sizeof(FwHeader)

bin_size = os.path.getsize(bin_path)
print("Binary size: " + str(hex(bin_size)))
bin_size = bin_size - fw_header_size
print("Binary size without header: " + str(hex(bin_size)))

binary = open(bin_path, "rb+") # make this more generic


binary.seek(fw_header_size)
binary_byte_array = binary.read()
crc32 = zlib.crc32(binary_byte_array)
crc32 = crc32 & 0xffffffff
print("CRC32 calculated as " + str(hex(crc32)))

fw_header = FwHeader()

binary.readinto(fw_header)
fw_header.active = True # Don't do this here eventually
fw_header.fw_version = 0x5A5A5A5A
fw_header.image_size = bin_size
fw_header.crc32 = crc32
binary.seek(0)
binary.write(fw_header)
binary.close()