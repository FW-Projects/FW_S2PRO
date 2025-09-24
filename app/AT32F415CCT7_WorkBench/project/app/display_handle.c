#include "display_handle.h"
#include "FWS2pro_handle.h"
#include "perf_counter.h"

/*local  control  */
static int8_t actual_refesh_time = 0;
static uint8_t last_output = 0;
/* in page dual and page single */

static void show_curve(void);

static void show_type(void);
static void show_set_temp(void);
static void show_actual_temp(void);
static void show_power_bar(void);
/* in page set cal and page set sleep */
static void show_cal_temp(void);
static void show_sleep_time(void);

/* in page dual and page single */
static void show_channel_state(void);
static void show_channel_value(void);
/* in all page */
static void show_icon(void);
static void page_switch(void);
static void number_change(void);
void show_navigation_bar(void);
/* icon */
static icon_t C_F_main_symbol;
static icon_t C_F_menu_symbol;
static icon_t ch1_frame;
static icon_t ch2_frame;
static icon_t ch3_frame;
static icon_t beep;
static icon_t run;
static icon_t ring;
static icon_t exit_1;
static icon_t return_1;
static icon_t confirm;
/* number */
static number type_1;
static number set_temp_1;
static number set_temp_2;
static number actual_temp_1;
static number power_data_1;
static number ch1_set_temp_1;
static number ch2_set_temp_1;
static number ch3_set_temp_1;
static number set_temp_3;
static number actual_temp_3;
static number cal_temp_1;
static number cal_set_temp_1;
static number sleep_time_1;
static number type_2;

void display_handle(void)
{
#if 1
    static uint8_t display_run = 0;
    switch (display_run)
    {
    case 0: // ������
        show_type();
        show_set_temp();
        show_actual_temp();
        show_power_bar();
        show_cal_temp();
        show_sleep_time();
        display_run++;
        break;
    case 1:
        show_curve();
        number_change();
        show_channel_state();
        show_channel_value();
        page_switch();
        show_icon();
		show_navigation_bar();
        display_run = 0;
        break;
    }

#endif
}

static void show_curve(void)
{
    static bool first = false;
    static uint16_t power_curve = 0;
	static uint16_t curve_refresh_time = CURVE_REFRESH_TIME;
    curve_refresh_time--;

//     if (!curve_refresh_time)
//     {
		if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
		{
			power_curve = sFWS2_t.base.pwm_out / 33;
		}
		else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
		{
			power_curve = sFWS2_t.base.pwm_out / 100;
		}

		if (sFWS2_t.page == CURVE_PAGE)
		{
			if (first == false)
			{
	            LCD_Fill(33, 38, 238, 185, BLACK);
//				TranferPicturetoTFT_LCD(0, 0, 320, 240, CURVE_PICTURE);
				first = true;
			}
			if(sFWS2_t.Direct_handle_error_state == HANDLE_OK)
				LCD_Show_Curve(32, 40, 237, 185, sFWS2_t.base.actual_temp, power_curve);
			
		}
		else
		{
			first = false;
		}
//	}
}

static void number_change(void)
{
    static bool first_in = false;

    if (first_in == false)
    {
        sFWS2_t.base.actual_temp_f_display = 9 * sFWS2_t.base.actual_temp / 5 + 32;
        sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;

        sFWS2_t.general_parameter.ch1_set_temp_f_display = 9 * sFWS2_t.general_parameter.ch1_set_temp / 5 + 32;
        sFWS2_t.general_parameter.ch2_set_temp_f_display = 9 * sFWS2_t.general_parameter.ch2_set_temp / 5 + 32;
        sFWS2_t.general_parameter.ch3_set_temp_f_display = 9 * sFWS2_t.general_parameter.ch3_set_temp / 5 + 32;

        sFWS2_t.base.sleep_time_count = sFWS2_t.base.set_sleep_time;
        first_in = true;
    }

    /* change show number */
    if (sFWS2_t.general_parameter.last_temp_unit != sFWS2_t.general_parameter.temp_unit)
    {
        if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
        {
            sFWS2_t.base.actual_temp_f_display = 9 * sFWS2_t.base.actual_temp / 5 + 32;
            sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;

            sFWS2_t.general_parameter.ch1_set_temp_f_display = 9 * sFWS2_t.general_parameter.ch1_set_temp / 5 + 32;
            sFWS2_t.general_parameter.ch2_set_temp_f_display = 9 * sFWS2_t.general_parameter.ch2_set_temp / 5 + 32;
            sFWS2_t.general_parameter.ch3_set_temp_f_display = 9 * sFWS2_t.general_parameter.ch3_set_temp / 5 + 32;

            set_temp_1.num = &sFWS2_t.base.set_temp;
            set_temp_2.num = &sFWS2_t.base.set_temp;
            set_temp_3.num = &sFWS2_t.base.set_temp;
            cal_temp_1.num = &sFWS2_t.base.cal_temp_c_display;
            actual_temp_1.num = &sFWS2_t.base.actual_temp;
            actual_temp_3.num = &sFWS2_t.base.actual_temp;

            ch1_set_temp_1.num = &sFWS2_t.general_parameter.ch1_set_temp;
            ch2_set_temp_1.num = &sFWS2_t.general_parameter.ch2_set_temp;
            ch3_set_temp_1.num = &sFWS2_t.general_parameter.ch3_set_temp;
        }
        else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
        {
            set_temp_1.num = &sFWS2_t.base.set_temp_f_display;
            set_temp_2.num = &sFWS2_t.base.set_temp_f_display;
            set_temp_3.num = &sFWS2_t.base.set_temp_f_display;
            cal_temp_1.num = &sFWS2_t.base.cal_temp_f_display;
            actual_temp_1.num = &sFWS2_t.base.actual_temp_f_display;
            actual_temp_3.num = &sFWS2_t.base.actual_temp_f_display;
            ch1_set_temp_1.num = &sFWS2_t.general_parameter.ch1_set_temp;
            ch2_set_temp_1.num = &sFWS2_t.general_parameter.ch2_set_temp;
            ch3_set_temp_1.num = &sFWS2_t.general_parameter.ch3_set_temp;
        }
    }
}

static void page_switch(void)
{
    static bool first_in = false;
    static uint16_t show_reset_time = 800;

    if (sFWS2_t.reset_flag == true)
    {
        if (show_reset_time == 800)
        {
            sFWS2_t.page = SET_RESET_DONE_PAGE;
        }

        show_reset_time--;

        if (show_reset_time <= 0)
        {
            show_reset_time = 800;
            sFWS2_t.reset_flag = false;
            sFWS2_t.page = SELECT_SET_RESET_PAGE;
        }
    }

    if (first_in == false)
    {
        if (sFWS2_t.work_mode == WORK_NORMAL)
        {
            if (sFWS2_t.Direct_handle_position == IN_POSSITION || sFWS2_t.Direct_handle_position == NOT_IN_POSSITION)
            {
                if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
                    sFWS2_t.page = WORK_PAGE_210;
                else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
                    sFWS2_t.page = WORK_PAGE_245;
                else
                    sFWS2_t.page = WORK_PAGE_210;
            }
        }
        else if (sFWS2_t.work_mode == WORK_CURVE)
        {
            sFWS2_t.page = CURVE_PAGE;
        }

        /* refresh navigation bar */
        //        TranferPicturetoTFT_LCD(0, 0, 320, 26, NAVIGATION_BAR);
        first_in = true;
    }

    if (sFWS2_t.work_mode == WORK_NORMAL)
    {
        if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
        {
            if ((sFWS2_t.Direct_handle_position == IN_POSSITION) ||
                (sFWS2_t.Direct_handle_position == NOT_IN_POSSITION))
            {
                if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
                    sFWS2_t.page = WORK_PAGE_210;
                else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
                    sFWS2_t.page = WORK_PAGE_245;
                else
                    sFWS2_t.page = WORK_PAGE_210;
            }
        }
    }
    else
    {
        // sFWS2_t.page = CURVE_PAGE;
    }

    if (sFWS2_t.last_page != sFWS2_t.page)
    {
        /* refresh base set temp */
        sFWS2_t.base.last_set_temp = RESET_VALUE;
        /* refresh base actual temp */
        sFWS2_t.base.last_actual_temp = RESET_VALUE;
        /* refresh base cal temp */
        sFWS2_t.base.last_cal_temp_c_display = RESET_VALUE;
        sFWS2_t.base.last_cal_temp_f_display = RESET_VALUE;
        /* refresh base sleep time */
        sFWS2_t.base.last_set_sleep_time = RESET_VALUE;

        /* refresh base ch1 set value */
        sFWS2_t.general_parameter.last_ch1_set_temp = RESET_VALUE;
        sFWS2_t.general_parameter.last_ch1_set_temp_f_display = RESET_VALUE;
        /* refresh base ch2 set value */
        sFWS2_t.general_parameter.last_ch2_set_temp = RESET_VALUE;
        sFWS2_t.general_parameter.last_ch2_set_temp_f_display = RESET_VALUE;
        /* refresh base ch3 set value */
        sFWS2_t.general_parameter.last_ch3_set_temp = RESET_VALUE;
        sFWS2_t.general_parameter.last_ch3_set_temp_f_display = RESET_VALUE;

        /* refresh base channel */
        sFWS2_t.general_parameter.last_ch = RESET_VALUE;
        /* refresh s2 channel */
        sFWS2_t.general_parameter.last_ch = RESET_VALUE;
        /* refresh beep icon */
//		sFWS2_t.general_parameter.last_speak_state = RESET_VALUE;
	/* refresh unit icon */
        sFWS2_t.general_parameter.last_temp_unit = RESET_VALUE;
        /*refresh handle work mode */
        sFWS2_t.last_work_mode = RESET_VALUE;
        /*refresh handle temo lock mode */
        sFWS2_t.general_parameter.last_display_lock_state = RESET_VALUE;
        sFWS2_t.Direct_last_handle_Heating_stick = RESET_VALUE;
        sFWS2_t.last_Direct_handle_state = RESET_VALUE;
        /* refresh page */
        TranferPicturetoTFT_LCD(0, 26, 320, 214, sFWS2_t.page);
        sFWS2_t.last_page = sFWS2_t.page;
		actual_refesh_time = 0;
		last_output = RESET_VALUE;
    }
}

static void show_type(void)
{
    if (sFWS2_t.Direct_last_handle_Heating_stick != sFWS2_t.Direct_handle_Heating_stick &&
        (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245))
    {
        if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
        {
            LCD_ShowString(type_1.x, type_1.y,
                           (uint8_t *)"S210", type_1.fc, type_1.bc,
                           type_1.sizey, type_1.mode);
            sFWS2_t.Direct_last_handle_Heating_stick = sFWS2_t.Direct_handle_Heating_stick;
        }
        else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
        {
            LCD_ShowString(type_1.x, type_1.y,
                           (uint8_t *)"S245", type_1.fc, type_1.bc,
                           type_1.sizey, type_1.mode);
            sFWS2_t.Direct_last_handle_Heating_stick = sFWS2_t.Direct_handle_Heating_stick;
        }
    }
}

static void show_set_temp(void)
{
    static uint8_t state = 0;
    static uint16_t time = SET_TEMP_TIME;
    static bool show_flage = false;
	static bool show_flag = false;
    switch (state)
    {
    case 0:
        if (sFWS2_t.Direct_handle_state == HANDLE_SLEEP && (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245))
        {
            state = 0;
            sFWS2_t.base.last_set_temp = sFWS2_t.base.set_temp;
            sFWS2_t.base.last_set_temp_f_display = sFWS2_t.base.set_temp_f_display;
            break;
        }

        if ((sFWS2_t.base.last_set_temp != sFWS2_t.base.set_temp) ||
            (sFWS2_t.base.last_set_temp_f_display != sFWS2_t.base.set_temp_f_display) ||
			sFWS2_t.last_Direct_handle_state != sFWS2_t.Direct_handle_state)
        {
            time = SET_TEMP_TIME;
            state++;
            break;
        }

        break;

    case 1:
        if (sFWS2_t.Direct_handle_state == HANDLE_SLEEP && (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245))
        {
            state = 0;
            sFWS2_t.base.last_set_temp = sFWS2_t.base.set_temp;
            sFWS2_t.base.last_set_temp_f_display = sFWS2_t.base.set_temp_f_display;
        }
        /* show set temp in page */
        else if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
        {
            if (show_flag == false)
            {
                show_flag = true;
                LCD_ShowIntNum(set_temp_1.x, set_temp_1.y, *set_temp_1.num,
                               set_temp_1.len, GREEN, set_temp_1.bc, set_temp_1.sizey);
                //				LCD_ShowIntNum(set_temp_2.x, set_temp_2.y, *set_temp_2.num,
                //						  set_temp_2.len, GREEN, set_temp_2.bc, set_temp_2.sizey);
                sFWS2_t.base.last_set_temp = sFWS2_t.base.set_temp;
                sFWS2_t.base.last_set_temp_f_display = sFWS2_t.base.set_temp_f_display;
            }
        }
        /* show base set temp in page cal */
        else if (sFWS2_t.page == SET_TEMP_CAL_PAGE)
        {
            if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
                LCD_ShowIntNum(cal_set_temp_1.x, cal_set_temp_1.y, sFWS2_t.base.set_temp_f_display,
                               cal_set_temp_1.len, WHITE, cal_set_temp_1.bc, cal_set_temp_1.sizey);
            else if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
                LCD_ShowIntNum(cal_set_temp_1.x, cal_set_temp_1.y, sFWS2_t.base.set_temp,
                               cal_set_temp_1.len, WHITE, cal_set_temp_1.bc, cal_set_temp_1.sizey);
        }
        else if (sFWS2_t.page == CURVE_PAGE)
        {
            if (show_flag == false)
            {
                show_flag = true;
                LCD_ShowIntNum(set_temp_3.x, set_temp_3.y, *set_temp_3.num,
                               set_temp_3.len, GREEN, set_temp_3.bc, set_temp_3.sizey);
                sFWS2_t.base.last_set_temp = sFWS2_t.base.set_temp;
                sFWS2_t.base.last_set_temp_f_display = sFWS2_t.base.set_temp_f_display;
            }
        }

        time--;

        if ((sFWS2_t.base.last_set_temp != sFWS2_t.base.set_temp) ||
            (sFWS2_t.base.last_set_temp_f_display != sFWS2_t.base.set_temp_f_display))
        {
            if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
            {
                sFWS2_t.base.set_temp_f_display = 9 * sFWS2_t.base.set_temp / 5 + 32;
            }

            show_flag = false;
            time = SET_TEMP_TIME;
            break;
        }

        if (time <= 0)
        {
            if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
            {
                LCD_ShowIntNum(set_temp_1.x, set_temp_1.y, *set_temp_1.num,
                               set_temp_1.len, GREEN, set_temp_1.bc, set_temp_1.sizey);
                //				LCD_ShowIntNum(set_temp_2.x, set_temp_2.y, *set_temp_2.num,
                //						  set_temp_2.len, GREEN, set_temp_2.bc, set_temp_2.sizey);
            }
            else if (sFWS2_t.page == CURVE_PAGE)
            {
                LCD_ShowIntNum(set_temp_3.x, set_temp_3.y, *set_temp_3.num,
                               set_temp_3.len, GREEN, set_temp_3.bc, set_temp_3.sizey);
            }

            show_flag = false;
            state = 0;
            break;
        }

        break;
    }
}

static void show_cal_temp(void)
{
    if (sFWS2_t.page != SET_TEMP_CAL_PAGE)
    {
        sFWS2_t.base.cal_temp_c_display = sFWS2_t.base.set_temp;
        sFWS2_t.base.cal_temp_f_display = sFWS2_t.base.set_temp_f_display;
    }

    if (sFWS2_t.base.last_cal_temp_c_display != sFWS2_t.base.cal_temp_c_display ||
        sFWS2_t.base.last_cal_temp_f_display != sFWS2_t.base.cal_temp_f_display)
    {
        if (sFWS2_t.page == SET_TEMP_CAL_PAGE)
        {
            if (sFWS2_t.set_cal == SET_CAL)
            {
                LCD_ShowIntNum(cal_temp_1.x, cal_temp_1.y, *cal_temp_1.num,
                               cal_temp_1.len, GREEN, cal_temp_1.bc, cal_temp_1.sizey);
            }
            else if (sFWS2_t.set_cal == SET_RETURN_CAL)
            {
                LCD_ShowIntNum(cal_temp_1.x, cal_temp_1.y, *cal_temp_1.num,
                               cal_temp_1.len, WHITE, cal_temp_1.bc, cal_temp_1.sizey);
            }
            sFWS2_t.base.last_cal_temp_c_display = sFWS2_t.base.cal_temp_c_display;
            sFWS2_t.base.last_cal_temp_f_display = sFWS2_t.base.cal_temp_f_display;
        }
    }
}

static void show_actual_temp(void)
{
    static uint8_t cal = 0;
    actual_refesh_time--;

    if (sFWS2_t.Direct_handle_state == HANDLE_SLEEP && sFWS2_t.page != CURVE_PAGE)
    {
        sFWS2_t.base.last_actual_temp = sFWS2_t.base.actual_temp;
        sFWS2_t.base.last_actual_temp_f_display = sFWS2_t.base.actual_temp_f_display;
    }


    if (actual_refesh_time <= 0)
    {
        if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
        {
            sFWS2_t.base.actual_temp_f_display = 9 * sFWS2_t.base.actual_temp / 5 + 32;

            if (sFWS2_t.base.cal_data != 0)
            {
                cal = 9 * sFWS2_t.base.cal_data / 5 + 32;
            }
        }
        else
        {
            cal = -sFWS2_t.base.cal_data;
        }

		 if (sFWS2_t.page == CURVE_PAGE || sFWS2_t.page == LOGO)
			actual_refesh_time = 10;
		 else	 
			actual_refesh_time = ACTUAL_TEMP_REFRESH_TIME;

        if ((sFWS2_t.base.last_actual_temp != sFWS2_t.base.actual_temp) ||
            (sFWS2_t.base.last_actual_temp_f_display != sFWS2_t.base.actual_temp_f_display))
        {
            sFWS2_t.base.last_actual_temp = sFWS2_t.base.actual_temp;
            sFWS2_t.base.last_actual_temp_f_display = sFWS2_t.base.actual_temp_f_display;

            /* show base actual temp in page single */
            if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
            {
                if (sFWS2_t.Direct_handle_position == IN_POSSITION)
                {
                    LCD_ShowIntNum(actual_temp_1.x, actual_temp_1.y, *actual_temp_1.num,
                                   actual_temp_1.len, actual_temp_1.fc, actual_temp_1.bc, actual_temp_1.sizey);
                }
                else
                {
                    if (sFWS2_t.general_parameter.display_lock_state == LOCK)
                    {
                        if ((sFWS2_t.base.actual_temp - sFWS2_t.base.cal_data) < (sFWS2_t.base.set_temp + LOCK_RANGE) &&
                            (sFWS2_t.base.actual_temp - sFWS2_t.base.cal_data) > (sFWS2_t.base.set_temp - LOCK_RANGE))
                        {
                            LCD_ShowIntNum(actual_temp_1.x, actual_temp_1.y, *set_temp_1.num,
                                           actual_temp_1.len, actual_temp_1.fc, actual_temp_1.bc, actual_temp_1.sizey);
                        }
                        else
                        {
                            LCD_ShowIntNum(actual_temp_1.x, actual_temp_1.y, *actual_temp_1.num + cal,
                                           actual_temp_1.len, actual_temp_1.fc, actual_temp_1.bc, actual_temp_1.sizey);
                        }
                    }
                    else if (sFWS2_t.general_parameter.display_lock_state == UNLOCK)
                    {
                        LCD_ShowIntNum(actual_temp_1.x, actual_temp_1.y, *actual_temp_1.num + cal,
                                       actual_temp_1.len, actual_temp_1.fc, actual_temp_1.bc, actual_temp_1.sizey);
                    }
                }
            }
            /* show base actual temp in page curve */
            else if (sFWS2_t.page == CURVE_PAGE)
            {

                if (sFWS2_t.Direct_handle_position == IN_POSSITION)
                {
                    LCD_ShowIntNum(actual_temp_3.x, actual_temp_3.y, *actual_temp_3.num,
                                   actual_temp_3.len, actual_temp_3.fc, actual_temp_3.bc, actual_temp_3.sizey);
                }
                else
                {
                    if (sFWS2_t.general_parameter.display_lock_state == LOCK)
                    {
                        if ((sFWS2_t.base.actual_temp - sFWS2_t.base.cal_data) < (sFWS2_t.base.set_temp + LOCK_RANGE) &&
                            (sFWS2_t.base.actual_temp - sFWS2_t.base.cal_data) > (sFWS2_t.base.set_temp - LOCK_RANGE))
                        {
                            LCD_ShowIntNum(actual_temp_3.x, actual_temp_3.y, *actual_temp_3.num + cal,
                                           actual_temp_3.len, actual_temp_3.fc, actual_temp_3.bc,
                                           actual_temp_3.sizey);
                        }
                        else
                        {
                            LCD_ShowIntNum(actual_temp_3.x, actual_temp_3.y, *actual_temp_3.num + cal,
                                           actual_temp_3.len, actual_temp_3.fc, actual_temp_3.bc,
                                           actual_temp_3.sizey);
                        }
                    }
                    else if (sFWS2_t.general_parameter.display_lock_state == UNLOCK)
                    {
                        LCD_ShowIntNum(actual_temp_3.x, actual_temp_3.y, *actual_temp_3.num + cal,
                                       actual_temp_3.len, actual_temp_3.fc, actual_temp_3.bc,
                                       actual_temp_3.sizey);
                    }
                }
            }
        }
    }
}

static void show_power_bar(void)
{
    static float output = 0;
    
    static uint8_t refesh_time = POWER_BAR_REFRESH_TIME;
    refesh_time--;

    if (refesh_time <= 0)
    {
        refesh_time = POWER_BAR_REFRESH_TIME;
        if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_210)
        {
            output = sFWS2_t.base.pwm_out / 33.33;
        }
        else if (sFWS2_t.Direct_handle_Heating_stick == HANDLE_245)
        {
            output = sFWS2_t.base.pwm_out / 100;
        }
        if (output <= 0)
        {
            output = 0;
        }

	   if (last_output != output)
	   {
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
			{
				DrawProgressBar(24, 40,
                                35, 180,
                                output, 0x3d9f, 0x20e4, SLIDER_ICON);
				LCD_ShowIntNum(power_data_1.x, power_data_1.y, output,
							   power_data_1.len, power_data_1.fc, power_data_1.bc, power_data_1.sizey);
			}

			last_output = output;
        }
    }
}

static void show_sleep_time(void)
{
    if (sFWS2_t.base.last_set_sleep_time != sFWS2_t.base.set_sleep_time)
    {
        if (sFWS2_t.page == SET_SLEEP_PAGE)
        {
            LCD_ShowIntNum(sleep_time_1.x, sleep_time_1.y, *sleep_time_1.num,
                           sleep_time_1.len, GREEN, sleep_time_1.bc,
                           sleep_time_1.sizey);
            sFWS2_t.base.last_set_sleep_time = sFWS2_t.base.set_sleep_time;
        }
    }
}

static void show_channel_state(void)
{
    static uint8_t clean_state = false;

    if ((sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE))
    {
        if (sFWS2_t.general_parameter.last_ch != sFWS2_t.general_parameter.ch)
        {
            if (sFWS2_t.general_parameter.last_ch == 1)
            {
                TranferPicturetoTFT_LCD(ch1_frame.x1, ch1_frame.y1, ch1_frame.length, ch1_frame.winth, UNSELECT_CH);
            }
            else if (sFWS2_t.general_parameter.last_ch == 2)
            {
                TranferPicturetoTFT_LCD(ch2_frame.x1, ch2_frame.y1, ch2_frame.length, ch2_frame.winth, UNSELECT_CH);
            }
            else if (sFWS2_t.general_parameter.last_ch == 3)
            {
                TranferPicturetoTFT_LCD(ch3_frame.x1, ch3_frame.y1, ch3_frame.length, ch3_frame.winth, UNSELECT_CH);
            }
            else
            {
                TranferPicturetoTFT_LCD(ch1_frame.x1, ch1_frame.y1, ch1_frame.length, ch1_frame.winth, UNSELECT_CH);
                TranferPicturetoTFT_LCD(ch2_frame.x1, ch2_frame.y1, ch2_frame.length, ch2_frame.winth, UNSELECT_CH);
                TranferPicturetoTFT_LCD(ch3_frame.x1, ch3_frame.y1, ch3_frame.length, ch3_frame.winth, UNSELECT_CH);
            }

            if (sFWS2_t.general_parameter.ch == 1)
            {
                TranferPicturetoTFT_LCD(ch1_frame.x1, ch1_frame.y1, ch1_frame.length, ch1_frame.winth, SELECT_CH);
            }
            else if (sFWS2_t.general_parameter.ch == 2)
            {
                TranferPicturetoTFT_LCD(ch2_frame.x1, ch2_frame.y1, ch2_frame.length, ch2_frame.winth, SELECT_CH);
            }
            else if (sFWS2_t.general_parameter.ch == 3)
            {
                TranferPicturetoTFT_LCD(ch3_frame.x1, ch3_frame.y1, ch3_frame.length, ch3_frame.winth, SELECT_CH);
            }

            clean_state = false;
            sFWS2_t.general_parameter.last_ch1_set_temp = RESET_VALUE;
            sFWS2_t.general_parameter.last_ch2_set_temp = RESET_VALUE;
            sFWS2_t.general_parameter.last_ch3_set_temp = RESET_VALUE;
            sFWS2_t.general_parameter.last_ch1_set_temp_f_display = RESET_VALUE;
            sFWS2_t.general_parameter.last_ch2_set_temp_f_display = RESET_VALUE;
            sFWS2_t.general_parameter.last_ch3_set_temp_f_display = RESET_VALUE;
            sFWS2_t.general_parameter.last_ch = sFWS2_t.general_parameter.ch;
        }

        if (sFWS2_t.general_parameter.ch != 0)
        {
            if ((((uint16_t)sFWS2_t.base.set_temp != (uint16_t)sFWS2_t.general_parameter.ch1_set_temp &&
                  (uint16_t)sFWS2_t.base.set_temp != (uint16_t)sFWS2_t.general_parameter.ch2_set_temp &&
                  (uint16_t)sFWS2_t.base.set_temp != (uint16_t)sFWS2_t.general_parameter.ch3_set_temp) ||
                 ((uint16_t)sFWS2_t.base.set_temp_f_display != (uint16_t)sFWS2_t.general_parameter.ch1_set_temp_f_display &&
                  (uint16_t)sFWS2_t.base.set_temp_f_display != (uint16_t)sFWS2_t.general_parameter.ch2_set_temp_f_display &&
                  (uint16_t)sFWS2_t.base.set_temp_f_display != (uint16_t)sFWS2_t.general_parameter.ch3_set_temp_f_display)) &&
                clean_state == false)
            {
                sFWS2_t.general_parameter.last_ch1_set_temp = RESET_VALUE;
                sFWS2_t.general_parameter.last_ch2_set_temp = RESET_VALUE;
                sFWS2_t.general_parameter.last_ch3_set_temp = RESET_VALUE;
                sFWS2_t.general_parameter.last_ch1_set_temp_f_display = RESET_VALUE;
                sFWS2_t.general_parameter.last_ch2_set_temp_f_display = RESET_VALUE;
                sFWS2_t.general_parameter.last_ch3_set_temp_f_display = RESET_VALUE;
                sFWS2_t.general_parameter.ch = 0;
                clean_state = true;
            }
        }
    }
}

static void show_channel_value(void)
{
    if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE)
    {
        if ((sFWS2_t.general_parameter.last_ch1_set_temp != sFWS2_t.general_parameter.ch1_set_temp) ||
            ((sFWS2_t.general_parameter.last_ch1_set_temp_f_display != sFWS2_t.general_parameter.ch1_set_temp_f_display)))
        {
            LCD_ShowIntNum(ch1_set_temp_1.x, ch1_set_temp_1.y, *ch1_set_temp_1.num,
                           ch1_set_temp_1.len, ch1_set_temp_1.fc, ch1_set_temp_1.bc, ch1_set_temp_1.sizey);
            sFWS2_t.general_parameter.last_ch1_set_temp = sFWS2_t.general_parameter.ch1_set_temp;
            sFWS2_t.general_parameter.last_ch1_set_temp_f_display = sFWS2_t.general_parameter.ch1_set_temp_f_display;
        }

        if ((sFWS2_t.general_parameter.last_ch2_set_temp != sFWS2_t.general_parameter.ch2_set_temp) ||
            ((sFWS2_t.general_parameter.last_ch2_set_temp_f_display != sFWS2_t.general_parameter.ch2_set_temp_f_display)))
        {
            LCD_ShowIntNum(ch2_set_temp_1.x, ch2_set_temp_1.y, *ch2_set_temp_1.num,
                           ch2_set_temp_1.len, ch2_set_temp_1.fc, ch2_set_temp_1.bc, ch2_set_temp_1.sizey);
            sFWS2_t.general_parameter.last_ch2_set_temp = sFWS2_t.general_parameter.ch2_set_temp;
            sFWS2_t.general_parameter.last_ch2_set_temp_f_display = sFWS2_t.general_parameter.ch2_set_temp_f_display;
        }

        if ((sFWS2_t.general_parameter.last_ch3_set_temp != sFWS2_t.general_parameter.ch3_set_temp) ||
            ((sFWS2_t.general_parameter.last_ch3_set_temp_f_display != sFWS2_t.general_parameter.ch3_set_temp_f_display)))
        {
            LCD_ShowIntNum(ch3_set_temp_1.x, ch3_set_temp_1.y, *ch3_set_temp_1.num,
                           ch3_set_temp_1.len, ch3_set_temp_1.fc, ch3_set_temp_1.bc, ch3_set_temp_1.sizey);
            sFWS2_t.general_parameter.last_ch3_set_temp = sFWS2_t.general_parameter.ch3_set_temp;
            sFWS2_t.general_parameter.last_ch3_set_temp_f_display = sFWS2_t.general_parameter.ch3_set_temp_f_display;
        }
    }
}

static void show_icon(void)
{
    static handle_error_state_e s1_last_handle_error_state = HANDLE_OK;
    static handle_error_state_e s2_last_handle_error_state = HANDLE_OK;

    

    /* set work mode page */
    if (sFWS2_t.last_work_mode != sFWS2_t.work_mode)
    {
        if (sFWS2_t.page == SET_WORK_PAGE)
        {
            if (sFWS2_t.set_work_mode == WORK_NORMAL)
            {
                TranferPicturetoTFT_LCD(ring.x1, ring.y1, ring.length, ring.winth, RING_ICON_PICTURE);
                TranferPicturetoTFT_LCD(ring.x1, ring.y1 + 32, ring.length, ring.winth, UN_RING_ICON_PICTURE);
            }
            else if (sFWS2_t.set_work_mode == WORK_CURVE)
            {
                TranferPicturetoTFT_LCD(ring.x1, ring.y1, ring.length, ring.winth, UN_RING_ICON_PICTURE);
                TranferPicturetoTFT_LCD(ring.x1, ring.y1 + 32, ring.length, ring.winth, RING_ICON_PICTURE);
            }
            sFWS2_t.last_work_mode = sFWS2_t.work_mode;
        }
    }

    /* set temp unit page */
    if (sFWS2_t.general_parameter.last_temp_unit != sFWS2_t.general_parameter.temp_unit)
    {
        if (sFWS2_t.page == SET_UNIT_PAGE)
        {
            if (sFWS2_t.set_temp_unit == FAHRENHEIT)
            {
                TranferPicturetoTFT_LCD(ring.x1, ring.y1, ring.length, ring.winth, RING_ICON_PICTURE);
                TranferPicturetoTFT_LCD(ring.x1, ring.y1 + 32, ring.length, ring.winth, UN_RING_ICON_PICTURE);
            }
            else if (sFWS2_t.set_temp_unit == CELSIUS)
            {
                TranferPicturetoTFT_LCD(ring.x1, ring.y1, ring.length, ring.winth, UN_RING_ICON_PICTURE);
                TranferPicturetoTFT_LCD(ring.x1, ring.y1 + 32, ring.length, ring.winth, RING_ICON_PICTURE);
            }
        }

        /* show unit icon */
        if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
        {
            if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
            {
                TranferPicturetoTFT_LCD(C_F_main_symbol.x1, C_F_main_symbol.y1, C_F_main_symbol.length, C_F_main_symbol.winth, MAIN_CELSIUS_ICON_PICTURE);
            }
            else if (sFWS2_t.page == SET_TEMP_CAL_PAGE)
            {
                TranferPicturetoTFT_LCD(C_F_menu_symbol.x1, C_F_menu_symbol.y1, C_F_menu_symbol.length, C_F_menu_symbol.winth, MENU_CELSIUS_ICON_PICTURE);
                TranferPicturetoTFT_LCD(C_F_menu_symbol.x1, C_F_menu_symbol.y1 + 39, C_F_menu_symbol.length, C_F_menu_symbol.winth, MENU_CELSIUS_ICON_PICTURE);
            }
        }
        else if (sFWS2_t.general_parameter.temp_unit == FAHRENHEIT)
        {
            if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
            {
                TranferPicturetoTFT_LCD(C_F_main_symbol.x1, C_F_main_symbol.y1, C_F_main_symbol.length, C_F_main_symbol.winth, MAIN_FAHRENHEIT_ICON_PICTURE);
            }
            else if (sFWS2_t.page == SET_TEMP_CAL_PAGE)
            {
                TranferPicturetoTFT_LCD(C_F_menu_symbol.x1, C_F_menu_symbol.y1, C_F_menu_symbol.length, C_F_menu_symbol.winth, MENU_FAHRENHEIT_ICON_PICTURE);
                TranferPicturetoTFT_LCD(C_F_menu_symbol.x1, C_F_menu_symbol.y1 + 39, C_F_menu_symbol.length, C_F_menu_symbol.winth, MENU_FAHRENHEIT_ICON_PICTURE);
            }
        }
        sFWS2_t.general_parameter.last_temp_unit = sFWS2_t.general_parameter.temp_unit;
    }

    /* set speak page */
//    if (sFWS2_t.general_parameter.last_speak_state != sFWS2_t.general_parameter.speak_state)
//    {
        if (sFWS2_t.page == SET_SPEAK_PAGE)
        {
            if (sFWS2_t.set_speak == SPEAKER_OPEN)
            {
                TranferPicturetoTFT_LCD(ring.x1, ring.y1, ring.length, ring.winth, RING_ICON_PICTURE);
                TranferPicturetoTFT_LCD(ring.x1, ring.y1 + 32, ring.length, ring.winth, UN_RING_ICON_PICTURE);
            }
            else if (sFWS2_t.set_speak == SPEAKER_CLOSE)
            {
                TranferPicturetoTFT_LCD(ring.x1, ring.y1, ring.length, ring.winth, UN_RING_ICON_PICTURE);
                TranferPicturetoTFT_LCD(ring.x1, ring.y1 + 32, ring.length, ring.winth, RING_ICON_PICTURE);
            }
//            sFWS2_t.general_parameter.last_speak_state = sFWS2_t.general_parameter.speak_state;
        }
//    }

    /* set temp lock page */
    if (sFWS2_t.general_parameter.last_display_lock_state != sFWS2_t.general_parameter.display_lock_state)
    {
        if (sFWS2_t.page == SET_TEMP_LOCK_PAGE)
        {
            if (sFWS2_t.set_lock == LOCK)
            {
                TranferPicturetoTFT_LCD(ring.x1, ring.y1, ring.length, ring.winth, RING_ICON_PICTURE);
                TranferPicturetoTFT_LCD(ring.x1, ring.y1 + 32, ring.length, ring.winth, UN_RING_ICON_PICTURE);
            }
            else if (sFWS2_t.set_lock == UNLOCK)
            {
                TranferPicturetoTFT_LCD(ring.x1, ring.y1, ring.length, ring.winth, UN_RING_ICON_PICTURE);
                TranferPicturetoTFT_LCD(ring.x1, ring.y1 + 32, ring.length, ring.winth, RING_ICON_PICTURE);
            }
            sFWS2_t.general_parameter.last_display_lock_state = sFWS2_t.general_parameter.display_lock_state;
        }
    }
    /* show exit icon */
    if (sFWS2_t.page == SELECT_SET_OTA_PICTURE)
    {
        TranferPicturetoTFT_LCD(exit_1.x1, exit_1.y1, exit_1.length, exit_1.winth, UN_EXIT_ICON_PICTURE);
    }
    else if (sFWS2_t.page == SELECT_EXIT_MENU_PAGE)
    {
        TranferPicturetoTFT_LCD(exit_1.x1, exit_1.y1, exit_1.length, exit_1.winth, EXIT_ICON_PICTURE);
    }

    /* show return icon */
    if (sFWS2_t.page == SET_WORK_PAGE ||
        sFWS2_t.page == SET_UNIT_PAGE ||
        sFWS2_t.page == SET_SPEAK_PAGE ||
        sFWS2_t.page == SET_TEMP_LOCK_PAGE ||
        sFWS2_t.page == SET_RESET_DONE_PAGE)
    {
        TranferPicturetoTFT_LCD(return_1.x1, return_1.y1, return_1.length, return_1.winth, UN_RETURN_ICON_PICTURE); // UN_RETURN_ICON_PICTURE
    }
    if (sFWS2_t.page == SET_TEMP_CAL_PAGE)
    {
        if (sFWS2_t.set_cal == SET_RETURN_CAL)
        {
            TranferPicturetoTFT_LCD(return_1.x1, return_1.y1, return_1.length, return_1.winth, RETURN_ICON_PICTURE);
        }
        else if (sFWS2_t.set_cal == SET_CAL)
        {
            TranferPicturetoTFT_LCD(return_1.x1, return_1.y1, return_1.length, return_1.winth, UN_RETURN_ICON_PICTURE);
        }
    }
    if (sFWS2_t.page == SET_SLEEP_PAGE)
    {
        if (sFWS2_t.set_sleep == SET_RETURN_SLEEP)
        {
            TranferPicturetoTFT_LCD(return_1.x1, return_1.y1, return_1.length, return_1.winth, RETURN_ICON_PICTURE);
        }
        else if (sFWS2_t.set_sleep == SET_SLEEP)
        {
            TranferPicturetoTFT_LCD(return_1.x1, return_1.y1, return_1.length, return_1.winth, UN_RETURN_ICON_PICTURE);
        }
    }
    if (sFWS2_t.page == SET_RESET_PAGE)
    {
        if (sFWS2_t.set_reset1 == RESET_RETURN)
        {
            TranferPicturetoTFT_LCD(return_1.x1, return_1.y1, return_1.length, return_1.winth, RETURN_ICON_PICTURE);
            TranferPicturetoTFT_LCD(confirm.x1, confirm.y1, confirm.length, confirm.winth, UN_CONFIRM_ICON_PICTURE);
        }
        else if (sFWS2_t.set_reset1 == RESET_CONFIRN)
        {
            TranferPicturetoTFT_LCD(return_1.x1, return_1.y1, return_1.length, return_1.winth, UN_RETURN_ICON_PICTURE);
            TranferPicturetoTFT_LCD(confirm.x1, confirm.y1, confirm.length, confirm.winth, CONFIRM_ICON_PICTURE);
        }
    }
    if (sFWS2_t.page == SET_RESET_DONE_PAGE)
    {
        TranferPicturetoTFT_LCD(confirm.x1, confirm.y1, confirm.length, confirm.winth, CONFIRM_ICON_PICTURE);
    }
    if (sFWS2_t.page == SET_VERSION_PAGE || sFWS2_t.page == SET_OTA_PAGE)
    {
        TranferPicturetoTFT_LCD(return_1.x1, return_1.y1, return_1.length, return_1.winth, RETURN_ICON_PICTURE);
    }

	/* show base handle error state*/
//	if (sFWS2_t.last_Direct_handle_error_state != sFWS2_t.Direct_handle_error_state || sFWS2_t.last_page != sFWS2_t.page)
//	{
//		sFWS2_t.last_Direct_handle_error_state = sFWS2_t.Direct_handle_error_state;

	if (sFWS2_t.last_Direct_handle_error_state != sFWS2_t.Direct_handle_error_state)
	{
		sFWS2_t.last_Direct_handle_error_state = sFWS2_t.Direct_handle_error_state;
		sFWS2_t.base.last_run_disp_state = RESET_VALUE;
		if (sFWS2_t.Direct_handle_error_state == HANDLE_OK)
		{
			sFWS2_t.last_page = RESET_VALUE;
		}
	}
		if (sFWS2_t.Direct_handle_error_state == HANDLE_OVER_TEMP_ERR)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
			{
				TranferPicturetoTFT_LCD(90, 75, 139, 118, OVER_TEMP_ICON);
			}
			if (sFWS2_t.page == CURVE_PAGE)
			{
				TranferPicturetoTFT_LCD(65, 55, 139, 118, OVER_TEMP_ICON);
			}
		}
		else if (sFWS2_t.Direct_handle_error_state == HANDLE_LOW_TEMP_ERR)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
			{
				TranferPicturetoTFT_LCD(90, 75, 139, 118, LOW_TEMP_ICON);
			}

			if (sFWS2_t.page == CURVE_PAGE)
			{
				TranferPicturetoTFT_LCD(65, 55, 139, 118, LOW_TEMP_ICON);
			}
		}
		else if (sFWS2_t.Direct_handle_error_state == HANDLE_NO_ERR)
		{
			if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245)
			{
				TranferPicturetoTFT_LCD(90, 75, 139, 118, HANDLE_NOT_EXIST_ICON);
			}

			if (sFWS2_t.page == CURVE_PAGE)
			{
				TranferPicturetoTFT_LCD(65, 55, 139, 118, HANDLE_NOT_EXIST_ICON);
			}
		}
//	}
}

void show_navigation_bar(void)
{
	static bool in_first = false;
	static handle_error_state_e last_state = HANDLE_OK;
	if(in_first == false)
	{
		TranferPicturetoTFT_LCD(0,0,320,26, NAVIGATION_BAR_PICTURE);
		sFWS2_t.general_parameter.last_speak_state = RESET_VALUE;
		in_first = true;
	}
	
	if(sFWS2_t.base.last_run_disp_state != sFWS2_t.base.run_disp_state)
	{
		if (sFWS2_t.Direct_handle_state == HANDLE_WORKING)
		{
			TranferPicturetoTFT_LCD(run.x1, run.y1, run.length, run.winth, RUN_ICON_PICTURE);
			sFWS2_t.general_parameter.last_temp_unit = RESET_VALUE;
		}
		else if(sFWS2_t.Direct_handle_state == HANDLE_SLEEP)
		{
			TranferPicturetoTFT_LCD(run.x1, run.y1, run.length, run.winth,SLEEP_ICON_PICTURE);
		}
		else
		{
			TranferPicturetoTFT_LCD(0,0,320,26, NAVIGATION_BAR_PICTURE);
			sFWS2_t.general_parameter.last_speak_state = RESET_VALUE; 
		}
		sFWS2_t.base.last_run_disp_state = sFWS2_t.base.run_disp_state;
		
	}
	    //    /* show handle work state */
	if (sFWS2_t.page == WORK_PAGE_210 || sFWS2_t.page == WORK_PAGE_245 || sFWS2_t.page == CURVE_PAGE )
	{
		if (sFWS2_t.last_Direct_handle_state != sFWS2_t.Direct_handle_state)
		{
		   if (sFWS2_t.Direct_handle_state == HANDLE_SLEEP && sFWS2_t.page != CURVE_PAGE )
		   {
			   TranferPicturetoTFT_LCD(70, 75, 200, 114, SLEEP_STATE_PICTURE);
		   }
		   sFWS2_t.last_Direct_handle_state = sFWS2_t.Direct_handle_state;
		   sFWS2_t.base.last_run_disp_state = RESET_VALUE;
		   
	   }
	}
	
	/* show beep icon */
    if (sFWS2_t.general_parameter.last_speak_state != sFWS2_t.general_parameter.speak_state && sFWS2_t.page != SET_SPEAK_PAGE)
    {
        if (sFWS2_t.general_parameter.speak_state == SPEAKER_OPEN)
        {
            TranferPicturetoTFT_LCD(beep.x1, beep.y1, beep.length, beep.winth, SPEAK_ICON_PICTURE);
        }
        else
        {
            TranferPicturetoTFT_LCD(beep.x1, beep.y1, beep.length, beep.winth, UN_SPEAK_ICON_PICTURE);
        }

        sFWS2_t.general_parameter.last_speak_state = sFWS2_t.general_parameter.speak_state;
    }
	

	
}


/* in page single */
static number type_1 =
    {
        // show S210/S245
        .x = 280,
        .y = 195,
        .len = 3,
        .fc = WHITE,
        .bc = BLACK,
        //	.sizey = 16,
        .sizey = 16,
        .mode = 0,
};

static number set_temp_1 =
    {
        // show small set temp
        .x = 190,
        .y = 38,
        .num = &sFWS2_t.base.set_temp,
        .len = 3,
        .fc = WHITE,
        .bc = 0x20e4,
        .sizey = 32,
        .mode = 0,
};

static number set_temp_2 =
    {
        // show big set temp
        .x = 80,
        .y = 120,
        .num = &sFWS2_t.base.set_temp,
        .len = 3,
        .fc = WHITE,
        .bc = 0x20e4,
        .sizey = 80,
        .mode = 0,
};

static number actual_temp_1 =
{
	.x = 68,
	.y = 73,
	.num = &sFWS2_t.base.actual_temp,
	.len = 3,
	.fc = WHITE,
	.bc = 0x0000,
	.sizey = 120,
	.mode = 0,
};

static number power_data_1 =
    {
        .x = 8,
        .y = 198,
        .num = &sFWS2_t.base.power_data,
        .len = 3,
        .fc = WHITE,
        .bc = BLACK,
        .sizey = 16,
        .mode = 0,
};

// static number type_2 =
//{
//	.x = 218,
//	.y = 43,
//	.len = 3,
//	.fc = WHITE,
//	.bc = 0x20e4,
//	.sizey = 16,
//	.mode = 0,
// };

static number ch1_set_temp_1 =
    {
        .x = 78,
        .y = 195,
        .num = &sFWS2_t.general_parameter.ch1_set_temp,
        .len = 3,
        .fc = WHITE,
        .bc = 0x3186,
        .sizey = 24,
        .mode = 0,
};

static number ch2_set_temp_1 =
    {
        .x = 142,
        .y = 195,
        .num = &sFWS2_t.general_parameter.ch2_set_temp,
        .len = 3,
        .fc = WHITE,
        .bc = 0x3186,
        .sizey = 24,
        .mode = 0,
};

static number ch3_set_temp_1 =
    {
        .x = 206,
        .y = 195,
        .num = &sFWS2_t.general_parameter.ch3_set_temp,
        .len = 3,
        .fc = WHITE,
        .bc = 0x3186,
        .sizey = 24,
        .mode = 0,
};

/* in page curve*/
static number set_temp_3 =
    {
        .x = 277,
        .y = 85,
        .num = &sFWS2_t.base.set_temp,
        .len = 3,
        .fc = WHITE,
        .bc = 0x3186,
        .sizey = 16,
        .mode = 0,
};
static number actual_temp_3 =
    {
        .x = 277,
        .y = 155,
        .num = &sFWS2_t.base.actual_temp,
        .len = 3,
        .fc = 0xfc80,
        .bc = 0x3186,
        .sizey = 16,
        .mode = 0,
};

static number cal_temp_1 =
    {
        .x = 190,
        .y = 127,
        .num = &sFWS2_t.base.cal_temp_c_display,
        .len = 3,
        .fc = WHITE,
        .bc = 0x3186,
        .sizey = 24,
        .mode = 0,
};

static number cal_temp_2 =
    {
        .x = 190,
        .y = 132,
        .num = &sFWS2_t.base.cal_temp_f_display,
        .len = 3,
        .fc = WHITE,
        .bc = 0x3186,
        .sizey = 32,
        .mode = 0,
};

static number cal_set_temp_1 =
    {
        .x = 190,
        .y = 88,
        .num = &sFWS2_t.base.set_temp,
        .len = 3,
        .fc = WHITE,
        .bc = 0x3186,
        .sizey = 24,
        .mode = 0,
};

static number sleep_time_1 =
    {
        .x = 160,
        .y = 103,
        .num = &sFWS2_t.base.set_sleep_time,
        .len = 3,
        .fc = WHITE,
        .bc = 0x3186,
        .sizey = 32,
        .mode = 0,
};

/***************/

/* other */

static icon_t C_F_main_symbol =
    {
        .x1 = 250,
        .y1 = 88,
        .length = 19,
        .winth = 22,
};
static icon_t C_F_menu_symbol =
    {
        .x1 = 225,
        .y1 = 90,
        .length = 18,
        .winth = 18,
};

static icon_t ch1_frame =
    {
        .x1 = 68,
        .y1 = 192,
        .length = 57,
        .winth = 31,
};
static icon_t ch2_frame =
    {
        .x1 = 132,
        .y1 = 192,
        .length = 57,
        .winth = 31,
};
static icon_t ch3_frame =
    {
        .x1 = 196,
        .y1 = 192,
        .length = 57,
        .winth = 31,
};

static icon_t beep =
    {
        .x1 = 267,
        .y1 = 3,
        .length = 19,
        .winth = 21,

};

static icon_t run =
    {
        .x1 = 228,
        .y1 = 4,
        .length = 36,
        .winth = 19,
};

static icon_t ring =
    {
        .x1 = 236,
        .y1 = 99,
        .length = 19,
        .winth = 19,
};

static icon_t exit_1 =
    {
        .x1 = 117,
        .y1 = 184,
        .length = 88,
        .winth = 34,
};

static icon_t return_1 =
    {
        .x1 = 128,
        .y1 = 187,
        .length = 66,
        .winth = 35,
};

static icon_t confirm =
    {
        .x1 = 132,
        .y1 = 132,
        .length = 56,
        .winth = 30,
};
