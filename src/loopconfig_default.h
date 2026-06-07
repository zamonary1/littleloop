// SPDX-License-Identifier: BSD-3-Clause

// Общие настройки
#define LOOP_AUTO_TICK_RATE 1000 // 1000 тиков в секунду
#define TASK_QUEUE_SIZE 50 // максимальное количество задач
#define DEFAULT_TASK_STATE enabled

// Настройки планировщика
// Измените при портировании на новое железо!

// #define LOOP_SYSTICK_FUNC SysTick->CNT
// #define LOOP_SYSTICK_MS_DELIMETER DELAY_MS_TIME
// #define LOOP_ENABLE_AUTO_SCHEDULER