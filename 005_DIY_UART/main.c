// ====================================================================
// GD32VF103 RGB LED Control via Serial Commands
// Combines LED control with USART communication
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
// Serial Command Buffer
// ====================================================================
#define BUFFER_SIZE 50
char serial_buffer[BUFFER_SIZE];
uint8_t buffer_index = 0;

// ====================================================================
// String Utility Functions (bare metal implementation)
// ====================================================================
int string_compare(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return (*str1 - *str2);
        }
        str1++;
        str2++;
    }
    return (*str1 - *str2);
}

void string_clear(char* str, int size) {
    for (int i = 0; i < size; i++) {
        str[i] = 0;
    }
}

void string_to_lower(char* str) {
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str = *str + 32;
        }
        str++;
    }
}

// ====================================================================
// LED States
// ====================================================================
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t rainbow_mode;  // Flag para modo rainbow
} led_state_t;

led_state_t current_led_state = {0, 0, 0, 0};

// ====================================================================
// Function Prototypes
// ====================================================================
void setup_usart0(void);
void led_init(void);
void delay_cycles(uint32_t cycles);
void process_serial_command(char* command);
void set_led_red(uint8_t state);
void set_led_green(uint8_t state);
void set_led_blue(uint8_t state);
void send_led_status(char* color, uint8_t state);
void rainbow_cycle(void);

// ====================================================================
// Main Function
// ====================================================================
int main(void) {
    
    // Initialize system
    SystemInit();
    setup_usart0();
    led_init();

    // Send welcome message
    diy_usart_send_string("=== RGB LED Control via Serial ===\r\n");
    diy_usart_send_string("Commands:\r\n");
    diy_usart_send_string("  !red     - Toggle red LED\r\n");
    diy_usart_send_string("  !green   - Toggle green LED\r\n");
    diy_usart_send_string("  !blue    - Toggle blue LED\r\n");
    diy_usart_send_string("  !off     - Turn off all LEDs\r\n");
    diy_usart_send_string("  !status  - Show current LED status\r\n");
    diy_usart_send_string("  !rainbows - Activate rainbow mode\r\n");
    diy_usart_send_string("Ready to receive commands...\r\n\r\n");

    while (1) {
        // Si modo rainbow está activo, ejecutar ciclo
        if (current_led_state.rainbow_mode) {
            rainbow_cycle();
        }
        
        if (diy_usart_is_data_available()) {
            uint8_t received_data = diy_usart_receive_byte();
            
            // Echo the received character
            diy_usart_send_byte(received_data);
            
            // Handle carriage return
            if (received_data == '\r') {
                diy_usart_send_byte('\n');
                
                // Null-terminate the buffer
                serial_buffer[buffer_index] = '\0';
                
                // Process the command
                if (buffer_index > 0) {
                    process_serial_command(serial_buffer);
                }
                
                // Reset buffer
                buffer_index = 0;
                string_clear(serial_buffer, BUFFER_SIZE);
            }
            // Handle regular characters
            else if (buffer_index < BUFFER_SIZE - 1) {
                serial_buffer[buffer_index] = received_data;
                buffer_index++;
            }
            // Handle buffer overflow
            else {
                diy_usart_send_string("\r\nBuffer overflow! Command too long.\r\n");
                buffer_index = 0;
                string_clear(serial_buffer, BUFFER_SIZE);
            }
        }
    }
    
    return 0;
}

// ====================================================================
// USART Setup Function
// ====================================================================
void setup_usart0(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    
    diy_usart_deinit(USART0);

    diy_usart_config_t usart_config = {
        .baudrate = 115200,
        .data_bite = 8,
        .stop_bit = 1,
        .parity = 0
    };

    diy_usart_config_f(USART0, &usart_config);

    diy_usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    diy_usart_receive_config(USART0, USART_RECEIVE_ENABLE);

    diy_usart_enable(USART0);
}

// ====================================================================
// LED Initialization Function
// ====================================================================
void led_init(void) {
    // Enable GPIO clocks
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    
    // Configure GPIO pins
    gpio_init(LED_GREEN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LED_GREEN_PIN);
    gpio_init(LED_BLUE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LED_BLUE_PIN);
    gpio_init(LED_RED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LED_RED_PIN);
    
    // Initialize all LEDs OFF (High = OFF for common cathode)
    gpio_bit_set(LED_GREEN_PORT, LED_GREEN_PIN);   // Green OFF
    gpio_bit_set(LED_BLUE_PORT, LED_BLUE_PIN);     // Blue OFF  
    gpio_bit_set(LED_RED_PORT, LED_RED_PIN);       // Red OFF
}

// ====================================================================
// Delay Function
// ====================================================================
__attribute__((optimize("O0")))
void delay_cycles(uint32_t cycles) {
    for (uint32_t i = 0; i < cycles; i++) {
        __asm__("nop");
    }
}

// ====================================================================
// Command Processing Function
// ====================================================================
void process_serial_command(char* command) {
    // Convert command to lowercase for case-insensitive comparison
    string_to_lower(command);
    
    // Process commands
    if (string_compare(command, "!red") == 0) {
        current_led_state.rainbow_mode = 0;  // Desactivar modo rainbow
        current_led_state.red = !current_led_state.red;
        set_led_red(current_led_state.red);
        send_led_status((char*)"Red", current_led_state.red);
    }
    else if (string_compare(command, "!green") == 0) {
        current_led_state.rainbow_mode = 0;  // Desactivar modo rainbow
        current_led_state.green = !current_led_state.green;
        set_led_green(current_led_state.green);
        send_led_status((char*)"Green", current_led_state.green);
    }
    else if (string_compare(command, "!blue") == 0) {
        current_led_state.rainbow_mode = 0;  // Desactivar modo rainbow
        current_led_state.blue = !current_led_state.blue;
        set_led_blue(current_led_state.blue);
        send_led_status((char*)"Blue", current_led_state.blue);
    }
    else if (string_compare(command, "!off") == 0) {
        current_led_state.rainbow_mode = 0;  // Desactivar modo rainbow
        current_led_state.red = 0;
        current_led_state.green = 0;
        current_led_state.blue = 0;
        set_led_red(0);
        set_led_green(0);
        set_led_blue(0);
        diy_usart_send_string("All LEDs turned OFF\r\n");
    }
    else if (string_compare(command, "!rainbows") == 0) {
        current_led_state.rainbow_mode = !current_led_state.rainbow_mode;
        if (current_led_state.rainbow_mode) {
            diy_usart_send_string("Rainbow mode ON! 🌈\r\n");
        } else {
            diy_usart_send_string("Rainbow mode OFF\r\n");
            // Apagar todos los LEDs al salir del modo rainbow
            set_led_red(0);
            set_led_green(0);
            set_led_blue(0);
            current_led_state.red = 0;
            current_led_state.green = 0;
            current_led_state.blue = 0;
        }
    }
    else if (string_compare(command, "!status") == 0) {
        if (current_led_state.rainbow_mode) {
            diy_usart_send_string("Rainbow mode is ACTIVE 🌈\r\n");
        } else {
            diy_usart_send_string("Current LED Status:\r\n");
            send_led_status((char*)"Red", current_led_state.red);
            send_led_status((char*)"Green", current_led_state.green);
            send_led_status((char*)"Blue", current_led_state.blue);
        }
    }
    else {
        diy_usart_send_string("Unknown command: ");
        diy_usart_send_string(command);
        diy_usart_send_string("\r\n");
        diy_usart_send_string("Valid commands: !red, !green, !blue, !off, !status, !rainbows\r\n");
    }
}

// ====================================================================
// LED Control Functions
// ====================================================================
void set_led_red(uint8_t state) {
    if (state) {
        gpio_bit_reset(LED_RED_PORT, LED_RED_PIN);  // Turn ON (Low = ON)
    } else {
        gpio_bit_set(LED_RED_PORT, LED_RED_PIN);    // Turn OFF (High = OFF)
    }
}

void set_led_green(uint8_t state) {
    if (state) {
        gpio_bit_reset(LED_GREEN_PORT, LED_GREEN_PIN);  // Turn ON
    } else {
        gpio_bit_set(LED_GREEN_PORT, LED_GREEN_PIN);    // Turn OFF
    }
}

void set_led_blue(uint8_t state) {
    if (state) {
        gpio_bit_reset(LED_BLUE_PORT, LED_BLUE_PIN);  // Turn ON
    } else {
        gpio_bit_set(LED_BLUE_PORT, LED_BLUE_PIN);    // Turn OFF
    }
}

// ====================================================================
// Status Reporting Function
// ====================================================================
void send_led_status(char* color, uint8_t state) {
    diy_usart_send_string(color);
    diy_usart_send_string(" LED is ");
    if (state) {
        diy_usart_send_string("ON");
    } else {
        diy_usart_send_string("OFF");
    }
    diy_usart_send_string("\r\n");
}

// ====================================================================
// Rainbow Effect Function
// ====================================================================
void rainbow_cycle(void) {
    static uint8_t step = 0;
    static uint32_t last_time = 0;
    static uint32_t current_time = 0;
    
    // Control de timing simple (cambiar cada ~300ms)
    current_time++;
    if (current_time - last_time < 300000) {
        return;  // No es tiempo de cambiar aún
    }
    last_time = current_time;
    
    // Ciclo de colores del arcoíris
    switch (step) {
        case 0: // Rojo
            set_led_red(1);
            set_led_green(0);
            set_led_blue(0);
            break;
        case 1: // Amarillo (Rojo + Verde)
            set_led_red(1);
            set_led_green(1);
            set_led_blue(0);
            break;
        case 2: // Verde
            set_led_red(0);
            set_led_green(1);
            set_led_blue(0);
            break;
        case 3: // Cian (Verde + Azul)
            set_led_red(0);
            set_led_green(1);
            set_led_blue(1);
            break;
        case 4: // Azul
            set_led_red(0);
            set_led_green(0);
            set_led_blue(1);
            break;
        case 5: // Magenta (Rojo + Azul)
            set_led_red(1);
            set_led_green(0);
            set_led_blue(1);
            break;
    }
    
    step++;
    if (step > 5) {
        step = 0;  // Reiniciar ciclo
    }
}

// ====================================================================
// End of implementation
// ====================================================================