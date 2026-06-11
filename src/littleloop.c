// SPDX-License-Identifier: BSD-3-Clause
#include "littleloop.h"
#include "littleloop_types.h"

static task_instance_t task_queue[TASK_QUEUE_SIZE] = {0};
static uint64_t _ticks_stepped = 0;

/* Добавляет новую задачу, возвращает индекс этой задачи
*/
int32_t lloop_add_task(void (*task_function_ptr)( void ), uint32_t call_interval_ticks) {

    if (call_interval_ticks == 0) return -20;

    // сначала находим первый свободный слот в очереди
    int32_t available_task_slot = -1;
    for (uint32_t t = 0; t<TASK_QUEUE_SIZE; t++) {
        if (task_queue[t].state == empty) { // пустой слот
            available_task_slot = t;
            break;
        }
    }
    if (available_task_slot == -1) {
        return -10; // ошибка, слоты памяти заполнены
    }

    // занимаем его
    task_queue[available_task_slot].task_function_ptr = task_function_ptr;
    task_queue[available_task_slot].call_period = call_interval_ticks;
    task_queue[available_task_slot].call_remaining = call_interval_ticks;
    task_queue[available_task_slot].state = DEFAULT_TASK_STATE;

    return available_task_slot;

}


void lloop_enable_task(uint32_t task_idx) {
    if (task_idx >= TASK_QUEUE_SIZE) {
        return;
    }

    task_queue[task_idx].state = enabled;
}

void lloop_disable_task(uint32_t task_idx) {
    if (task_idx >= TASK_QUEUE_SIZE) {
        return;
    }

    task_queue[task_idx].state = disabled;
}

void lloop_step_tick() {
    _ticks_stepped++;
    for (uint32_t t = 0; t<TASK_QUEUE_SIZE; t++) {
        switch (task_queue[t].state) {
            case empty:
                break;
            case disabled:
                break;
            case enabled:
                if (task_queue[t].call_remaining > 0)
                    task_queue[t].call_remaining--;
                else { // == 0
                    // сбрасываем счетчик
                    task_queue[t].call_remaining = task_queue[t].call_period;
                    // вызываем метод
                    task_queue[t].task_function_ptr();
                }

        }
    }
}

uint64_t lloop_get_ticks() {
    return _ticks_stepped;
}

#ifdef LOOP_ENABLE_AUTO_SCHEDULER

void lloop_start_auto_scheduler() {
    uint32_t last_tick_time = LOOP_MILLIS_TIMER_GET;
    for (;;) {
        if (LOOP_MILLIS_TIMER_GET - last_tick_time >= LOOP_TICK_DELAY_MS) {
            lloop_step_tick();
            last_tick_time = LOOP_MILLIS_TIMER_GET;
        }
        #ifdef LOOP_AUTO_YIELD
            yield();
        #endif
    }
}

#else

#pragma message("LOOP_ENABLE_AUTO_SCHEDULER not defined!")

#endif // LOOP_ENABLE_AUTO_SCHEDULER
