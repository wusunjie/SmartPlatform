/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL            DMA_CHANNEL_4
#define USARTx_TX_DMA_STREAM             DMA1_Stream6
#define USARTx_RX_DMA_CHANNEL            DMA_CHANNEL_4
#define USARTx_RX_DMA_STREAM             DMA1_Stream5

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn               DMA1_Stream6_IRQn
#define USARTx_DMA_RX_IRQn               DMA1_Stream5_IRQn
#define USARTx_DMA_TX_IRQHandler         DMA1_Stream6_IRQHandler
#define USARTx_DMA_RX_IRQHandler         DMA1_Stream5_IRQHandler
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

#define USART_DEVICE_NAME                AiThinkerA7
#define USART_DEVICE_ID                  DEV_AITHINKER_A7_ID
#define USART_DEVICE_BAUDRATE            115200

#include "../../common/Source/serialport.c"