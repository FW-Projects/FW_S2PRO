#ifndef _OUTPUT_HANDLE_H
#define _OUTPUT_HANDLE_H
#include <stdbool.h>
#include "wk_system.h"
#define OUTPUT_HANDLE_TIME 200
typedef enum
{
    FAN_STOP = 0,
    FAN_WORKING,
} fan_state_t;

typedef enum
{
    HOT_STOP = 0,
    HOT_WORKING,
    HOT_WORKING_OVERLOAD
} hot_state_t;

typedef struct
{
    bool open;
    fan_state_t fan_state;
    hot_state_t hot_state;
    uint8_t channel;
} output_t;

extern float direct_handle_pid_out;

void pwm_control(void);
void sleep_control(void);
void output_handle(void);
uint8_t linear_correction(uint16_t user_set_temp);

#endif
