#include "FWS2pro_handle.h"
#include "PID_operation.h"
#include "beep_handle.h"
#include "key_handle.h"
#include "temp_error_handle.h"
#include "output_handle.h"
#include "adc_filter.h"

void check_hot_error(void)
{
	//	sFWS2_t.base.error_data = move_average_filter(&s1_adc) >> 3;

	if (gpio_input_data_bit_read(GPIOB, GPIO_PINS_4) == 1 &&
		gpio_input_data_bit_read(GPIOB, GPIO_PINS_3) == 1) // 检测是否有手柄 没有报错er4
	{
		sFWS2_t.Direct_handle_Heating_stick = HANDLE_NO;
		sFWS2_t.base.error_time++;
		if (sFWS2_t.base.error_time >= ERROR_TIME_OUT)
		{
			sFWS2_t.Direct_handle_error_state = HANDLE_NO_ERR;
			sFWS2_t.base.error_time = 0;
			sFWS2_t.display_parameter.display_number = Disp_Err;
			sbeep.status = BEEP_LONG;
		}
	}
	else
	{
		if ((sFWS2_t.base.actual_temp <= LOW_TEMP_VAL) ||
			(sFWS2_t.base.actual_temp >= OVER_TEMP_VAL))
		{ // 错误报警
			sFWS2_t.base.error_time++;

			if (sFWS2_t.base.error_time >= ERROR_TIME_OUT / 2)
			{
				if (sFWS2_t.base.actual_temp >= OVER_TEMP_VAL)
				{
					sFWS2_t.Direct_handle_error_state = HANDLE_OVER_TEMP_ERR;
					sFWS2_t.display_parameter.display_number = Disp_Err;
					sFWS2_t.base.error_time = 0;
					sbeep.status = BEEP_LONG;
					sFWS2_t.general_parameter.hot_mode = 0;
				}
			}

			if (sFWS2_t.base.error_time >= ERROR_TIME_OUT)
			{
				if (sFWS2_t.base.actual_temp <= LOW_TEMP_VAL)
				{
					sFWS2_t.Direct_handle_error_state = HANDLE_LOW_TEMP_ERR;
					sFWS2_t.display_parameter.display_number = Disp_Err;
					sFWS2_t.base.error_time = 0;
					sbeep.status = BEEP_LONG;
					sFWS2_t.general_parameter.hot_mode = 0;
				}
				sFWS2_t.base.error_time = 0;
			}
		}
		else{
			sFWS2_t.Direct_handle_error_state = HANDLE_OK;
			sFWS2_t.base.error_time = 0;
//			if (sFWS2_t.general_parameter.key_on_flag == 0)
//				sFWS2_t.display_parameter.display_number = Disp_Real;
		}
	}
}