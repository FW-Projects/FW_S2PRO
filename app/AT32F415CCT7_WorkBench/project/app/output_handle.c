#include "output_handle.h"
#include "FWS2pro_handle.h"
#include "PID_operation.h"
#include "wk_system.h"
#include "math.h"
#include "perf_counter.h"
#include "adc_filter.h"
#include "display_handle.h"

static void check_error_state(void);
static void check_handle(void);
static void check_rpc(void);
uint8_t Check_temp_difference(uint16_t in_set, uint16_t in_real, char in_dif);

uint8_t off_set_buff[26] = {8, 7, 5, 4, 3, 2, 2, 2, 1, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0};

void output_handle(void)
{
	check_handle();
	check_error_state();  
//	check_current();
}


static void check_error_state(void)
{
	if (sFWS2_t.Direct_handle_state == HANDLE_WORKING)
	{
		if (sFWS2_t.base.actual_temp > MAX_ACTUAL_TEMP || sFWS2_t.base.actual_temp < MIN_ACTUAL_TEMP || sFWS2_t.Direct_handle_Heating_stick == HANDLE_NO)
		{
			sFWS2_t.base.error_time++;

			if (sFWS2_t.base.error_time >= ERROR_TIME)
			{
				if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_NO)
				{
					sFWS2_t.Direct_handle_error_state = HANDLE_NO_ERR;
				}
				else if(sFWS2_t.base.actual_temp > MAX_MAX_ACTUAL_TEMP)
				{
					sFWS2_t.Direct_handle_error_state = HANDLE_NO_ERR;
				}
				else if (sFWS2_t.base.actual_temp > MAX_ACTUAL_TEMP && sFWS2_t.base.actual_temp < MAX_MAX_ACTUAL_TEMP)
				{
					sFWS2_t.Direct_handle_error_state = HANDLE_OVER_TEMP_ERR;
				}
				else if (sFWS2_t.base.actual_temp < MIN_ACTUAL_TEMP &&
						sFWS2_t.Direct_handle_position == NOT_IN_POSSITION &&
						sFWS2_t.Direct_handle_error_state != HANDLE_OVER_CURRENT)
				{
					sFWS2_t.Direct_handle_error_state = HANDLE_LOW_TEMP_ERR;
				}
			}
		}
		else
		{
			sFWS2_t.base.error_time = 0;
			if(sFWS2_t.Direct_handle_error_state  != HANDLE_OVER_CURRENT)
				sFWS2_t.Direct_handle_error_state = HANDLE_OK;
		}
	}
//	else if (sFWS2_t.Direct_handle_state == HANDLE_SLEEP)
	else 
	{
		sFWS2_t.base.error_time = 0;
		if(sFWS2_t.Direct_handle_error_state  != HANDLE_OVER_CURRENT)
			sFWS2_t.Direct_handle_error_state = HANDLE_OK;
	}
}

void check_current(void)
{
	static uint8_t over_times = 0x00;
	/*over current check*/
	sFWS2_t.base.current_data = get_adcval(ADC_CHANNEL_8);

	if(sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
	{
		if(sFWS2_t.base.current_data > 2200)
		{
			sFWS2_t.Direct_handle_error_state = HANDLE_OVER_CURRENT;
		}
	}
	else if(sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
	{
		if(sFWS2_t.base.current_data > 2300)
		{
			sFWS2_t.Direct_handle_error_state = HANDLE_OVER_CURRENT;
		}
	}
	

}

void check_handle(void)
{
	static bool select_flag = false;
	static uint8_t rpc_times = 0;
	static uint8_t last_position_state = NOT_IN_POSSITION;
	static uint8_t sleep_times = 0, no_sleep_times = 0;
	// check sleep
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_8) == true)
	{
		no_sleep_times++;
		sleep_times = 0;
		if (no_sleep_times > 1)
		{
			no_sleep_times = 0;
			sFWS2_t.Direct_handle_position = NOT_IN_POSSITION;
			if(last_position_state == IN_POSSITION)
			{
				actual_refesh_time = 0;
			}
			last_position_state = sFWS2_t.Direct_handle_position;
		}
	}
	else
	{
		sleep_times++;
		no_sleep_times = 0;
		if (sleep_times > 1)
		{
			sleep_times = 0;
			sFWS2_t.Direct_handle_position = IN_POSSITION;
			last_position_state = sFWS2_t.Direct_handle_position;
		}
	}

	// check type
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_3) == true && gpio_input_data_bit_read(GPIOB, GPIO_PINS_4) == false)
	{
		sFWS2_t.Direct_handle_Heating_stick = HANDLE_245;
	}
	else if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_3) == false && gpio_input_data_bit_read(GPIOB, GPIO_PINS_4) == true)
	{
		sFWS2_t.Direct_handle_Heating_stick = HANDLE_210;
	}
	else 
	{
		sFWS2_t.Direct_handle_Heating_stick = HANDLE_NO;
	}

	// check rpc
	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_6) == false)
	{
		rpc_times++;
		if(rpc_times > 5)
		{
			rpc_times = 0;
			sFWS2_t.Direct_handle_rpc = IN_RPC;
		}
		
	}
	else
	{
		rpc_times = 0;
		sFWS2_t.Direct_handle_rpc = NOT_IN_RPC;
	}
}

void pwm_control(void)
{
	static handle_heating_stick_e last_handle_type = RESET_VALUE;
	static float last_set_temp = 0;
	static float temp = 0;
	static uint16_t hot_temp = 0;
	static uint8_t v_num = 0;
	temp = sFWS2_t.base.actual_temp - sFWS2_t.base.cal_data;

	if (last_set_temp != sFWS2_t.base.set_temp)
	{
		PID_Clear_I(&pid_210);
		PID_Clear_I(&pid_245);
		last_set_temp = sFWS2_t.base.set_temp;
	}

	if (last_handle_type != sFWS2_t.Direct_handle_Heating_stick)
	{
		if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
		{
			PID_Clear_I(&pid_210);
			gpio_bits_reset(GPIOA, GPIO_PINS_2); // ÇÐ»»±¶Êý
		}
		else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
		{
			PID_Clear_I(&pid_245);
			gpio_bits_set(GPIOA, GPIO_PINS_2); // ÇÐ»»±¶Êý
		}
		last_handle_type = sFWS2_t.Direct_handle_Heating_stick;
	}

	/* get temp */
	sFWS2_t.base.actual_temp = move_average_filter(&s1_adc) >> 2;

	/* pid control */
	if (sFWS2_t.Direct_handle_error_state == HANDLE_OK && sFWS2_t.Direct_handle_state == HANDLE_WORKING)
	{
		if (temp <= (sFWS2_t.base.set_temp + 30) &&
			temp >= (sFWS2_t.base.set_temp - 30))
		{
			if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
			{
				sFWS2_t.base.pwm_out = PID_Position_Calc(&pid_245, sFWS2_t.base.set_temp, temp);
			}
			else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
			{
				sFWS2_t.base.pwm_out = PID_Position_Calc(&pid_210, sFWS2_t.base.set_temp, temp);
			}
		}
		else if (temp > (sFWS2_t.base.set_temp + 30))
		{
			// È«¹¦ÂÊ¼õÎÂ
			PID_Clear_I(&pid_210);
			PID_Clear_I(&pid_245);
			sFWS2_t.base.pwm_out = 0;
		}
		else
		{
			PID_Clear_I(&pid_210);
			PID_Clear_I(&pid_245);
			if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
			{
				sFWS2_t.base.pwm_out = 10000;
			}
			else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
			{
				sFWS2_t.base.pwm_out = 3333;
			}
		}
	}
	else if (sFWS2_t.Direct_handle_state == HANDLE_SLEEP)
	{
		sFWS2_t.base.pwm_out = 0;
	}
	else if(sFWS2_t.Direct_handle_state == HANDLE_RPC)
	{
		sFWS2_t.base.pwm_out = 0;
	}
	else if (sFWS2_t.Direct_handle_error_state != HANDLE_OK)
	{
		sFWS2_t.base.pwm_out = 0x00;
	}
}

void sleep_control(void)
{
	static bool wake_up_flag = false;
	static bool first_in = false;
	static uint16_t time_count_ms = 0;
	static uint8_t time_count_s = 0;

	if (sFWS2_t.Direct_handle_position == IN_POSSITION)
	{
		first_in = false;

		if (sFWS2_t.base.sleep_time_count)
		{
			sFWS2_t.Direct_handle_state = HANDLE_WORKING;
			time_count_ms++;

			if (time_count_ms >= 1000)
			{
				
				time_count_ms = 0;
				time_count_s++;

				if (time_count_s >= 60)
				{
					time_count_s = 0;
					sFWS2_t.base.sleep_time_count--;
				}
			}
		}
		else
		{
			if (sFWS2_t.Direct_handle_state == HANDLE_WORKING)
			{
				sFWS2_t.Direct_handle_state = HANDLE_SLEEP;
			}
		}
	}
	else if (sFWS2_t.Direct_handle_position == NOT_IN_POSSITION)
	{
		if (first_in == false)
		{
			first_in = true;
			sFWS2_t.Direct_handle_state = HANDLE_WORKING;
			sFWS2_t.base.sleep_time_count = sFWS2_t.base.set_sleep_time;
			time_count_ms = 0;
			time_count_s = 0;
		}
	}

	/* check base waken time */
	if (sFWS2_t.Direct_handle_state == HANDLE_WAKEN)
	{
		wake_up_flag = true;
	}

	if (wake_up_flag)
	{
		sFWS2_t.base.waken_time_count++;

		if (sFWS2_t.base.waken_time_count >= 3000)
		{
			wake_up_flag = false;
			sFWS2_t.base.waken_time_count = 0;

			if (sFWS2_t.Direct_handle_position == IN_POSSITION)
			{
				sFWS2_t.Direct_handle_state = HANDLE_SLEEP;
			}
			else
			{
				sFWS2_t.Direct_handle_state = HANDLE_WORKING;
			}
		}
	}
}

void rpc_control(void)
{
	if(sFWS2_t.Direct_handle_rpc == IN_RPC)
	{
		sFWS2_t.Direct_handle_state = HANDLE_RPC;
	}
	else if(sFWS2_t.Direct_handle_rpc == NOT_IN_RPC)
	{
		sFWS2_t.Direct_handle_state = HANDLE_WORKING;
	}
}


uint8_t linear_correction(uint16_t user_set_temp)
{
	static uint8_t n = 0;
	static uint16_t check_start = 0;
	static uint8_t check_value = 0;

	if (user_set_temp < 300)
	{
		check_value = 0;
		check_start = 200;

		for (n = 0; n < 10; n++)
		{
			if (user_set_temp <= (check_start + check_value))
			{
				return (off_set_buff[n]);
			}
			else
			{
				check_start += 10;
			}
		}
	}
	else
	{
		check_value = 0;
		check_start = 300;

		for (n = 10; n < 27; n++)
		{
			if (user_set_temp <= (check_start + check_value))
			{
				return (off_set_buff[n]);
			}
			else
			{
				check_start += 10;
			}
		}
	}

	return (off_set_buff[n]);
}
