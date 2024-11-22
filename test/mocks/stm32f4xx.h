#include "stm32f4xx_hal.h"

// Mock implementation of HAL_Init
HAL_StatusTypeDef HAL_Init(void) {
    return HAL_OK;
}

// Mock implementation of HAL_DeInit
HAL_StatusTypeDef HAL_DeInit(void) {
    return HAL_OK;
}

// Mock implementation of HAL_MspInit
__weak void HAL_MspInit(void) {
    // Do nothing
}

// Mock implementation of HAL_MspDeInit
__weak void HAL_MspDeInit(void) {
    // Do nothing
}

// Mock implementation of HAL_InitTick
__weak HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    return HAL_OK;
}

// Mock implementation of HAL_IncTick
__weak void HAL_IncTick(void) {
    // Do nothing
}

// Mock implementation of HAL_GetTick
__weak uint32_t HAL_GetTick(void) {
    return 0;
}

// Mock implementation of HAL_GetTickPrio
uint32_t HAL_GetTickPrio(void) {
    return 0;
}

// Mock implementation of HAL_SetTickFreq
HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq) {
    return HAL_OK;
}

// Mock implementation of HAL_GetTickFreq
HAL_TickFreqTypeDef HAL_GetTickFreq(void) {
    return HAL_TICK_FREQ_DEFAULT;
}

// Mock implementation of HAL_Delay
__weak void HAL_Delay(uint32_t Delay) {
    // Do nothing
}

// Mock implementation of HAL_SuspendTick
__weak void HAL_SuspendTick(void) {
    // Do nothing
}

// Mock implementation of HAL_ResumeTick
__weak void HAL_ResumeTick(void) {
    // Do nothing
}

// Mock implementation of HAL_GetHalVersion
uint32_t HAL_GetHalVersion(void) {
    return __STM32F4xx_HAL_VERSION;
}

// Mock implementation of HAL_GetREVID
uint32_t HAL_GetREVID(void) {
    return 0;
}

// Mock implementation of HAL_GetDEVID
uint32_t HAL_GetDEVID(void) {
    return 0;
}

// Mock implementation of HAL_DBGMCU_EnableDBGSleepMode
void HAL_DBGMCU_EnableDBGSleepMode(void) {
    // Do nothing
}

// Mock implementation of HAL_DBGMCU_DisableDBGSleepMode
void HAL_DBGMCU_DisableDBGSleepMode(void) {
    // Do nothing
}

// Mock implementation of HAL_DBGMCU_EnableDBGStopMode
void HAL_DBGMCU_EnableDBGStopMode(void) {
    // Do nothing
}

// Mock implementation of HAL_DBGMCU_DisableDBGStopMode
void HAL_DBGMCU_DisableDBGStopMode(void) {
    // Do nothing
}

// Mock implementation of HAL_DBGMCU_EnableDBGStandbyMode
void HAL_DBGMCU_EnableDBGStandbyMode(void) {
    // Do nothing
}

// Mock implementation of HAL_DBGMCU_DisableDBGStandbyMode
void HAL_DBGMCU_DisableDBGStandbyMode(void) {
    // Do nothing
}

// Mock implementation of HAL_EnableCompensationCell
void HAL_EnableCompensationCell(void) {
    // Do nothing
}

// Mock implementation of HAL_DisableCompensationCell
void HAL_DisableCompensationCell(void) {
    // Do nothing
}

// Mock implementation of HAL_GetUIDw0
uint32_t HAL_GetUIDw0(void) {
    return 0;
}

// Mock implementation of HAL_GetUIDw1
uint32_t HAL_GetUIDw1(void) {
    return 0;
}

// Mock implementation of HAL_GetUIDw2
uint32_t HAL_GetUIDw2(void) {
    return 0;
}

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx)|| defined(STM32F439xx) ||\
    defined(STM32F469xx) || defined(STM32F479xx)
// Mock implementation of HAL_EnableMemorySwappingBank
void HAL_EnableMemorySwappingBank(void) {
    // Do nothing
}

// Mock implementation of HAL_DisableMemorySwappingBank
void HAL_DisableMemorySwappingBank(void) {
    // Do nothing
}
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */