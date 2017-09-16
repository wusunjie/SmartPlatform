#include "device.h"
#include "boardcfg.h"
#include "stm32f4xx_hal.h"

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_LED STM32F4 DISCOVERY LOW LEVEL LED
  * @{
  */

#define LED_PIN                         GPIO_PIN_6
#define LED_GPIO_PORT                   GPIOF
#define LED_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()

static void BSP_LED_Init(void);

DEVICE_DEFINE(ledgpio_led, DEV_LEDGPIO_ID);

static void BSP_LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO_LED Clock */
    LED_GPIO_CLK_ENABLE();

    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);
}

DEVICE_FUNC_DEFINE_OPEN(ledgpio_led)
{
    BSP_LED_Init();
}

DEVICE_FUNC_DEFINE_CLOSE(ledgpio_led)
{
    LED_GPIO_CLK_DISABLE();
}

DEVICE_FUNC_DEFINE_WRITE(ledgpio_led)
{
    if (len < 1) {
        return -1;
    }

    if (buf[0]) {
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET);
    }
    else {
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);
    }

    return 1;
}

DEVICE_FUNC_DEFINE_READ(ledgpio_led)
{
    if (len < 1) {
        return -1;
    }
    if (GPIO_PIN_SET == HAL_GPIO_ReadPin(LED_GPIO_PORT, LED_PIN)) {
        buf[0] = 1;
    }
    else {
        buf[0] = 0;
    }

    return 1;
}

DEVICE_FUNC_DEFINE_LSEEK(ledgpio_led)
{
    return -1;
}