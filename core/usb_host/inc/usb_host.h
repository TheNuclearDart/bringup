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
   private:
      USBH_HandleTypeDef usbHostHandle;
      AppState applicationStatus;
};

#endif