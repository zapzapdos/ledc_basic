/* LEDC (LED Controller) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//Configurando o PWM 1 
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO_1        (5) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_1
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Resolução do timer de 13 bits
#define LEDC_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_FREQUENCY          (5000) // Frequência do PWM em 5kHz

//Configurando o PWM 2
#define LEDC_OUTPUT_IO_2        (4) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_2

//Configurando a máscara do LED
#define GPIO_OUTPUT_IO_LED0  16
#define GPIO_OUTPUT_IO_LED1  32
#define GPIO_LED_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_LED0) | (1ULL<<GPIO_OUTPUT_IO_LED1))



static const char *TAG = "example";
static SemaphoreHandle_t semaphore_pwm = NULL;


/* Warning:
 * For ESP32, ESP32S2, ESP32S3, ESP32C3, ESP32C2, ESP32C6, ESP32H2, ESP32P4 targets,
 * when LEDC_DUTY_RES selects the maximum duty resolution (i.e. value equal to SOC_LEDC_TIMER_BIT_WIDTH),
 * 100% duty cycle is not reachable (duty cannot be set to (2 ** SOC_LEDC_TIMER_BIT_WIDTH)).
 */

static void example_ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void app_main(void)
{
    
    uint32_t duty_teste = 2048;
    bool cresce = true;
    // Set the LEDC peripheral configuration
    example_ledc_init();
    // Set duty to 50%
    while(1){
        vTaskDelay(pdMS_TO_TICKS(10));
        if(duty_teste>= 8000 && cresce){
            cresce = false;
        }
        if(cresce){
            duty_teste = duty_teste + 20;
        }
        if(!cresce){
            duty_teste = duty_teste - 20;
        }
        if(duty_teste< 200 && !cresce){
            cresce = true;
        }
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty_teste));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
        
    }
    
    // Update duty to apply the new value
    
}
