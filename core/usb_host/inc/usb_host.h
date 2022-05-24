// License needed
#ifndef USB_HOST_H
#define USB_HOST_H

#include "usbh_core.h"
#include "usbh_cdc.h"

class USB_Host
{
   public:
      enum class Error
      {
         SUCCESS,
         FAILURE
      };
      enum class AppState
      {
         IDLE,
         START,
         READY,
         DISCONNECT
      };
      USB_Host(void); // Constructor
      void start(void); // Should return an error instead of void. Need a std error type...
      static void userFuncWrapper(USBH_HandleTypeDef *pHostHandle, uint8_t id); // Must be implemented by application, consisting of "usb_host->userFunc(params)."
   private:
      USBH_HandleTypeDef usbHostHandle;
      AppState applicationStatus;
      void userFunc(USBH_HandleTypeDef *pHostHandle, uint8_t id); // Prototype needs to match userfunc callback proto
};

#endif