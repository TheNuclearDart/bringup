#include "crc.h"

namespace
{
   bool initialized = false;
   CRC_Handler crc;
}

CRC_Handler::CRC_Handler()
{
   this->crc_instance = 
   {
      .Instance = CRC,
      .Init =
      {
         .DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE,
         .DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE,
         .CRCLength               = CRC_POLYLENGTH_32B,
         .InputDataInversionMode  = CRC_INPUTDATA_INVERSION_BYTE,
         .OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE,
      },
      .InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES
   };
}

void CRC_Handler::init(void)
{
   if (initialized)
   {
      return; // Assert, print, do something
   }
   
   HAL_CRC_Init(&this->crc_instance);
}

uint32_t CRC_Handler::calculate(uint32_t *buffer, uint32_t length)
{
   uint32_t crc = HAL_CRC_Calculate(&this->crc_instance, buffer, length);

   crc = ~crc; // Invert the CRC to match standard format

   return crc;
}

uint32_t crc_calculate(uint32_t *buffer, uint32_t length)
{
   return crc.calculate(buffer, length);
}

void crc_init(void)
{
   crc.init();
}