#ifndef __ASSERT_H
#define __ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__, NULL))
  #define assert_msg(expr, msg) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__, (uint8_t *)msg))
  void assert_failed(uint8_t *file, uint32_t line, uint8_t *msg);
#else
  #define assert_param(expr) ((void)0U)
  #define assert_msg(expr, msg) ((void)0U)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif