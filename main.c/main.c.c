#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "display.h"  

// Defines
#define BUTTON_A_PIN 5
#define SW 22
#define VRX 27
#define VRY 26
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12
#define RED_LED_PIN 13
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define display_ADDRESS 0x3C
#define SQUARE_SIZE 8
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// Variáveis Globais
display_t ssd;  // Estrutura do display display
uint slice_led_b, slice_led_r;
uint16_t vrx_value, vry_value;
int square_x = DISPLAY_WIDTH / 2 - SQUARE_SIZE / 2;
int square_y = DISPLAY_HEIGHT / 2 - SQUARE_SIZE / 2;
uint16_t led_b_level = 0, led_r_level = 0;
volatile bool state_led = true;
volatile uint8_t state_border = 0;
absolute_time_t debounce;

// Funções
void button_init(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
}

void setup_joystick() {
    adc_init();
    adc_gpio_init(VRY);
    adc_gpio_init(VRX);
}

void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value) {
    adc_select_input(ADC_CHANNEL_0);
    sleep_us(2);
    *vry_value = adc_read();
    adc_select_input(ADC_CHANNEL_1);
    sleep_us(2);
    *vrx_value = adc_read();
}

void led_init(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
}

void pwm_led_setup(uint led, uint *slice, uint16_t level) {
    gpio_set_function(led, GPIO_FUNC_PWM);
    *slice = pwm_gpio_to_slice_num(led);
    pwm_set_clkdiv(*slice, 4.0);
    pwm_set_wrap(*slice, 4095);
    pwm_set_gpio_level(led, 0);
    pwm_set_enabled(*slice, true);
}

void button_irq_handler(uint gpio, uint32_t events) {
    if (time_reached(debounce)) {
        if (gpio == SW) {
            state_border = (state_border + 1) % 4;  // Alterna entre 0, 1, 2 e 3
            gpio_put(GREEN_LED_PIN, !gpio_get(GREEN_LED_PIN));
        } else if (gpio == BUTTON_A_PIN) {
            state_led = !state_led;
        }
        debounce = delayed_by_ms(get_absolute_time(), 200);
    }
}


int main() {
    stdio_init_all();
    button_init(SW);
    button_init(BUTTON_A_PIN);
    setup_joystick();
    pwm_led_setup(BLUE_LED_PIN, &slice_led_b, led_b_level);
    pwm_led_setup(RED_LED_PIN, &slice_led_r, led_r_level);
    led_init(GREEN_LED_PIN);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    display_init(&ssd, DISPLAY_WIDTH, DISPLAY_HEIGHT, false, display_ADDRESS, I2C_PORT);
    display_config(&ssd);

    gpio_set_irq_enabled_with_callback(SW, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
    debounce = delayed_by_ms(get_absolute_time(), 200);

    while (true) {
        joystick_read_axis(&vrx_value, &vry_value);
        update_square_position(&square_x, &square_y, vrx_value, vry_value);

        if (state_led) {
            led_r_level = (vrx_value >= 2400 || vrx_value <= 2120) ? abs(vrx_value - 2047) * 2 : 0;
            led_b_level = (vry_value >= 2100 || vry_value <= 1850) ? abs(vry_value - 2047) * 2 : 0;
        } else {
            led_r_level = 0;
            led_b_level = 0;
        }
        pwm_set_gpio_level(RED_LED_PIN, led_r_level);
        pwm_set_gpio_level(BLUE_LED_PIN, led_b_level);

        display_fill(&ssd, false);

        if (state_border == 0) {
            display_rect(&ssd, 3, 3, 122, 58, true);
        } else if (state_border == 1) {
            display_rect_hearts(&ssd, 3, 3, 122, 58, true);
        } else if (state_border == 2) {
            display_rect_stars(&ssd, 3, 3, 122, 58, true);
        } else {
            display_rect_squares(&ssd, 3, 3, 122, 58, true);
        }

        display_rect(&ssd, square_x, square_y, SQUARE_SIZE, SQUARE_SIZE, true);
        display_send_data(&ssd);

        sleep_ms(40);
    }
}
