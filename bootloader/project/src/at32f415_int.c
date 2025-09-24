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
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "flash.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */
#define LOCAL_DEVICE_ID 0x02
/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */
extern bool iap_done;
uint32_t flash_add = 0x08004000;
uint8_t usart1_tx_buff[22];
uint32_t usart1_crc_buff[258];
uint8_t usart1_rx_buff[1036];
uint8_t pro_buff[2048];
/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */
void usart1_send_ok(void);
void usart1_send_error(void);
void copy_uint8_array(uint8_t* dest, const uint8_t* src, size_t start_offset, size_t length);
uint32_t convert_data(const uint8_t *input_array, uint32_t *output_array,
                      uint32_t start_index, uint32_t end_index);
void copy_array(const uint8_t *src, uint8_t *dst, 
                uint16_t src_start, uint16_t dst_start, 
                uint16_t length);
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

  /* add user code end SysTick_IRQ 0 */

  wk_timebase_handler();

  /* add user code begin SysTick_IRQ 1 */

  /* add user code end SysTick_IRQ 1 */
}

/**
  * @brief  this function handles USART1 handler.
  * @param  none
  * @retval none
  */
void USART1_IRQHandler(void)
{
  /* add user code begin USART1_IRQ 0 */
    static uint16_t reval = 0;
    static uint16_t count = 0;
    static uint8_t data_count = 0;
	
    if (usart_interrupt_flag_get(USART1, USART_RDBF_FLAG) != RESET)
    {
        reval = usart_data_receive(USART1);
        usart1_rx_buff[count] = reval;
        count++;
        if (count == 1036)
        {
            count = 0;
            /* check data */
            if (usart1_rx_buff[0] == 0xD1 && usart1_rx_buff[1035] == 0xF0)
            {
				convert_data(usart1_rx_buff,usart1_crc_buff,1,1030);
                uint32_t crc_value = crc_block_calculate(usart1_crc_buff, 258);
                crc_data_reset();

                if (usart1_rx_buff[1031] == ((crc_value >> 24) & 0xff)  &&
                        usart1_rx_buff[1032] == ((crc_value >> 16) & 0xff)  &&
                        usart1_rx_buff[1033] == ((crc_value >> 8)  & 0xff)   &&
                        usart1_rx_buff[1034] == (crc_value & 0xff))
                {
                    /* write data to app memory */
					if(data_count == 0)
					{
						copy_array(usart1_rx_buff,pro_buff,7,0,1024);
                        data_count++;
					}
					else if(data_count == 1)
					{
						copy_array(usart1_rx_buff,pro_buff,7,1024,1024);
						flash_2kb_write(flash_add, pro_buff);
                        flash_add += 2048;
						data_count = 0;
					}
                    /* send ok */
                    usart1_send_ok();
                    /* the end */
                    if (usart1_rx_buff[2] == 0x04)
                    {
                        /* check app starting address whether 0x08xxxxxx */
                       if (((*(uint32_t *)(0x08004000 + 4)) & 0xFF000000) == 0x08000000)
                        {
							iap_done = true;
                        }
                    }
                }
                else
                {
                    /* send error */
                    usart1_send_error();
                }
            }
            else
            {
                /* send error */
                usart1_send_error();
            }
        }

    }
  /* add user code end USART1_IRQ 0 */
  /* add user code begin USART1_IRQ 1 */

  /* add user code end USART1_IRQ 1 */
}

/* add user code begin 1 */



uint32_t convert_data(const uint8_t *input_array, uint32_t *output_array,
                      uint32_t start_index, uint32_t end_index)
{
    // �������
    if (input_array == NULL || output_array == NULL)
    {
        return 0;
    }

    if (start_index > end_index)
    {
        return 0;
    }

    uint32_t byte_count = end_index - start_index + 1; // Ҫ��ȡ���ֽ���
    const uint8_t *ptr = &input_array[start_index]; // ָ���һ��Ҫ��ȡ���ֽ�
    uint32_t i;
    uint32_t output_index = 0;

    // ����������4�ֽڿ飨���ģʽ��
    for (i = 0; i < byte_count / 4; i++)
    {
        output_array[output_index++] = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
        ptr += 4;
    }

    // ����ʣ���ֽڣ�����4�ֽ�ʱ���㣬���ģʽ��
    if (byte_count % 4 != 0)
    {
        uint32_t word = 0;
        uint8_t remaining = byte_count % 4;

        for (uint8_t j = 0; j < remaining; j++)
        {
            word |= ptr[j] << (24 - (j * 8)); // ��һ���ֽڷ������λ
        }

        output_array[output_index++] = word;
    }

    return output_index; // ����ת����32λ��������
}

void copy_array(const uint8_t *src, uint8_t *dst, 
                uint16_t src_start, uint16_t dst_start, 
                uint16_t length) {
    // ����ʵ�ʵ��ڴ��ַ
    const uint8_t *src_ptr = src + src_start;
    uint8_t *dst_ptr = dst + dst_start;
    
    // ִ�и��Ʋ���
    for (uint16_t i = 0; i < length; i++) {
        dst_ptr[i] = src_ptr[i];
    }
}

void copy_uint8_array(uint8_t* dest, const uint8_t* src, size_t start_offset, size_t length) {
    // ����ʵ����ʼָ��
    const uint8_t* src_start = src + start_offset;
    
    // ������ʼ�Ƕ��벿�֣����ֽڸ��ƣ�
    while (((uintptr_t)src_start % sizeof(uint32_t)) != 0 && length > 0) {
        *dest++ = *src_start++;
        length--;
    }
    
    // ʹ��32λ�ָ������Ч��
    if (length >= sizeof(uint32_t)) {
        size_t word_count = length / sizeof(uint32_t);
        uint32_t* dest32 = (uint32_t*)dest;
        const uint32_t* src32 = (const uint32_t*)src_start;
        
        // �ֶ��븴��
        for (size_t i = 0; i < word_count; i++) {
            *dest32++ = *src32++;
        }
        
        // ����ָ���ʣ�೤��
        dest = (uint8_t*)dest32;
        src_start = (const uint8_t*)src32;
        length %= sizeof(uint32_t);
    }
    
    // ����ʣ���ֽ�
    for (size_t i = 0; i < length; i++) {
        *dest++ = *src_start++;
    }
}




void usart1_send_ok(void)
{
    uint32_t crc_value = 0;
    usart1_tx_buff[0] = 0xD1;
    usart1_tx_buff[1] = 0x02;
    usart1_tx_buff[2] = 0x02;
    usart1_tx_buff[3] = 0x00;
    usart1_tx_buff[4] = LOCAL_DEVICE_ID;
    usart1_tx_buff[5] = 0x00;
    usart1_tx_buff[6] = 0x0A;
    usart1_tx_buff[7] = 0x1D;
    usart1_tx_buff[8] = 0x00;
    usart1_tx_buff[9] = 0x00;
    usart1_tx_buff[10] = 0x00;
    usart1_tx_buff[11] = 0x00;
    usart1_tx_buff[12] = 0x00;
    usart1_tx_buff[13] = 0x00;
    usart1_tx_buff[14] = 0x00;
    usart1_tx_buff[15] = 0x00;
    usart1_tx_buff[16] = 0x00;
	convert_data(usart1_tx_buff,usart1_crc_buff,1,16);
    crc_value = crc_block_calculate(usart1_crc_buff, 4);
    crc_data_reset();
    usart1_tx_buff[17] = ((crc_value >> 24) & 0xff);
    usart1_tx_buff[18] = ((crc_value >> 16) & 0xff);
    usart1_tx_buff[19] = ((crc_value >> 8) & 0xff);
    usart1_tx_buff[20] = (crc_value & 0xff);
    usart1_tx_buff[21] = 0xF0;

    for (uint8_t i = 0; i < 22; i++)
    {
        usart_data_transmit(USART1, usart1_tx_buff[i ]);
        
        while (usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
    }
}

void usart1_send_error(void)
{
    uint32_t crc_value = 0;
    usart1_tx_buff[0] = 0xD1;
    usart1_tx_buff[1] = 0x02;
    usart1_tx_buff[2] = 0x02;
    usart1_tx_buff[3] = 0x00;
    usart1_tx_buff[4] = 0x00;
    usart1_tx_buff[5] = 0x00;
    usart1_tx_buff[6] = 0x0A;
    usart1_tx_buff[7] = 0x18;
    usart1_tx_buff[8] = 0x00;
    usart1_tx_buff[9] = 0x00;
    usart1_tx_buff[10] = 0x00;
    usart1_tx_buff[11] = 0x00;
    usart1_tx_buff[12] = 0x00;
    usart1_tx_buff[13] = 0x00;
    usart1_tx_buff[14] = 0x00;
    usart1_tx_buff[15] = 0x00;
    usart1_tx_buff[16] = 0x00;
	convert_data(usart1_tx_buff,usart1_crc_buff,1,16);
    crc_value = crc_block_calculate(usart1_crc_buff, 4);
    crc_data_reset();
    usart1_tx_buff[17] = ((crc_value >> 24) & 0xff);
    usart1_tx_buff[18] = ((crc_value >> 16) & 0xff);
    usart1_tx_buff[19] = ((crc_value >> 8) & 0xff);
    usart1_tx_buff[20] = (crc_value & 0xff);
    usart1_tx_buff[21] = 0xF0;

    for (uint8_t i = 0; i < 22; i++)
    {
        usart_data_transmit(USART1, usart1_tx_buff[i ]);

        while (usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
    }
}
/* add user code end 1 */
