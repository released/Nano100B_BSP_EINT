/***************************************************************************//**
 * @file     main.c
 * @version  V1.00
 * $Revision: 3 $
 * $Date: 14/09/11 7:08p $
 * @brief    A project template for Nano100BN series MCUs
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "Nano100Series.h"

#define USE_INT0_PB8
//#define USE_INT0_PB9
//#define USE_INT0_PB14
//#define USE_INT0_PC12

void EINT0_IRQHandler(void)
{
	#if defined (USE_INT0_PB8)	
    /* For PB.8, clear the INT flag */
    PB->ISRC = BIT8;
    printf("PB.8 EINT0 occurred. \r\n");
	#elif defined (USE_INT0_PB9)
    PB->ISRC = BIT9;
    printf("PB.9 EINT0 occurred. \r\n");
	#elif defined (USE_INT0_PB14)
    PB->ISRC = BIT14;
    printf("PB.14 EINT0 occurred. \r\n");
	#elif defined (USE_INT0_PC12)
    PC->ISRC = BIT12;
    printf("PC.12 EINT0 occurred. \r\n");

	#endif
	
}


void EINT1_IRQHandler(void)
{
    /* For PB.15, clear the INT flag */
    PB->ISRC = BIT15;

    printf("PB.15 EINT1 occurred. \r\n");
}


void UART0_Init(void)
{
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);

	printf("\r\nCLK_GetCPUFreq : %8d\r\n",CLK_GetCPUFreq());
	printf("CLK_GetHXTFreq : %8d\r\n",CLK_GetHXTFreq());
	printf("CLK_GetLXTFreq : %8d\r\n",CLK_GetLXTFreq());	
	printf("CLK_GetPCLK0Freq : %8d\r\n",CLK_GetHCLKFreq());
	printf("CLK_GetPCLK1Freq : %8d\r\n",CLK_GetPLLClockFreq());


}


void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Set HCLK source form HXT and HCLK source divide 1  */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT,CLK_HCLK_CLK_DIVIDER(1));

    /* Enable external 12MHz HXT, 32KHz LXT and HIRC */
    CLK_EnableXtalRC(CLK_PWRCTL_HXT_EN_Msk | CLK_PWRCTL_LXT_EN_Msk | CLK_PWRCTL_HIRC_EN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_HXT_STB_Msk | CLK_CLKSTATUS_LXT_STB_Msk | CLK_CLKSTATUS_HIRC_STB_Msk);

    /*  Set HCLK frequency 42MHz */
    CLK_SetCoreClock(42000000);

    /* Enable IP clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(UART0_MODULE,CLK_CLKSEL1_UART_S_HXT,CLK_UART_CLK_DIVIDER(1));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();


    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->PB_L_MFP &= ~(SYS_PB_L_MFP_PB0_MFP_Msk | SYS_PB_L_MFP_PB1_MFP_Msk);
    SYS->PB_L_MFP |= (SYS_PB_L_MFP_PB1_MFP_UART0_TX | SYS_PB_L_MFP_PB0_MFP_UART0_RX);

	#if defined (USE_INT0_PB8)	
    SYS->PB_H_MFP &= ~( SYS_PB_H_MFP_PB8_MFP_Msk );
    SYS->PB_H_MFP |= (SYS_PB_H_MFP_PB8_MFP_EXT_INT0);
	#elif defined (USE_INT0_PB9)
    SYS->PB_H_MFP &= ~( SYS_PB_H_MFP_PB9_MFP_Msk );
    SYS->PB_H_MFP |= (SYS_PB_H_MFP_PB9_MFP_EXT_INT0);
	#elif defined (USE_INT0_PB14)
    SYS->PB_H_MFP &= ~( SYS_PB_H_MFP_PB14_MFP_Msk );
    SYS->PB_H_MFP |= (SYS_PB_H_MFP_PB14_MFP_EXT_INT0);
	#elif defined (USE_INT0_PC12)
    SYS->PC_H_MFP &= ~( SYS_PC_H_MFP_PC12_MFP_Msk );
    SYS->PC_H_MFP |= (SYS_PC_H_MFP_PC12_MFP_EXT_INT0);
	
	#endif

	SYS->PB_H_MFP &= ~( SYS_PB_H_MFP_PB15_MFP_Msk );
	SYS->PB_H_MFP |= (SYS_PB_H_MFP_PB15_MFP_EXT_INT1);

	
    /* Lock protected registers */
    SYS_LockReg();
}



int main()
{

    SYS_Init();
	
    /* Init UART to 115200-8n1 for print message */


	/*
		PB.0 : UART0_RX , PIN44
		PB.1 : UART0_TX , PIN45
	
	*/
    UART0_Init();

	#if defined (USE_INT0_PB8)	//PIN126
    GPIO_SetMode(PB, BIT8, GPIO_PMD_INPUT);
    GPIO_EnableEINT0(PB, 8, GPIO_INT_FALLING);

	#elif defined (USE_INT0_PB9)		//PIN64
    GPIO_SetMode(PB, BIT9, GPIO_PMD_INPUT);
    GPIO_EnableEINT0(PB, 9, GPIO_INT_FALLING);
	#elif defined (USE_INT0_PB14)		//PIN2
    GPIO_SetMode(PB, BIT14, GPIO_PMD_INPUT);
    GPIO_EnableEINT0(PB, 14, GPIO_INT_FALLING);
	#elif defined (USE_INT0_PC12)		//PIN71
    GPIO_SetMode(PC, BIT12, GPIO_PMD_INPUT);
    GPIO_EnableEINT0(PC, 12, GPIO_INT_FALLING);
	
	#endif
	
    NVIC_EnableIRQ(EINT0_IRQn);

	//PIN111
    GPIO_SetMode(PB, BIT15, GPIO_PMD_INPUT);
    GPIO_EnableEINT1(PB, 15, GPIO_INT_FALLING);
    NVIC_EnableIRQ(EINT1_IRQn);

    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_HCLK, GPIO_DBCLKSEL_1);

	#if defined (USE_INT0_PB8)	
    GPIO_ENABLE_DEBOUNCE(PB, BIT8);
	#elif defined (USE_INT0_PB9)
    GPIO_ENABLE_DEBOUNCE(PB, BIT9);
	#elif defined (USE_INT0_PB14)
    GPIO_ENABLE_DEBOUNCE(PB, BIT14);
	#elif defined (USE_INT0_PC12)
    GPIO_ENABLE_DEBOUNCE(PC, BIT12);
	
	#endif
    GPIO_ENABLE_DEBOUNCE(PB, BIT15);


    while(1);

}

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
