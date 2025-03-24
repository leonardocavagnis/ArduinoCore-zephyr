/*
 * Copyright (c) 2024 Espressif Systems (Shanghai) Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define SDMMC_FREQ_DEFAULT   20000
#define SDMMC_FREQ_HIGHSPEED 40000
#define SDMMC_FREQ_PROBING   400
#define SDMMC_FREQ_52M       52000
#define SDMMC_FREQ_26M       26000

#define SDMMC_DATA_ERR_MASK                                                                        \
	(uint32_t)(SDMMC_INTMASK_DTO | SDMMC_INTMASK_DCRC | SDMMC_INTMASK_HTO |                    \
		   SDMMC_INTMASK_SBE | SDMMC_INTMASK_EBE)

#define SDMMC_DMA_DONE_MASK                                                                        \
	(uint32_t)(SDMMC_IDMAC_INTMASK_RI | SDMMC_IDMAC_INTMASK_TI | SDMMC_IDMAC_INTMASK_NI)

#define SDMMC_CMD_ERR_MASK                                                                         \
	(uint32_t)(SDMMC_INTMASK_RTO | SDMMC_INTMASK_RCRC | SDMMC_INTMASK_RESP_ERR)

enum sdmmc_req_state {
	SDMMC_IDLE,
	SDMMC_SENDING_CMD,
	SDMMC_SENDING_DATA,
	SDMMC_BUSY,
};

/* SDHC command flags */
#define SCF_ITSDONE     0x0001
#define SCF_CMD(flags)  ((flags) & 0x00f0)
#define SCF_CMD_AC      0x0000
#define SCF_CMD_ADTC    0x0010
#define SCF_CMD_BC      0x0020
#define SCF_CMD_BCR     0x0030
#define SCF_CMD_READ    0x0040
#define SCF_RSP_BSY     0x0100
#define SCF_RSP_136     0x0200
#define SCF_RSP_CRC     0x0400
#define SCF_RSP_IDX     0x0800
#define SCF_RSP_PRESENT 0x1000
/* response types */
#define SCF_RSP_R0      0
#define SCF_RSP_R1      (SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_IDX)
#define SCF_RSP_R1B     (SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_IDX | SCF_RSP_BSY)
#define SCF_RSP_R2      (SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_136)
#define SCF_RSP_R3      (SCF_RSP_PRESENT)
#define SCF_RSP_R4      (SCF_RSP_PRESENT)
#define SCF_RSP_R5      (SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_IDX)
#define SCF_RSP_R5B     (SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_IDX | SCF_RSP_BSY)
#define SCF_RSP_R6      (SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_IDX)
#define SCF_RSP_R7      (SCF_RSP_PRESENT | SCF_RSP_CRC | SCF_RSP_IDX)
/* special flags */
#define SCF_WAIT_BUSY   0x2000

#define SD_OCR_SDHC_CAP (1 << 30)
#define SD_OCR_VOL_MASK 0xFF8000

/* For debug only */
static const char *const timingStr[] = {"UNKNOWN", "LEGACY", "HS",    "SDR12", "SDR25", "SDR50",
					"SDR104",  "DDR50",  "DDR52", "HS200", "HS400"};

struct sdmmc_transfer_state {
	uint8_t *ptr;
	size_t size_remaining;
	size_t next_desc;
	size_t desc_remaining;
};

struct sdmmc_event {
	uint32_t header_DUMMY;
	uint32_t sdmmc_status;
	uint32_t dma_status;
};

/**
 * Host contexts
 */
struct host_ctx {
	intr_handle_t intr_handle;
	struct k_msgq *event_queue;
};

/**
 * SD/MMC command information
 */
struct sdmmc_command {
	uint32_t opcode;
	uint32_t arg;
	uint32_t response[4];
	void *data;
	size_t datalen;
	size_t buflen;
	size_t blklen;
	int flags;
	esp_err_t error;
	uint32_t timeout_ms;
};

/**
 * @brief Convert ESP to Zephyr error codes
 *
 * @param ret_esp ESP return value
 *
 * @return Zephyr error code
 */
static __attribute__((always_inline)) inline int err_esp2zep(int ret_esp)
{
	int ret;

	switch (ret_esp) {
	/* Treating the error codes most relevant to be individuated */
	case ESP_ERR_INVALID_ARG:
		ret = -EINVAL;
		break;
	case ESP_ERR_TIMEOUT:
		ret = -ETIMEDOUT;
		break;
	case ESP_ERR_NOT_FOUND:
		ret = -ENODEV;
		break;
	case ESP_ERR_INVALID_STATE:
		ret = -EACCES;
		break;
	default:
		ret = -EIO;
		break;
	}

	return ret;
}
