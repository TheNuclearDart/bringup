// License needed

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_cdc.h"

USB_Host::USB_Host(void)
{
   this->applicationStatus = AppState::IDLE;

   if (USBH_Init(&this->usbHostHandle, USB_Host::userFuncWrapper, 0) != USBH_OK)
   {
      // Error/assert or something.
   }
   if (USBH_RegisterClass(&this->usbHostHandle, USBH_CDC_CLASS) != USBH_OK)
   {
      // Error
   }
}

void USB_Host::start(void)
{
   if (USBH_Start(&this->usbHostHandle) != USBH_OK)
   {
      // Should return error here instead of asserting.
   }
}

void USB_Host::userFunc(USBH_HandleTypeDef *pHostHandle, uint8_t id)
{
   switch(id)
   {
      case HOST_USER_SELECT_CONFIGURATION:
      break;
      
      case HOST_USER_DISCONNECTION:
         this->applicationStatus = AppState::DISCONNECT;
         break;
      
      case HOST_USER_CLASS_ACTIVE:
         this->applicationStatus = AppState::READY;
      
      case HOST_USER_CONNECTION:
         this->applicationStatus = AppState::START;
      
      default:
         break;
   }
}

void USB_Host::process(void)
{
   USBH_Process(&this->usbHostHandle);
}

void USB_Host::cdc_transmit(uint8_t *data, uint32_t size)
{
   USBH_CDC_Transmit(&this->usbHostHandle, data, size);
}