#include <zephyr/kernel.h>
#include <zephyrInternal.h>

void _on_1200_bps() {
    uint32_t tmp = (uint32_t) & (RTC->BKP0R);
    tmp += (RTC_BKP_DR0 * 4U);
    *(__IO uint32_t *)tmp = (uint32_t)0xDF59;
    NVIC_SystemReset();
}

void initVariant(void) {
    // check the BLUE LED
    /* Set led1 inactive since the Arduino bootloader leaves it active */
    const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
    if (!gpio_is_ready_dt(&led2)) {
        return;
    }

    gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);
}
