// Keyglove controller source code - Logitech R400 controller hack
// 01/05/2011 by Mike Cochrane <mikec@mikenz.geek.nz>
//
// Based on code by Niels Teusink <niels@teusink.net>
//   http://blog.teusink.net/2010/07/cybaby-software-you-can-use-to-hack.html
//   SPI code by klk64 (http://klk64.com/arduino-spi/)
//   Cypress stuff based on code by dunk (http://sites.google.com/site/mrdunk/Home/rc-system-firmware)
// License: GPLv3

#ifndef _LOGITECHR400_H_
#define _LOGITECHR400_H_

// minimal Arduino support wrapper if we're outside of the Arduino environment
#ifndef ARDUINO
    #include "ArduinoWrapper.h"
#else
    #if ARDUINO < 100
        #include "WProgram.h"
    #else
        #include "Arduino.h"
    #endif
#endif

#include <stdarg.h>

#define READ                  0x00
#define WRITE                 0x80

#define CHANNEL_ADR           0x00
#define TX_LENGTH_ADR         0x01
#define TX_CTRL_ADR           0x02
#define TX_CFG_ADR            0x03
#define TX_IRQ_STATUS_ADR     0x04
#define RX_CTRL_ADR           0x05
#define RX_CFG_ADR            0x06
#define RX_IRQ_STATUS_ADR     0x07
#define RX_STATUS_ADR         0x08
#define RX_COUNT_ADR          0x09
#define RX_LENGTH_ADR         0x0A
#define PWR_CTRL_ADR          0x0B
#define XTAL_CTRL_ADR         0x0C
#define IO_CFG_ADR            0x0D
#define GPIO_CTRL_ADR         0x0E
#define XACT_CFG_ADR          0x0F
#define FRAMING_CFG_ADR       0x10
#define DATA32_THOLD_ADR      0x11
#define DATA64_THOLD_ADR      0x12
#define RSSI_ADR              0x13
#define EOP_CTRL_ADR          0x14
#define CRC_SEED_LSB_ADR      0x15
#define CRC_SEED_MSB_ADR      0x16
#define TX_CRC_LSB_ADR        0x17
#define TX_CRC_MSB_ADR        0x18
#define RX_CRC_LSB_ADR        0x19
#define RX_CRC_MSB_ADR        0x1A
#define TX_OFFSET_LSB_ADR     0x1B
#define TX_OFFSET_MSB_ADR     0x1C
#define MODE_OVERRIDE_ADR     0x1D
#define RX_OVERRIDE_ADR       0x1E
#define TX_OVERRIDE_ADR       0x1F
#define TX_BUFFER_ADR         0x20
#define RX_BUFFER_ADR         0x21
#define SOP_CODE_ADR          0x22
#define DATA_CODE_ADR         0x23
#define PREAMBLE_ADR          0x24
#define MFG_ID_ADR            0x25
#define XTAL_CFG_ADR          0x26
#define CLK_OVERRIDE_ADR      0x27
#define CLK_EN_ADR            0x28
#define RX_ABORT_ADR          0x29
#define AUTO_CAL_TIME_ADR     0x32
#define AUTO_CAL_OFFSET_ADR   0x35
#define ANALOG_CTRL_ADR       0x39

#define TXC_IRQ               0x02
#define TXE_IRQ               0x01

#define TX_GO                 0x80
#define TX_CLR                0x40

// define SPI pins
#ifdef __AVR_ATmega1280__
  // Arduino MEGA
  #define MOSI 2       // Arduino pin 51
  #define MISO 3       // Arduino pin 50
  #define SCLK 1       // Arduino pin 52
  #define SS 0         // Arduino pin 53
#else
  // Arduino Duemillanove
  #define MOSI 3       // Arduino pin 11
  #define MISO 4       // Arduino pin 12
  #define SCLK 5       // Arduino pin 13
  #define SS 2         // Arduino pin 10 
#endif

#define SPIPORT PORTB
#define SPIDDR  DDRB

#define SPI_TIMEOUT 16000
#define SB(x,n) (x|=(1<<n))  // set bit
#define CB(x,n) (x&=~(1<<n)) // clear bit

class LogitechR400 {
    public:
        LogitechR400();
        void init();
        
        void set_modifier(uint8_t c);
        void set_key1(uint8_t c);
        void set_key2(uint8_t c);
        void set_key3(uint8_t c);
        void set_key4(uint8_t c);
        void set_key5(uint8_t c);
        void set_key6(uint8_t c);
        void send_now();
        void set_buttons(uint8_t left, uint8_t middle, uint8_t right);
        void move(uint8_t x, uint8_t y);
        void move(uint8_t x, uint8_t y, uint8_t z);
        void scroll(uint8_t s);
    
    private:
        void write(uint8_t reg, uint8_t data);
        uint8_t read(uint8_t reg);
        uint8_t SPI(volatile uint8_t data);
        void setSopCode(uint16_t sopno);
        bool sendPacket(uint16_t count, ...);

        uint8_t clr;
        uint8_t spi_err;
        uint8_t mouse_toggle;
        uint8_t keyboard_toggle;

        uint8_t miso;
        uint8_t mosi;
        uint8_t sclk; 
        uint8_t ss;
        uint8_t mouse_buttons;
        uint8_t keyboard_keys[6];
        uint8_t keyboard_modifier_keys;
        
};

#endif /* _LOGITECHR400_H_ */


