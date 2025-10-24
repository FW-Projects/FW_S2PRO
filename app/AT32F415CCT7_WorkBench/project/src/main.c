/* add user code begin Header */
/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
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

/* Includes ------------------------------------------------------------------*/
#include "at32f415_wk_config.h"
#include "wk_adc.h"
#include "wk_crc.h"
#include "wk_debug.h"
#include "wk_spi.h"
#include "wk_tmr.h"
#include "wk_usart.h"
#include "wk_wdt.h"
#include "wk_dma.h"
#include "wk_gpio.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "FWS2pro_handle.h"
#include "sensor.h"
#include "iap.h"
#include "tmt.h"
#include "perf_counter.h"
#include "EventRecorder.h"
#include "tmt.h"
#include "beep_handle.h"
#include "key_handle.h"
#include "flash_handle.h"
#include "output_handle.h"
#include "PID_operation.h"
#include "adc_filter.h"
#include "display_handle.h"
#include "lcd.h"
#include "PC_comm_handle.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */
#define FEED_DOG_TIME 100
/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */
void iap_task(void);
void sensor_task(void);
void feed_dog_task(void);
void beep_task(void);
void key_task(void);
void flash_task(void);
void adc_task(void);
void output_task(void);
void key_gpio_in(void);
void disp_task(void);
void pc_task(void);
void output_task(void);
/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */
  nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x4000);
  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* init debug function. */
  wk_debug_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config. */
  wk_timebase_init();

  /* init gpio function. */
  wk_gpio_config();

  /* init dma1 channel1 */
  wk_dma1_channel1_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL1, 
                        DMA1_CHANNEL1_PERIPHERAL_BASE_ADDR, 
                        DMA1_CHANNEL1_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL1_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  /* init usart1 function. */
  wk_usart1_init();

  /* init spi1 function. */
  wk_spi1_init();

  /* init spi2 function. */
  wk_spi2_init();

  /* init adc1 function. */
  wk_adc1_init();

  /* init tmr2 function. */
  wk_tmr2_init();

  /* init crc function. */
  wk_crc_init();

  /* init wdt function. */
  wk_wdt_init();

  /* add user code begin 2 */
  tmt_init();
  tmt.create(feed_dog_task, FEED_DOG_TIME);
  tmt.create(iap_task, IAP_HANDLE_TIME);
  tmt.create(flash_task, FLASH_HANDLE_TIME);
  tmt.create(disp_task, DISPLAY_TASK_TIME);
  tmt.create(pc_task, PC_HANDLE_TIME);
  tmt.create(output_task, OUTPUT_HANDLE_TIME);
  filter_init(&s1_adc, ADC_CHANNEL_1);
  filter_init(&s1_current, ADC_CHANNEL_8);
  BSP_UsartInit();
  PID_Init(&pid_210, 50, 0.1, 10, 3333);
//  PID_Init(&pid_210, 50, 0.2, 10, 3333);
  PID_Init(&pid_245, 100, 0.4, 10, 10000);
  gpio_bits_set(GPIOB, GPIO_PINS_11); // LCD_OPEN
  LCD_Init();                                                                                                                                                                                                                          
  LCD_Clear(BLACK);
  TranferPicturetoTFT_LCD(0, 0, 320, 240, LOGO_PICTURE);
  TranferPicturetoTFT_LCD(0, 0, 320, 240, LOGO_PICTURE);
  FWS2_Init(&sFWS2_t);
  init_cycle_counter(true);
  EventRecorderInitialize(0, 1);
  delay_ms(3000);
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
    tmt.run();
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */
/**
 * @brief  iap task function
 * @param  none
 * @retval none
 */
void iap_task(void)
{
  iap_command_handle();
}

/**
 * @brief  display task function
 * @param  none
 * @retval none
 */
void disp_task(void)
{
  //	if (sFWS2_t.init_flag)
  display_handle();
}

/**
 * @brief  display task function
 * @param  none
 * @retval none
 */
void output_task(void)
{
  output_handle();
}

/**
  * @brief  beep task function

  * @param  none
  * @retval none
  */
void beep_task(void)
{
  beep_handle();
}

/**
 * @brief  key task function
 * @param  none
 * @retval none
 */
void key_task(void)
{
#if 1
  key_handle();
#endif
}

/**
 * @brief  flash task function
 * @param  none
 * @retval none
 */
void flash_task(void)
{
#if 1
  FlashProc();
#endif
}

void feed_dog_task(void)
{
  static bool first_in = false;
  if (first_in == false)
  {
    first_in = true;
    /* if enabled, please feed the dog through wdt_counter_reload() function */
    //	 wdt_enable();
  }
  wdt_counter_reload();
}

void pc_task(void)
{
  pc_comm_handle();
}
/* add user code end 4 */
