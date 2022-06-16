#include "crc32.h"

namespace
{
   bool initialized = false;
   CRC32 crc32;
}

CRC32::CRC32()
{
   this->crc_instance = 
   {
      .Instance = CRC,
      .Init =
      {
         .DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE,
         .DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE,
         .InputDataInversionMode  = CRC_INPUTDATA_INVERSION_BYTE,
         .OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE,
      },
      .InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES
   };
}

void CRC32::init(void)
{
   if (initialized)
   {
      return; // Assert, print, do something
   }
   
   HAL_CRC_Init(&this->crc_instance);
}

uint32_t CRC32::calculate(uint32_t *buffer, uint32_t length)
{
   uint32_t crc = HAL_CRC_Calculate(&this->crc_instance, buffer, length);

   crc = ~crc; // Invert the CRC to match standard format

   return crc;
}

uint32_t crc32_calculate(uint32_t *buffer, uint32_t length)
{
   return crc32.calculate(buffer, length);
}

void crc32_init(void)
{
   crc32.init();
}