// ====================================================================
// GD32VF103 RGB LED - HIGH-LEVEL API VERSION
// Using official GD32VF103 RCU and GPIO libraries
// ====================================================================

#include <stdint.h>
#include "gd32vf103.h"

// ====================================================================
// LED Pin Definitions
// ====================================================================
#define LED_GREEN_PORT      GPIOA
#define LED_GREEN_PIN       GPIO_PIN_1

#define LED_BLUE_PORT       GPIOA  
#define LED_BLUE_PIN        GPIO_PIN_2

#define LED_RED_PORT        GPIOC
#define LED_RED_PIN         GPIO_PIN_13

// ====================================================================
// Delay function
// ====================================================================
__attribute__((optimize("O0")))
void delay_cycles(uint32_t cycles) {
    for (uint32_t i = 0; i < cycles; i++) {
        __asm__("nop");
    }
}

// ====================================================================
// LED Control Functions using GPIO API
// ====================================================================
void led_init(void) {
    // Enable GPIO clocks using RCU API
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    
    // Configure GPIO pins using GPIO API
    // Green LED (PA1) - Push-pull output, 2MHz
    gpio_init(LED_GREEN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LED_GREEN_PIN);
    
    // Blue LED (PA2) - Push-pull output, 2MHz  
    gpio_init(LED_BLUE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LED_BLUE_PIN);
    
    // Red LED (PC13) - Push-pull output, 2MHz
    gpio_init(LED_RED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LED_RED_PIN);
    
    // Initialize all LEDs OFF (High = OFF for common cathode)
    gpio_bit_set(LED_GREEN_PORT, LED_GREEN_PIN);   // Green OFF
    gpio_bit_set(LED_BLUE_PORT, LED_BLUE_PIN);     // Blue OFF  
    gpio_bit_set(LED_RED_PORT, LED_RED_PIN);       // Red OFF
}

void led_green_on(void) {
    gpio_bit_reset(LED_GREEN_PORT, LED_GREEN_PIN);
}

void led_green_off(void) {
    gpio_bit_set(LED_GREEN_PORT, LED_GREEN_PIN);
}

void led_blue_on(void) {
    gpio_bit_reset(LED_BLUE_PORT, LED_BLUE_PIN);
}

void led_blue_off(void) {
    gpio_bit_set(LED_BLUE_PORT, LED_BLUE_PIN);
}

void led_red_on(void) {
    gpio_bit_reset(LED_RED_PORT, LED_RED_PIN);
}

void led_red_off(void) {
    gpio_bit_set(LED_RED_PORT, LED_RED_PIN);
}

// ====================================================================
// MAIN FUNCTION - Using high-level GPIO API
// ====================================================================
int main(void) {
    
    // ================================================================
    // 1. Initialize LEDs using high-level API
    // ================================================================
    led_init();
    
    // ================================================================
    // 2. Main loop - RGB color cycling using GPIO API functions
    // ================================================================
    #define DELAY_TIME 100000
    
    while (1) {
        // State 1: Green ON (Green color)
        led_green_on();
        delay_cycles(DELAY_TIME);
        
        // State 2: Red ON (Yellow = Red + Green)
        led_red_on();
        delay_cycles(DELAY_TIME);
        
        // State 3: Blue ON (White = Red + Green + Blue)
        led_blue_on();
        delay_cycles(DELAY_TIME);
        
        // State 4: Green OFF (Magenta = Red + Blue)
        led_green_off();
        delay_cycles(DELAY_TIME);
        
        // State 5: Red OFF (Blue only)
        led_red_off();
        delay_cycles(DELAY_TIME);
        
        // State 6: Blue OFF (All LEDs OFF)
        led_blue_off();
        delay_cycles(DELAY_TIME);
    }
    
    return 0;
}

// ====================================================================
// End of high-level API implementation
// ====================================================================