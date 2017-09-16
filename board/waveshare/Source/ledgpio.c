#include "device.h"
#include "boardcfg.h"
#include "stm32f4xx_hal.h"

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_LED STM32F4 DISCOVERY LOW LEVEL LED
  * @{
  */
#define LEDn                             4

#define LED4_PIN                         GPIO_PIN_6
#define LED4_GPIO_PORT                   GPIOF
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()

#define LED3_PIN                         GPIO_PIN_7
#define LED3_GPIO_PORT                   GPIOF
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()
  
#define LED5_PIN                         GPIO_PIN_8
#define LED5_GPIO_PORT                   GPIOF
#define LED5_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()

#define LED6_PIN                         GPIO_PIN_9
#define LED6_GPIO_PORT                   GPIOF
#define LED6_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 1) LED3_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 2) LED5_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 3) LED6_GPIO_CLK_ENABLE(); \
                                           }while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED3_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 2) LED5_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 3) LED6_GPIO_CLK_DISABLE(); \
                                            }while(0)

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED4_GPIO_PORT,
                                 LED3_GPIO_PORT,
                                 LED5_GPIO_PORT,
                                 LED6_GPIO_PORT};
static const uint16_t GPIO_PIN[LEDn] = {LED4_PIN,
                                 LED3_PIN,
                                 LED5_PIN,
                                 LED6_PIN};

static int current = 0;

static void BSP_LED_Init(int Led);

static void BSP_LED_On(int Led);

static void BSP_LED_Off(int Led);

static GPIO_PinState BSP_LED_Read(int led);

DEVICE_DEFINE(ledgpio_led, DEV_LEDGPIO_ID);

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6
  */
static void BSP_LED_Init(int Led)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO_LED Clock */
    LEDx_GPIO_CLK_ENABLE(Led);

    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.Pin = GPIO_PIN[Led];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6  
  */
static void BSP_LED_On(int Led)
{
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6 
  */
static void BSP_LED_Off(int Led)
{
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

static GPIO_PinState BSP_LED_Read(int Led)
{
    return HAL_GPIO_ReadPin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

DEVICE_FUNC_DEFINE_OPEN(ledgpio_led)
{
    BSP_LED_Init(0);
    BSP_LED_Init(1);
    BSP_LED_Init(2);
    BSP_LED_Init(3);
}

DEVICE_FUNC_DEFINE_CLOSE(ledgpio_led)
{
}

DEVICE_FUNC_DEFINE_WRITE(ledgpio_led)
{
    if (len < 1) {
        return -1;
    }

    if (buf[0]) {
        BSP_LED_On(current);
    }
    else {
        BSP_LED_Off(current);
    }

    return 1;
}

DEVICE_FUNC_DEFINE_READ(ledgpio_led)
{
    if (len < 1) {
        return -1;
    }
    if (GPIO_PIN_SET == BSP_LED_Read(current)) {
        buf[0] = 1;
    }
    else {
        buf[0] = 0;
    }

    return 1;
}

DEVICE_FUNC_DEFINE_LSEEK(ledgpio_led)
{
    switch (dir)
    {
        case SEEK_SET:
        {
            current = ptr;
            return 0;
        }
        break;
        default:
        break;
    }
    return -1;
}