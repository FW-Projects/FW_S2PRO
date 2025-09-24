#include "key_handle.h"
#include "beep_handle.h"
#include "FWS2pro_handle.h"
#include "cmsis_armclang.h"
#include "PID_operation.h"
static handle_event EVENT = END_EVENT;
void KeyProc(void);

static KEY_VALUE get_key_up()
{
	if (READ_KEY_UP == 0)
	{
		return K_PRESS;
	}
	else
	{
		return K_RELEASE;
	}
}

static KEY_VALUE get_key_down()
{
	if (READ_KEY_DOWN == 0)
	{
		return K_PRESS;
	}
	else
	{
		return K_RELEASE;
	}
}

static KEY_VALUE get_key_ch1()
{
	if (READ_KEY_CH1 == 0)
	{
		return K_PRESS;
	}
	else
	{
		return K_RELEASE;
	}
}

static KEY_VALUE get_key_ch2()
{
	if (READ_KEY_CH2 == 0)
	{
		return K_PRESS;
	}

	else
	{
		return K_RELEASE;
	}
}

static KEY_VALUE get_key_ch3()
{
	if (READ_KEY_CH3 == 0)
	{
		return K_PRESS;
	}
	else
	{
		return K_RELEASE;
	}
}

static KEY_VALUE get_key_set()
{
	if (READ_KEY_SET == 0)
	{
		return K_PRESS;
	}
	else
	{
		return K_RELEASE;
	}
}

static KEY keys[] =
	{
		{KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_ch1},  // ch1
		{KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_ch2},  // ch2
		{KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_ch3},  // ch3
		{KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_up},   // add
		{KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_down}, // reduce
		{KS_RELEASE, 0, KEY_CYCLE_TIME, get_key_set},  // set
};

static KEY_EVENT key_up, key_down, key_ch1, key_ch2, key_ch3, key_set;

void KeyProc(void)
{
	static bool set_done = false;
	if (get_key_down() == K_RELEASE &&
		get_key_ch3() == K_RELEASE &&
		get_key_ch1() == K_RELEASE &&
		get_key_set() == K_RELEASE &&
		get_key_ch2() == K_RELEASE &&
		get_key_up() == K_RELEASE)
	{
		set_done = false;
	}

	/* scan key begin */
	key_ch1 = key_event_check(&keys[0], LONG_PRESS_TIME);
	key_ch2 = key_event_check(&keys[1], LONG_PRESS_TIME);
	key_ch3 = key_event_check(&keys[2], LONG_PRESS_TIME);
	key_up = key_event_check(&keys[3], LONG_PRESS_TIME);
	key_down = key_event_check(&keys[4], LONG_PRESS_TIME);
	key_set = key_event_check(&keys[5], LONG_PRESS_TIME);
	/* scan key end */

	if (set_done == 0)
	{
		/*get event*/
		/* key no set perss */
		if (key_ch1 == KE_PRESS)
		{
			EVENT = GET_CHANNEL_1;
			sbeep.status = BEEP_SHORT;
			set_done = TRUE;
		}
		else if (key_ch2 == KE_PRESS)
		{
			EVENT = GET_CHANNEL_2;
			sbeep.status = BEEP_SHORT;
			set_done = TRUE;
		}
		else if (key_ch3 == KE_PRESS)
		{
			EVENT = GET_CHANNEL_3;
			sbeep.status = BEEP_SHORT;
			set_done = TRUE;
		}
		else if (key_down == KE_PRESS)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				// 减少温度
				EVENT = TEMP_REDUCE;
			}
			else if (sFWS2_t.page >= SELECT_SET_WORK_PAGE && sFWS2_t.page <= SELECT_EXIT_MENU_PAGE)
			{
				// 切换菜单
				EVENT = PAGE_REDUCE;
			}
			else if (sFWS2_t.page == SET_WORK_PAGE)
			{
				// 设置工作模式
				EVENT = SET_WORK_MODE;
			}
			else if (sFWS2_t.page == SET_UNIT_PAGE)
			{
				// 设置温度单位
				EVENT = SET_TEMP_UNIT;
			}
			else if (sFWS2_t.page == SET_TEMP_CAL_PAGE)
			{
				// 设置温度校准
				if (sFWS2_t.set_cal == SET_CAL)
				{
					EVENT = SET_CAL_TEMP_REDUCE;
				}
				else if (sFWS2_t.set_cal == SET_RETURN_CAL)
				{
					sFWS2_t.page = SET_TEMP_CAL_PAGE;
					sFWS2_t.set_cal = SET_CAL;
					sFWS2_t.base.last_cal_temp_c_display = RESET_VALUE;
					sFWS2_t.base.last_cal_temp_f_display = RESET_VALUE;
				}
			}
			else if (sFWS2_t.page == SET_SPEAK_PAGE)
			{
				// 设置声音
				EVENT = SET_SPEAK_MODE;
			}
			else if (sFWS2_t.page == SET_TEMP_LOCK_PAGE)
			{
				// 设置温度锁
				EVENT = SET_TEMP_LOCK;
			}
			else if (sFWS2_t.page == SET_SLEEP_PAGE)
			{
				// 设置睡眠时间
				if (sFWS2_t.set_sleep == SET_SLEEP)
				{
					EVENT = SET_SLEEP_TIME_REDUCE;
				}
				else if (sFWS2_t.set_sleep == SET_RETURN_SLEEP)
				{
					sFWS2_t.page = SET_SLEEP_PAGE;
					sFWS2_t.set_sleep = SET_SLEEP;
				}
			}
			else if (sFWS2_t.page == SET_RESET_PAGE)
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
			else if (sFWS2_t.page == SET_RESET_DONE_PAGE)
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
			else if (sFWS2_t.page == SET_VERSION_PAGE)
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
			set_done = TRUE;
		}
		else if (key_up == KE_PRESS)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				// 增加温度
				EVENT = TEMP_ADD;
			}
			else if (sFWS2_t.page >= SELECT_SET_WORK_PAGE && sFWS2_t.page <= SELECT_EXIT_MENU_PAGE)
			{
				// 切换菜单
				EVENT = PAGE_ADD;
			}
			else if (sFWS2_t.page == SET_WORK_PAGE)
			{
				// 设置工作模式
				EVENT = SET_WORK_MODE;
			}
			else if (sFWS2_t.page == SET_UNIT_PAGE)
			{
				// 设置温度单位
				EVENT = SET_TEMP_UNIT;
			}
			else if (sFWS2_t.page == SET_TEMP_CAL_PAGE)
			{
				// 设置温度校准
				if (sFWS2_t.set_cal == SET_CAL)
				{
					EVENT = SET_CAL_TEMP_ADD;
				}
				else if (sFWS2_t.set_cal == SET_RETURN_CAL)
				{
					sFWS2_t.page = SET_TEMP_CAL_PAGE;
					sFWS2_t.set_cal = SET_CAL;
					sFWS2_t.base.last_cal_temp_c_display = RESET_VALUE;
					sFWS2_t.base.last_cal_temp_f_display = RESET_VALUE;
				}
			}
			else if (sFWS2_t.page == SET_SPEAK_PAGE)
			{
				// 设置声音
				EVENT = SET_SPEAK_MODE;
			}
			else if (sFWS2_t.page == SET_TEMP_LOCK_PAGE)
			{
				// 设置温度锁
				EVENT = SET_TEMP_LOCK;
			}
			else if (sFWS2_t.page == SET_SLEEP_PAGE)
			{
				// 设置睡眠时间
				if (sFWS2_t.set_sleep == SET_SLEEP)
				{
					EVENT = SET_SLEEP_TIME_ADD;
				}
				else if (sFWS2_t.set_sleep == SET_RETURN_SLEEP)
				{
					sFWS2_t.page = SET_SLEEP_PAGE;
					sFWS2_t.set_sleep = SET_SLEEP;
				}
			}
			else if (sFWS2_t.page == SET_RESET_PAGE)
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
			else if (sFWS2_t.page == SET_RESET_DONE_PAGE)
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
			else if (sFWS2_t.page == SET_VERSION_PAGE)
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
			set_done = TRUE;
		}
		/* key  perss */
		else if (key_set == KE_PRESS)
		{
			if (sFWS2_t.page != WORK_PAGE_210 &&
				sFWS2_t.page != WORK_PAGE_245 &&
				sFWS2_t.page != CURVE_PAGE)
			{
				switch (sFWS2_t.page)
				{
				case SELECT_SET_WORK_PAGE:
					sFWS2_t.page = SET_WORK_PAGE;
					break;
				case SELECT_SET_UNIT_PAGE:
					sFWS2_t.page = SET_UNIT_PAGE;
					break;
				case SELECT_SET_TEMP_CAL_PAGE:
					sFWS2_t.page = SET_TEMP_CAL_PAGE;
					sFWS2_t.set_cal = SET_CAL;
					break;
				case SELECT_SET_SPEAK_PAGE:
					sFWS2_t.page = SET_SPEAK_PAGE;
					break;
				case SELECT_SET_TEMP_LOCK_PAGE:
					sFWS2_t.page = SET_TEMP_LOCK_PAGE;
					break;
				case SELECT_SET_SLEEP_PAGE:
					sFWS2_t.page = SET_SLEEP_PAGE;
					sFWS2_t.set_sleep = SET_SLEEP;
					break;
				case SELECT_SET_RESET_PAGE:
					sFWS2_t.page = SET_RESET_PAGE;
					sFWS2_t.set_reset1 = RESET_RETURN;
					break;
				case SELECT_SET_VERSION_PAGE:
					sFWS2_t.page = SET_VERSION_PAGE;
					sFWS2_t.set_vision = VISION_RETURN;
					break;
				case SELECT_SET_OTA_PAGE:
					sFWS2_t.page = SET_OTA_PAGE;
					break;
				case SELECT_EXIT_MENU_PAGE:
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
				case SET_WORK_PAGE:
					sFWS2_t.page = SELECT_SET_WORK_PAGE;
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
				case SET_UNIT_PAGE:
					sFWS2_t.page = SELECT_SET_UNIT_PAGE;
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
				case SET_TEMP_CAL_PAGE:
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
						sFWS2_t.page = SELECT_SET_TEMP_CAL_PAGE;
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
				case SET_SPEAK_PAGE:
					sFWS2_t.page = SELECT_SET_SPEAK_PAGE;
					sFWS2_t.set_speak = sFWS2_t.general_parameter.speak_state ;
					if (sFWS2_t.set_speak == SPEAKER_OPEN)
					{
						sFWS2_t.general_parameter.speak_state = SPEAKER_OPEN;
					}
					else if (sFWS2_t.set_speak == SPEAKER_CLOSE)
					{
						sFWS2_t.general_parameter.speak_state = SPEAKER_CLOSE;
					}
					break;
				case SET_TEMP_LOCK_PAGE:
					sFWS2_t.page = SELECT_SET_TEMP_LOCK_PAGE;
					if (sFWS2_t.set_lock == UNLOCK)
					{
						sFWS2_t.general_parameter.display_lock_state = UNLOCK;
					}
					else if (sFWS2_t.set_lock == LOCK)
					{
						sFWS2_t.general_parameter.display_lock_state = LOCK;
					}
					break;
				case SET_SLEEP_PAGE:
					if (sFWS2_t.set_sleep == SET_RETURN_SLEEP)
					{
						sFWS2_t.page = SELECT_SET_SLEEP_PAGE;
					}
					else if (sFWS2_t.set_sleep == SET_SLEEP)
					{
						sFWS2_t.set_sleep = SET_RETURN_SLEEP;
					}
					break;
				case SET_RESET_PAGE:
					if (sFWS2_t.set_reset1 == RESET_CONFIRN)
					{
						sFWS2_t.page = SET_RESET_DONE_PAGE;
						sFWS2_t.set_reset2 = RESET2_CONFIRN;
						EVENT = SET_RESET;
					}
					else if (sFWS2_t.set_reset1 == RESET_RETURN)
					{
						sFWS2_t.page = SELECT_SET_RESET_PAGE;
					}
					break;
				case SET_RESET_DONE_PAGE:
					if (sFWS2_t.set_reset2 == RESET2_CONFIRN)
					{
						sFWS2_t.page = SELECT_SET_RESET_PAGE;
					}
					break;
				case SET_VERSION_PAGE:
					sFWS2_t.page = SELECT_SET_VERSION_PAGE;
					break;
				case SET_OTA_PAGE:
					sFWS2_t.page = SELECT_SET_OTA_PAGE;
					break;
				default:
					break;
				}
			}
			set_done = true;
			sbeep.status = BEEP_SHORT;
		}
		/* key long perss */
		if (key_set == KE_LONG_PRESS)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				EVENT = ENTER_MENU;
			}

			sbeep.status = BEEP_LONG;
			set_done = TRUE;
		}
		else if (key_ch1 == KE_LONG_PRESS)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				EVENT = SET_CHANNEL_1;
			}
			sbeep.status = BEEP_LONG;
			set_done = TRUE;
		}
		else if (key_ch2 == KE_LONG_PRESS)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				EVENT = SET_CHANNEL_2;
			}
			sbeep.status = BEEP_LONG;
			set_done = TRUE;
		}
		else if (key_ch3 == KE_LONG_PRESS)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				EVENT = SET_CHANNEL_3;
			}
			sbeep.status = BEEP_LONG;
			set_done = TRUE;
		}
		else if (key_down == KE_LONG_PRESS)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				// 减少温度
				EVENT = TEMP_REDUCE_FIVE;
			}
			else if (sFWS2_t.page == SET_TEMP_CAL_PAGE)
			{
				// 设置温度校准
				if (sFWS2_t.set_cal == SET_CAL)
				{
					EVENT = SET_CAL_TEMP_REDUCE_FIVE;
				}
			}
			else if (sFWS2_t.page == SET_SLEEP_PAGE)
			{
				// 设置睡眠时间
				if (sFWS2_t.set_sleep == SET_SLEEP)
				{
					EVENT = SET_SLEEP_TIME_REDUCE_FIVE;
				}
			}
		}
		else if (key_up == KE_LONG_PRESS)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
			{
				// 减少温度
				EVENT = TEMP_ADD_FIVE;
			}
			else if (sFWS2_t.page == SET_TEMP_CAL_PAGE)
			{
				// 设置温度校准
				if (sFWS2_t.set_cal == SET_CAL)
				{
					EVENT = SET_CAL_TEMP_ADD_FIVE;
				}
			}
			else if (sFWS2_t.page == SET_SLEEP_PAGE)
			{
				// 设置睡眠时间
				if (sFWS2_t.set_sleep == SET_SLEEP)
				{
					EVENT = SET_SLEEP_TIME_ADD_FIVE;
				}
			}
		}
	}
}

static void key_event_handle(void)
{
	static int8_t delay_time = 0;
	switch (EVENT)
	{
	case ENTER_MENU:
		sFWS2_t.page = SELECT_SET_WORK_PAGE;
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
		if (sFWS2_t.page >= SELECT_EXIT_MENU_PAGE)
		{
			sFWS2_t.page = SELECT_SET_WORK_PAGE;
		}
		else
		{
			sFWS2_t.page++;
		}
		EVENT = END_EVENT;
		break;
	case PAGE_REDUCE:
		if (sFWS2_t.page <= SELECT_SET_WORK_PAGE)
		{
			sFWS2_t.page = SELECT_EXIT_MENU_PAGE;
		}
		else
		{
			sFWS2_t.page--;
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
		sFWS2_t.display_parameter.display_number = Disp_Set;
		break;
	case GET_CHANNEL_2:
		sFWS2_t.base.set_temp = sFWS2_t.general_parameter.ch2_set_temp;
		if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{ // 华氏度
			sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.general_parameter.ch2_set_temp / 5 + 32;
		}
		sFWS2_t.general_parameter.ch = 2;
		sFWS2_t.display_parameter.display_number = Disp_Set;
		break;
	case GET_CHANNEL_3:
		sFWS2_t.base.set_temp = sFWS2_t.general_parameter.ch3_set_temp;
		if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{ // 华氏度
			sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.general_parameter.ch3_set_temp / 5 + 32;
		}
		sFWS2_t.general_parameter.ch = 3;
		sFWS2_t.display_parameter.display_number = Disp_Set;

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
		EVENT = END_EVENT;
		break;
	case TEMP_ADD:
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
		}

		sbeep.status = BEEP_SHORT;
		EVENT = END_EVENT;
		break;

	case TEMP_ADD_FIVE:
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
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{
			if (sFWS2_t.base.set_temp_f_display >= 842)
			{
				sFWS2_t.base.set_temp_f_display = 842;
			}
			else
			{
				sFWS2_t.base.set_temp_f_display += 5;
			}
		}
		sFWS2_t.general_parameter.ch = 0;
		sFWS2_t.display_parameter.display_number = Disp_Set;
		sbeep.status = BEEP_SHORT;
		EVENT = END_EVENT;
		break;

	case TEMP_REDUCE:
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
		}
		sFWS2_t.general_parameter.ch = 0;
		sFWS2_t.display_parameter.display_number = Disp_Set;
		sbeep.status = BEEP_SHORT;
		EVENT = END_EVENT;
		break;

	case TEMP_REDUCE_FIVE:
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
		}
		else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
		{
			if (sFWS2_t.base.set_temp_f_display <= 392)
			{
				sFWS2_t.base.set_temp_f_display = 392;
			}
			else
			{
				sFWS2_t.base.set_temp_f_display -= 5;
			}
		}

		sbeep.status = BEEP_SHORT;
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
			sFWS2_t.base.set_sleep_time++;
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
			sFWS2_t.base.set_sleep_time--;
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
				sFWS2_t.base.set_sleep_time += 5;
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
				sFWS2_t.base.set_sleep_time -= 5;
			}
		}

		sFWS2_t.base.sleep_time_count = sFWS2_t.base.set_sleep_time;
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

	if (get_key_up() == K_PRESS ||
		get_key_down() == K_PRESS ||
		get_key_ch1() == K_PRESS ||
		get_key_ch2() == K_PRESS ||
		get_key_ch3() == K_PRESS ||
		get_key_set() == K_PRESS)
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
	KeyProc();
	key_event_handle();
}
