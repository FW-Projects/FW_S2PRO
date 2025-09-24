#ifndef _KEY_H
#define _KEY_H

#include <stdio.h>
#include "at32f415.h"

/* input */

#define KEY_UP_PORT GPIOB
#define KEY_UP_PIN GPIO_PINS_9
#define READ_KEY_UP gpio_input_data_bit_read(KEY_UP_PORT, KEY_UP_PIN)

#define KEY_DOWN_GPIO_PORT GPIOA
#define KEY_DOWN_PIN GPIO_PINS_12
#define READ_KEY_DOWN gpio_input_data_bit_read(KEY_DOWN_GPIO_PORT, KEY_DOWN_PIN)

#define KEY_CH1_PORT GPIOB
#define KEY_CH1_PIN GPIO_PINS_5
#define READ_KEY_CH1 gpio_input_data_bit_read(KEY_CH1_PORT, KEY_CH1_PIN)

#define KEY_CH2_PORT GPIOA
#define KEY_CH2_PIN GPIO_PINS_15
#define READ_KEY_CH2 gpio_input_data_bit_read(KEY_CH2_PORT, KEY_CH2_PIN)

#define KEY_CH3_PORT GPIOF
#define KEY_CH3_PIN GPIO_PINS_7
#define READ_KEY_CH3 gpio_input_data_bit_read(KEY_CH3_PORT, KEY_CH3_PIN)

#define KEY_SET_PORT GPIOF
#define KEY_SET_PIN GPIO_PINS_6
#define READ_KEY_SET gpio_input_data_bit_read(KEY_SET_PORT, KEY_SET_PIN)

// #define KEY_TOUCH_PORT           GPIOB
// #define KEY_TOUCH_PIN            GPIO_PINS_2
// #define READ_KEY_TOUCH           gpio_input_data_bit_read(KEY_TOUCH_PORT, KEY_TOUCH_PIN)

#define LONG_PRESS_TIME 40
#define KEY_CYCLE_TIME 50
#define KEY_NUMBER 6

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        K_RELEASE,
        K_PRESS,
    } KEY_VALUE;

    typedef enum
    {
        KS_RELEASE,
        // KS_LONG_RELEASE,
        KS_CHECK,
        KS_PRESS,
        KS_LONG_PRESS,
    } KEY_STATUS;

    typedef enum
    {
        KE_PRESS,
        KE_RELEASE,
        KE_LONG_PRESS,
        KE_LONG_RELEASE,
        KE_NONE,
    } KEY_EVENT;

    typedef struct
    {
        KEY_STATUS status;
        int count;
        int key_cycle_time;
        KEY_VALUE (*get)(void);
    } KEY;

    KEY_EVENT key_event_check(KEY *key, int interval);

#ifdef __cplusplus
}
#endif
#endif
