#ifndef __STM32F4x7_ETH_BSP_H
#define __STM32F4x7_ETH_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

static void ETH_GPIO_Config(void);
static void ETH_NVIC_Config(void);
static void ETH_MACDMA_Config(void);
	 
void  ETH_BSP_Config(void);

#ifdef __cplusplus
}
#endif

#endif  /* __STM32F4x7_ETH_BSP_H */



