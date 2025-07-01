#include <stdint.h>
#include "diy_gd32vf103_usart.h"

void diy_usart_deinit(uint32_t usart_periph)
{
    switch (usart_periph)
    {
    case USART0:
        rcu_periph_clock_enable(RCU_USART0RST);
        rcu_periph_clock_disable(RCU_USART0RST);
        break;
    
    default:
        break;
    }

}

void diy_usart_baudrate_set(uint32_t usart_periph, uint32_t baudval)
{
    uint32_t uclk=0U, intdiv=0U, fradiv=0U, udiv=0U;

    switch (usart_periph)
    {
    case USART0:
        uclk = rcu_clock_freq_get(CK_APB2);
        break;
    
    default:
        break;
    }

    udiv = (uclk+baudval/2U)/baudval;
    intdiv = udiv & (0x0000fff0U);
    fradiv = udiv & (0x0000000fU);

    USART_BAUD(usart_periph) = ((USART_BAUD_FRADIV | USART_BAUD_INTDIV) & (intdiv | fradiv));
}

void diy_usart_parity_config(uint32_t usart_periph, uint32_t paritycfg)
{
    /* clear USART_CTL0 PM,PCEN bits */
    USART_CTL0(usart_periph) &= ~(USART_CTL0_PM | USART_CTL0_PCEN);
    /* configure USART parity mode */
    USART_CTL0(usart_periph) |= paritycfg ;
}


void diy_usart_word_length_set(uint32_t usart_periph, uint32_t wlen)
{
    /* clear USART_CTL0 WL bit */
    USART_CTL0(usart_periph) &= ~USART_CTL0_WL;
    /* configure USART word length */
    USART_CTL0(usart_periph) |= wlen;
}

void diy_usart_stop_bit_set(uint32_t usart_periph, uint32_t stblen)
{
    /* clear USART_CTL1 STB bits */
    USART_CTL1(usart_periph) &= ~USART_CTL1_STB; 
    /* configure USART stop bits */
    USART_CTL1(usart_periph) |= stblen;
}

void diy_usart_enable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) |= USART_CTL0_UEN;
}

void diy_usart_disable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) &= ~(USART_CTL0_UEN);
}

void diy_usart_transmit_config(uint32_t usart_periph, uint32_t txconfig)
{
    uint32_t ctl = 0U;
    
    ctl = USART_CTL0(usart_periph);
    ctl &= ~USART_CTL0_TEN;
    ctl |= txconfig;
    /* configure transfer mode */
    USART_CTL0(usart_periph) = ctl;
}

void diy_usart_data_transmit(uint32_t usart_periph, uint32_t data)
{
    USART_DATA(usart_periph) = USART_DATA_DATA & data;
}


FlagStatus diy_usart_flag_get(uint32_t usart_periph, usart_flag_enum flag)
{
    if(RESET != (USART_REG_VAL(usart_periph, flag) & BIT(USART_BIT_POS(flag)))){
        return SET;
    }else{
        return RESET;
    }
}
