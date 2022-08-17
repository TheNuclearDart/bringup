#include "sdram_init.h"
#include "stm32f7xx.h"

#include "stm32f7xx_hal_sdram.h"

namespace
{
   SDRAM_HandleTypeDef sdram_handle = 
   {
      .Instance = FMC_SDRAM_DEVICE,
      .Init     = 
      {
         .SDBank             = FMC_SDRAM_BANK1,
         .ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8,
         .RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12,
         .MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16, // This probably isn't right, need to check
         .InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_2,
         .CASLatency         = FMC_SDRAM_CAS_LATENCY_2, // Might need to be 2
         .WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE, // ??
         .SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2, // Also wrong?? This is a pain in my ass, truly.
         .ReadBurst          = FMC_SDRAM_RBURST_ENABLE,  // I believe this chip supports this
         .ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0,  // Also likely wrong
      }
   };

   // Don't know that these are actually correct, they might need some adjustment
   FMC_SDRAM_TimingTypeDef sdram_timing =
   {
      .LoadToActiveDelay    = 2,
      .ExitSelfRefreshDelay = 7,
      .SelfRefreshTime      = 4,
      .RowCycleDelay        = 7,
      .WriteRecoveryTime    = 3,
      .RPDelay              = 2,
      .RCDDelay             = 2
   };

   FMC_SDRAM_CommandTypeDef command;

   #define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
   #define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
   #define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
   #define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
   #define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
   #define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
   #define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
   #define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
   #define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
   #define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
   #define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)
}

/* Taken from the ST Micro Demo for SDRAM. Need to properly include license for this. */
static void BSP_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
  volatile uint32_t tmpmrd = 0;
  /* Step 3:  Configure a clock configuration enable command */
  Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0xFFFF);

  /* Step 4: Insert 100 us minimum delay */
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);

  /* Step 5: Configure a PALL (precharge all) command */
  Command->CommandMode = FMC_SDRAM_CMD_PALL;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0xFFFF);

  /* Step 6 : Configure a Auto-Refresh command */
  Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 8;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0xFFFF);

  /* Step 7: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_2           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0xFFFF);

  /* Step 8: Set the refresh rate counter */
  /* (15.62 us x Freq) - 20 */
  /* Set the device refresh counter */
  hsdram->Instance->SDRTR |= ((uint32_t)((1292)<< 1));
}

RamError sdram_init(void)
{
   RamError error = RamError::SUCCESS;

   if (HAL_SDRAM_Init(&sdram_handle, &sdram_timing) != HAL_OK)
   {
      return RamError::FAILED;
   }

   BSP_SDRAM_Initialization_Sequence(&sdram_handle, &command);

   return error;
}

void sdram_write_word(uint32_t word, uint32_t *dest)
{
   HAL_SDRAM_Write_32b(&sdram_handle, dest, &word, 1);
}

uint32_t sdram_read_word(uint32_t *addr)
{
   uint32_t word;
   HAL_SDRAM_Read_32b(&sdram_handle, addr, &word, 1);

   return word;
}