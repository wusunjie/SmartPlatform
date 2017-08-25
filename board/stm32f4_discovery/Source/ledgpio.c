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

typedef enum
{
    LED4 = 0,
    LED3 = 1,
    LED5 = 2,
    LED6 = 3
} Led_TypeDef;

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED4_GPIO_PORT,
                                 LED3_GPIO_PORT,
                                 LED5_GPIO_PORT,
                                 LED6_GPIO_PORT};
static const uint16_t GPIO_PIN[LEDn] = {LED4_PIN,
                                 LED3_PIN,
                                 LED5_PIN,
                                 LED6_PIN};

static void BSP_LED_Init(Led_TypeDef Led);

static void BSP_LED_On(Led_TypeDef Led);

static void BSP_LED_Off(Led_TypeDef Led);

static GPIO_PinState BSP_LED_Read(Led_TypeDef led);

#pragma GCC push_options
#pragma GCC optimize("O0")

DEVICE_DEFINE(ledgpio_led4, DEV_LEDGPIO1_ID);
DEVICE_DEFINE(ledgpio_led3, DEV_LEDGPIO2_ID);
DEVICE_DEFINE(ledgpio_led5, DEV_LEDGPIO3_ID);
DEVICE_DEFINE(ledgpio_led6, DEV_LEDGPIO4_ID);

#pragma GCC pop_options

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6
  */
static void BSP_LED_Init(Led_TypeDef Led)
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
static void BSP_LED_On(Led_TypeDef Led)
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
static void BSP_LED_Off(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

static GPIO_PinState BSP_LED_Read(Led_TypeDef Led)
{
    return HAL_GPIO_ReadPin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

DEVICE_FUNC_DEFINE_OPEN(ledgpio_led4)
{
    BSP_LED_Init(LED4);
}

DEVICE_FUNC_DEFINE_CLOSE(ledgpio_led4)
{
}

DEVICE_FUNC_DEFINE_WRITE(ledgpio_led4)
{
    if (len < 1) {
        return -1;
    }

    if (buf[0]) {
        BSP_LED_On(LED4);
    }
    else {
        BSP_LED_Off(LED4);
    }

    return 1;
}

DEVICE_FUNC_DEFINE_READ(ledgpio_led4)
{
    if (len < 1) {
        return -1;
    }
    if (GPIO_PIN_SET == BSP_LED_Read(LED4)) {
        buf[0] = 1;
    }
    else {
        buf[0] = 0;
    }

    return 1;
}

DEVICE_FUNC_DEFINE_OPEN(ledgpio_led3)
{
    BSP_LED_Init(LED3);
}

DEVICE_FUNC_DEFINE_CLOSE(ledgpio_led3)
{
}

DEVICE_FUNC_DEFINE_WRITE(ledgpio_led3)
{
    if (len < 1) {
        return -1;
    }

    if (buf[0]) {
        BSP_LED_On(LED3);
    }
    else {
        BSP_LED_Off(LED3);
    }

    return 1;
}

DEVICE_FUNC_DEFINE_READ(ledgpio_led3)
{
    if (len < 1) {
        return -1;
    }
    if (GPIO_PIN_SET == BSP_LED_Read(LED3)) {
        buf[0] = 1;
    }
    else {
        buf[0] = 0;
    }

    return 1;
}

DEVICE_FUNC_DEFINE_OPEN(ledgpio_led5)
{
    BSP_LED_Init(LED5);
}

DEVICE_FUNC_DEFINE_CLOSE(ledgpio_led5)
{
}

DEVICE_FUNC_DEFINE_WRITE(ledgpio_led5)
{
    if (len < 1) {
        return -1;
    }

    if (buf[0]) {
        BSP_LED_On(LED5);
    }
    else {
        BSP_LED_Off(LED5);
    }

    return 1;
}

DEVICE_FUNC_DEFINE_READ(ledgpio_led5)
{
    if (len < 1) {
        return -1;
    }
    if (GPIO_PIN_SET == BSP_LED_Read(LED5)) {
        buf[0] = 1;
    }
    else {
        buf[0] = 0;
    }

    return 1;
}

DEVICE_FUNC_DEFINE_OPEN(ledgpio_led6)
{
    BSP_LED_Init(LED6);
}

DEVICE_FUNC_DEFINE_CLOSE(ledgpio_led6)
{
}

DEVICE_FUNC_DEFINE_WRITE(ledgpio_led6)
{
    if (len < 1) {
        return -1;
    }

    if (buf[0]) {
        BSP_LED_On(LED6);
    }
    else {
        BSP_LED_Off(LED6);
    }

    return 1;
}

DEVICE_FUNC_DEFINE_READ(ledgpio_led6)
{
    if (len < 1) {
        return -1;
    }
    if (GPIO_PIN_SET == BSP_LED_Read(LED6)) {
        buf[0] = 1;
    }
    else {
        buf[0] = 0;
    }

    return 1;
}
