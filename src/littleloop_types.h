// SPDX-License-Identifier: BSD-3-Clause
#include <stdint.h>

typedef enum task_state{
    empty, // 0 это стандартное значение при инициальзации, так что считаем все задачи пустыми со старта
    disabled,
    enabled,
} task_state;

struct {
    void (*task_function_ptr)( void );
    uint32_t call_period; // частота вызова процедуры
    uint32_t call_remaining; // счетчик тиков с последнего вызова, идёт вниз
    task_state state;

} typedef task_instance_t;
