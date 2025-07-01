
#include <stdint.h>
#include "gd32vf103.h"

 #define DELAY_TIME 10000000

void setup_usart0(void);
void send_byte(uint8_t data);
void send_string(char* str);
uint8_t receive_byte(void);
uint8_t is_data_available(void);

__attribute__((optimize("O0")))
void delay_cycles(uint32_t cycles);



int main(void) {
    
    SystemInit();
    setup_usart0();

    send_string("=== Echo USART Demo ===\r\n");
    send_string("Escribe algo y te lo devolvere:\r\n");

    while (1)
    {
        if (is_data_available()) {
            // Recibir el byte
            uint8_t received_data = receive_byte();
            
            // Echo: reenviar el mismo byte
            send_byte(received_data);
            
            // Opcional: Si es Enter (\r), también enviar nueva línea
            if (received_data == '\r') {
                send_byte('\n');
            }
        }
    }
    
    return 0;
}

//==============================================================================//

//==============================================================================//

void setup_usart0(void){
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    
    diy_usart_deinit(USART0);
    diy_usart_baudrate_set(USART0, 115200U);
    diy_usart_word_length_set(USART0, USART_WL_8BIT);
    diy_usart_stop_bit_set(USART0, USART_STB_1BIT);
    diy_usart_parity_config(USART0, USART_PM_NONE);

    diy_usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    diy_usart_receive_config(USART0, USART_RECEIVE_ENABLE);

    /* Enable USART0 */
    diy_usart_enable(USART0);
}

void send_byte(uint8_t data){
    while (RESET == diy_usart_flag_get(USART0, USART_FLAG_TBE));
    
    diy_usart_data_transmit(USART0, data);

    while (RESET == diy_usart_flag_get(USART0, USART_FLAG_TC));
    
}

void send_string(char* str) {
    while (*str) {
        send_byte(*str);
        str++;
    }
}

uint8_t receive_byte(void) {
    
    while (RESET == diy_usart_flag_get(USART0, USART_FLAG_RBNE));
    
    
    return (uint8_t)diy_usart_data_receive(USART0);
}

uint8_t is_data_available(void) {
   
    return (diy_usart_flag_get(USART0, USART_FLAG_RBNE) == SET) ? 1 : 0;
}

void delay_cycles(uint32_t cycles) {
    for (uint32_t i = 0; i < cycles; i++) {
        __asm__("nop");
    }
}