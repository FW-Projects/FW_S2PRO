#include "key_handle.h"
#include "beep_handle.h"
#include "FWS2pro_handle.h"
#include "cmsis_armclang.h"
#include "PID_operation.h"

KEY_DEFINE(key_up, GPIOA, GPIO_PINS_12, 0);
KEY_DEFINE(key_down, GPIOB, GPIO_PINS_9, 0);
//KEY_DEFINE(key_up, GPIOB, GPIO_PINS_9, 0);
//KEY_DEFINE(key_down, GPIOA, GPIO_PINS_12, 0);
KEY_DEFINE(key_ch1, GPIOB, GPIO_PINS_5, 0);
KEY_DEFINE(key_ch2, GPIOA, GPIO_PINS_15, 0);
KEY_DEFINE(key_ch3, GPIOF, GPIO_PINS_7, 0);
KEY_DEFINE(key_set, GPIOF, GPIO_PINS_6, 0);
KEY_DEFINE(key_rpc, GPIOB, GPIO_PINS_6, 0);

#define KEY_COUNT 7
static handle_event EVENT = END_EVENT;
key_obj *keys[KEY_COUNT] = {&key_up, &key_down, &key_ch1, &key_ch2, &key_ch3, &key_set, &key_rpc};
key_event_t key_event[KEY_COUNT];

void handle_key_events(void)
{
	static bool first_in = false;

	if (first_in == false)
	{
		first_in = true;
		key_set_scan_interval(KEY_TASK_TIME);
		key_set_timing_simple(&key_up, KEY_TASK_TIME, 1500, 0, 20);
		key_set_timing_simple(&key_down, 0, 1500, 0, 20);
		key_set_timing_simple(&key_ch1, KEY_TASK_TIME, 1500, 0, 0);
		key_set_timing_simple(&key_ch2, KEY_TASK_TIME, 1500, 0, 0);
		key_set_timing_simple(&key_ch3, KEY_TASK_TIME, 1500, 0, 0);
		key_set_timing_simple(&key_set, KEY_TASK_TIME, 1500, 300, 0);
		key_set_timing_simple(&key_rpc, KEY_TASK_TIME, 1500, 0, 0);
	}
	for (int i = 0; i < KEY_COUNT; i++)
	{
		key_event[i] = key_event_check(keys[i]);
	}
	
	if (key_event[0] != KEY_NONE)
	{
		switch (key_event[0])
		{
		case KEY_SHORT_PRESS:
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				// 增加温度
				EVENT = TEMP_ADD;
			}
			else if (sFWS2_t.page >= SELECT_SET_WORK_PAGE_CN && sFWS2_t.page <= SELECT_EXIT_MENU_PAGE_CN)
			{
				// 切换菜单
//				EVENT = PAGE_ADD;
				EVENT = PAGE_REDUCE;
			}
			else if (sFWS2_t.page == SET_WORK_PAGE_CN)
			{
				// 设置工作模式
				EVENT = SET_WORK_MODE;
			}
			else if (sFWS2_t.page == SET_UNIT_PAGE_CN)
			{
				// 设置温度单位
				EVENT = SET_TEMP_UNIT;
			}
			else if (sFWS2_t.page == SET_TEMP_CAL_PAGE_CN)
			{
				// 设置温度校准
				if (sFWS2_t.set_cal == SET_CAL)
				{
					EVENT = SET_CAL_TEMP_ADD;
				}
				else if (sFWS2_t.set_cal == SET_RETURN_CAL)
				{
					sFWS2_t.page = SET_TEMP_CAL_PAGE_CN;
					sFWS2_t.set_cal = SET_CAL;
					sFWS2_t.base.last_cal_temp_c_display = RESET_VALUE;
					sFWS2_t.base.last_cal_temp_f_display = RESET_VALUE;
				}
			}
			else if (sFWS2_t.page == SET_SPEAK_PAGE_CN)
			{
				// 设置声音
				EVENT = SET_SPEAK_MODE;
			}
			else if (sFWS2_t.page == SET_TEMP_LOCK_PAGE_CN)
			{
				// 设置温度锁
				EVENT = SET_TEMP_LOCK;
			}
			else if (sFWS2_t.page == SET_SLEEP_PAGE_CN)
			{
				// 设置睡眠时间
				if (sFWS2_t.set_sleep == SET_SLEEP)
				{
					EVENT = SET_SLEEP_TIME_ADD;
				}
				else if (sFWS2_t.set_sleep == SET_RETURN_SLEEP)
				{
					sFWS2_t.page = SET_SLEEP_PAGE_CN;
					sFWS2_t.set_sleep = SET_SLEEP;
					sFWS2_t.base.last_set_sleep_time = RESET_VALUE;
				}
			}
			else if (sFWS2_t.page == SET_LANGUAGE_PAGE_CN || sFWS2_t.page == SET_LANGUAGE_PAGE_ENG)
			{
				EVENT = SET_LANGUAGE_MODE;
			}
			else if (sFWS2_t.page == SET_RESET_PAGE_CN)
			{
				// 设置恢复出厂设置
				if (sFWS2_t.set_reset1 == RESET_RETURN)
				{
					sFWS2_t.set_reset1 = RESET_CONFIRN;
				}
				else if (sFWS2_t.set_reset1 == RESET_CONFIRN)
				{
					sFWS2_t.set_reset1 = RESET_RETURN;
				}
			}
			else if (sFWS2_t.page == SET_RESET_DONE_PAGE_CN)
			{
				// 设置恢复出厂设置确认
				if (sFWS2_t.set_reset2 == RESET2_RETURN)
				{
					sFWS2_t.set_reset2 = RESET2_CONFIRN;
				}
				else if (sFWS2_t.set_reset2 == RESET2_CONFIRN)
				{
					sFWS2_t.set_reset2 = RESET2_RETURN;
				}
			}
			else if (sFWS2_t.page == SET_VERSION_PAGE_CN)
			{
				// 设置版本信息
				if (sFWS2_t.set_vision == VISION_RETURN)
				{
					sFWS2_t.set_vision = VISION_START;
				}
				else if (sFWS2_t.set_vision == VISION_START)
				{
					sFWS2_t.set_vision = VISION_RETURN;
				}
			}
			sbeep.status = BEEP_SHORT;
			break;
		}
		case KEY_LONG_PRESS_CONTINUE:
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				// 减少温度
				EVENT = TEMP_ADD_FIVE;
			}
			else if (sFWS2_t.page == SET_TEMP_CAL_PAGE_CN)
			{
				// 设置温度校准
				if (sFWS2_t.set_cal == SET_CAL)
				{
					EVENT = SET_CAL_TEMP_ADD_FIVE;
				}
			}
			else if (sFWS2_t.page == SET_SLEEP_PAGE_CN)
			{
				// 设置睡眠时间
				if (sFWS2_t.set_sleep == SET_SLEEP)
				{
					EVENT = SET_SLEEP_TIME_ADD_FIVE;
				}
			}
			break;
		}
		default:
			break;
		}
	}
	if (key_event[1] != KEY_NONE)
	{
		switch (key_event[1])
		{
			case KEY_SHORT_PRESS:
			{
				if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
				{
					// 减少温度
					EVENT = TEMP_REDUCE;
				}
				else if (sFWS2_t.page >= SELECT_SET_WORK_PAGE_CN && sFWS2_t.page <= SELECT_EXIT_MENU_PAGE_CN)
				{
					// 切换菜单
//					EVENT = PAGE_REDUCE;
					EVENT = PAGE_ADD;
				}
				else if (sFWS2_t.page == SET_WORK_PAGE_CN)
				{
					// 设置工作模式
					EVENT = SET_WORK_MODE;
				}
				else if (sFWS2_t.page == SET_UNIT_PAGE_CN)
				{
					// 设置温度单位
					EVENT = SET_TEMP_UNIT;
				}
				else if (sFWS2_t.page == SET_TEMP_CAL_PAGE_CN)
				{
					// 设置温度校准
					if (sFWS2_t.set_cal == SET_CAL)
					{
						EVENT = SET_CAL_TEMP_REDUCE;
					}
					else if (sFWS2_t.set_cal == SET_RETURN_CAL)
					{
						sFWS2_t.page = SET_TEMP_CAL_PAGE_CN;
						sFWS2_t.set_cal = SET_CAL;
						sFWS2_t.base.last_cal_temp_c_display = RESET_VALUE;
						sFWS2_t.base.last_cal_temp_f_display = RESET_VALUE;
					}
				}
				else if (sFWS2_t.page == SET_SPEAK_PAGE_CN)
				{
					// 设置声音
					EVENT = SET_SPEAK_MODE;
				}
				else if (sFWS2_t.page == SET_TEMP_LOCK_PAGE_CN)
				{
					// 设置温度锁
					EVENT = SET_TEMP_LOCK;
				}
				else if (sFWS2_t.page == SET_SLEEP_PAGE_CN)
				{
					// 设置睡眠时间
					if (sFWS2_t.set_sleep == SET_SLEEP)
					{
						EVENT = SET_SLEEP_TIME_REDUCE;
					}
					else if (sFWS2_t.set_sleep == SET_RETURN_SLEEP)
					{
						sFWS2_t.page = SET_SLEEP_PAGE_CN;
						sFWS2_t.set_sleep = SET_SLEEP;
						sFWS2_t.base.last_set_sleep_time = RESET_VALUE;
					}
				}
				else if (sFWS2_t.page == SET_LANGUAGE_PAGE_CN || sFWS2_t.page == SET_LANGUAGE_PAGE_ENG)
				{
					EVENT = SET_LANGUAGE_MODE;
				}
				else if (sFWS2_t.page == SET_RESET_PAGE_CN)
				{
					// 设置恢复出厂设置
					if (sFWS2_t.set_reset1 == RESET_RETURN)
					{
						sFWS2_t.set_reset1 = RESET_CONFIRN;
					}
					else if (sFWS2_t.set_reset1 == RESET_CONFIRN)
					{
						sFWS2_t.set_reset1 = RESET_RETURN;
					}
				}
				else if (sFWS2_t.page == SET_RESET_DONE_PAGE_CN)
				{
					// 设置恢复出厂设置确认
					if (sFWS2_t.set_reset2 == RESET2_RETURN)
					{
						sFWS2_t.set_reset2 = RESET2_CONFIRN;
					}
					else if (sFWS2_t.set_reset2 == RESET2_CONFIRN)
					{
						sFWS2_t.set_reset2 = RESET2_RETURN;
					}
				}
				else if (sFWS2_t.page == SET_VERSION_PAGE_CN)
				{
					// 设置版本信息
					if (sFWS2_t.set_vision == VISION_RETURN)
					{
						sFWS2_t.set_vision = VISION_START;
					}
					else if (sFWS2_t.set_vision == VISION_START)
					{
						sFWS2_t.set_vision = VISION_RETURN;
					}
				}
				sbeep.status = BEEP_SHORT;
				break;
			}
			case KEY_LONG_PRESS_CONTINUE:
			{
				if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
				{
					// 减少温度
					EVENT = TEMP_REDUCE_FIVE;
				}
				else if (sFWS2_t.page == SET_TEMP_CAL_PAGE_CN)
				{
					// 设置温度校准
					if (sFWS2_t.set_cal == SET_CAL)
					{
						EVENT = SET_CAL_TEMP_REDUCE_FIVE;
					}
				}
				else if (sFWS2_t.page == SET_SLEEP_PAGE_CN)
				{
					// 设置睡眠时间
					if (sFWS2_t.set_sleep == SET_SLEEP)
					{
						EVENT = SET_SLEEP_TIME_REDUCE_FIVE;
					}
				}
				break;
			}
//			case KEY_NONE:break;
//			case KEY_PRESS:break;
//			case KEY_LONG_PRESS:break;
//			case KEY_DOUBLE_CLICK:break;
		}
	}
	if (key_event[2] != KEY_NONE)
	{
		switch (key_event[2])
		{
		case KEY_SHORT_PRESS:
			EVENT = GET_CHANNEL_1;
			sbeep.status = BEEP_SHORT;
			break;
		case KEY_LONG_PRESS:
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				EVENT = SET_CHANNEL_1;
			}
			sbeep.status = BEEP_LONG;
			break;
		default:
			break;
		}
	}
	if (key_event[3] != KEY_NONE)
	{
		switch (key_event[3])
		{
		case KEY_SHORT_PRESS:
			EVENT = GET_CHANNEL_2;
			sbeep.status = BEEP_SHORT;
			break;
		case KEY_LONG_PRESS:
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				EVENT = SET_CHANNEL_2;
			}
			sbeep.status = BEEP_LONG;
			break;
		default:
			break;
		}
	}
	if (key_event[4] != KEY_NONE)
	{
		switch (key_event[4])
		{
		case KEY_SHORT_PRESS:
			EVENT = GET_CHANNEL_3;
			sbeep.status = BEEP_SHORT;
			break;
		case KEY_LONG_PRESS:
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				EVENT = SET_CHANNEL_3;
			}
			sbeep.status = BEEP_LONG;
			break;
		default:
			break;
		}
	}
	if (key_event[5] != KEY_NONE)
	{
		switch (key_event[5])
		{
		case KEY_SHORT_PRESS:
			if (sFWS2_t.page != WORK_PAGE_210 &&
				sFWS2_t.page != WORK_PAGE_245 &&
				sFWS2_t.page != CURVE_PAGE)
			{
				switch (sFWS2_t.page)
				{
				case SELECT_SET_WORK_PAGE_CN:
					sFWS2_t.page = SET_WORK_PAGE_CN;
					break;
				case SELECT_SET_UNIT_PAGE_CN:
					sFWS2_t.page = SET_UNIT_PAGE_CN;
					break;
				case SELECT_SET_TEMP_CAL_PAGE_CN:
					sFWS2_t.page = SET_TEMP_CAL_PAGE_CN;
					sFWS2_t.set_cal = SET_CAL;
					break;
				case SELECT_SET_SPEAK_PAGE_CN:
					sFWS2_t.page = SET_SPEAK_PAGE_CN;
					break;
				case SELECT_SET_TEMP_LOCK_PAGE_CN:
					sFWS2_t.page = SET_TEMP_LOCK_PAGE_CN;
					break;
				case SELECT_SET_SLEEP_PAGE_CN:
					sFWS2_t.page = SET_SLEEP_PAGE_CN;
					sFWS2_t.set_sleep = SET_SLEEP;
					break;
				case SELECT_SET_LANGUAGE_PAGE_CN:
					if(sFWS2_t.general_parameter.language_state == CHINESE )
					{
						sFWS2_t.page = SET_LANGUAGE_PAGE_CN;
						sFWS2_t.set_language = CHINESE;
					}
					else if(sFWS2_t.general_parameter.language_state == ENGLISH )
					{
						sFWS2_t.page = SET_LANGUAGE_PAGE_ENG;
						sFWS2_t.set_language = ENGLISH;
					}
					break;
				case SELECT_SET_RESET_PAGE_CN:
					sFWS2_t.page = SET_RESET_PAGE_CN;
					sFWS2_t.set_reset1 = RESET_RETURN;
					break;
				case SELECT_SET_VERSION_PAGE_CN:
					sFWS2_t.page = SET_VERSION_PAGE_CN;
					sFWS2_t.set_vision = VISION_RETURN;
					break;
//				case SELECT_SET_OTA_PAGE_CN:
//					sFWS2_t.page = SET_OTA_PAGE_CN;
//					break;
				case SELECT_SET_SUPPORT_PAGE_CN:
					sFWS2_t.page = SET_SUPPORT_PAGE_CN;
					break;
				case SELECT_EXIT_MENU_PAGE_CN:
					if (sFWS2_t.work_mode == WORK_NORMAL)
					{
						if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
						{
							sFWS2_t.page = WORK_PAGE_210;
						}
						else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
						{
							sFWS2_t.page = WORK_PAGE_245;
						}
						else
						{
							sFWS2_t.page = WORK_PAGE_210;
						}
					}
					else if (sFWS2_t.work_mode == WORK_CURVE)
					{
						sFWS2_t.page = CURVE_PAGE;
					}
					break;
				case SET_WORK_PAGE_CN:
					sFWS2_t.page = SELECT_SET_WORK_PAGE_CN;
					sFWS2_t.set_work_mode = sFWS2_t.work_mode;
					if (sFWS2_t.set_work_mode == WORK_NORMAL)
					{
						sFWS2_t.work_mode = WORK_NORMAL;
					}
					else if (sFWS2_t.set_work_mode == WORK_CURVE)
					{
						sFWS2_t.work_mode = WORK_CURVE;
					}
					break;
				case SET_UNIT_PAGE_CN:
					sFWS2_t.page = SELECT_SET_UNIT_PAGE_CN;
					sFWS2_t.set_temp_unit = sFWS2_t.general_parameter.temp_unit;
					if (sFWS2_t.set_temp_unit == CELSIUS)
					{
						sFWS2_t.general_parameter.temp_unit = CELSIUS;
						sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
					}
					else if (sFWS2_t.set_temp_unit == FAHRENHEIT)
					{
						sFWS2_t.general_parameter.temp_unit = FAHRENHEIT;
						sFWS2_t.base.set_temp = (sFWS2_t.base.set_temp_f_display - 32) * 5 / 9;
					}
					break;
				case SET_TEMP_CAL_PAGE_CN:
					if (sFWS2_t.set_cal == SET_RETURN_CAL)
					{
						sFWS2_t.base.cal_data += sFWS2_t.base.set_temp - sFWS2_t.base.cal_temp_c_display;

						if (sFWS2_t.base.cal_data >= MAX_CAL_TEMP)
						{
							sFWS2_t.base.cal_data = MAX_CAL_TEMP;
						}

						if (sFWS2_t.base.cal_data <= MIN_CAL_TEMP)
						{
							sFWS2_t.base.cal_data = MIN_CAL_TEMP;
						}
						sFWS2_t.page = SELECT_SET_TEMP_CAL_PAGE_CN;
						sFWS2_t.base.last_cal_temp_c_display = RESET_VALUE;
						sFWS2_t.base.last_cal_temp_f_display = RESET_VALUE;
					}
					else if (sFWS2_t.set_cal == SET_CAL)
					{
						sFWS2_t.set_cal = SET_RETURN_CAL;
						sFWS2_t.base.last_cal_temp_c_display = RESET_VALUE;
						sFWS2_t.base.last_cal_temp_f_display = RESET_VALUE;
					}
					break;
				case SET_SPEAK_PAGE_CN:
					sFWS2_t.page = SELECT_SET_SPEAK_PAGE_CN;
					sFWS2_t.set_speak = sFWS2_t.general_parameter.speak_state;
					if (sFWS2_t.set_speak == SPEAKER_OPEN)
					{
						sFWS2_t.general_parameter.speak_state = SPEAKER_OPEN;
					}
					else if (sFWS2_t.set_speak == SPEAKER_CLOSE)
					{
						sFWS2_t.general_parameter.speak_state = SPEAKER_CLOSE;
					}
					break;
				case SET_TEMP_LOCK_PAGE_CN:
					sFWS2_t.page = SELECT_SET_TEMP_LOCK_PAGE_CN;
					if (sFWS2_t.set_lock == UNLOCK)
					{
						sFWS2_t.general_parameter.display_lock_state = UNLOCK;
					}
					else if (sFWS2_t.set_lock == LOCK)
					{
						sFWS2_t.general_parameter.display_lock_state = LOCK;
					}
					break;
				case SET_SLEEP_PAGE_CN:
					sFWS2_t.base.last_set_sleep_time = RESET_VALUE;
					if (sFWS2_t.set_sleep == SET_RETURN_SLEEP)
					{
						sFWS2_t.page = SELECT_SET_SLEEP_PAGE_CN;
					}
					else if (sFWS2_t.set_sleep == SET_SLEEP)
					{
						sFWS2_t.set_sleep = SET_RETURN_SLEEP;
					}
					break;
				case SET_LANGUAGE_PAGE_CN:
					sFWS2_t.page = SELECT_SET_LANGUAGE_PAGE_CN;
					sFWS2_t.general_parameter.language_state = sFWS2_t.set_language;
					break;
				case SET_LANGUAGE_PAGE_ENG:
					sFWS2_t.page = SELECT_SET_LANGUAGE_PAGE_CN;
					sFWS2_t.general_parameter.language_state = sFWS2_t.set_language;
					break;
				case SET_RESET_PAGE_CN:
					if (sFWS2_t.set_reset1 == RESET_CONFIRN)
					{
						sFWS2_t.page = SET_RESET_DONE_PAGE_CN;
						sFWS2_t.set_reset2 = RESET2_CONFIRN;
						EVENT = SET_RESET;
					}
					else if (sFWS2_t.set_reset1 == RESET_RETURN)
					{
						sFWS2_t.page = SELECT_SET_RESET_PAGE_CN;
					}
					break;
				case SET_RESET_DONE_PAGE_CN:
					if (sFWS2_t.set_reset2 == RESET2_CONFIRN)
					{
						sFWS2_t.page = SELECT_SET_RESET_PAGE_CN;
					}
					break;
				case SET_VERSION_PAGE_CN:
					sFWS2_t.page = SELECT_SET_VERSION_PAGE_CN;
					break;
//				case SET_OTA_PAGE_CN:
//					sFWS2_t.page = SELECT_SET_OTA_PAGE_CN;
//					break;
				case SET_SUPPORT_PAGE_CN:
					sFWS2_t.page = SELECT_SET_SUPPORT_PAGE_CN;
					break;
				default:
					break;
				}
			}
			sbeep.status = BEEP_SHORT;
			break;
		case KEY_LONG_PRESS:
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				EVENT = ENTER_MENU;
			}
			else if (sFWS2_t.page != WORK_PAGE_210 || sFWS2_t.page != WORK_PAGE_245 || sFWS2_t.page != CURVE_PAGE)
			{
				EVENT = EXIT_MENU;
			}
			sbeep.status = BEEP_LONG;
			break;
		case KEY_DOUBLE_CLICK:
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
				{
					sFWS2_t.page = CURVE_PAGE;
					sFWS2_t.work_mode = WORK_CURVE;
					sFWS2_t.set_work_mode = sFWS2_t.work_mode;
				}
				else if (sFWS2_t.page == CURVE_PAGE)
				{
					if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
					{
						sFWS2_t.page = WORK_PAGE_210;
					}
					else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
					{
						sFWS2_t.page = WORK_PAGE_245;
					}
					sFWS2_t.work_mode = WORK_NORMAL;
					sFWS2_t.set_work_mode = sFWS2_t.work_mode;
				}
				sbeep.status = BEEP_LONG;
			}
			break;
		default:
			break;
		}
	}
	if (key_event[6] != KEY_NONE)
	{
		switch (key_event[6])
		{
		case KEY_SHORT_PRESS:
			EVENT = CHANGE_CH;
			sbeep.status = BEEP_SHORT;
			break;
//		case KEY_LONG_PRESS_CONTINUE:
//			sFWS2_t.rpc_state = true;
//			break;
		default:
			break;
		}
	}
}

static void key_event_handle(void)
{
	static int8_t delay_time = 0;
	switch (EVENT)
	{
	case ENTER_MENU:
		sFWS2_t.page = SELECT_SET_WORK_PAGE_CN;
		EVENT = END_EVENT;
		break;
	case EXIT_MENU:
		if (sFWS2_t.work_mode == WORK_NORMAL)
		{
			if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
			{
				sFWS2_t.page = WORK_PAGE_210;
			}
			else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
			{
				sFWS2_t.page = WORK_PAGE_245;
			}
			else
				sFWS2_t.page = WORK_PAGE_210;
		}
		else if (sFWS2_t.work_mode == WORK_CURVE)
		{
			sFWS2_t.page = CURVE_PAGE;
		}
		EVENT = END_EVENT;
		break;
	case PAGE_ADD:
		if (sFWS2_t.page >= SELECT_EXIT_MENU_PAGE_CN)
		{
			sFWS2_t.page = SELECT_SET_WORK_PAGE_CN;
		}
		else
		{
			sFWS2_t.page += 2;
		}
		EVENT = END_EVENT;
		break;
	case PAGE_REDUCE:
		if (sFWS2_t.page <= SELECT_SET_WORK_PAGE_CN)
		{
			sFWS2_t.page = SELECT_EXIT_MENU_PAGE_CN;
		}
		else
		{
			sFWS2_t.page-=2;
		}
		EVENT = END_EVENT;
		break;
	case GET_CHANNEL_1:
		sFWS2_t.base.set_temp = sFWS2_t.general_parameter.ch1_set_temp;
		if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{ // 华氏度
			sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.general_parameter.ch1_set_temp / 5 + 32;
		}
		sFWS2_t.general_parameter.ch = 1;
		PID_Clear(&pid_210);
		PID_Clear(&pid_245);
		EVENT = END_EVENT;
		break;
	case GET_CHANNEL_2:

		sFWS2_t.base.set_temp = sFWS2_t.general_parameter.ch2_set_temp;
		if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{ // 华氏度
			sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.general_parameter.ch2_set_temp / 5 + 32;
		}
		sFWS2_t.general_parameter.ch = 2;
		PID_Clear(&pid_210);
		PID_Clear(&pid_245);
		EVENT = END_EVENT;
		break;
	case GET_CHANNEL_3:

		sFWS2_t.base.set_temp = sFWS2_t.general_parameter.ch3_set_temp;
		if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{ // 华氏度
			sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.general_parameter.ch3_set_temp / 5 + 32;
		}
		sFWS2_t.general_parameter.ch = 3;
		PID_Clear(&pid_210);
		PID_Clear(&pid_245);
		EVENT = END_EVENT;
		break;
	case CHANGE_CH:
		sFWS2_t.general_parameter.ch++;
		if (sFWS2_t.general_parameter.ch > 3)
			sFWS2_t.general_parameter.ch = 1;
		if (sFWS2_t.general_parameter.ch == 1)
		{
			sFWS2_t.base.set_temp = sFWS2_t.general_parameter.ch1_set_temp;
			if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
			{ // 华氏度
				sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.general_parameter.ch1_set_temp / 5 + 32;
			}
		}
		else if (sFWS2_t.general_parameter.ch == 2)
		{
			sFWS2_t.base.set_temp = sFWS2_t.general_parameter.ch2_set_temp;
			if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
			{ // 华氏度
				sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.general_parameter.ch2_set_temp / 5 + 32;
			}
		}
		else if (sFWS2_t.general_parameter.ch == 3)
		{
			sFWS2_t.base.set_temp = sFWS2_t.general_parameter.ch3_set_temp;
			if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
			{ // 华氏度
				sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.general_parameter.ch3_set_temp / 5 + 32;
			}
		}
		PID_Clear(&pid_210);
		PID_Clear(&pid_245);
		EVENT = END_EVENT;
		break;
	case SET_CHANNEL_3:
		if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
		{
			sFWS2_t.general_parameter.ch3_set_temp = sFWS2_t.base.set_temp;
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{
			sFWS2_t.general_parameter.ch3_set_temp_f_display = sFWS2_t.base.set_temp_f_display;
		}
		sFWS2_t.general_parameter.ch = 3;
		EVENT = END_EVENT;
		break;
	case SET_CHANNEL_2:
		if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
		{
			sFWS2_t.general_parameter.ch2_set_temp = sFWS2_t.base.set_temp;
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{
			sFWS2_t.general_parameter.ch2_set_temp_f_display = sFWS2_t.base.set_temp_f_display;
		}
		sFWS2_t.general_parameter.ch = 2;
		EVENT = END_EVENT;
		break;
	case SET_CHANNEL_1:
		if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
		{
			sFWS2_t.general_parameter.ch1_set_temp = sFWS2_t.base.set_temp;
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{
			sFWS2_t.general_parameter.ch1_set_temp_f_display = sFWS2_t.base.set_temp_f_display;
		}
		sFWS2_t.general_parameter.ch = 1;
		EVENT = END_EVENT;
		break;
	case TEMP_ADD:
		PID_Clear_I(&pid_210);
		PID_Clear_I(&pid_245);
		if (sFWS2_t.general_parameter.temp_unit == CELSIUS) // 摄氏度
		{
			if (sFWS2_t.base.set_temp >= MAX_SET_TEMP_VAL)
			{
				sFWS2_t.base.set_temp = MAX_SET_TEMP_VAL;
			}
			else
			{
				sFWS2_t.base.set_temp++;
			}
			sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT) // 华氏度
		{
			if (sFWS2_t.base.set_temp_f_display >= 842)
			{
				sFWS2_t.base.set_temp_f_display = 842;
			}
			else
			{
				sFWS2_t.base.set_temp_f_display++;
			}
			sFWS2_t.base.set_temp = (sFWS2_t.base.set_temp_f_display - 32) * 5 / 9;
		}
		sFWS2_t.general_parameter.ch = 0;
		sbeep.status = BEEP_SHORT;
		EVENT = END_EVENT;
		break;

	case TEMP_ADD_FIVE:
		PID_Clear_I(&pid_210);
		PID_Clear_I(&pid_245);
		delay_time--;
		if (delay_time <= 0)
		{
			delay_time = KEY_LONG_PRESS_TIME;
			PID_Clear_I(&pid_210);
			PID_Clear_I(&pid_245);
			if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
			{
				if (sFWS2_t.base.set_temp >= MAX_SET_TEMP_VAL - 5)
				{
					sFWS2_t.base.set_temp = MAX_SET_TEMP_VAL;
				}
				else
				{
					sFWS2_t.base.set_temp += 5;
				}
				sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
			}
			else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
			{
				if (sFWS2_t.base.set_temp_f_display >= 842)
				{
					sFWS2_t.base.set_temp_f_display = 842;
				}
				else
				{
					sFWS2_t.base.set_temp_f_display += 41;
				}
				sFWS2_t.base.set_temp = (sFWS2_t.base.set_temp_f_display - 32) * 5 / 9;
			}
			sFWS2_t.general_parameter.ch = 0;
			sbeep.status = BEEP_SHORT;
		}
		EVENT = END_EVENT;
		break;

	case TEMP_REDUCE:
		PID_Clear_I(&pid_210);
		PID_Clear_I(&pid_245);
		if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
		{
			if (sFWS2_t.base.set_temp <= MIN_SET_TEMP_VAL)
			{
				sFWS2_t.base.set_temp = MIN_SET_TEMP_VAL;
			}
			else
			{
				sFWS2_t.base.set_temp--;
			}
			sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{
			if (sFWS2_t.base.set_temp_f_display <= 392)
			{
				sFWS2_t.base.set_temp_f_display = 392;
			}
			else
			{
				sFWS2_t.base.set_temp_f_display--;
			}
			sFWS2_t.base.set_temp = (sFWS2_t.base.set_temp_f_display - 32) * 5 / 9;
		}
		sFWS2_t.general_parameter.ch = 0;
		sbeep.status = BEEP_SHORT;
		EVENT = END_EVENT;
		break;

	case TEMP_REDUCE_FIVE:
		delay_time--;
		if (delay_time <= 0)
		{
			delay_time = KEY_LONG_PRESS_TIME;
			PID_Clear_I(&pid_210);
			PID_Clear_I(&pid_245);
			if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
			{
				if (sFWS2_t.base.set_temp <= MIN_SET_TEMP_VAL + 5)
				{
					sFWS2_t.base.set_temp = MIN_SET_TEMP_VAL;
				}
				else
				{
					sFWS2_t.base.set_temp -= 5;
				}
				sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
			}
			else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
			{
				if (sFWS2_t.base.set_temp_f_display <= 392)
				{
					sFWS2_t.base.set_temp_f_display = 392;
				}
				else
				{
					sFWS2_t.base.set_temp_f_display -= 41;
				}
				sFWS2_t.base.set_temp = (sFWS2_t.base.set_temp_f_display - 32) * 5 / 9;
			}
			sFWS2_t.general_parameter.ch = 0;
			sbeep.status = BEEP_SHORT;
		}
		EVENT = END_EVENT;
		break;
	case SET_WORK_MODE:
		if (sFWS2_t.work_mode == WORK_NORMAL)
		{
			sFWS2_t.work_mode = WORK_CURVE;
			sFWS2_t.set_work_mode = sFWS2_t.work_mode;
		}
		else if (sFWS2_t.work_mode == WORK_CURVE)
		{
			sFWS2_t.work_mode = WORK_NORMAL;
			sFWS2_t.set_work_mode = sFWS2_t.work_mode;
		}
		EVENT = END_EVENT;
		break;
	case SET_TEMP_UNIT:
		if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
		{
			sFWS2_t.general_parameter.temp_unit = FAHRENHEIT;
			sFWS2_t.set_temp_unit = sFWS2_t.general_parameter.temp_unit;
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{
			sFWS2_t.general_parameter.temp_unit = CELSIUS;
			sFWS2_t.set_temp_unit = sFWS2_t.general_parameter.temp_unit;
		}
		EVENT = END_EVENT;
		break;
	case SET_TEMP_LOCK:
		if (sFWS2_t.general_parameter.display_lock_state == UNLOCK)
		{
			sFWS2_t.general_parameter.display_lock_state = LOCK;
			sFWS2_t.set_lock = sFWS2_t.general_parameter.display_lock_state;
		}
		else if (sFWS2_t.general_parameter.display_lock_state == LOCK)
		{
			sFWS2_t.general_parameter.display_lock_state = UNLOCK;
			sFWS2_t.set_lock = sFWS2_t.general_parameter.display_lock_state;
		}
		EVENT = END_EVENT;
		break;
	case SET_SPEAK_MODE:
		if (sFWS2_t.general_parameter.speak_state == SPEAKER_OPEN)
		{
			sFWS2_t.general_parameter.speak_state = SPEAKER_CLOSE;
			sFWS2_t.set_speak = sFWS2_t.general_parameter.speak_state;
		}
		else if (sFWS2_t.general_parameter.speak_state == SPEAKER_CLOSE)
		{
			sFWS2_t.general_parameter.speak_state = SPEAKER_OPEN;
			sFWS2_t.set_speak = sFWS2_t.general_parameter.speak_state;
		}
		sFWS2_t.general_parameter.last_speak_state = RESET_VALUE;
		EVENT = END_EVENT;
		break;
	case SET_CAL_TEMP_ADD:
		if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
		{
			if (sFWS2_t.base.cal_temp_c_display >= sFWS2_t.base.set_temp + MAX_CAL_TEMP)
			{
				sFWS2_t.base.cal_temp_c_display = sFWS2_t.base.set_temp + MAX_CAL_TEMP;
			}
			else
			{
				sFWS2_t.base.cal_temp_c_display++;
			}

			sFWS2_t.base.cal_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{
			if (sFWS2_t.base.cal_temp_f_display >= sFWS2_t.base.set_temp_f_display + MAX_CAL_TEMP_F)
			{
				sFWS2_t.base.cal_temp_f_display = sFWS2_t.base.set_temp_f_display + MAX_CAL_TEMP_F;
			}
			else
			{
				sFWS2_t.base.cal_temp_f_display++;
			}

			sFWS2_t.base.cal_temp_c_display = (sFWS2_t.base.cal_temp_f_display - 32) * 5 / 9;
		}

		EVENT = END_EVENT;
		break;

	case SET_CAL_TEMP_REDUCE:
		if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
		{
			if (sFWS2_t.base.cal_temp_c_display <= sFWS2_t.base.set_temp + MIN_CAL_TEMP)
			{
				sFWS2_t.base.cal_temp_c_display = sFWS2_t.base.set_temp + MIN_CAL_TEMP;
			}
			else
			{
				sFWS2_t.base.cal_temp_c_display--;
			}

			sFWS2_t.base.cal_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{
			if (sFWS2_t.base.cal_temp_f_display >= sFWS2_t.base.set_temp_f_display + MIN_CAL_TEMP_F)
			{
				sFWS2_t.base.cal_temp_f_display = sFWS2_t.base.set_temp_f_display + MIN_CAL_TEMP_F;
			}
			else
			{
				sFWS2_t.base.cal_temp_f_display--;
			}

			sFWS2_t.base.cal_temp_c_display = (sFWS2_t.base.cal_temp_f_display - 32) * 5 / 9;
		}

		EVENT = END_EVENT;
		break;
	case SET_CAL_TEMP_ADD_FIVE:
		delay_time--;
		if (delay_time <= 0)
		{
			delay_time = KEY_LONG_PRESS_TIME;
			sbeep.status = BEEP_SHORT;

			if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
			{
				if (sFWS2_t.base.cal_temp_c_display >= sFWS2_t.base.set_temp + MAX_CAL_TEMP - 5)
				{
					sFWS2_t.base.cal_temp_c_display = sFWS2_t.base.set_temp + MAX_CAL_TEMP;
				}
				else
				{
					sFWS2_t.base.cal_temp_c_display += 5;
				}

				sFWS2_t.base.cal_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
			}
			else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
			{
				if (sFWS2_t.base.cal_temp_f_display >= sFWS2_t.base.set_temp_f_display + MAX_CAL_TEMP_F - 41)
				{
					sFWS2_t.base.cal_temp_f_display = sFWS2_t.base.set_temp_f_display + MAX_CAL_TEMP_F;
				}
				else
				{
					sFWS2_t.base.cal_temp_f_display += 41;
				}

				sFWS2_t.base.cal_temp_c_display = (sFWS2_t.base.cal_temp_f_display - 32) * 5 / 9;
			}
		}
		EVENT = END_EVENT;
		break;

	case SET_CAL_TEMP_REDUCE_FIVE:
		delay_time--;

		if (delay_time <= 0)
		{
			delay_time = KEY_LONG_PRESS_TIME;
			sbeep.status = BEEP_SHORT;

			if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
			{
				if (sFWS2_t.base.cal_temp_c_display <= sFWS2_t.base.set_temp + MIN_CAL_TEMP)
				{
					sFWS2_t.base.cal_temp_c_display = sFWS2_t.base.set_temp + MIN_CAL_TEMP;
				}
				else
				{
					sFWS2_t.base.cal_temp_c_display--;
				}

				sFWS2_t.base.cal_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
			}
			else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
			{
				if (sFWS2_t.base.cal_temp_f_display <= sFWS2_t.base.set_temp_f_display + MIN_CAL_TEMP_F + 41)
				{
					sFWS2_t.base.cal_temp_f_display = sFWS2_t.base.set_temp_f_display + MIN_CAL_TEMP_F;
				}
				else
				{
					sFWS2_t.base.cal_temp_f_display -= 5;
				}

				sFWS2_t.base.cal_temp_c_display = (sFWS2_t.base.cal_temp_f_display - 32) * 5 / 9;
			}
		}
		EVENT = END_EVENT;
		break;
	case SET_SLEEP_TIME_ADD:
		if (sFWS2_t.base.set_sleep_time >= MAX_SLEEP_TIME)
		{
			sFWS2_t.base.set_sleep_time = MAX_SLEEP_TIME;
		}
		else 
		{
			sFWS2_t.base.set_sleep_time += 0.1;
		}
		sFWS2_t.base.sleep_time_count = sFWS2_t.base.set_sleep_time;
		EVENT = END_EVENT;
		break;
	case SET_SLEEP_TIME_REDUCE:
		if (sFWS2_t.base.set_sleep_time <= MIN_SLEEP_TIME)
		{
			sFWS2_t.base.set_sleep_time = MIN_SLEEP_TIME;
		}
		else
		{
			sFWS2_t.base.set_sleep_time -= 0.1;
		}
		sFWS2_t.base.sleep_time_count = sFWS2_t.base.set_sleep_time;
		EVENT = END_EVENT;
		break;
	case SET_SLEEP_TIME_ADD_FIVE:
		delay_time--;
		if (delay_time <= 0)
		{
			delay_time = KEY_LONG_PRESS_TIME;
			sbeep.status = BEEP_SHORT;
			if (sFWS2_t.base.set_sleep_time >= MAX_SLEEP_TIME - 5)
			{
				sFWS2_t.base.set_sleep_time = MAX_SLEEP_TIME;
			}
			else
			{
				sFWS2_t.base.set_sleep_time += 5.0;
			}
		}
		sFWS2_t.base.sleep_time_count = sFWS2_t.base.set_sleep_time;
		EVENT = END_EVENT;
		break;
	case SET_SLEEP_TIME_REDUCE_FIVE:
		delay_time--;
		if (delay_time <= 0)
		{
			delay_time = KEY_LONG_PRESS_TIME;
			sbeep.status = BEEP_SHORT;
			if (sFWS2_t.base.set_sleep_time <= MIN_SLEEP_TIME + 5)
			{
				sFWS2_t.base.set_sleep_time = MIN_SLEEP_TIME;
			}
			else
			{
				sFWS2_t.base.set_sleep_time -= 5.0;
			}
		}
		sFWS2_t.base.sleep_time_count = sFWS2_t.base.set_sleep_time;
		EVENT = END_EVENT;
		break;

	case SET_LANGUAGE_MODE:
		if(sFWS2_t.general_parameter.language_state == CHINESE)
		{
			sFWS2_t.general_parameter.language_state = ENGLISH;
			sFWS2_t.set_language = ENGLISH;
			sFWS2_t.page = SET_LANGUAGE_PICTURE_ENG;
		}
		else if(sFWS2_t.general_parameter.language_state == ENGLISH)
		{
			sFWS2_t.general_parameter.language_state = CHINESE;
			sFWS2_t.set_language = CHINESE;
			sFWS2_t.page = SET_LANGUAGE_PICTURE_CN;
		}
		EVENT = END_EVENT;
		break;
	case SET_RESET:
		sFWS2_t.reset_flag = true;
		/* reset value */
		/* reset handle data */
		sFWS2_t.base.set_temp = 340;
		sFWS2_t.base.cal_data = 0;
		sFWS2_t.general_parameter.ch1_set_temp = 280;
		sFWS2_t.general_parameter.ch2_set_temp = 320;
		sFWS2_t.general_parameter.ch3_set_temp = 360;
		sFWS2_t.base.set_sleep_time = 0;
		/* reset function data */
		sFWS2_t.general_parameter.temp_unit = CELSIUS;	
		sFWS2_t.general_parameter.speak_state = SPEAKER_OPEN;
		sFWS2_t.general_parameter.display_lock_state = LOCK;
		/* reset other data */
		sFWS2_t.work_mode = WORK_NORMAL;
		EVENT = END_EVENT;
		break;
	case END_EVENT:
		break;
	}
}

void key_handle(void)
{
	static handle_event event = END_EVENT;
	static uint8_t set_done = FALSE;
#if 1

	if (key_event[0] != KEY_NONE ||
		key_event[1] != KEY_NONE ||
		key_event[2] != KEY_NONE ||
		key_event[3] != KEY_NONE ||
		key_event[4] != KEY_NONE ||
		key_event[5] != KEY_NONE)
	{
		if (sFWS2_t.Direct_handle_state == HANDLE_SLEEP)
		{
			sFWS2_t.Direct_handle_state = HANDLE_WAKEN;
		}
		else if (sFWS2_t.Direct_handle_state == HANDLE_WAKEN)
		{
			sFWS2_t.base.waken_time_count = 0;
		}
	}
#endif
	handle_key_events();
	key_event_handle();
}
