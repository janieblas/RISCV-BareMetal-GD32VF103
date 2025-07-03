
#include <stdint.h>
#include "gd32vf103.h"

 #define DELAY_TIME 10000000

void setup_usart0(void);

__attribute__((optimize("O0")))
void delay_cycles(uint32_t cycles);



int main(void) {
    
    SystemInit();
    setup_usart0();

    diy_usart_send_string("=== Echo USART Demo ===\r\n");
    diy_usart_send_string("Escribe algo y te lo devolvere:\r\n");

    while (1)
    {
        if (diy_usart_is_data_available()) {
            // Recibir el byte
            uint8_t received_data = diy_usart_receive_byte();
            
            // Echo: reenviar el mismo byte
            diy_usart_send_byte(received_data);
            
            if (received_data == '\r') {
                diy_usart_send_byte('\n');
            }
        }
    }
    
    return 0;
}

//==============================================================================//

void setup_usart0(void){
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

    /* Enable USART0 */
    diy_usart_enable(USART0);
}

void delay_cycles(uint32_t cycles) {
    for (uint32_t i = 0; i < cycles; i++) {
        __asm__("nop");
    }
}