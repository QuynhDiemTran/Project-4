#include "main.h"
#include "stdint.h"
#include "stm32f4xx.h"

typedef struct
{
    uint32_t GPIOx_MODER;
    uint32_t GPIOx_OTYPER;
    uint32_t GPIOx_OSPEEDR;
    uint32_t GPIOx_PUPDR;
    uint32_t GPIOx_IDR;
    uint32_t GPIOx_ODR;
    uint32_t GPIOx_BSRR;
    uint32_t GPIOx_LCKR;
    uint32_t GPIOx_AFRL;
    uint32_t GPIOx_AFRH;
}GPIO_t;

typedef struct
{
    uint32_t RCC_CR;
    uint32_t RCC_PLLCFGR;
    uint32_t RCC_CFGR;
    uint32_t RCC_CIR;
    uint32_t RCC_AHB1RSTR;
    uint32_t RCC_AHB2RSTR;
    uint32_t RCC_AHB3RSTR;
    uint32_t Reserved;
    uint32_t RCC_APB1RSTR;
    uint32_t RCC_APB2RSTR;
    uint32_t Reserved1[2];
    uint32_t RCC_AHB1ENR;
    uint32_t RCC_AHB2ENR;
    uint32_t RCC_AHB3ENR;
    uint32_t Reserved2;
    uint32_t RCC_APB1ENR;
    uint32_t RCC_APB2ENR;
    uint32_t Reserved3[2];
    uint32_t RCC_AHB1LPENR;
    uint32_t RCC_AHB2LPENR;
    uint32_t RCC_AHB3LPENR;
    uint32_t Reserved4;
    uint32_t RCC_APB1LPENR;
    uint32_t RCC_APB2LPENR;
    uint32_t Reserved5[2];
    uint32_t RCC_BDCR;
    uint32_t RCC_CSR;
    uint32_t Reserved6[2];
    uint32_t RCC_SSCGR;
    uint32_t RCC_PLLI2SCFGR;
}RCC_t;

typedef struct
{
    uint32_t SYSCFG_MEMRMP;
    uint32_t SYSCFG_PMC;
    uint32_t SYSCFG_EXTICR[4];
    uint32_t SYSCFG_CMPCR;
}SYSCFG_t;

typedef struct
{
    uint32_t EXTI_IMR;
    uint32_t EXTI_EMR;
    uint32_t EXTI_RTSR;
    uint32_t EXTI_FTSR;
    uint32_t EXTI_SWIER;
    uint32_t EXTI_PR;
}EXTI_t ;

void Delay(int n ){
    for(int i=0;i<n;i++){}
}
uint32_t flag = 0,count = 0;

void Init_GPIO(RCC_t *RCCx,GPIO_t *GPIOxA)
{
    RCCx->RCC_AHB1ENR |= (1<<0);

    // Button
    GPIOxA->GPIOx_MODER &= ~(3 << 0);  // A0 là input
    GPIOxA->GPIOx_PUPDR &= ~(3 << 0);  // Đưa hai bit 1, 2 về 00
    GPIOxA->GPIOx_PUPDR |=  (1 << 0);  // Đưa về dạng 01 (Pull-Up)

    // Led
    GPIOxA->GPIOx_MODER  &=~(3 << 2);
    GPIOxA->GPIOx_MODER  |= (1 << 2);
    GPIOxA->GPIOx_OTYPER &=~(1 << 1);

}
void Init_FigureEXTI(RCC_t *RCCx,SYSCFG_t *SYSCFGx, EXTI_t *EXTIx)
{
    RCCx->RCC_APB2ENR |= (1<<14);

    SYSCFGx->SYSCFG_EXTICR[0] &=~(0xF<<0);              // configure EXTI (use PA0  )

    EXTIx->EXTI_IMR  |=(1<<0);                          // Disable the mask

    EXTIx->EXTI_RTSR |= (1<<0);                         // Enable rising edge trigger
    EXTIx->EXTI_FTSR &=~(1<<0);                         // Disable falling edge  trigger

    NVIC_SetPriority(EXTI0_IRQn,1);                     // Set Priority
    NVIC_EnableIRQ(EXTI0_IRQn);                         // Enable Interrupt
}

void EXTI0_IRQHandler(void)
{
     EXTI_t *EXTIx = (EXTI_t *)0x40013C00;
     GPIO_t *GPIOxA = (GPIO_t *)0x40020000;

     if(EXTIx->EXTI_PR &(1<<0))                                     // check the interrup's status
     {
        EXTIx->EXTI_PR |= (1<<0);                                   // Clear
        //GPIOxA->GPIOx_ODR ^= (1 << 1); // Toggle LED
        //flag = 1;
        count = count +1 ;
     }
     if(count % 2 == 1){
        GPIOxA->GPIOx_ODR |= (1<<1);
     }
     else
     {
         GPIOxA->GPIOx_ODR &=~(1<<1);
     }
}

int main(void)
{
    RCC_t *RCCx = (RCC_t*)0x40023800;
    SYSCFG_t *SYSCFGx = (SYSCFG_t*)0x40013800;
    EXTI_t *EXTIx = (EXTI_t*)0x40013C00;
    GPIO_t *GPIOxA = (GPIO_t*)0x40020000;

    Init_GPIO(RCCx,GPIOxA);
    Init_FigureEXTI(RCCx,SYSCFGx,EXTIx);

    while(1)
    {


    }
}


















