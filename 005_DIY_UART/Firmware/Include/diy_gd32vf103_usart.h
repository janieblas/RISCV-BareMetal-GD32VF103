#ifndef DIY_GD32VF103_H
#define DIY_GD32VF103_H

#include "gd32vf103.h"

/* USART definitions */
/* USARTx(x=0,1,2)/UARTx(x=3,4) definitions */
#define USART1                        USART_BASE                        /*!< USART1 base address */
#define USART2                        (USART_BASE+(0x00000400U))        /*!< USART2 base address */
#define USART0                        (USART_BASE+(0x0000F400U))        /*!< USART0 base address */

/* Register access macro */
#define REG32(addr)                   (*(volatile uint32_t *)(uint32_t)(addr))

/* USART registers definitions (del original) */
#define USART_STAT(usartx)            REG32((usartx) + (0x00000000U))   /*!< USART status register */
#define USART_DATA(usartx)            REG32((usartx) + (0x00000004U))   /*!< USART data register */
#define USART_BAUD(usartx)            REG32((usartx) + (0x00000008U))   /*!< USART baud rate register */
#define USART_CTL0(usartx)            REG32((usartx) + (0x0000000CU))   /*!< USART control register 0 */
#define USART_CTL1(usartx)            REG32((usartx) + (0x00000010U))   /*!< USART control register 1 */
#define USART_CTL2(usartx)            REG32((usartx) + (0x00000014U))   /*!< USART control register 2 */
#define USART_GP(usartx)              REG32((usartx) + (0x00000018U))   /*!< USART guard time and prescaler register */

/* USARTx_DATA */
#define USART_DATA_DATA               BITS(0,8)                         /*!< transmit or read data value */

/* USARTx_BAUD */
#define USART_BAUD_FRADIV             BITS(0,3)                         /*!< fraction part of baud-rate divider */
#define USART_BAUD_INTDIV             BITS(4,15)                        /*!< integer part of baud-rate divider */

/* USARTx_CTL0 */
#define USART_CTL0_SBKCMD             BIT(0)                            /*!< send break command */
#define USART_CTL0_RWU                BIT(1)                            /*!< receiver wakeup from mute mode */
#define USART_CTL0_REN                BIT(2)                            /*!< receiver enable */
#define USART_CTL0_TEN                BIT(3)                            /*!< transmitter enable */
#define USART_CTL0_IDLEIE             BIT(4)                            /*!< idle line detected interrupt enable */
#define USART_CTL0_RBNEIE             BIT(5)                            /*!< read data buffer not empty interrupt and overrun error interrupt enable */
#define USART_CTL0_TCIE               BIT(6)                            /*!< transmission complete interrupt enable */
#define USART_CTL0_TBEIE              BIT(7)                            /*!< transmitter buffer empty interrupt enable */
#define USART_CTL0_PERRIE             BIT(8)                            /*!< parity error interrupt enable */
#define USART_CTL0_PM                 BIT(9)                            /*!< parity mode */
#define USART_CTL0_PCEN               BIT(10)                           /*!< parity check function enable */
#define USART_CTL0_WM                 BIT(11)                           /*!< wakeup method in mute mode */
#define USART_CTL0_WL                 BIT(12)                           /*!< word length */
#define USART_CTL0_UEN                BIT(13)                           /*!< USART enable */

/* USARTx_CTL1 */
#define USART_CTL1_ADDR               BITS(0,3)                         /*!< address of USART */
#define USART_CTL1_LBLEN              BIT(5)                            /*!< LIN break frame length */
#define USART_CTL1_LBDIE              BIT(6)                            /*!< LIN break detected interrupt eanble */
#define USART_CTL1_CLEN               BIT(8)                            /*!< CK length */
#define USART_CTL1_CPH                BIT(9)                            /*!< CK phase */
#define USART_CTL1_CPL                BIT(10)                           /*!< CK polarity */
#define USART_CTL1_CKEN               BIT(11)                           /*!< CK pin enable */
#define USART_CTL1_STB                BITS(12,13)                       /*!< STOP bits length */
#define USART_CTL1_LMEN               BIT(14)                           /*!< LIN mode enable */

/* USARTx_CTL2 */
#define USART_CTL2_ERRIE              BIT(0)                            /*!< error interrupt enable */
#define USART_CTL2_IREN               BIT(1)                            /*!< IrDA mode enable */
#define USART_CTL2_IRLP               BIT(2)                            /*!< IrDA low-power */
#define USART_CTL2_HDEN               BIT(3)                            /*!< half-duplex enable */
#define USART_CTL2_NKEN               BIT(4)                            /*!< NACK enable in smartcard mode */
#define USART_CTL2_SCEN               BIT(5)                            /*!< smartcard mode enable */
#define USART_CTL2_DENR               BIT(6)                            /*!< DMA request enable for reception */
#define USART_CTL2_DENT               BIT(7)                            /*!< DMA request enable for transmission */
#define USART_CTL2_RTSEN              BIT(8)                            /*!< RTS enable */
#define USART_CTL2_CTSEN              BIT(9)                            /*!< CTS enable */
#define USART_CTL2_CTSIE              BIT(10)                           /*!< CTS interrupt enable */

/* constants definitions */
/* define the USART bit position and its register index offset */
#define USART_REGIDX_BIT(regidx, bitpos)     (((uint32_t)(regidx) << 6) | (uint32_t)(bitpos))
#define USART_REG_VAL(usartx, offset)        (REG32((usartx) + (((uint32_t)(offset) & (0x0000FFFFU)) >> 6)))
#define USART_BIT_POS(val)                   ((uint32_t)(val) & (0x0000001FU))
#define USART_REGIDX_BIT2(regidx, bitpos, regidx2, bitpos2)   (((uint32_t)(regidx2) << 22) | (uint32_t)((bitpos2) << 16)\
                                                              | (((uint32_t)(regidx) << 6) | (uint32_t)(bitpos)))
#define USART_REG_VAL2(usartx, offset)       (REG32((usartx) + ((uint32_t)(offset) >> 22)))
#define USART_BIT_POS2(val)                  (((uint32_t)(val) & (0x001F0000U)) >> 16)

/* register offset */
#define USART_STAT_REG_OFFSET                     (0x00000000U)        	/*!< STAT register offset */
#define USART_CTL0_REG_OFFSET                     (0x0000000CU)        	/*!< CTL0 register offset */
#define USART_CTL1_REG_OFFSET                     (0x00000010U)        	/*!< CTL1 register offset */
#define USART_CTL2_REG_OFFSET                     (0x00000014U)        	/*!< CTL2 register offset */


/* USART transmitter configure */
#define CTL0_TEN(regval)              (BIT(3) & ((uint32_t)(regval) << 3))
#define USART_TRANSMIT_ENABLE         CTL0_TEN(1)                       /*!< enable transmitter */
#define USART_TRANSMIT_DISABLE        CTL0_TEN(0)                       /*!< disable transmitter */

/* USART receiver configure */
#define CTL0_REN(regval)              (BIT(2) & ((uint32_t)(regval) << 2))
#define USART_RECEIVE_ENABLE          CTL0_REN(1)                       /*!< enable receiver */
#define USART_RECEIVE_DISABLE         CTL0_REN(0)                       /*!< disable receiver */

/* USART parity bits definitions */
#define CTL0_PM(regval)               (BITS(9,10) & ((uint32_t)(regval) << 9))
#define USART_PM_NONE                 CTL0_PM(0)                        /*!< no parity */
#define USART_PM_EVEN                 CTL0_PM(2)                        /*!< even parity */
#define USART_PM_ODD                  CTL0_PM(3)                        /*!< odd parity */

/* USART word length definitions */
#define CTL0_WL(regval)               (BIT(12) & ((uint32_t)(regval) << 12))
#define USART_WL_8BIT                 CTL0_WL(0)                        /*!< 8 bits */
#define USART_WL_9BIT                 CTL0_WL(1)                        /*!< 9 bits */

/* USART stop bits definitions */
#define CTL1_STB(regval)              (BITS(12,13) & ((uint32_t)(regval) << 12))
#define USART_STB_1BIT                CTL1_STB(0)                       /*!< 1 bit */
#define USART_STB_0_5BIT              CTL1_STB(1)                       /*!< 0.5 bit */
#define USART_STB_2BIT                CTL1_STB(2)                       /*!< 2 bits */
#define USART_STB_1_5BIT              CTL1_STB(3)                       /*!< 1.5 bits */

typedef struct {
 uint32_t baudrate;
 uint8_t data_bite;
 uint8_t stop_bit;
 uint8_t parity;
 uint8_t flow_control;
}diy_usart_config_t;

typedef enum {
    USART_STATE_RESET = 0,
    USART_STATE_READY,
    USART_STATE_BUSY_TX,
    USART_STATE_BUSY_RX,
    USART_STATE_ERROR
} diy_usart_state_t;

typedef enum {
    USART_OK = 0,
    USART_ERROR,
    USART_TIMEOUT,
    USART_BUSY
} diy_usart_status_t;

/* USART flags */
typedef enum
{
    /* flags in STAT register */
    USART_FLAG_CTSF = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 9U),      /*!< CTS change flag */
    USART_FLAG_LBDF = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 8U),      /*!< LIN break detected flag */
    USART_FLAG_TBE = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 7U),       /*!< transmit data buffer empty */
    USART_FLAG_TC = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 6U),        /*!< transmission complete */
    USART_FLAG_RBNE = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 5U),      /*!< read data buffer not empty */
    USART_FLAG_IDLEF = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 4U),     /*!< IDLE frame detected flag */
    USART_FLAG_ORERR = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 3U),     /*!< overrun error */
    USART_FLAG_NERR = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 2U),      /*!< noise error flag */
    USART_FLAG_FERR = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 1U),      /*!< frame error flag */
    USART_FLAG_PERR = USART_REGIDX_BIT(USART_STAT_REG_OFFSET, 0U),      /*!< parity error flag */
}usart_flag_enum;


// initialization functions 
void diy_usart_deinit(uint32_t usart_periph);
void diy_usart_baudrate_set(uint32_t usart_periph, uint32_t baudval);
void diy_usart_parity_config(uint32_t usart_periph, uint32_t paritycfg);
void diy_usart_word_length_set(uint32_t usart_periph, uint32_t wlen);
void diy_usart_stop_bit_set(uint32_t usart_periph, uint32_t stblen);

// USART normal mode communication
void diy_usart_enable(uint32_t usart_periph);
void diy_usart_disable(uint32_t usart_periph);
void diy_usart_transmit_config(uint32_t usart_periph, uint32_t txconfig);
void diy_usart_receive_config(uint32_t usart_periph, uint32_t rxconfig);
void diy_usart_data_transmit(uint32_t usart_periph, uint32_t data);
uint16_t diy_usart_data_receive(uint32_t usart_periph);

//flag functions
FlagStatus diy_usart_flag_get(uint32_t usart_periph, usart_flag_enum flag);

// hardware flow communication 
void diy_usart_hardware_flow_rts_config(uint32_t usart_periph, uint32_t rtsconfig);
void diy_usart_hardware_flow_cts_config(uint32_t usart_periph, uint32_t ctsconfig);

#endif //DIY_GD32VF103_H