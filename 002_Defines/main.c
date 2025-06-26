// ====================================================================
// GD32VF103 RGB LED - OPTIMIZED VERSION USING HEADER DEFINITIONS
// Memory initialization done in boot.S
// ====================================================================

#include <stdint.h>
#include "gd32vf103.h"

// ====================================================================
// REGISTER DEFINITIONS USING HEADER MACROS
// ====================================================================

// RCC (Reset and Clock Control) Registers - Using header base addresses
#define RCU_APB2EN          REG32(RCU_BASE + 0x18U)
#define RCU_APB2EN_PAEN     BIT(2)    // GPIOA clock enable
#define RCU_APB2EN_PCEN     BIT(4)    // GPIOC clock enable

// GPIOA Registers - Using header base addresses
#define GPIOA_CTL0          REG32(GPIO_BASE + 0x00U)        // Control register low
#define GPIOA_OCTL          REG32(GPIO_BASE + 0x0CU)        // Output control register

// GPIOC Registers - Using header base addresses  
#define GPIOC_CTL1          REG32(GPIO_BASE + 0x400U + 0x04U)  // Control register high
#define GPIOC_OCTL          REG32(GPIO_BASE + 0x400U + 0x0CU)  // Output control register

// GPIO Configuration using header bit macros
#define GPIO_CTL0_MD1       BITS(4, 5)   // Pin 1 mode bits
#define GPIO_CTL0_CTL1      BITS(6, 7)   // Pin 1 config bits
#define GPIO_CTL0_MD2       BITS(8, 9)   // Pin 2 mode bits  
#define GPIO_CTL0_CTL2      BITS(10, 11) // Pin 2 config bits
#define GPIO_CTL1_MD13      BITS(20, 21) // Pin 13 mode bits
#define GPIO_CTL1_CTL13     BITS(22, 23) // Pin 13 config bits

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
// MAIN FUNCTION - Using header definitions
// ====================================================================
int main(void) {
    
    // ================================================================
    // 1. Enable GPIO clocks using header macros
    // ================================================================
    RCU_APB2EN |= (RCU_APB2EN_PAEN | RCU_APB2EN_PCEN);
    
    // ================================================================
    // 2. Configure GPIO pins as outputs using header bit manipulation
    // PA1 = Green LED, PA2 = Blue LED, PC13 = Red LED
    // ================================================================
    
    // Configure GPIOA Pin 1 (Green) as 2MHz push-pull output
    GPIOA_CTL0 &= ~(GPIO_CTL0_MD1 | GPIO_CTL0_CTL1);
    GPIOA_CTL0 |= (0x2 << 4);  // MODE1 = 10 (2MHz output)
    
    // Configure GPIOA Pin 2 (Blue) as 2MHz push-pull output  
    GPIOA_CTL0 &= ~(GPIO_CTL0_MD2 | GPIO_CTL0_CTL2);
    GPIOA_CTL0 |= (0x2 << 8);  // MODE2 = 10 (2MHz output)
    
    // Configure GPIOC Pin 13 (Red) as 2MHz push-pull output
    GPIOC_CTL1 &= ~(GPIO_CTL1_MD13 | GPIO_CTL1_CTL13);
    GPIOC_CTL1 |= (0x2 << 20); // MODE13 = 10 (2MHz output)
    
    // ================================================================
    // 3. Initialize all LEDs OFF using header bit macros
    // ================================================================
    GPIOA_OCTL |= BIT(1);   // Green LED OFF
    GPIOA_OCTL |= BIT(2);   // Blue LED OFF
    GPIOC_OCTL |= BIT(13);  // Red LED OFF
    
    // ================================================================
    // 4. Main loop - RGB color cycling using header bit macros
    // ================================================================
    #define DELAY_TIME 100000
    
    while (1) {
        // State 1: Green ON (Green color)
        GPIOA_OCTL &= ~BIT(1);  // Green LED ON
        delay_cycles(DELAY_TIME);
        
        // State 2: Red ON (Yellow = Red + Green)
        GPIOC_OCTL &= ~BIT(13); // Red LED ON  
        delay_cycles(DELAY_TIME);
        
        // State 3: Blue ON (White = Red + Green + Blue)
        GPIOA_OCTL &= ~BIT(2);  // Blue LED ON
        delay_cycles(DELAY_TIME);
        
        // State 4: Green OFF (Magenta = Red + Blue)
        GPIOA_OCTL |= BIT(1);   // Green LED OFF
        delay_cycles(DELAY_TIME);
        
        // State 5: Red OFF (Blue only)
        GPIOC_OCTL |= BIT(13);  // Red LED OFF
        delay_cycles(DELAY_TIME);
        
        // State 6: Blue OFF (All LEDs OFF)
        GPIOA_OCTL |= BIT(2);   // Blue LED OFF
        delay_cycles(DELAY_TIME);
    }
    
    return 0;
}

// ====================================================================
// End of optimized implementation using header definitions
// ====================================================================