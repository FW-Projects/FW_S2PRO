#include "flash_handle.h"
#include "FWS2pro_handle.h"

static void get_data_from_a(void);
static void get_data_from_b(void);
static void check_data_all(void);
static void get_reset_data(void);


uint16_t data_check(uint32_t address)
{
    static uint16_t data;
    data = flash_read_halfword(address);

    if (data == 0xffff)
    {
        return FALSE;
    }
    else
    {
        return data;
    }
}

uint16_t data_check_len(uint32_t address, uint8_t len)
{
    volatile uint16_t data;
    uint8_t i;

    for (i = 0; i < len; i += DATA_SIZE)
    {
        data = flash_read_halfword(address + i);

        if (data == 0xFFFF)
        {
            break;
        }
    }

    return data;
}

void FlashProc(void)
{
#if 1
    static flash_handle_t sflash;
	static bool first_start_flag = FALSE;
	
    static uint16_t last_direct_set_temp = 0;
    static uint16_t last_set_direct_cal_temp = 0;
    static uint8_t last_temp_uint = CELSIUS;
    static uint8_t last_speak_state = SPEAKER_OPEN;
    static uint8_t last_display_lock_state = LOCK;
    static uint8_t last_set_sleep_time = 0;
    static uint16_t last_ch1_set_temp = 0;
    static uint16_t last_ch2_set_temp = 0;
    static uint16_t last_ch3_set_temp = 0;
	static work_mode_e last_show_mode = WORK_NORMAL;
    static uint16_t flash_version = 0;
    static uint8_t flash_count = 0;
    static uint16_t a_ver;
    static uint16_t b_ver;

    switch (sflash.state)
    {
    case FLASH_START:
        if (FALSE == first_start_flag)
        {
            /* 检测主存储区和备份存储区数据 */
            if (data_check_len(A_LAST_DIRECT_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF && data_check_len(B_LAST_DIRECT_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
            {
                /* 检查哪个区的数据最新 */
                a_ver = flash_read_halfword(A_FLASH_VERSION_ADDRESS);
                b_ver = flash_read_halfword(B_FLASH_VERSION_ADDRESS);

                if (a_ver > b_ver)
                {
                   get_data_from_a();
					
                }
                else
                {
                    get_data_from_b();
                }
            }
            /* check area a data  */
            else if (data_check_len(A_LAST_DIRECT_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
            {
               get_data_from_a();
            }
            /* check area b data  */
            else if (data_check_len(B_LAST_DIRECT_SET_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
            {
               get_data_from_b();
            }
            else
            {
                get_reset_data();
            }
			check_data_all();
			
            last_direct_set_temp = sFWS2_t.base.set_temp;
            last_set_direct_cal_temp = sFWS2_t.base.cal_data;
            last_temp_uint = sFWS2_t.general_parameter.temp_unit;
            last_speak_state = sFWS2_t.general_parameter.speak_state;
            last_display_lock_state = sFWS2_t.general_parameter.display_lock_state;
            last_ch1_set_temp = sFWS2_t.general_parameter.ch1_set_temp;
            last_ch2_set_temp = sFWS2_t.general_parameter.ch2_set_temp;
            last_ch3_set_temp = sFWS2_t.general_parameter.ch3_set_temp;
            last_set_sleep_time = sFWS2_t.base.set_sleep_time;
			last_show_mode = sFWS2_t.work_mode;
            first_start_flag = TRUE;
            /* system run */
            sFWS2_t.init_flag = TRUE;
        }
        else
        {
			sflash.state = FLASH_HANDLE_DATA;
			break;
        }

        break;

    case FLASH_HANDLE_DATA:
        if (last_direct_set_temp != sFWS2_t.base.set_temp ||
            last_set_direct_cal_temp != sFWS2_t.base.cal_data ||
            last_temp_uint != sFWS2_t.general_parameter.temp_unit ||
            last_speak_state != sFWS2_t.general_parameter.speak_state ||
            last_display_lock_state != sFWS2_t.general_parameter.display_lock_state ||
            last_set_sleep_time != sFWS2_t.base.set_sleep_time ||
            last_ch1_set_temp != sFWS2_t.general_parameter.ch1_set_temp ||
            last_ch2_set_temp != sFWS2_t.general_parameter.ch2_set_temp ||
            last_ch3_set_temp != sFWS2_t.general_parameter.ch3_set_temp ||
			last_show_mode != sFWS2_t.work_mode)
        {
            flash_unlock();

            if (flash_count % 2 != FALSE)
            {
                flash_sector_erase(A_LAST_DIRECT_SET_TEMP_ADDRESS);
                flash_halfword_program(A_LAST_DIRECT_SET_TEMP_ADDRESS, sFWS2_t.base.set_temp);
				flash_halfword_program(A_LAST_DIRECT_SET_CAL_TEMP, sFWS2_t.base.cal_data);
				flash_halfword_program(A_LAST_TEMP_UINT, sFWS2_t.general_parameter.temp_unit);
				flash_halfword_program(A_LAST_SPEAK_STATE, sFWS2_t.general_parameter.speak_state);
				flash_halfword_program(A_LAST_DISPLAY_LOCK_STATE, sFWS2_t.general_parameter.display_lock_state);
            }
            else
            {
               flash_sector_erase(B_LAST_DIRECT_SET_TEMP_ADDRESS);
                flash_halfword_program(B_LAST_DIRECT_SET_TEMP_ADDRESS, sFWS2_t.base.set_temp);
				flash_halfword_program(B_LAST_DIRECT_SET_CAL_TEMP, sFWS2_t.base.cal_data);
				flash_halfword_program(B_LAST_TEMP_UINT, sFWS2_t.general_parameter.temp_unit);
				flash_halfword_program(B_LAST_SPEAK_STATE, sFWS2_t.general_parameter.speak_state);
				flash_halfword_program(B_LAST_DISPLAY_LOCK_STATE, sFWS2_t.general_parameter.display_lock_state);
            }

            last_direct_set_temp = sFWS2_t.base.set_temp;
			last_set_direct_cal_temp = sFWS2_t.base.cal_data;
			last_temp_uint = sFWS2_t.general_parameter.temp_unit;
			last_speak_state = sFWS2_t.general_parameter.speak_state;
			last_display_lock_state = sFWS2_t.general_parameter.display_lock_state;
            sflash.state++;
            break;
        }

        break;

    case FLASH_GENERAL_DATA:
        if (flash_count % 2 != FALSE)
        {
			flash_halfword_program(A_LAST_CH1_SET_TEMP, sFWS2_t.general_parameter.ch1_set_temp);
			flash_halfword_program(A_LAST_CH2_SET_TEMP, sFWS2_t.general_parameter.ch2_set_temp);
			flash_halfword_program(A_LAST_CH3_SET_TEMP, sFWS2_t.general_parameter.ch3_set_temp);
			flash_halfword_program(A_LAST_SET_SLEEP_TIME, sFWS2_t.base.set_sleep_time);
			flash_halfword_program(A_LAST_SHOW_MODE_ADDRESS, sFWS2_t.work_mode);
        }
        else
        {
            flash_halfword_program(B_LAST_CH1_SET_TEMP, sFWS2_t.general_parameter.ch1_set_temp);
			flash_halfword_program(B_LAST_CH2_SET_TEMP, sFWS2_t.general_parameter.ch2_set_temp);
			flash_halfword_program(B_LAST_CH3_SET_TEMP, sFWS2_t.general_parameter.ch3_set_temp);
			flash_halfword_program(B_LAST_SET_SLEEP_TIME, sFWS2_t.base.set_sleep_time);
			flash_halfword_program(B_LAST_SHOW_MODE_ADDRESS, sFWS2_t.work_mode);
        }
		last_ch1_set_temp = sFWS2_t.general_parameter.ch1_set_temp;
        last_ch2_set_temp = sFWS2_t.general_parameter.ch2_set_temp;
        last_ch3_set_temp = sFWS2_t.general_parameter.ch3_set_temp;
        last_set_sleep_time = sFWS2_t.base.set_sleep_time;
		last_show_mode = sFWS2_t.work_mode;
        sflash.state++;
        break;
    case FLASH_FINSH:
        if (flash_count % 2 != FALSE)
        {
            flash_halfword_program(A_FLASH_VERSION_ADDRESS, flash_version);
        }
        else
        {
            flash_halfword_program(B_FLASH_VERSION_ADDRESS, flash_version);
        }
        flash_lock();
        flash_version++;
        flash_count++;
        sflash.state = FLASH_HANDLE_DATA;
        break;
    }

#endif
}
void get_data_from_a(void)
{
	sFWS2_t.base.set_temp = flash_read_halfword(A_LAST_DIRECT_SET_TEMP_ADDRESS);
	sFWS2_t.base.cal_data = flash_read_halfword(A_LAST_DIRECT_SET_CAL_TEMP);
	sFWS2_t.general_parameter.temp_unit = flash_read_halfword(A_LAST_TEMP_UINT);
	sFWS2_t.general_parameter.speak_state = flash_read_halfword(A_LAST_SPEAK_STATE);
	sFWS2_t.general_parameter.display_lock_state = flash_read_halfword(A_LAST_DISPLAY_LOCK_STATE);
	sFWS2_t.general_parameter.ch1_set_temp = flash_read_halfword(A_LAST_CH1_SET_TEMP);
	sFWS2_t.general_parameter.ch2_set_temp = flash_read_halfword(A_LAST_CH2_SET_TEMP);
	sFWS2_t.general_parameter.ch3_set_temp = flash_read_halfword(A_LAST_CH3_SET_TEMP);
	sFWS2_t.base.set_sleep_time = flash_read_halfword(A_LAST_SET_SLEEP_TIME);
	sFWS2_t.work_mode = flash_read_halfword(A_LAST_SHOW_MODE_ADDRESS);
}

void get_data_from_b(void)
{
	sFWS2_t.base.set_temp = flash_read_halfword(B_LAST_DIRECT_SET_TEMP_ADDRESS);
	sFWS2_t.base.cal_data = flash_read_halfword(B_LAST_DIRECT_SET_CAL_TEMP);
	sFWS2_t.general_parameter.temp_unit = flash_read_halfword(B_LAST_TEMP_UINT);
	sFWS2_t.general_parameter.speak_state = flash_read_halfword(B_LAST_SPEAK_STATE);
	sFWS2_t.general_parameter.display_lock_state = flash_read_halfword(B_LAST_DISPLAY_LOCK_STATE);
	sFWS2_t.general_parameter.ch1_set_temp = flash_read_halfword(B_LAST_CH1_SET_TEMP);
	sFWS2_t.general_parameter.ch2_set_temp = flash_read_halfword(B_LAST_CH2_SET_TEMP);
	sFWS2_t.general_parameter.ch3_set_temp = flash_read_halfword(B_LAST_CH3_SET_TEMP);
	sFWS2_t.base.set_sleep_time = flash_read_halfword(B_LAST_SET_SLEEP_TIME);
	sFWS2_t.work_mode = flash_read_halfword(B_LAST_SHOW_MODE_ADDRESS);
}

static void get_reset_data(void)
{
	sFWS2_t.base.set_temp = 380;
	sFWS2_t.base.cal_data = 0;
	sFWS2_t.general_parameter.temp_unit = CELSIUS;
	sFWS2_t.general_parameter.speak_state = SPEAKER_OPEN;
	sFWS2_t.general_parameter.display_lock_state = LOCK;
	sFWS2_t.general_parameter.ch1_set_temp = 320;
	sFWS2_t.general_parameter.ch2_set_temp = 350;
	sFWS2_t.general_parameter.ch3_set_temp = 380;
	sFWS2_t.base.set_sleep_time = 0x00;
	sFWS2_t.work_mode = WORK_NORMAL;
//	sFWS2_t.work_mode = WORK_CURVE;
	
}

static void check_data_all(void)
{
	if (sFWS2_t.base.set_temp > MAX_SET_TEMP_VAL || sFWS2_t.base.set_temp < MIN_SET_TEMP_VAL)
	{
		sFWS2_t.base.set_temp = 380;
	}

	if (sFWS2_t.general_parameter.temp_unit != CELSIUS && sFWS2_t.general_parameter.temp_unit != FAHRENHEIT)
	{
		sFWS2_t.general_parameter.temp_unit = CELSIUS;
	}

	if (sFWS2_t.general_parameter.speak_state != SPEAKER_OPEN && sFWS2_t.general_parameter.speak_state != SPEAKER_CLOSE)
	{
		sFWS2_t.general_parameter.speak_state = SPEAKER_OPEN;
	}

	if (sFWS2_t.general_parameter.display_lock_state != UNLOCK && sFWS2_t.general_parameter.display_lock_state != LOCK)
	{
		sFWS2_t.general_parameter.display_lock_state = LOCK;
	}

	if (sFWS2_t.base.set_sleep_time > 99 || sFWS2_t.base.set_sleep_time < 0)
	{
		sFWS2_t.base.set_sleep_time = 0x00;
	}

	if (sFWS2_t.general_parameter.ota_state != OTA_OFF && sFWS2_t.general_parameter.ota_state != OTA_ON)
	{
		sFWS2_t.general_parameter.ota_state = OTA_OFF;
	}

	if (sFWS2_t.general_parameter.uart_state != UART_CLOSE && sFWS2_t.general_parameter.uart_state != UART_OPEN)
	{
		sFWS2_t.general_parameter.uart_state = UART_CLOSE;
	}

	if (sFWS2_t.base.cal_data > 100 || sFWS2_t.base.cal_data < -100)
	{
		sFWS2_t.base.cal_data = 0;
	}

	if (sFWS2_t.general_parameter.ch1_set_temp > MAX_SET_TEMP_VAL || sFWS2_t.general_parameter.ch1_set_temp < MIN_SET_TEMP_VAL)
	{
		sFWS2_t.general_parameter.ch1_set_temp = 320;
	}

	if (sFWS2_t.general_parameter.ch2_set_temp > MAX_SET_TEMP_VAL || sFWS2_t.general_parameter.ch2_set_temp < MIN_SET_TEMP_VAL)
	{
		sFWS2_t.general_parameter.ch2_set_temp = 350;
	}

	if (sFWS2_t.general_parameter.ch3_set_temp > MAX_SET_TEMP_VAL || sFWS2_t.general_parameter.ch3_set_temp < MIN_SET_TEMP_VAL)
	{
		sFWS2_t.general_parameter.ch3_set_temp = 380;
	}
	
	if (sFWS2_t.work_mode != WORK_NORMAL && sFWS2_t.work_mode != WORK_CURVE)
    {
        sFWS2_t.work_mode = WORK_NORMAL;
//		sFWS2_t.work_mode = WORK_CURVE;
    }
	
}

