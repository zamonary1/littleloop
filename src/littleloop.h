// SPDX-License-Identifier: BSD-3-Clause
#ifndef LITTTLELOOP_LITTLELOOP_H
#define LITTTLELOOP_LITTLELOOP_H



#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if __has_include("loopconfig.h")
    #include "loopconfig.h"
#else 
    #pragma message("loopconfig.h has not been found! \nImporting provided loopconfig_default.h")
    #include "loopconfig_default.h"
#endif

#ifndef LOOP_ENABLE_AUTO_SCHEDULER
// если LOOP_ENABLE_AUTO_SCHEDULER объявлен в loopconfig.h
// то не перезаписываем конфигурацию. В ином случае пытаемся
// определить что было импортировано и исспользуем методы оттуда.

    #ifdef ARDUINO // авто конфигурация для arduino (wiring)
        #include <Arduino.h>

        #define LOOP_SYSTICK_FUNC millis()
        #define LOOP_SYSTICK_MS_DELIMETER 1
        #define LOOP_ENABLE_AUTO_SCHEDULER
        #define LOOP_AUTO_YIELD
    #endif
#endif

#ifdef LOOP_ENABLE_AUTO_SCHEDULER
    #if LOOP_AUTO_TICK_RATE > 1000
        #error "LOOP_AUTO_TICK_RATE cannot exceed 1000 (would cause LOOP_TICK_DELAY_MS = 0)"
    #endif
    #define LOOP_TICK_DELAY_MS (1000 / (LOOP_AUTO_TICK_RATE))
    #define LOOP_MILLIS_TIMER_GET (LOOP_SYSTICK_FUNC / LOOP_SYSTICK_MS_DELIMETER)

    #ifdef ARDUINO
        // оптимизация для arduino
        #define LOOP_MILLIS_TIMER_GET millis()
    #endif
    
#endif

int32_t lloop_add_task(void (*task_function_ptr)( void ), uint32_t call_interval_ticks);
void lloop_enable_task(uint32_t task_idx);
void lloop_disable_task(uint32_t task_idx);
void lloop_step_tick();
void lloop_start_auto_scheduler();

#ifdef __cplusplus
}
#endif

#endif // LITTTLELOOP_LITTLELOOP_H