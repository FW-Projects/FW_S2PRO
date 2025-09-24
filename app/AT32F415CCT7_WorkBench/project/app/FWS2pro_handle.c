#include "FWS2pro_handle.h"
#include <string.h>

FWS2_Handle sFWS2_t;

void FWS2_Init(FWS2_Handle *FWS2)
{
	FWS2->FWS2_STATE = FWS2_INIT,

	/* start the Direct handle init of  */
		FWS2->base.actual_temp = 0,
	FWS2->base.set_temp = 380,
	FWS2->base.set_temp_f_display = 380;
	FWS2->base.set_sleep_time = 0x00,
	FWS2->base.sleep_time_count = 0x00,
	FWS2->base.error_time = 0,
	FWS2->base.cal_data = 0,
	FWS2->base.cal_temp_c_display = 0,
	FWS2->base.cal_temp_f_display = 0,
	FWS2->base.cal_data = 0,
	FWS2->base.run_disp_state = 0;
	FWS2->Direct_handle_state = HANDLE_WAKEN,
	FWS2->Direct_handle_position = IN_POSSITION,
	FWS2->Direct_handle_error_state = HANDLE_OK,
	FWS2->Direct_handle_Heating_stick = HANDLE_NO,			// ·¢ÈÈ°ô

	FWS2->Direct_handle_rpc = NOT_IN_RPC,

	memset(FWS2->base.temp_buff, 185, CURVE_BUFF_SIZE);
	memset(FWS2->base.power_buff, 185, CURVE_BUFF_SIZE);

	/* end the  Direct handle init of*/

	/* start the general init of  */
	FWS2->general_parameter.temp_unit = CELSIUS,
	FWS2->general_parameter.speak_state = SPEAKER_OPEN,
	FWS2->general_parameter.display_lock_state = LOCK,
	FWS2->general_parameter.ota_state = OTA_OFF,
	FWS2->general_parameter.uart_state = UART_CLOSE,
	
	FWS2->general_parameter.last_Heating_stick = 1;

	FWS2->general_parameter.ch = 0,
	FWS2->general_parameter.last_ch = 0,
	FWS2->general_parameter.save_ch = 0,

	/* ch set */
		FWS2->general_parameter.ch1_set_temp = 320,
	FWS2->general_parameter.ch2_set_temp = 350,
	FWS2->general_parameter.ch3_set_temp = 380,

	/* en working set */
		/* end the general init of*/

	FWS2->reset_flag = 0;
	FWS2->work_mode = WORK_NORMAL;
//	FWS2->work_mode = WORK_CURVE;
	FWS2->page = LOGO;
	FWS2->last_page = LOGO;
	FWS2->base.pwm_out = 0x00;
	
	FWS2->init_flag          = false;
}
