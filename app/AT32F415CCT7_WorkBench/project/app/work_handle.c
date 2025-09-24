#include "work_handle.h"
#include "FWS2pro_handle.h"
#include "PID_operation.h"
#include "beep_handle.h"
#include "key_handle.h"
#include "temp_error_handle.h"
#include "output_handle.h"

void check_handle_position(void) // ¼ì²âÐÝÃß
{
	/* check direct handle position */
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_8)) // ²»ÐÝÃß
	{
		sFWS2_t.Direct_handle_position = NOT_IN_POSSITION;
		sFWS2_t.Direct_handle_state = HANDLE_WORKING; // ÏÈÅÐ¶ÏÓÐÃ»ÓÐ±¨¾¯ÔÙÅÐ¶ÏÓÐÃ»ÓÐÐÝÃß
	}
	else // ÐÝÃß
	{
		sFWS2_t.Direct_handle_position = IN_POSSITION;
		sFWS2_t.Direct_handle_state = HANDLE_SLEEP;
	}
}

void check_handle_rpc(void)
{
	/* check direct handle »»ÊÖ±ú */
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_6))
	{
		sFWS2_t.Direct_handle_rpc = NOT_IN_RPC;
		sFWS2_t.Direct_handle_state = HANDLE_WORKING;
	}
	else
	{
		sFWS2_t.Direct_handle_rpc = IN_RPC;
		sFWS2_t.Direct_handle_state = HANDLE_RPC; // »»ÊÖ±ú
	}
}

void check_handle_Heating_element(void) // ¼ì²âÊÖ±ú·¢ÈÈÐ¾
{
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_3) == 0) // 210
	{
		sFWS2_t.Direct_handle_Heating_stick = HANDLE_210;
		if (sFWS2_t.general_parameter.last_Heating_stick != sFWS2_t.Direct_handle_Heating_stick)
		{
			sFWS2_t.general_parameter.last_Heating_stick = sFWS2_t.Direct_handle_Heating_stick;
			PID_Init_210(&direct_pid, 50, 0.5, 10, 20000);
			gpio_bits_reset(GPIOB, GPIO_PINS_14); // ÇÐ»»±¶Êý
		}
	}
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_4) == 0) // 245
	{

		sFWS2_t.Direct_handle_Heating_stick = HANDLE_245;
		if (sFWS2_t.general_parameter.last_Heating_stick != sFWS2_t.Direct_handle_Heating_stick)
		{
			sFWS2_t.general_parameter.last_Heating_stick = sFWS2_t.Direct_handle_Heating_stick;
			PID_Init_245(&direct_pid, 250, 0.5, 100, 59999);
			gpio_bits_set(GPIOB, GPIO_PINS_14); // ÇÐ»»245±¶Êý
		}
	}
}
void check_handle_magnification(void)
{
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_4) == 0)
	{
		gpio_bits_reset(GPIOA, GPIO_PINS_2); // ÇÐ»»210±¶Êý
		PID_Init_210(&direct_pid, 50, 0.5, 10, 20000);
	}
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_5) == 0)
	{
		gpio_bits_set(GPIOA, GPIO_PINS_2); // ÇÐ»»245±¶Êý
		PID_Init_245(&direct_pid, 250, 0.5,100, 59999);
	}
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_4) == 1 && gpio_input_data_bit_read(GPIOB, GPIO_PINS_5) == 1)
	{
		gpio_bits_reset(GPIOB, GPIO_PINS_14); // ÇÐ»»210±¶Êý
		PID_Init_210(&direct_pid, 50, 0.5, 10, 20000);
	}
}

void Direct_handle_switch(void)
{
	check_handle_Heating_element(); // ¼ì²âÊÖ±ú
	check_handle_rpc();				// ¼ì²â»»ÊÖ±ú
	if(sFWS2_t.Direct_handle_rpc == NOT_IN_RPC)
		check_handle_position();		// ¼ì²âÐÝÃß

	if (sFWS2_t.Direct_handle_error_state == HANDLE_OK) // ÎÞ±¨´í
	{
		switch (sFWS2_t.Direct_handle_state)
		{
		case HANDLE_SLEEP:
			if (sFWS2_t.base.sleep_time == 0x00) // ½øÈëÐÝÃß
			{
				if (sFWS2_t.base.actual_temp <= 200)
				{
					if (sFWS2_t.general_parameter.key_on_flag == 0)
						sFWS2_t.display_parameter.display_number = Disp_Slp;
				}
				else
				{
					if (sFWS2_t.general_parameter.key_on_flag == 0)
						sFWS2_t.display_parameter.display_number = Disp_Real;
				}
			}
			else
			{
				if (sFWS2_t.general_parameter.key_on_flag == 0)
					sFWS2_t.display_parameter.display_number = Disp_Real;
			}
			break;

		case HANDLE_WORKING:
			tmr_counter_enable(TMR2, TRUE);
			sFWS2_t.base.sleep_time = sFWS2_t.base.set_sleep_time;
			if (sFWS2_t.general_parameter.key_on_flag == 0)
			{
				if (sFWS2_t.general_parameter.display_lock_state == UNLOCK)
				{ // ÎÞËøÏÔ
					sFWS2_t.display_parameter.display_number = Disp_Real;
				}
				else 
				{ // ËøÏÔÊ¾
					if (PID_HOT != Check_temp_difference(sFWS2_t.base.set_temp,sFWS2_t.base.actual_temp,10))
					{ // ²»ËøÏÔÊ¾
						sFWS2_t.display_parameter.display_number = Disp_Real;
					}
					else
					{ // ËøÏÔÊ¾
						sFWS2_t.display_parameter.display_number = Disp_Set;
//						sFWS2_t.display_parameter.display_number = Disp_Real;
					}
				}
			}
			break;
		case HANDLE_RPC: // »»ÊÖ±ú
			//			sbeep.status = BEEP_LONG;
			sFWS2_t.base.sleep_time = sFWS2_t.base.set_sleep_time;
			if (sFWS2_t.general_parameter.key_on_flag == 0)
				sFWS2_t.display_parameter.display_number = Disp_Flick;
			break;
		}
	}
}
