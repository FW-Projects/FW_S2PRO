//#ifndef _KEY_H
//#define _KEY_H

//#include <stdio.h>
//#include "at32f415.h"

///* input */

//#define KEY_UP_PORT GPIOA
//#define KEY_UP_PIN GPIO_PINS_12
//#define READ_KEY_UP gpio_input_data_bit_read(KEY_UP_PORT, KEY_UP_PIN)

//#define KEY_DOWN_GPIO_PORT GPIOB
//#define KEY_DOWN_PIN GPIO_PINS_9
//#define READ_KEY_DOWN gpio_input_data_bit_read(KEY_DOWN_GPIO_PORT, KEY_DOWN_PIN)

//#define KEY_CH1_PORT GPIOB
//#define KEY_CH1_PIN GPIO_PINS_5
//#define READ_KEY_CH1 gpio_input_data_bit_read(KEY_CH1_PORT, KEY_CH1_PIN)

//#define KEY_CH2_PORT GPIOA
//#define KEY_CH2_PIN GPIO_PINS_15
//#define READ_KEY_CH2 gpio_input_data_bit_read(KEY_CH2_PORT, KEY_CH2_PIN)

//#define KEY_CH3_PORT GPIOF
//#define KEY_CH3_PIN GPIO_PINS_7
//#define READ_KEY_CH3 gpio_input_data_bit_read(KEY_CH3_PORT, KEY_CH3_PIN)

//#define KEY_SET_PORT GPIOF
//#define KEY_SET_PIN GPIO_PINS_6
//#define READ_KEY_SET gpio_input_data_bit_read(KEY_SET_PORT, KEY_SET_PIN)

//#define KEY_RPC_PORT GPIOB
//#define KEY_RPC_PIN GPIO_PINS_6
//#define READ_KEY_RPC gpio_input_data_bit_read(KEY_RPC_PORT, KEY_RPC_PIN)

//#define LONG_PRESS_TIME 40
//#define KEY_CYCLE_TIME 50
//#define KEY_NUMBER 6

//#ifdef __cplusplus
//extern "C"
//{
//#endif

//    typedef enum
//    {
//        K_RELEASE,
//        K_PRESS,
//    } KEY_VALUE;

//    typedef enum
//    {
//        KS_RELEASE,
//        // KS_LONG_RELEASE,
//        KS_CHECK,
//        KS_PRESS,
//        KS_LONG_PRESS,
//    } KEY_STATUS;

//    typedef enum
//    {
//        KE_PRESS,
//        KE_RELEASE,
//        KE_LONG_PRESS,
//        KE_LONG_RELEASE,
//        KE_NONE,
//    } KEY_EVENT;

//    typedef struct
//    {
//        KEY_STATUS status;
//        int count;
//        int key_cycle_time;
//        KEY_VALUE (*get)(void);
//    } KEY;

//    KEY_EVENT key_event_check(KEY *key, int interval);

//#ifdef __cplusplus
//}
//#endif
//#endif

#if 0
#ifndef _KEY_H
#define _KEY_H

#include <stdio.h>
#include "at32f403a_407.h"

/* input */

#define EC11_L_PORT             GPIOC
#define EC11_L_PIN              GPIO_PINS_10
#define READ_EC11_L             gpio_input_data_bit_read(EC11_L_PORT, EC11_L_PIN)

#define EC11_R_PORT             GPIOC
#define EC11_R_PIN              GPIO_PINS_13
#define READ_EC11_R             gpio_input_data_bit_read(EC11_R_PORT, EC11_R_PIN)

#define CH1_PORT                GPIOC
#define CH1_PIN                 GPIO_PINS_14
#define READ_CH1                gpio_input_data_bit_read(CH1_PORT, CH1_PIN)

#define CH2_PORT                GPIOC
#define CH2_PIN                 GPIO_PINS_12
#define READ_CH2                gpio_input_data_bit_read(CH2_PORT, CH2_PIN)

#define CH3_PORT                GPIOC
#define CH3_PIN                 GPIO_PINS_11
#define READ_CH3                gpio_input_data_bit_read(CH3_PORT, CH3_PIN)

#define ENT_PORT                GPIOD
#define ENT_PIN                 GPIO_PINS_2
#define READ_ENT                gpio_input_data_bit_read(ENT_PORT, ENT_PIN)


#define LONG_PRESS_TIME          8 
#define KEY_CYCLE_TIME           10
#define KEY_NUMBER               6
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    K_RELEASE,
    K_PRESS,
} KEY_VALUE;

typedef enum {
    KS_RELEASE,
	KS_CHECK,
    KS_PRESS,
	KS_DOUBLE_PRESS,
	KS_LONG_PRESS,
} KEY_STATUS;

typedef enum {
    KE_PRESS,
	KE_DOUBLE_PRESS,
    KE_RELEASE,
    KE_LONG_PRESS,
    KE_LONG_RELEASE,
    KE_NONE,
} KEY_EVENT;

typedef struct {
    KEY_STATUS status;
    uint16_t count;
	uint16_t key_cycle_time;
    KEY_VALUE (*get)(void);
} KEY;

KEY_EVENT key_event_check(KEY *key, int interval);

#ifdef __cplusplus
}
#endif
#endif
#endif
#ifndef __KEY_H
#define __KEY_H

#include <stdio.h>
#include "at32f415.h"

// �����¼�����
typedef enum {
    KEY_NONE = 0,           // ���¼�
    KEY_PRESS,              // ���£�˲ʱ�¼���
    KEY_SHORT_PRESS,        // �̰����ͷź󴥷���
    KEY_LONG_PRESS,         // �������ﵽ��ֵʱ������
    KEY_LONG_PRESS_CONTINUE,// ��������������������
    KEY_DOUBLE_CLICK        // ˫��
} key_event_t;

// ����״̬���ڲ�ʹ�ã�
typedef enum {
    STATE_RELEASED = 0,     // �ͷ�״̬
    STATE_PRESSED,          // ����״̬
    STATE_DEBOUNCE,         // ����״̬
    STATE_WAIT_DOUBLE,      // �ȴ�˫��״̬
    STATE_LONG_PRESSED      // ����״̬
} key_state_t;

// ʱ������ṹ��
typedef struct {
    uint16_t debounce_ms;           // ����ʱ�䣨���룩
    uint16_t long_press_ms;         // ������ֵʱ�䣨���룩
    uint16_t double_click_interval_ms; // ˫�����ʱ�䣨���룩
    uint16_t long_press_continue_interval_ms; // ��������������������룩
} key_timing_config_t;

// �����ṹ��
typedef struct {
    // Ӳ�����ã��û����ã�
    gpio_type *port;        // GPIO�˿�ָ�룬��GPIOC
    uint16_t pin;           // GPIO���ţ���GPIO_PINS_13
    uint8_t active_level;   // �����ƽ��0=�͵�ƽ��Ч��1=�ߵ�ƽ��Ч
    
    // ״̬���������ڲ�ʹ�ã�
    key_state_t state;
    uint16_t press_count;   // ���¼�������λ��ɨ�����ڣ�
    uint16_t release_count; // �ͷż���
    uint16_t long_press_continue_count; // ������������
    uint8_t click_count;    // �������
    uint8_t long_press_triggered; // �����Ѵ�����־
    
    // ʱ��������ڲ����㣬����ɨ�����ڣ�
    uint8_t debounce_count;          // ��������ɨ�����
    uint8_t long_press_count;        // ��������ɨ�����  
    uint8_t double_click_count;      // ˫���������ɨ�����
    uint8_t long_press_continue_interval; // ���������������
} key_obj;

/**
 * @brief ���尴������ĺ�
 * @param name ����������
 * @param port GPIO�˿ڣ���GPIOC
 * @param pin GPIO���ţ���GPIO_PINS_13  
 * @param active_lvl �����ƽ��0=�͵�ƽ��Ч��1=�ߵ�ƽ��Ч
 */
#define KEY_DEFINE(name, port, pin, active_lvl) \
    key_obj name = { \
        port, pin, active_lvl,        /* Ӳ������ */ \
        STATE_RELEASED, 0, 0, 0, 0, 0, /* ״̬���� */ \
        1, 50, 15, 10                 /* Ĭ��ʱ����� */ \
    }

/**
 * @brief ���ð���ɨ�����ڣ�����������ʱ�����ǰ���ã�
 * @param scan_interval_ms ɨ����ʱ�䣨���룩
 */
void key_set_scan_interval(uint16_t scan_interval_ms);

/**
 * @brief ���ð�������ʱ����������뵥λ��
 * @param key ��������ָ��
 * @param timing ʱ������ṹ��
 */
void key_set_timing(key_obj *key, key_timing_config_t *timing);

/**
 * @brief �������ð���ʱ��������򻯰棩
 * @param key ��������ָ��
 * @param debounce_ms ����ʱ�䣨���룩
 * @param long_press_ms ������ֵʱ�䣨���룩
 * @param double_click_interval_ms ˫�����ʱ�䣨���룩
 * @param long_press_continue_interval_ms ��������������������룬0��ʾ���ã�
 */
void key_set_timing_simple(key_obj *key, 
                          uint16_t debounce_ms, 
                          uint16_t long_press_ms, 
                          uint16_t double_click_interval_ms,
                          uint16_t long_press_continue_interval_ms);

/**
 * @brief �����¼���⺯�����������Ե��ã�
 * @param key ��������ָ��
 * @return ��⵽���¼�����
 */
key_event_t key_event_check(key_obj *key);

/**
 * @brief ��鰴���Ƿ��ڰ���״̬
 * @param key ��������
 * @return 1=���£�0=�ͷ�
 */
uint8_t key_is_pressed(key_obj *key);

/**
 * @brief ��鰴���Ƿ��ڳ���״̬
 * @param key ��������
 * @return 1=������0=�ǳ���
 */
uint8_t key_is_long_pressed(key_obj *key);

#endif