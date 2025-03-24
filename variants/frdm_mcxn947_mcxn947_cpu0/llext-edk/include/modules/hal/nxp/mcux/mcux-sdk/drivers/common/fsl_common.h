/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022,2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSL_COMMON_H_
#define FSL_COMMON_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#if defined(__ICCARM__) || (defined(__CC_ARM) || defined(__ARMCC_VERSION)) || defined(__GNUC__)
#include <stddef.h>
#endif

#include "fsl_device_registers.h"

/*!
 * @addtogroup ksdk_common
 * @{
 */

/*******************************************************************************
 * Configurations
 ******************************************************************************/

/*! @brief Macro to use the default weak IRQ handler in drivers. */
#ifndef FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ
#define FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ 1
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group)*100L) + (code)))

/*! @brief Construct the version number for drivers.
 *
 * The driver version is a 32-bit number, for both 32-bit platforms(such as Cortex M)
 * and 16-bit platforms(such as DSC).
 *
 * @verbatim

   | Unused    || Major Version || Minor Version ||  Bug Fix    |
   31        25  24           17  16            9  8            0

   @endverbatim
 */
#define MAKE_VERSION(major, minor, bugfix) (((major)*65536L) + ((minor)*256L) + (bugfix))

/*! @name Driver version */
/*! @{ */
/*! @brief common driver version. */
#define FSL_COMMON_DRIVER_VERSION (MAKE_VERSION(2, 4, 3))
/*! @} */

/*! @name Debug console type definition. */
/*! @{ */
#define DEBUG_CONSOLE_DEVICE_TYPE_NONE       0U
#define DEBUG_CONSOLE_DEVICE_TYPE_UART       1U
#define DEBUG_CONSOLE_DEVICE_TYPE_LPUART     2U
#define DEBUG_CONSOLE_DEVICE_TYPE_LPSCI      3U
#define DEBUG_CONSOLE_DEVICE_TYPE_USBCDC     4U
#define DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM   5U
#define DEBUG_CONSOLE_DEVICE_TYPE_IUART      6U
#define DEBUG_CONSOLE_DEVICE_TYPE_VUSART     7U
#define DEBUG_CONSOLE_DEVICE_TYPE_MINI_USART 8U
#define DEBUG_CONSOLE_DEVICE_TYPE_SWO        9U
#define DEBUG_CONSOLE_DEVICE_TYPE_QSCI       10U
/*! @} */

/*! @brief Status group numbers. */
enum _status_groups
{
    kStatusGroup_Generic               = 0,
    kStatusGroup_FLASH                 = 1,
    kStatusGroup_LPSPI                 = 4,
    kStatusGroup_FLEXIO_SPI            = 5,
    kStatusGroup_DSPI                  = 6,
    kStatusGroup_FLEXIO_UART           = 7,
    kStatusGroup_FLEXIO_I2C            = 8,
    kStatusGroup_LPI2C                 = 9,
    kStatusGroup_UART                  = 10,
    kStatusGroup_I2C                   = 11,
    kStatusGroup_LPSCI                 = 12,
    kStatusGroup_LPUART                = 13,
    kStatusGroup_SPI                   = 14,
    kStatusGroup_XRDC                  = 15,
    kStatusGroup_SEMA42                = 16,
    kStatusGroup_SDHC                  = 17,
    kStatusGroup_SDMMC                 = 18,
    kStatusGroup_SAI                   = 19,
    kStatusGroup_MCG                   = 20,
    kStatusGroup_SCG                   = 21,
    kStatusGroup_SDSPI                 = 22,
    kStatusGroup_FLEXIO_I2S            = 23,
    kStatusGroup_FLEXIO_MCULCD         = 24,
    kStatusGroup_FLASHIAP              = 25,
    kStatusGroup_FLEXCOMM_I2C          = 26,
    kStatusGroup_I2S                   = 27,
    kStatusGroup_IUART                 = 28,
    kStatusGroup_CSI                   = 29,
    kStatusGroup_MIPI_DSI              = 30,
    kStatusGroup_SDRAMC                = 35,
    kStatusGroup_POWER                 = 39,
    kStatusGroup_ENET                  = 40,
    kStatusGroup_PHY                   = 41,
    kStatusGroup_TRGMUX                = 42,
    kStatusGroup_SMARTCARD             = 43,
    kStatusGroup_LMEM                  = 44,
    kStatusGroup_QSPI                  = 45,
    kStatusGroup_DMA                   = 50,
    kStatusGroup_EDMA                  = 51,
    kStatusGroup_DMAMGR                = 52,
    kStatusGroup_FLEXCAN               = 53,
    kStatusGroup_LTC                   = 54,
    kStatusGroup_FLEXIO_CAMERA         = 55,
    kStatusGroup_LPC_SPI               = 56,
    kStatusGroup_LPC_USART             = 57,
    kStatusGroup_DMIC                  = 58,
    kStatusGroup_SDIF                  = 59,
    kStatusGroup_SPIFI                 = 60,
    kStatusGroup_OTP                   = 61,
    kStatusGroup_MCAN                  = 62,
    kStatusGroup_CAAM                  = 63,
    kStatusGroup_ECSPI                 = 64,
    kStatusGroup_USDHC                 = 65,
    kStatusGroup_LPC_I2C               = 66,
    kStatusGroup_DCP                   = 67,
    kStatusGroup_MSCAN                 = 68,
    kStatusGroup_ESAI                  = 69,
    kStatusGroup_FLEXSPI               = 70,
    kStatusGroup_MMDC                  = 71,
    kStatusGroup_PDM                   = 72,
    kStatusGroup_SDMA                  = 73,
    kStatusGroup_ICS                   = 74,
    kStatusGroup_SPDIF                 = 75,
    kStatusGroup_LPC_MINISPI           = 76,
    kStatusGroup_HASHCRYPT             = 77,
    kStatusGroup_LPC_SPI_SSP           = 78,
    kStatusGroup_I3C                   = 79,
    kStatusGroup_LPC_I2C_1             = 97,
    kStatusGroup_NOTIFIER              = 98,
    kStatusGroup_DebugConsole          = 99,
    kStatusGroup_SEMC                  = 100,
    kStatusGroup_ApplicationRangeStart = 101,
    kStatusGroup_IAP                   = 102,
    kStatusGroup_SFA                   = 103,
    kStatusGroup_SPC                   = 104,
    kStatusGroup_PUF                   = 105,
    kStatusGroup_TOUCH_PANEL           = 106,
    kStatusGroup_VBAT                  = 107,
    kStatusGroup_XSPI                  = 108,
    kStatusGroup_PNGDEC                = 109,
    kStatusGroup_JPEGDEC               = 110,

    kStatusGroup_HAL_GPIO       = 121,
    kStatusGroup_HAL_UART       = 122,
    kStatusGroup_HAL_TIMER      = 123,
    kStatusGroup_HAL_SPI        = 124,
    kStatusGroup_HAL_I2C        = 125,
    kStatusGroup_HAL_FLASH      = 126,
    kStatusGroup_HAL_PWM        = 127,
    kStatusGroup_HAL_RNG        = 128,
    kStatusGroup_HAL_I2S        = 129,
    kStatusGroup_HAL_ADC_SENSOR = 130,
    kStatusGroup_TIMERMANAGER   = 135,
    kStatusGroup_SERIALMANAGER  = 136,
    kStatusGroup_LED            = 137,
    kStatusGroup_BUTTON         = 138,
    kStatusGroup_EXTERN_EEPROM  = 139,
    kStatusGroup_SHELL          = 140,
    kStatusGroup_MEM_MANAGER    = 141,
    kStatusGroup_LIST           = 142,
    kStatusGroup_OSA            = 143,
    kStatusGroup_COMMON_TASK    = 144,
    kStatusGroup_MSG            = 145,
    kStatusGroup_SDK_OCOTP      = 146,
    kStatusGroup_SDK_FLEXSPINOR = 147,
    kStatusGroup_CODEC          = 148,
    kStatusGroup_ASRC           = 149,
    kStatusGroup_OTFAD          = 150,
    kStatusGroup_SDIOSLV        = 151,
    kStatusGroup_MECC           = 152,
    kStatusGroup_ENET_QOS       = 153,
    kStatusGroup_LOG            = 154,
    kStatusGroup_I3CBUS         = 155,
    kStatusGroup_QSCI           = 156,
    kStatusGroup_ELEMU          = 157,
    kStatusGroup_QUEUEDSPI      = 158,
    kStatusGroup_POWER_MANAGER  = 159,
    kStatusGroup_IPED           = 160,
    kStatusGroup_ELS_PKC        = 161,
    kStatusGroup_CSS_PKC        = 162,
    kStatusGroup_HOSTIF         = 163,
    kStatusGroup_CLIF           = 164,
    kStatusGroup_BMA            = 165,
    kStatusGroup_NETC           = 166,
    kStatusGroup_ELE            = 167,
    kStatusGroup_GLIKEY         = 168,
    kStatusGroup_AON_POWER      = 169,
    kStatusGroup_AON_COMMON     = 170,
};

/*! \public
 * @brief Generic status return codes.
 */
enum
{
    kStatus_Success         = MAKE_STATUS(kStatusGroup_Generic, 0),
    kStatus_Fail            = MAKE_STATUS(kStatusGroup_Generic, 1),
    kStatus_ReadOnly        = MAKE_STATUS(kStatusGroup_Generic, 2),
    kStatus_OutOfRange      = MAKE_STATUS(kStatusGroup_Generic, 3),
    kStatus_InvalidArgument = MAKE_STATUS(kStatusGroup_Generic, 4),
    kStatus_Timeout         = MAKE_STATUS(kStatusGroup_Generic, 5),
    kStatus_NoTransferInProgress =
        MAKE_STATUS(kStatusGroup_Generic, 6),
    kStatus_Busy = MAKE_STATUS(kStatusGroup_Generic, 7),
    kStatus_NoData =
        MAKE_STATUS(kStatusGroup_Generic, 8),
};

/*! @brief Type used for all status and error return values. */
typedef int32_t status_t;

#ifdef __ZEPHYR__
#include <zephyr/sys/util.h>
#else
/*!
 * @name Min/max macros
 * @{
 */
#if !defined(MIN)
/*! Computes the minimum of \a a and \a b. */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#if !defined(MAX)
/*! Computes the maximum of \a a and \a b. */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
/*! @} */

/*! @brief Computes the number of elements in an array. */
#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif
#endif /* __ZEPHYR__ */

/*! @name UINT16_MAX/UINT32_MAX value */
/*! @{ */
#if !defined(UINT16_MAX)
/*! Max value of uint16_t type. */
#define UINT16_MAX ((uint16_t)-1)
#endif

#if !defined(UINT32_MAX)
/*! Max value of uint32_t type. */
#define UINT32_MAX ((uint32_t)-1)
#endif
/*! @} */

/*! @name UINTPTR_SIZE value */
/* @{ */
#if !defined(UINTPTR_SIZE)
#if UINTPTR_MAX > UINT32_MAX
  #define UINTPTR_SIZE 8
#elif UINTPTR_MAX > UINT16_MAX
  #define UINTPTR_SIZE 4
#else
  #error "UINTPTR_SIZE is unknown!"
#endif
#endif
/* @} */

/*! Macro to get upper 32 bits of a 64-bit value */
#if !defined(UINT64_H)
#define UINT64_H(X)        ((uint32_t)((((uint64_t) (X)) >> 32U) & 0x0FFFFFFFFULL))
#endif

/*! Macro to get lower 32 bits of a 64-bit value */
#if !defined(UINT64_L)
#define UINT64_L(X)        ((uint32_t)(((uint64_t) (X)) & 0x0FFFFFFFFULL))
#endif

/*!
 * @def SUPPRESS_FALL_THROUGH_WARNING()
 *
 * For switch case code block, if case section ends without "break;" statement, there wil be
 * fallthrough warning with compiler flag -Wextra or -Wimplicit-fallthrough=n when using armgcc.
 * To suppress this warning, "SUPPRESS_FALL_THROUGH_WARNING();" need to be added at the end of each
 * case section which misses "break;"statement.
 */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
#define SUPPRESS_FALL_THROUGH_WARNING() __attribute__((fallthrough))
#else
#define SUPPRESS_FALL_THROUGH_WARNING()
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

#if !((defined(__DSC__) && defined(__CW__)))
/*!
 * @brief Allocate memory with given alignment and aligned size.
 *
 * This is provided to support the dynamically allocated memory
 * used in cache-able region.
 * @param size The length required to malloc.
 * @param alignbytes The alignment size.
 * @retval The allocated memory.
 */
void *SDK_Malloc(size_t size, size_t alignbytes);

/*!
 * @brief Free memory.
 *
 * @param ptr The memory to be release.
 */
void SDK_Free(void *ptr);
#endif

/*!
 * @brief Delay at least for some time.
 *  Please note that, this API uses while loop for delay, different run-time environments make the time not precise,
 *  if precise delay count was needed, please implement a new delay function with hardware timer.
 *
 * @param delayTime_us  Delay time in unit of microsecond.
 * @param coreClock_Hz  Core clock frequency with Hz.
 */
void SDK_DelayAtLeastUs(uint32_t delayTime_us, uint32_t coreClock_Hz);

#if defined(__cplusplus)
}
#endif

/*! @} */

#if (defined(__DSC__) && defined(__CW__))
#include "fsl_common_dsc.h"
#elif defined(__XTENSA__)
#include "fsl_common_dsp.h"
#elif defined(__riscv)
#include "fsl_common_riscv.h"
#else
#include "fsl_common_arm.h"
#endif

#endif /* FSL_COMMON_H_ */
