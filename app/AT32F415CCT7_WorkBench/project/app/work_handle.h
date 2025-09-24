#ifndef _WORK_HANDLE_H
#define _WORK_HANDLE_H
#include "at32f415.h"


#define WORK_HANDLE_TIME      10
//#define ERROR_TIME_OUT        150  
#define SLEEP_TIME_OUT        30

void Direct_handle_switch(void);
void check_handle_Heating_element(void);  //ºÏ≤‚ ÷±˙∑¢»»–æ
void check_handle_magnification(void);
#endif
