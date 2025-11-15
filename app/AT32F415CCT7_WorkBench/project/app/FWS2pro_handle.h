#ifndef _FWG2_HANDLE_H
#define _FWG2_HANDLE_H
#include "at32f415.h"
#include "stdbool.h"

/*     GPIO DEFINE
 *     DIRECT  HANDLE HOT OUTPUT   (TMR2_CHANNEL_1)
 *     DIRECT  HANDLE FAN OUTPUT   (TMR9_CHANNEL_2)
 *     DIRECT  HANDLE HOT CHECK    (ADC_CHANNEL_10)
 */
 
#define MAX_210_OUTDATA 4500
#define MAX_245_OUTDATA 10000
 
#define MAX_SET_TEMP_VAL 450
#define MIN_SET_TEMP_VAL 200

#define MAX_CAL_TEMP 100
#define MIN_CAL_TEMP -100

#define MAX_CAL_TEMP_F 212
#define MIN_CAL_TEMP_F -148

#define MAX_SLEEP_TIME 999
#define MIN_SLEEP_TIME 0

#define MAX_MAX_ACTUAL_TEMP 700
#define MAX_ACTUAL_TEMP 550
#define MIN_ACTUAL_TEMP 60
#define ERROR_TIME 10

#define OVER_CURRENT_DATA 800


#define RESET_VALUE 255

#define KEY_LONG_PRESS_TIME 10

// #define OVER_TEMP_VAL 505
// #define LOW_TEMP_VAL 65

#define MAX_SET_WIND_VAL 200
#define MIN_SET_WIND_VAL 1

#define OVER_WIND_RATE 534
#define LOW_WIND_RATE 10

#define ENHANCE_TEMP 50

#define Disp_Set 0
#define Disp_Real 1
#define Disp_Err 2
#define Disp_Slp 3
#define Disp_Flick 4
#define Disp_Write_Ch 5
#define Disp_On_Off 6
#define Disp_Cal 7

#define FIRST_MENU_LEVEL 1
#define SECOND_MENU_LEVEL 2

#define OUT_SET 0
#define CALIBRATION_TEMP 1
#define C_F_CONVERT 2
#define DISPLAY_LOCK 3
#define SOUND_SWITCH 4

#define ADD_HOT 0
#define REDUCE_HOT 1
#define PID_HOT 2

#define ACTUAL_TEMP_REFRESH_TIME 50

#define POWER_BAR_REFRESH_TIME 100

#define SET_TEMP_TIME 100

#define LOCK_RANGE 20
#define CURVE_REFRESH_TIME 0
#define CURVE_BUFF_SIZE 205

//#define LOGO_PICTURE 1
//#define WORK_210_PICTURE 2
//#define WORK_245_PICTURE 3
//#define SELECT_SET_WORK_PICTURE_CN 4
//#define SELECT_SET_UNIT_PICTURE_CN 5
//#define SELECT_SET_TEMP_CAL_PICTURE_CN 6
//#define SELECT_SET_SPEAK_PICTURE_CN 7
//#define SELECT_SET_TEMP_LOCK_PICTURE_CN 8
//#define SELECT_SET_SLEEP_PICTURE_CN 9
//#define SELECT_SET_RESET_PICTURE_CN 10
//#define SELECT_SET_VERSION_PICTURE_CN 11
//#define SELECT_SET_OTA_PICTURE_CN 12
//#define SELECT_SET_SUPPORT_PICTURE_CN 13
//#define SELECT_EXIT_MENU_PICTURE_CN 14
//#define CURVE_PICTURE 15
//#define SET_WORK_PICTURE_CN 16
//#define SET_UNIT_PICTURE_CN 17
//#define SET_TEMP_CAL_PICTURE_CN 18
//#define SET_SPEAK_PICTURE_CN 19
//#define SET_TEMP_LOCK_PICTURE_CN 20
//#define SET_SLEEP_PICTURE_CN 21
//#define SET_RESET_PICTURE_CN 22
//#define SET_RESET_DONE_PICTURE_CN 23
//#define SET_VERSION_PICTURE_CN 24
//#define SET_OTA_PICTURE_CN 25
//#define SET_SUPPORT_PICTURE_CN 26
///* some icons */
////#define UNSELECT_CH_PICTURE 27
////#define SELECT_CH_PICTURE 28
//#define MENU_CELSIUS_ICON_PICTURE 27
//#define MENU_FAHRENHEIT_ICON_PICTURE 28
//#define SLEEP_STATE_PICTURE_CN 29
//#define LOW_TEMP_ICON_PICTURE_CN 30
//#define UN_RETURN_ICON_PICTURE_CN 31
//#define RETURN_ICON_PICTURE_CN 32
//#define OVER_TEMP_ICON_PICTURE_CN 33
//#define CONFIRM_ICON_PICTURE_CN 34
//#define UN_CONFIRM_ICON_PICTURE_CN 35
//#define SPEAK_ICON_PICTURE 36
//#define UN_SPEAK_ICON_PICTURE 37
//#define UN_EXIT_ICON_PICTURE_CN 38
//#define EXIT_ICON_PICTURE_CN 39
//#define HANDLE_NOT_EXIST_ICON_PICTURE_CN 40
//#define SLEEP_ICON_PICTURE 41
//#define RING_ICON_PICTURE 42
//#define UN_RING_ICON_PICTURE 43
//#define RUN_ICON_PICTURE 44
//#define MAIN_CELSIUS_ICON_PICTURE 45
//#define MAIN_FAHRENHEIT_ICON_PICTURE 46
//#define SLIDER_ICON_PICTURE 47
//#define NAVIGATION_BAR_PICTURE 48
//#define UNSELECT_CH1_PICTURE 49
//#define UNSELECT_CH2_PICTURE 50
//#define UNSELECT_CH3_PICTURE 51
//#define SELECT_CH1_PICTURE 52
//#define SELECT_CH2_PICTURE 53
//#define SELECT_CH3_PICTURE 54
//#define HANDLE_ABNORMAL_PICTURE_CN 55

#define LOGO_PICTURE 1
#define WORK_210_PICTURE 2
#define WORK_245_PICTURE 3
#define SELECT_SET_WORK_PICTURE_CN 4
#define SELECT_SET_WORK_PICTURE_ENG 5
#define SELECT_SET_UNIT_PICTURE_CN 6
#define SELECT_SET_UNIT_PICTURE_ENG 7
#define SELECT_SET_TEMP_CAL_PICTURE_CN 8
#define SELECT_SET_TEMP_CAL_PICTURE_ENG 9
#define SELECT_SET_SPEAK_PICTURE_CN 10
#define SELECT_SET_SPEAK_PICTURE_ENG 11
#define SELECT_SET_TEMP_LOCK_PICTURE_CN 12
#define SELECT_SET_TEMP_LOCK_PICTURE_ENG 13
#define SELECT_SET_SLEEP_PICTURE_CN 14
#define SELECT_SET_SLEEP_PICTURE_ENG 15
#define SELECT_SET_LANGUAGE_PICTURE_CN 16
#define SELECT_SET_LANGUAGE_PICTURE_ENG 17
#define SELECT_SET_VERSION_PICTURE_CN 18
#define SELECT_SET_VERSION_PICTURE_ENG 19
#define SELECT_SET_RESET_PICTURE_CN 20
#define SELECT_SET_RESET_PICTURE_ENG 21
//#define SELECT_SET_OTA_PICTURE_CN 22
//#define SELECT_SET_OTA_PICTURE_ENG 23
#define SELECT_SET_SUPPORT_PICTURE_CN 22
#define SELECT_SET_SUPPORT_PICTURE_ENG 23
#define SELECT_EXIT_MENU_PICTURE_CN 24
#define SELECT_EXIT_MENU_PICTURE_ENG 25
#define CURVE_PICTURE 26
#define SET_WORK_PICTURE_CN 27
#define SET_WORK_PICTURE_ENG 28
#define SET_UNIT_PICTURE_CN 29
#define SET_UNIT_PICTURE_ENG 30
#define SET_TEMP_CAL_PICTURE_CN 31
#define SET_TEMP_CAL_PICTURE_ENG 32
#define SET_SPEAK_PICTURE_CN 33
#define SET_SPEAK_PICTURE_ENG 34
#define SET_TEMP_LOCK_PICTURE_CN 35
#define SET_TEMP_LOCK_PICTURE_ENG 36
#define SET_SLEEP_PICTURE_CN 37
#define SET_SLEEP_PICTURE_ENG 38
#define SET_LANGUAGE_PICTURE_CN 39
#define SET_LANGUAGE_PICTURE_ENG 40
#define SET_LANGUAGE_PICTURE_NOTHING 41
#define SET_VERSION_PICTURE_CN 42
#define SET_VERSION_PICTURE_ENG 43
#define SET_RESET_PICTURE_CN 44
#define SET_RESET_PICTURE_ENG 45
#define SET_RESET_DONE_PICTURE_CN 46
#define SET_RESET_DONE_PICTURE_ENG 47
//#define SET_OTA_PICTURE_CN 50
//#define SET_OTA_PICTURE_ENG 51
#define SET_SUPPORT_PICTURE_CN 48
#define SET_SUPPORT_PICTURE_ENG 49

/* some icons */
#define MENU_CELSIUS_ICON_PICTURE 50
#define MENU_FAHRENHEIT_ICON_PICTURE 51
#define MAIN_CELSIUS_ICON_PICTURE 52
#define MAIN_FAHRENHEIT_ICON_PICTURE 53
#define UNSELECT_CH1_PICTURE 54
#define UNSELECT_CH2_PICTURE 55
#define UNSELECT_CH3_PICTURE 56
#define SELECT_CH1_PICTURE 57
#define SELECT_CH2_PICTURE 58
#define SELECT_CH3_PICTURE 59
#define SPEAK_ICON_PICTURE 60
#define UN_SPEAK_ICON_PICTURE 61
#define RING_ICON_PICTURE 62
#define UN_RING_ICON_PICTURE 63
#define SLEEP_STATE_PICTURE_CN 64
#define SLEEP_STATE_PICTURE_ENG 65
#define LOW_TEMP_ICON_PICTURE_CN 66
#define LOW_TEMP_ICON_PICTURE_ENG 67
#define OVER_TEMP_ICON_PICTURE_CN 68
#define OVER_TEMP_ICON_PICTURE_ENG 69
#define HANDLE_ABNORMAL_PICTURE_CN 70
#define HANDLE_ABNORMAL_PICTURE_ENG 71
#define HANDLE_NOT_EXIST_ICON_PICTURE_CN 72
#define HANDLE_NOT_EXIST_ICON_PICTURE_ENG 73
#define UN_RETURN_ICON_PICTURE_CN 74
#define RETURN_ICON_PICTURE_CN 75
#define UN_RETURN_ICON_PICTURE_ENG 76
#define RETURN_ICON_PICTURE_ENG 77
#define UN_CONFIRM_ICON_PICTURE_CN 78
#define CONFIRM_ICON_PICTURE_CN 79
#define UN_CONFIRM_ICON_PICTURE_ENG 80
#define CONFIRM_ICON_PICTURE_ENG 81
#define UN_EXIT_ICON_PICTURE_CN 82
#define EXIT_ICON_PICTURE_CN 83
#define UN_EXIT_ICON_PICTURE_ENG 84
#define EXIT_ICON_PICTURE_ENG 85
#define SLIDER_ICON_PICTURE 86
#define NAVIGATION_BAR_PICTURE 87




typedef enum
{
    FWS2_INIT = 0,
    FWS2_WORKING,

} fws2_state_e;

typedef enum
{
    HANDLE_OK = 0,
    HANDLE_FAN_ERR,
    HANDLE_LOW_TEMP_ERR,
    HANDLE_OVER_TEMP_ERR,
	HANDLE_OVER_CURRENT,
    HANDLE_NO_ERR,
} handle_error_state_e;

typedef enum
{
    NOT_IN_POSSITION = 0,
    IN_POSSITION,
} handle_position_e;

typedef enum
{
    HANDLE_210 = 0,
    HANDLE_245,
    HANDLE_NO,
} handle_heating_stick_e;

typedef enum
{
    NOT_IN_RPC = 0,
    IN_RPC,
} handle_heating_rpc_e;

typedef enum
{
    HANDLE_SLEEP = 0,
    HANDLE_WORKING,
    HANDLE_WAKEN,
	HANDLE_RPC,
} handle_state_e;

#define ABNORMAL 0
#define NORMAL 1

typedef enum
{
    WORK_NORMAL = 0,
    WORK_CURVE,
} work_mode_e;

typedef enum
{
    CELSIUS = 0,
    FAHRENHEIT,
    RETURN_C_F,
} fws2_temp_unit_e;

typedef enum
{
    SET_RETURN_CAL = 0,
    SET_CAL,
} set_cal_e;

typedef enum
{
    SPEAKER_CLOSE = 0,
    SPEAKER_OPEN,
    RETURN_SPEAK,
} fws2_speaker_state_e;

typedef enum
{
    UNLOCK = 0,
    LOCK,
    RETURN_LOCK,
} fws2_temp_lock_state_e;

typedef enum
{
    SET_RETURN_SLEEP = 0,
    SET_SLEEP,
} set_sleep_e;


typedef enum
{
    RESET_RETURN = 0,
    RESET_CONFIRN,
} set_reset_e1;
typedef enum
{
    RESET2_RETURN = 0,
    RESET2_CONFIRN,
} set_reset_e2;

typedef enum
{
    VISION_RETURN = 0,
    VISION_START,
} set_vision_e;

typedef enum
{
    OTA_OFF = 0,
    OTA_ON,
} fws2_ota_state_e;

typedef enum 
{
	CHINESE = 0,
	ENGLISH,
} sfws2_language_e;
typedef enum
{
    UART_CLOSE = 0,
    UART_OPEN = 0,

} fws2_uart_state_e;

typedef struct
{
    float actual_temp;
    float actual_temp_f_display;
    float last_actual_temp;
    float last_actual_temp_f_display;
    float set_temp;
    float last_set_temp;
    float set_temp_f_display;
    float last_set_temp_f_display;
    float cal_data;
    float cal_temp_c_display;
    float last_cal_temp_c_display;
    float cal_temp_f_display;
    float last_cal_temp_f_display;
	float current_data;
    float pwm_out;
	float cpu_temp;
    float set_sleep_time;
    float last_set_sleep_time;
    float sleep_time_count;
    float waken_time_count;
    uint16_t error_time;
    handle_state_e last_state;
    bool run_disp_state;
    bool last_run_disp_state;
    float last_power_data;

    uint8_t temp_buff[CURVE_BUFF_SIZE];
    uint8_t power_buff[CURVE_BUFF_SIZE];
} handle_parameter_t;

typedef enum
{
    LOGO = 1,
    WORK_PAGE_210,
    WORK_PAGE_245,
    SELECT_SET_WORK_PAGE_CN,
	SELECT_SET_WORK_PAGE_ENG,
    SELECT_SET_UNIT_PAGE_CN,
	SELECT_SET_UNIT_PAGE_ENG,
    SELECT_SET_TEMP_CAL_PAGE_CN,
	SELECT_SET_TEMP_CAL_PAGE_ENG,
    SELECT_SET_SPEAK_PAGE_CN,
	SELECT_SET_SPEAK_PAGE_ENG,
    SELECT_SET_TEMP_LOCK_PAGE_CN,
	SELECT_SET_TEMP_LOCK_PAGE_ENG,
    SELECT_SET_SLEEP_PAGE_CN,
	SELECT_SET_SLEEP_PAGE_ENG,
	SELECT_SET_LANGUAGE_PAGE_CN,
	SELECT_SET_LANGUAGE_PAGE_ENG,
	SELECT_SET_VERSION_PAGE_CN,
	SELECT_SET_VERSION_PAGE_ENG,
    SELECT_SET_RESET_PAGE_CN,
	SELECT_SET_RESET_PAGE_ENG,
//    SELECT_SET_OTA_PAGE_CN,
//	SELECT_SET_OTA_PAGE_ENG,
	SELECT_SET_SUPPORT_PAGE_CN,
	SELECT_SET_SUPPORT_PAGE_ENG,
    SELECT_EXIT_MENU_PAGE_CN,
	SELECT_EXIT_MENU_PAGE_ENG,
    CURVE_PAGE,
    SET_WORK_PAGE_CN,
	SET_WORK_PAGE_ENG,
    SET_UNIT_PAGE_CN,
	SET_UNIT_PAGE_ENG,
    SET_TEMP_CAL_PAGE_CN,
	SET_TEMP_CAL_PAGE_ENG,
    SET_SPEAK_PAGE_CN,
	SET_SPEAK_PAGE_ENG,
    SET_TEMP_LOCK_PAGE_CN,
	SET_TEMP_LOCK_PAGE_ENG,
    SET_SLEEP_PAGE_CN,
	SET_SLEEP_PAGE_ENG,
	SET_LANGUAGE_PAGE_CN,
	SET_LANGUAGE_PAGE_ENG, 
	SET_LANGUAGE_PAGE_NOTHING, 
	SET_VERSION_PAGE_CN,
	SET_VERSION_PAGE_ENG,
    SET_RESET_PAGE_CN,
	SET_RESET_PAGE_ENG,
    SET_RESET_DONE_PAGE_CN,
	SET_RESET_DONE_PAGE_ENG,
//    SET_OTA_PAGE_CN,
//	SET_OTA_PAGE_ENG,
	SET_SUPPORT_PAGE_CN,
	SET_SUPPORT_PAGE_ENG,
    /* some icons */
    MENU_CELSIUS_ICON,    
    MENU_FAHRENHEIT_ICON, 
	MAIN_CELSIUS_ICON,
    MAIN_FAHRENHEIT_ICON,
	UNSELECT_CH1, 
	UNSELECT_CH2, 
	UNSELECT_CH3, 
	SELECT_CH1, 
	SELECT_CH2, 
	SELECT_CH3, 
	SPEAK_ICON,           
    UN_SPEAK_ICON,  
	RING_ICON,
	UN_RING_ICON,
    SLEEP_STATE_CN,    
	SLEEP_STATE_ENG,  	
    LOW_TEMP_ICON_CN,  
	LOW_TEMP_ICON_ENG,	
	OVER_TEMP_ICON_CN,
	OVER_TEMP_ICON_ENG,
	HANDLE_ABNORMAL_CN,
	HANDLE_ABNORMAL_ENG,
	HANDLE_NOT_EXIST_ICON_CN,
	HANDLE_NOT_EXIST_ICON_ENG,
    UN_RETURN_ICON_CN,
	RETURN_ICON_CN,
	UN_RETURN_ICON_ENG,	
	RETURN_ICON_ENG,	       
    UN_CONFIRM_ICON_CN,
	CONFIRM_ICON_CN,
	UN_CONFIRM_ICON_ENG,
	CONFIRM_ICON_ENG,       
    UN_EXIT_ICON_CN,
	EXIT_ICON_CN,
	UN_EXIT_ICON_ENG,
	EXIT_ICON_ENG,
    SLIDER_ICON,
	NAVIGATION_BAR,
} page_e;

typedef struct
{
    fws2_temp_unit_e temp_unit;
    fws2_temp_unit_e last_temp_unit;
    fws2_speaker_state_e speak_state;
    fws2_speaker_state_e last_speak_state;
    fws2_temp_lock_state_e display_lock_state;
    fws2_temp_lock_state_e last_display_lock_state;
    fws2_ota_state_e ota_state;
    fws2_uart_state_e uart_state;
	sfws2_language_e language_state;
    uint8_t ch;
    uint8_t last_ch;
    uint8_t save_ch;

    /* ch set */
    float ch1_set_temp;
    float last_ch1_set_temp;
    float ch2_set_temp;
    float last_ch2_set_temp;
    float ch3_set_temp;
    float last_ch3_set_temp;
    float ch1_set_temp_f_display;
    float ch2_set_temp_f_display;
    float ch3_set_temp_f_display;
    float last_ch1_set_temp_f_display;
    float last_ch2_set_temp_f_display;
    float last_ch3_set_temp_f_display;

    uint8_t last_Heating_stick;

} general_parameter_t;

typedef struct
{
    fws2_state_e FWS2_STATE;

    handle_parameter_t base;
    handle_state_e Direct_handle_state;
    handle_state_e last_Direct_handle_state;
    handle_position_e Direct_handle_position;
    handle_heating_stick_e Direct_handle_Heating_stick;
    handle_heating_stick_e Direct_last_handle_Heating_stick;
    handle_heating_rpc_e Direct_handle_rpc;
    handle_error_state_e Direct_handle_error_state;
    handle_error_state_e last_Direct_handle_error_state;
    general_parameter_t general_parameter;

    work_mode_e work_mode;
    work_mode_e last_work_mode;
    work_mode_e set_work_mode;

    set_cal_e set_cal;
    fws2_speaker_state_e set_speak;
    fws2_temp_lock_state_e set_lock;
    set_sleep_e set_sleep;
	sfws2_language_e set_language;
    set_reset_e1 set_reset1;
    set_reset_e2 set_reset2;
    set_vision_e set_vision;
    fws2_temp_unit_e set_temp_unit;
    page_e page;
    page_e last_page;

    bool reset_flag;
    bool init_flag;
	bool rpc_state;

} FWS2_Handle;

extern FWS2_Handle sFWS2_t;

void FWS2_Init(FWS2_Handle *FWS2);

#endif
