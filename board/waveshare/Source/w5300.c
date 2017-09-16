#include "stm32f4xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#include "device.h"
#include "boardcfg.h"

static SRAM_HandleTypeDef SRAMHandle;
static DMA_HandleTypeDef hdma_tx;
static TaskHandle_t cur = NULL;
static __IO uint8_t *current = NULL;
static __IO uint16_t rxtxXferCount = 0;

DEVICE_DEFINE(W5300, DEV_W5300_ID);

/**
  * @brief  SRAM MSP Init.
  * @param  hsram: pointer to a SRAM_HandleTypeDef structure that contains
  *                the configuration information for SRAM module.
  * @retval None
  */
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    __HAL_RCC_DMA2_CLK_ENABLE();

    __HAL_DMA_RESET_HANDLE_STATE(&hdma_tx);

	/*##-3- Configure the DMA streams ##########################################*/
    /* Configure the DMA handler for Transmission process */
    hdma_tx.Instance                 = DMA2_Stream0;

    hdma_tx.Init.Channel             = DMA_CHANNEL_1;
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_MEMORY;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
    hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

    HAL_DMA_Init(&hdma_tx);

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(hsram, hdma, hdma_tx);

    /*##-4- Configure the NVIC for DMA #########################################*/
    /* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/**
  * @brief  SRAM MSP DeInit.
  * @param  hsram: pointer to a SRAM_HandleTypeDef structure that contains
  *                the configuration information for SRAM module.
  * @retval None
  */
void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef *hsram)
{
    HAL_DMA_DeInit(&hdma_tx);

    HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);
}

/**
  * @brief  DMA transfer complete callback.
  * @param  hdma: pointer to a SRAM_HandleTypeDef structure that contains
  *                the configuration information for SRAM module.
  * @retval None
  */
void HAL_SRAM_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{

}

/**
  * @brief  DMA transfer complete error callback.
  * @param  hdma: pointer to a SRAM_HandleTypeDef structure that contains
  *                the configuration information for SRAM module.
  * @retval None
  */
void HAL_SRAM_DMA_XferErrorCallback(DMA_HandleTypeDef *hdma)
{

}

void DMA2_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(SRAMHandle.hdma);
}

DEVICE_FUNC_DEFINE_OPEN(W5300)
{
    FMC_NORSRAM_TimingTypeDef timing;

    timing.AddressSetupTime = 1;
    timing.AddressHoldTime = 1;
    timing.DataSetupTime = 2;
    timing.BusTurnAroundDuration = 0;
    timing.CLKDivision = 0;
    timing.DataLatency = 0;
    timing.AccessMode = FSMC_ACCESS_MODE_A;

    __HAL_SRAM_RESET_HANDLE_STATE(&SRAMHandle);

    SRAMHandle.Init.NSBank = FSMC_NORSRAM_BANK3;
    SRAMHandle.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    SRAMHandle.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    SRAMHandle.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    SRAMHandle.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    SRAMHandle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    SRAMHandle.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
    SRAMHandle.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    SRAMHandle.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    SRAMHandle.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    SRAMHandle.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    SRAMHandle.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    SRAMHandle.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;

    HAL_SRAM_Init(&SRAMHandle, &timing, NULL);
}

DEVICE_FUNC_DEFINE_CLOSE(W5300)
{
    HAL_SRAM_DeInit(&SRAMHandle);
}

DEVICE_FUNC_DEFINE_WRITE(W5300)
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
    if(HAL_SRAM_Write_DMA(&SRAMHandle, (uint32_t *)current, (uint32_t *)buf, len) != HAL_OK) {
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

DEVICE_FUNC_DEFINE_READ(W5300)
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
    if(HAL_SRAM_Read_DMA(&SRAMHandle, (uint32_t *)current, (uint32_t *)buf, len) != HAL_OK) {
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

DEVICE_FUNC_DEFINE_LSEEK(W5300)
{
    return -1;
}
