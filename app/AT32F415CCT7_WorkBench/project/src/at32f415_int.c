/* add user code begin Header */
/**
 **************************************************************************
 * @file     at32f415_int.c
 * @brief    main interrupt service routines.
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */
/* add user code end Header */

/* includes ------------------------------------------------------------------*/
#include "at32f415_int.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "iap_usart.h"
#include "tmt.h"
#include "iap.h"
#include "tmt.h"
#include <math.h>
#include "FWS2pro_Handle.h"
#include "tmt.h"
#include "output_handle.h"
#include "beep_handle.h"
#include "PID_operation.h"
#include "PC_comm_handle.h"
#include "perf_counter.h"
#include "key_handle.h"
#include "adc_filter.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */
#define PID_RUN_TIME         10
#define ADC_VREF            (3.3)
#define ADC_TEMP_BASE       (1.26)
#define ADC_TEMP_SLOPE      (-0.00423)

#define GET_MCU_TEMP_TIME   500
/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */
__IO uint16_t adc1_ordinary_value = 0;
/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/* external variables ---------------------------------------------------------*/
/* add user code begin external variables */

/* add user code end external variables */

/**
  * @brief  this function handles nmi exception.
  * @param  none
  * @retval none
  */
void NMI_Handler(void)
{
  /* add user code begin NonMaskableInt_IRQ 0 */

  /* add user code end NonMaskableInt_IRQ 0 */

  /* add user code begin NonMaskableInt_IRQ 1 */

  /* add user code end NonMaskableInt_IRQ 1 */
}

/**
  * @brief  this function handles hard fault exception.
  * @param  none
  * @retval none
  */
void HardFault_Handler(void)
{
  /* add user code begin HardFault_IRQ 0 */

  /* add user code end HardFault_IRQ 0 */
  /* go to infinite loop when hard fault exception occurs */
  while (1)
  {
    /* add user code begin W1_HardFault_IRQ 0 */

    /* add user code end W1_HardFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles memory manage exception.
  * @param  none
  * @retval none
  */
void MemManage_Handler(void)
{
  /* add user code begin MemoryManagement_IRQ 0 */

  /* add user code end MemoryManagement_IRQ 0 */
  /* go to infinite loop when memory manage exception occurs */
  while (1)
  {
    /* add user code begin W1_MemoryManagement_IRQ 0 */

    /* add user code end W1_MemoryManagement_IRQ 0 */
  }
}

/**
  * @brief  this function handles bus fault exception.
  * @param  none
  * @retval none
  */
void BusFault_Handler(void)
{
  /* add user code begin BusFault_IRQ 0 */

  /* add user code end BusFault_IRQ 0 */
  /* go to infinite loop when bus fault exception occurs */
  while (1)
  {
    /* add user code begin W1_BusFault_IRQ 0 */

    /* add user code end W1_BusFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles usage fault exception.
  * @param  none
  * @retval none
  */
void UsageFault_Handler(void)
{
  /* add user code begin UsageFault_IRQ 0 */

  /* add user code end UsageFault_IRQ 0 */
  /* go to infinite loop when usage fault exception occurs */
  while (1)
  {
    /* add user code begin W1_UsageFault_IRQ 0 */

    /* add user code end W1_UsageFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles svcall exception.
  * @param  none
  * @retval none
  */
void SVC_Handler(void)
{
  /* add user code begin SVCall_IRQ 0 */

  /* add user code end SVCall_IRQ 0 */
  /* add user code begin SVCall_IRQ 1 */

  /* add user code end SVCall_IRQ 1 */
}

/**
  * @brief  this function handles debug monitor exception.
  * @param  none
  * @retval none
  */
void DebugMon_Handler(void)
{
  /* add user code begin DebugMonitor_IRQ 0 */

  /* add user code end DebugMonitor_IRQ 0 */
  /* add user code begin DebugMonitor_IRQ 1 */

  /* add user code end DebugMonitor_IRQ 1 */
}

/**
  * @brief  this function handles pendsv_handler exception.
  * @param  none
  * @retval none
  */
void PendSV_Handler(void)
{
  /* add user code begin PendSV_IRQ 0 */

  /* add user code end PendSV_IRQ 0 */
  /* add user code begin PendSV_IRQ 1 */

  /* add user code end PendSV_IRQ 1 */
}

/**
  * @brief  this function handles systick handler.
  * @param  none
  * @retval none
  */
void SysTick_Handler(void)
{
  /* add user code begin SysTick_IRQ 0 */
  static uint8_t pid_run_time = 10;
  static uint8_t beep_run_time = BEEP_HANDLE_TIME;
  static uint8_t key_run_time = KEY_HANDLE_TIME;
  static uint8_t beep_warning_time = 10;
	static uint16_t get_mcu_temp_time = GET_MCU_TEMP_TIME;
	
  beep_run_time--;

  if (!beep_run_time)
  {
    beep_run_time = BEEP_HANDLE_TIME;

    if (sFWS2_t.general_parameter.speak_state == SPEAKER_OPEN)
    {
      beep_handle();
      if (sFWS2_t.Direct_handle_error_state != HANDLE_OK)
      {
        beep_warning_time--;
        if (!beep_warning_time)
        {
          beep_warning_time = 40;
          sbeep.status = BEEP_LONG;
        }
      }
    }
  }
  key_run_time--;
  if (!key_run_time)
  {
    key_run_time = KEY_HANDLE_TIME;
    key_handle();
  }

  pid_run_time--;
  if (!pid_run_time)
  {
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, 0);
    delay_us(120);
	check_current();
    pwm_control();
    tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_4, sFWS2_t.base.pwm_out);
    tmr_counter_enable(TMR2, TRUE);
    pid_run_time = 10;
  }
  
   get_mcu_temp_time --;

    if (!get_mcu_temp_time)
    {
       /* get mcu temp */
       adc1_ordinary_value = get_adcval(ADC_CHANNEL_16);
       sFWS2_t.base.cpu_temp = (ADC_TEMP_BASE - (double)adc1_ordinary_value * ADC_VREF / 4096) / ADC_TEMP_SLOPE + 25;
       get_mcu_temp_time = GET_MCU_TEMP_TIME;
    }
  
  
	rpc_control();

	sleep_control();

  
  

  /* add user code end SysTick_IRQ 0 */

  wk_timebase_handler();

  /* add user code begin SysTick_IRQ 1 */
  tmt.tick();
  USART1_TimeOutCounter();
  /* add user code end SysTick_IRQ 1 */
}


/* add user code begin 1 */

/* add user code end 1 */
