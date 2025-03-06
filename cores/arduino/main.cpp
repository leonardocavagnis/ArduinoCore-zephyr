/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"
#include <cstdint>
#ifdef CONFIG_LLEXT
#include <zephyr/llext/symbol.h>
#endif

#ifdef CONFIG_MULTITHREADING
void start_static_threads();
#endif

int main(void) {
#if (DT_NODE_HAS_PROP(DT_PATH(zephyr_user), cdc_acm) && CONFIG_USB_CDC_ACM)
  Serial.begin(115200);
#endif

#ifdef CONFIG_MULTITHREADING
  start_static_threads();
#endif

  setup();

  for (;;) {
    loop();
  #if 0 //(DT_NODE_HAS_PROP(DT_PATH(zephyr_user), cdc_acm) && CONFIG_USB_CDC_ACM) || DT_NODE_HAS_PROP(DT_PATH(zephyr_user), serials)
    if (arduino::serialEventRun) arduino::serialEventRun();
  #endif
  }

  return 0;
}

#ifdef CONFIG_LLEXT
LL_EXTENSION_SYMBOL(main);
#endif

/* These magic symbols are provided by the linker.  */
extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));

static void __libc_init_array (void)
{
  size_t count;
  size_t i;

  count = __preinit_array_end - __preinit_array_start;
  for (i = 0; i < count; i++)
    __preinit_array_start[i] ();

  count = __init_array_end - __init_array_start;
  for (i = 0; i < count; i++)
    __init_array_start[i] ();
}

extern "C" __attribute__((section(".entry_point"), used)) void entry_point(k_thread_stack_t* stack, size_t stack_size) {
  // copy .data in the right place
  // .bss should already be in the right place
  // call constructors
  extern uintptr_t _sidata;
  extern uintptr_t _sdata;
  extern uintptr_t _edata;
  extern uintptr_t _sbss;
  extern uintptr_t _ebss;
  //__asm volatile ("cpsie i");
  memcpy(&_sdata, &_sidata, &_edata - &_sdata);
  memset(&_sbss, 0, &_ebss - &_sbss);
  __libc_init_array();
  main();
}