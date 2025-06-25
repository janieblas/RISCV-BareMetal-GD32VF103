// ====================================================================
// GD32VF103 RGB LED - MINIMAL STANDALONE VERSION
// Memory initialization done in boot.S
// ====================================================================

#include <stdint.h>

// ====================================================================
// DIRECT MEMORY ADDRESSES - GD32VF103 Hardware Registers
// ====================================================================

// RCC (Reset and Clock Control) Registers
#define RCC_APB2ENR         (*(volatile uint32_t*)0x40021018)
#define RCC_APB2ENR_IOPAEN  (1 << 2)    // GPIOA clock enable
#define RCC_APB2ENR_IOPCEN  (1 << 4)    // GPIOC clock enable

// GPIOA Registers  
#define GPIOA_CRL           (*(volatile uint32_t*)0x40010800)
#define GPIOA_ODR           (*(volatile uint32_t*)0x4001080C)

// GPIOC Registers
#define GPIOC_CRH           (*(volatile uint32_t*)0x40011004) 
#define GPIOC_ODR           (*(volatile uint32_t*)0x4001100C)

// GPIO Configuration Masks
#define GPIO_CRL_MODE1      (0x3 << 4)   // Pin 1 mode bits
#define GPIO_CRL_CNF1       (0x3 << 6)   // Pin 1 config bits
#define GPIO_CRL_MODE2      (0x3 << 8)   // Pin 2 mode bits  
#define GPIO_CRL_CNF2       (0x3 << 10)  // Pin 2 config bits
#define GPIO_CRH_MODE13     (0x3 << 20)  // Pin 13 mode bits
#define GPIO_CRH_CNF13      (0x3 << 22)  // Pin 13 config bits

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
// MAIN FUNCTION - Pure hardware register manipulation
// ====================================================================
int main(void) {
    
    // ================================================================
    // 1. Enable GPIO clocks via direct register access
    // ================================================================
    RCC_APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN);
    
    // ================================================================
    // 2. Configure GPIO pins as outputs via direct register access
    // PA1 = Green LED, PA2 = Blue LED, PC13 = Red LED
    // ================================================================
    
    // Configure GPIOA Pin 1 (Green) as 2MHz push-pull output
    GPIOA_CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
    GPIOA_CRL |= (0x2 << 4);  // MODE1 = 10 (2MHz output)
    
    // Configure GPIOA Pin 2 (Blue) as 2MHz push-pull output  
    GPIOA_CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
    GPIOA_CRL |= (0x2 << 8);  // MODE2 = 10 (2MHz output)
    
    // Configure GPIOC Pin 13 (Red) as 2MHz push-pull output
    GPIOC_CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC_CRH |= (0x2 << 20); // MODE13 = 10 (2MHz output)
    
    // ================================================================
    // 3. Initialize all LEDs OFF (High = LED OFF for common cathode)
    // ================================================================
    GPIOA_ODR |= (1 << 1);   // Green LED OFF
    GPIOA_ODR |= (1 << 2);   // Blue LED OFF
    GPIOC_ODR |= (1 << 13);  // Red LED OFF
    
    // ================================================================
    // 4. Main loop - RGB color cycling via direct register writes
    // ================================================================
    #define DELAY_TIME 100000
    
    while (1) {
        // State 1: Green ON (Green color)
        GPIOA_ODR &= ~(1 << 1);  // Green LED ON
        delay_cycles(DELAY_TIME);
        
        // State 2: Red ON (Yellow = Red + Green)
        GPIOC_ODR &= ~(1 << 13); // Red LED ON  
        delay_cycles(DELAY_TIME);
        
        // State 3: Blue ON (White = Red + Green + Blue)
        GPIOA_ODR &= ~(1 << 2);  // Blue LED ON
        delay_cycles(DELAY_TIME);
        
        // State 4: Green OFF (Magenta = Red + Blue)
        GPIOA_ODR |= (1 << 1);   // Green LED OFF
        delay_cycles(DELAY_TIME);
        
        // State 5: Red OFF (Blue only)
        GPIOC_ODR |= (1 << 13);  // Red LED OFF
        delay_cycles(DELAY_TIME);
        
        // State 6: Blue OFF (All LEDs OFF)
        GPIOA_ODR |= (1 << 2);   // Blue LED OFF
        delay_cycles(DELAY_TIME);
    }
    
    return 0;
}

// ====================================================================
// End of minimal standalone implementation
// ====================================================================