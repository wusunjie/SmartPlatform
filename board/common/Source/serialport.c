#include "device.h"
#include "boardcfg.h"

#include "FreeRTOS.h"
#include "task.h"

static UART_HandleTypeDef UartHandle;
static DMA_HandleTypeDef hdma_tx;
static DMA_HandleTypeDef hdma_rx;
static TaskHandle_t cur = NULL;
static __IO uint16_t rxtxXferCount = 0;

DEVICE_DEFINE(USART_DEVICE_NAME, USART_DEVICE_ID);

/* Exported functions ------------------------------------------------------- */

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();
    /* Enable USART2 clock */
    USARTx_CLK_ENABLE(); 
    /* Enable DMA1 clock */
    DMAx_CLK_ENABLE();   

    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USARTx_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;

    HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = USARTx_RX_PIN;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;

    HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Configure the DMA streams ##########################################*/
    /* Configure the DMA handler for Transmission process */
    hdma_tx.Instance                 = USARTx_TX_DMA_STREAM;

    hdma_tx.Init.Channel             = USARTx_TX_DMA_CHANNEL;
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
    hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
    hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

    HAL_DMA_Init(&hdma_tx);   

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(huart, hdmatx, hdma_tx);

    /* Configure the DMA handler for Transmission process */
    hdma_rx.Instance                 = USARTx_RX_DMA_STREAM;

    hdma_rx.Init.Channel             = USARTx_RX_DMA_CHANNEL;
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_NORMAL;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
    hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
    hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4; 

    HAL_DMA_Init(&hdma_rx);

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(huart, hdmarx, hdma_rx);

    /*##-4- Configure the NVIC for DMA #########################################*/
    /* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
    HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);

    /* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
    HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USARTx_DMA_RX_IRQn);

    /* NVIC configuration for USART TC interrupt */
    HAL_NVIC_SetPriority(USARTx_IRQn, 0, 0);
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_IDLE);
    HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    /*##-1- Reset peripherals ##################################################*/
    USARTx_FORCE_RESET();
    USARTx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks #################################*/
    /* Configure UART Tx as alternate function  */
    HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
    /* Configure UART Rx as alternate function  */
    HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

    /*##-3- Disable the DMA Streams ############################################*/
    /* De-Initialize the DMA Stream associate to transmission process */
    HAL_DMA_DeInit(&hdma_tx); 
    /* De-Initialize the DMA Stream associate to reception process */
    HAL_DMA_DeInit(&hdma_rx);

    /*##-4- Disable the NVIC for DMA ###########################################*/
    HAL_NVIC_DisableIRQ(USARTx_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(USARTx_DMA_RX_IRQn);
}

DEVICE_FUNC_DEFINE_OPEN(USART_DEVICE_NAME)
{
    UartHandle.Instance          = USARTx;

    UartHandle.Init.BaudRate     = USART_DEVICE_BAUDRATE;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&UartHandle);
}

DEVICE_FUNC_DEFINE_CLOSE(USART_DEVICE_NAME)
{
    HAL_UART_DeInit(&UartHandle);
}

DEVICE_FUNC_DEFINE_WRITE(USART_DEVICE_NAME)
{
    if (!len) {
        return -1;
    }

    cur = xTaskGetCurrentTaskHandle();

    if (!cur) {
        return -1;
    }

    xTaskNotifyStateClear(cur);

    rxtxXferCount = len;

    /*##-2- Start the transmission process #####################################*/  
    /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
    if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)buf, len) != HAL_OK) {
        return -1;
    }

    /*##-3- Wait for the end of the transfer ###################################*/  
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    if (!rxtxXferCount) {
        return -1;
    }
    else {
        return rxtxXferCount;
    }

    return len;
}

DEVICE_FUNC_DEFINE_READ(USART_DEVICE_NAME)
{
    if (!len) {
        return -1;
    }

    cur = xTaskGetCurrentTaskHandle();

    if (!cur) {
        return -1;
    }

    rxtxXferCount = len;

    xTaskNotifyStateClear(cur);

    /*##-2- Start the transmission process #####################################*/  
    /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
    if(HAL_UART_Receive_DMA(&UartHandle, (uint8_t*)buf, len) != HAL_OK) {
        return -1;
    }

    /*##-3- Wait for the end of the transfer ###################################*/  
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    if (!rxtxXferCount) {
        return -1;
    }
    else {
        return rxtxXferCount;
    }
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles DMA RX interrupt request.  
  * @param  None
  * @retval None   
  */
void USARTx_DMA_RX_IRQHandler(void)
{
    HAL_DMA_IRQHandler(UartHandle.hdmarx);
}

/**
  * @brief  This function handles DMA TX interrupt request.
  * @param  None
  * @retval None   
  */
void USARTx_DMA_TX_IRQHandler(void)
{
    HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

/**
  * @brief  This function handles USARTx interrupt request.
  * @param  None
  * @retval None
  */
void USARTx_IRQHandler(void)
{
    uint32_t isrflags   = READ_REG(UartHandle.Instance->SR);
    uint32_t cr1its     = READ_REG(UartHandle.Instance->CR1);
    uint32_t errorflags = 0x00U;

    /* If no error occurs */
    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));

    if(errorflags == RESET) {
        /* UART in mode Receiver -------------------------------------------------*/
        if(((isrflags & USART_SR_IDLE) != RESET) && ((cr1its & USART_CR1_IDLEIE) != RESET)) {
            __HAL_UART_CLEAR_IDLEFLAG(&UartHandle);
            if (UartHandle.hdmarx != NULL) {
                DMA_HandleTypeDef *hdma = UartHandle.hdmarx;
                rxtxXferCount = UartHandle.RxXferSize - __HAL_DMA_GET_COUNTER(hdma);
                HAL_DMA_Abort(hdma);
                UartHandle.RxXferCount = 0;
                if (UartHandle.RxState == HAL_UART_STATE_BUSY_TX_RX) {
                    UartHandle.RxState = HAL_UART_STATE_BUSY_TX;
                }
                else {
                    UartHandle.RxState = HAL_UART_STATE_READY;
                }
                vTaskNotifyGiveFromISR(cur, NULL);
                return;
            }
        }
    }
    else if (RESET != (isrflags & (uint32_t)USART_SR_ORE)) {
        __HAL_UART_CLEAR_OREFLAG(&UartHandle);
    }

    HAL_UART_IRQHandler(&UartHandle);
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete */
    vTaskNotifyGiveFromISR(cur, NULL);
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete */
    vTaskNotifyGiveFromISR(cur, NULL);
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    rxtxXferCount = 0;
    vTaskNotifyGiveFromISR(cur, NULL);
}
