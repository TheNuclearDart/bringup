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
   this->crc_mode = mode::BIT32; // Default to 32B
}

void CRC_Handler::init(void)
{
   if (initialized)
   {
      return; // Assert, print, do something
   }
   
   HAL_CRC_Init(&this->crc_instance);
}

uint32_t CRC_Handler::calculate(uint32_t *buffer, uint32_t length, mode crc_mode)
{
   if (crc_mode != this->crc_mode)
   {
      this->change_mode(crc_mode);
   }

   uint32_t crc = HAL_CRC_Calculate(&this->crc_instance, buffer, length);

   crc = ~crc; // Invert the CRC to match standard format (I don't know if this is going to be the right thing in all calculations... works for 32 bit)

   return crc;
}

uint32_t crc_calculate(uint32_t *buffer, uint32_t length, CRC_Handler::mode crc_mode)
{
   return crc.calculate(buffer, length, crc_mode);
}

uint32_t crc32_calculate(uint32_t *buffer, uint32_t length)
{
   return crc.calculate(buffer, length, CRC_Handler::mode::BIT32);
}

void crc_init(void)
{
   crc.init();
}

void CRC_Handler::change_mode(mode mode)
{
   HAL_CRC_DeInit(&this->crc_instance);

   switch (mode)
   {
      case mode::BIT16:
         this->crc_instance.Init.CRCLength            = CRC_POLYLENGTH_16B;
         this->crc_instance.Init.DefaultInitValueUse  = DEFAULT_POLYNOMIAL_DISABLE;
         this->crc_instance.Init.GeneratingPolynomial = 0x1021; // Is this correct?
         this->crc_instance.Init.DefaultInitValueUse  = DEFAULT_INIT_VALUE_DISABLE;
         this->crc_instance.Init.InitValue            = 0x0000;
         break;
      case mode::BIT32:
      // Fallthrough, 32 is default for now
      default:
         this->crc_instance.Init.CRCLength            = CRC_POLYLENGTH_32B;
         this->crc_instance.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
         this->crc_instance.Init.DefaultInitValueUse  = DEFAULT_INIT_VALUE_ENABLE;
         break;
   }

   HAL_CRC_Init(&this->crc_instance);
}