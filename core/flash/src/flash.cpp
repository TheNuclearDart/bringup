// License needed

#include "flash.h"

#include "stm32f7xx.h"

static uint32_t flash_get_sector_from_addr(uint8_t *addr)
{
   // There must be a cleaner way, or address defines we can use here
   if (reinterpret_cast<uint32_t>(addr) >= 0x8000000 && reinterpret_cast<uint32_t>(addr) < 0x8008000)
   {
      return FLASH_SECTOR_0;
   }
   else if (reinterpret_cast<uint32_t>(addr) >= 0x8008000 && reinterpret_cast<uint32_t>(addr) < 0x8010000)
   {
      return FLASH_SECTOR_1;
   }
   else if (reinterpret_cast<uint32_t>(addr) >= 0x8010000 && reinterpret_cast<uint32_t>(addr) < 0x8018000)
   {
      return FLASH_SECTOR_2;
   }
   else if (reinterpret_cast<uint32_t>(addr) >= 0x8018000 && reinterpret_cast<uint32_t>(addr) < 0x8020000)
   {
      return FLASH_SECTOR_3;
   }
   else if (reinterpret_cast<uint32_t>(addr) >= 0x8020000 && reinterpret_cast<uint32_t>(addr) < 0x8040000)
   {
      return FLASH_SECTOR_4;
   }
   else if (reinterpret_cast<uint32_t>(addr) >= 0x8040000 && reinterpret_cast<uint32_t>(addr) < 0x8080000)
   {
      return FLASH_SECTOR_5;
   }
   else if (reinterpret_cast<uint32_t>(addr) >= 0x8080000 && reinterpret_cast<uint32_t>(addr) < 0x80C0000)
   {
      return FLASH_SECTOR_6;
   }
   else if (reinterpret_cast<uint32_t>(addr) >= 0x80C0000 && reinterpret_cast<uint32_t>(addr) < 0x8100000)
   {
      return FLASH_SECTOR_7;
   }

   return UINT32_MAX;
}

flash_error_e flash_write(uint8_t *dest_ptr, uint8_t *src_ptr, uint32_t size)
{
   HAL_FLASH_Unlock();
   
   // Determine sector ranges to erase using input address and size.
   uint32_t start_sector = flash_get_sector_from_addr(dest_ptr);
   uint32_t end_sector   = flash_get_sector_from_addr(dest_ptr+size);

   FLASH_EraseInitTypeDef erase_init;
   uint32_t               sector_error;

   erase_init.NbSectors = (end_sector - start_sector) + 1;
   erase_init.Sector    = start_sector;
   erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
   erase_init.VoltageRange = FLASH_VOLTAGE_RANGE_3; // Is this correct?

   // Erase destination sectors of flash. This will wipe out everything in those sectors!
   for (uint32_t i = start_sector; i <= end_sector; i++)
   {
      if (HAL_FLASHEx_Erase(&erase_init, &sector_error) != HAL_OK)
      {
         return flash_error_e::GEN_FAILURE;
      }
   }

   // Write the data to flash now.
   for (uint32_t i = 0; i < size; i++)
   {
      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, (reinterpret_cast<uint32_t>(dest_ptr) + i), src_ptr[i]) != HAL_OK)
      {
         return flash_error_e::GEN_FAILURE;
      }
   }

   HAL_FLASH_Lock();

   return flash_error_e::SUCCESS;
}

/**
 * @brief Clear single byte of flash at addr to 0. Since "erased" state of flash is 0xFF,
 *        a byte can be cleared without a full sector erase.
 * 
 * @param byte_ptr Pointer to the byte to be cleared
 * @return flash_error_e Status of operation
 */
flash_error_e flash_clear_byte(uint8_t *byte_ptr)
{
   HAL_FLASH_Unlock();

   if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, reinterpret_cast<uint32_t>(byte_ptr), 0) != HAL_OK)
   {
      HAL_FLASH_Lock();
      return flash_error_e::GEN_FAILURE;
   }

   HAL_FLASH_Lock();

   return flash_error_e::SUCCESS;
}