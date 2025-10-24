#include "key.h"
#if 0
KEY_EVENT key_event_check(KEY *key, int interval)
{
    static KEY_EVENT key_event = KE_NONE;

    switch (key->status)
    {
    case KS_RELEASE:
        if (key->get() == K_PRESS)
        {
            key->count = 0;
			key->key_cycle_time = KEY_CYCLE_TIME;
            key->status = KS_CHECK;
        }
        else
        {
            key_event = KE_NONE;
            key->status = KS_RELEASE;
        }

        break;

    case KS_CHECK:
        if (key->get() == K_PRESS)
        {
            key->count ++;
        }

        //		 if(key->get() == K_RELEASE)
        //		{
        //		    key->status = KS_PRESS;
        //		}
        key->key_cycle_time --;

        if (!key->key_cycle_time)
        {
            if (key->count == 1)
            {
                key->status = KS_PRESS;
            }
            else if (key->count > 1  && key->count < interval)
            {
                //key->status = KS_DOUBLE_PRESS;
				key->status = KS_PRESS;
            }
            else if (key->count >= interval)
            {
                key->status = KS_LONG_PRESS;
            }
            else
            {
                key->status = KS_PRESS;
            }
            break;
        }

        break;

    case KS_DOUBLE_PRESS:
        key->status = KS_RELEASE;
        key_event = KE_DOUBLE_PRESS;
        break;
    case KS_LONG_PRESS:
        if (key->get() == K_PRESS)
        {
            key->status = KS_LONG_PRESS;
            key_event = KE_LONG_PRESS;
        }
        else
        {
            key->status = KS_RELEASE;
        }
        break;
    case KS_PRESS:
        key->status = KS_RELEASE;
        key_event = KE_PRESS;
        break;
    }

    return key_event;
}
#endif
#include "key.h"
#include <stddef.h>

// ȫ��ɨ������Ĭ��Ϊ20ms��
static uint16_t g_scan_interval_ms = 20;

/**
 * @brief ����ɨ������
 */
void key_set_scan_interval(uint16_t scan_interval_ms) {
    if (scan_interval_ms > 0) {
        g_scan_interval_ms = scan_interval_ms;
    }
}

/**
 * @brief ������ʱ��ת��Ϊɨ�����
 */
static uint16_t ms_to_scan_count(uint16_t ms) {
    if (ms == 0 || g_scan_interval_ms == 0) return 0;
    return (ms + g_scan_interval_ms - 1) / g_scan_interval_ms;
}

/**
 * @brief ��ȡ������ǰ��ƽ״̬
 */
static uint8_t read_key_level(key_obj *key) {
    uint8_t gpio_level = gpio_input_data_bit_read(key->port, key->pin) ? 1 : 0;
    return (gpio_level == key->active_level) ? 1 : 0;
}

/**
 * @brief ���ð�������ʱ�����
 */
void key_set_timing(key_obj *key, key_timing_config_t *timing) {
    if (key == NULL || timing == NULL) return;
    
    key->debounce_count = ms_to_scan_count(timing->debounce_ms);
    key->long_press_count = ms_to_scan_count(timing->long_press_ms);
    key->double_click_count = ms_to_scan_count(timing->double_click_interval_ms);
    key->long_press_continue_interval = ms_to_scan_count(timing->long_press_continue_interval_ms);
    
    // ȷ����Сֵ
    if (key->debounce_count == 0 && timing->debounce_ms > 0) {
        key->debounce_count = 1;
    }
}

/**
 * @brief �������ð���ʱ�����
 */
void key_set_timing_simple(key_obj *key, 
                          uint16_t debounce_ms, 
                          uint16_t long_press_ms, 
                          uint16_t double_click_interval_ms,
                          uint16_t long_press_continue_interval_ms) {
    key_timing_config_t timing = {
        .debounce_ms = debounce_ms,
        .long_press_ms = long_press_ms,
        .double_click_interval_ms = double_click_interval_ms,
        .long_press_continue_interval_ms = long_press_continue_interval_ms
    };
    
    key_set_timing(key, &timing);
}

key_event_t key_event_check(key_obj *key) {
    uint8_t current_level = read_key_level(key);
    key_event_t event = KEY_NONE;
    
    switch (key->state) {
        case STATE_RELEASED:
            if (current_level) {
                key->state = STATE_DEBOUNCE;
                key->press_count = 0;
                key->long_press_triggered = 0;
                key->long_press_continue_count = 0;
            }
            key->release_count = 0;
            break;
            
        case STATE_DEBOUNCE:
            if (current_level) {
                key->press_count++;
                if (key->press_count >= key->debounce_count) {
                    key->state = STATE_PRESSED;
                    event = KEY_PRESS;
                    key->click_count++;
                    key->press_count = 0;
                }
            } else {
                key->state = STATE_RELEASED;
            }
            break;
            
        case STATE_PRESSED:
            if (!current_level) {
                key->state = STATE_DEBOUNCE;
                key->release_count = 0;
                
                if (key->press_count < key->long_press_count) {
                    // �̰��ͷ�
                    if (key->click_count == 1) {
                        // ��һ�ε���������ȴ�˫��״̬
                        key->state = STATE_WAIT_DOUBLE;
                        // �������¼����ȴ����ܵĵڶ��ε��
                    } else if (key->click_count >= 2) {
                        // �ڶ��ε���ͷţ�����˫���¼�
                        event = KEY_DOUBLE_CLICK;
                        key->click_count = 0;
                        key->state = STATE_RELEASED; // ˫����ɺ�ֱ�ӻص��ͷ�״̬
                    }
                } else {
                    // �����ͷţ������̰��¼�����ѡ�����߿��Դ��������ͷ��¼���
                    event = KEY_SHORT_PRESS;
                    key->click_count = 0;
                }
                key->press_count = 0;
                key->long_press_triggered = 0;
            } else {
                key->press_count++;
                
                // ����Ƿ�ﵽ����ʱ��
                if (key->press_count >= key->long_press_count) {
                    if (!key->long_press_triggered) {
                        // ��һ�δﵽ������ֵ
                        event = KEY_LONG_PRESS;
                        key->long_press_triggered = 1;
                        key->state = STATE_LONG_PRESSED;
                        key->long_press_continue_count = 0;
                        key->click_count = 0; // ����ʱ���õ������������Ӱ��˫��
                    }
                }
            }
            break;
            
        case STATE_LONG_PRESSED:
            if (!current_level) {
                key->state = STATE_DEBOUNCE;
                key->long_press_triggered = 0;
                key->press_count = 0;
            } else {
                // ��������״̬
                key->long_press_continue_count++;
                
                // ����Ƿ�ﵽ�����������
                if (key->long_press_continue_interval > 0 && 
                    key->long_press_continue_count >= key->long_press_continue_interval) {
                    event = KEY_LONG_PRESS_CONTINUE;
                    key->long_press_continue_count = 0;
                }
            }
            break;
            
        case STATE_WAIT_DOUBLE:
            if (current_level) {
                // �ڶ��ΰ��¿�ʼ
                key->state = STATE_DEBOUNCE;
                key->press_count = 0;
                key->release_count = 0;
            } else {
                key->release_count++;
                if (key->release_count >= key->double_click_count) {
                    // ��ʱ��ȷ��Ϊ����
                    if (key->click_count == 1) {
                        event = KEY_SHORT_PRESS;
                    }
                    key->state = STATE_RELEASED;
                    key->click_count = 0;
                }
            }
            break;
    }
    
    return event;
}

uint8_t key_is_pressed(key_obj *key) {
    return (key->state == STATE_PRESSED || key->state == STATE_LONG_PRESSED);
}

uint8_t key_is_long_pressed(key_obj *key) {
    return (key->state == STATE_LONG_PRESSED);
}