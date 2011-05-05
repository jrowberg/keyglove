// Keyglove controller source code - Logitech R400 controller hack
// 01/05/2011 by Mike Cochrane <mikec@mikenz.geek.nz>
//
// Dismantle RX400 handheld, connect the Arduino SPI pins to the 
// equivalent test points on the RX400. Move the touch senors that 
// would normally be on those pins as per pins.h.
//
// WARNING: This connects to the first found RX400 compatible reciever
// This may not necessarily be the one plugged into your computer.
//
// Based on code by Niels Teusink <niels@teusink.net>
//   http://blog.teusink.net/2010/07/cybaby-software-you-can-use-to-hack.html
//   SPI code by klk64 (http://klk64.com/arduino-spi/)
//   Cypress stuff based on code by dunk (http://sites.google.com/site/mrdunk/Home/rc-system-firmware)
// License: GPLv3

#include "WProgram.h"
#include "logitech.h"
#ifdef round
    #undef round
#endif

logitech::logitech() {
    keyboard_keys[0] = 0;
    keyboard_keys[1] = 0;
    keyboard_keys[2] = 0;
    keyboard_keys[3] = 0;
    keyboard_keys[4] = 0;
    keyboard_keys[5] = 0;
    keyboard_modifier_keys = 0;
    mouse_buttons = 0;
    
    mouse_toggle = 1;
    keyboard_toggle = 1;
}

void logitech::init() {
    // Init SPI
    spi_err = 0;
    SB(SPIDDR, MOSI);
    SB(SPIDDR, SCLK);
    SB(SPIDDR, SS); 
    CB(SPIDDR, MISO);
    SB(SPIPORT, SS); 
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
    clr=SPSR;
    clr=SPDR;   
    delay(10);

    write(MODE_OVERRIDE_ADR,    0x01);		// MODE_OVERRIDE, RST (Reset module)
    write(CHANNEL_ADR,          0x4D);
    write(TX_CFG_ADR,           0x0E);		// TX_CFG, 
    write(ANALOG_CTRL_ADR,      0x01);
    write(RX_CFG_ADR,           0x48);		// RX_CFG_ADR, overwrite (RXOW EN)
    write(0x0F,                 0x13);		// XACT_CFG,END_STATE,ACK EN, ACK_TO_15X
    write(FRAMING_CFG_ADR,      0xA4);		// FRAMING_CFG, LEN EN
    write(DATA32_THOLD_ADR,     0x03);		// DATA32_THOLD
    write(CRC_SEED_LSB_ADR,     0x00);		// CRC_SEED_LSB
    write(PREAMBLE_ADR,         0x01); 
    write(PREAMBLE_ADR,         0x33);
    write(PREAMBLE_ADR,         0x33);
    write(TX_OFFSET_LSB_ADR,    0x55);		// TX_OFFSET_LSB
    write(TX_OFFSET_MSB_ADR,    0x05);		// TX_OFFSET_MSB
    write(XTAL_CFG_ADR,         0x08);		// XTAL_CFG  (must)
    write(AUTO_CAL_TIME_ADR,    0x3C);		// AUTO_CAL_TIME  (must)
    write(AUTO_CAL_OFFSET_ADR,  0x14);		// AUTO_CAL_OFFSET  (must)
    
    #ifdef SERIAL_DEBUG
        Serial.println("SPI Init complete.\n");
    #endif /* SERIAL_DEBUG */

    // Scan for a controller to connect to 
    Serial.println("Scanning for devices\n");
    bool found = false;
    for (int sop = 0; sop <= 10 && !found; sop++) {
        for (int i = 0; i <= 0x62 && !found; i++) {
            if (i == 0) {
                #ifdef SERIAL_DEBUG
                    Serial.print("Scanning for SOP_CODE 0x");
                    Serial.println(sop, HEX);
                #endif /* SERIAL_DEBUG */
                setSopCode(sop);
            }
            write(CHANNEL_ADR, i);
            write(XACT_CFG_ADR, 0xA7);
            if (!sendPacket(2, 0x41, 0x00)) {
                #ifdef SERIAL_DEBUG
                    Serial.print("Found device on channel: 0x");
                    Serial.println(i, HEX); 
                #endif /* SERIAL_DEBUG */
                found = true;
            }
        } 
    }
    #ifdef SERIAL_DEBUG
        Serial.println("Scan complete!");
    #endif /* SERIAL_DEBUG */
    
}

int logitech::sendPacket(int count, ...) {
    write(TX_LENGTH_ADR, count);     // Set the transmit length
    write(TX_CTRL_ADR, TX_CLR);      // Clear the transmit buffer
    
    va_list ap;
    va_start(ap, count);
    while (count){
        write(TX_BUFFER_ADR, va_arg(ap, int)); // Add the bytes to send
        count--;
    }
    va_end(ap);
    
    write(TX_CTRL_ADR, TX_GO);      // Transmit the packet
    
    unsigned int RadioState = 0;
    while (!RadioState) {
        RadioState = read(TX_IRQ_STATUS_ADR) & (TXE_IRQ | TXC_IRQ);
    }
    
    return (RadioState & TXE_IRQ) ? 1 : 0;
}


void logitech::setSopCode(int sopno) {
    switch (sopno) {
        case 0:
            write(SOP_CODE_ADR,0x82); 
            write(SOP_CODE_ADR,0xC7); 
            write(SOP_CODE_ADR,0x90); 
            write(SOP_CODE_ADR,0x36); 
            write(SOP_CODE_ADR,0x21); 
            write(SOP_CODE_ADR,0x9E); 
            write(SOP_CODE_ADR,0xFF); 
            write(SOP_CODE_ADR,0x17);
            break;
        case 1: 
            write(SOP_CODE_ADR,0x3C); write(SOP_CODE_ADR,0x37); write(SOP_CODE_ADR,0xCC); write(SOP_CODE_ADR,0x91);
            write(SOP_CODE_ADR,0xE2); write(SOP_CODE_ADR,0x00); write(SOP_CODE_ADR,0xCC); write(SOP_CODE_ADR,0x91);
            break;
        case 2:
            write(SOP_CODE_ADR,0x9B); write(SOP_CODE_ADR,0xC5); write(SOP_CODE_ADR,0xA1); write(SOP_CODE_ADR,0x0F);
            write(SOP_CODE_ADR,0xAD); write(SOP_CODE_ADR,0x39); write(SOP_CODE_ADR,0xA2); write(SOP_CODE_ADR,0x0F);
            break;
        case 3:
            write(SOP_CODE_ADR,0xEF); write(SOP_CODE_ADR,0x64); write(SOP_CODE_ADR,0xB0); write(SOP_CODE_ADR,0x2A);
            write(SOP_CODE_ADR,0xD2); write(SOP_CODE_ADR,0x8F); write(SOP_CODE_ADR,0xB1); write(SOP_CODE_ADR,0x2A);
            break;
        case 4:
            write(SOP_CODE_ADR,0x66); write(SOP_CODE_ADR,0xCD); write(SOP_CODE_ADR,0x7C); write(SOP_CODE_ADR,0x50);
            write(SOP_CODE_ADR,0xDD); write(SOP_CODE_ADR,0x26); write(SOP_CODE_ADR,0x7C); write(SOP_CODE_ADR,0x50);
            break;
        case 5:
            write(SOP_CODE_ADR,0x5C); write(SOP_CODE_ADR,0xE1); write(SOP_CODE_ADR,0xF6); write(SOP_CODE_ADR,0x44);
            write(SOP_CODE_ADR,0xAD); write(SOP_CODE_ADR,0x16); write(SOP_CODE_ADR,0xF6); write(SOP_CODE_ADR,0x44);
            break;
        case 6:
            write(SOP_CODE_ADR,0x5A); write(SOP_CODE_ADR,0xCC); write(SOP_CODE_ADR,0xAE); write(SOP_CODE_ADR,0x46);
            write(SOP_CODE_ADR,0xB6); write(SOP_CODE_ADR,0x31); write(SOP_CODE_ADR,0xAE); write(SOP_CODE_ADR,0x46);
            break;
        case 7:
            write(SOP_CODE_ADR,0xA1); write(SOP_CODE_ADR,0x78); write(SOP_CODE_ADR,0xDC); write(SOP_CODE_ADR,0x3C);
            write(SOP_CODE_ADR,0x9E); write(SOP_CODE_ADR,0x82); write(SOP_CODE_ADR,0xDC); write(SOP_CODE_ADR,0x3C);
            break;
        case 8:
            write(SOP_CODE_ADR,0xB9); write(SOP_CODE_ADR,0x8E); write(SOP_CODE_ADR,0x19); write(SOP_CODE_ADR,0x74);
            write(SOP_CODE_ADR,0x6F); write(SOP_CODE_ADR,0x65); write(SOP_CODE_ADR,0x18); write(SOP_CODE_ADR,0x74);
            break;
        case 9:
            write(SOP_CODE_ADR,0xDF); write(SOP_CODE_ADR,0xB1); write(SOP_CODE_ADR,0xC0); write(SOP_CODE_ADR,0x49);
            write(SOP_CODE_ADR,0x62); write(SOP_CODE_ADR,0xDF); write(SOP_CODE_ADR,0xC1); write(SOP_CODE_ADR,0x49);
            break;
        case 10:
            write(SOP_CODE_ADR,0x97); write(SOP_CODE_ADR,0xE5); write(SOP_CODE_ADR,0x14); write(SOP_CODE_ADR,0x72);
            write(SOP_CODE_ADR,0x7F); write(SOP_CODE_ADR,0x1A); write(SOP_CODE_ADR,0x14); write(SOP_CODE_ADR,0x72);
            break;
    } 
}

byte logitech::SPI(volatile char data) {
    long c = 0;
    SPDR = data;
    // start the transmission by loading the output
    // byte into the SPI data register

    // check status register for transfer finished flag
    while (!(SPSR & (1<<SPIF))) {
        if (++c == SPI_TIMEOUT) {
            spi_err = 1;
            break;
        }
    }

    return SPDR;    //return the received byte
}

byte logitech::read(char reg) {
    byte res;
    CB(SPIPORT, SS);           
    SPI(READ|reg);
    res = SPI(0xFF);
    SB(SPIPORT, SS);  
    delay(1);
    return res;
}


void logitech::write(char reg, char data) {
    CB(SPIPORT, SS);
    SPI(WRITE|reg);
    SPI(data);
    SB(SPIPORT, SS);  
    delay(1);
}

void logitech::set_modifier(uint8_t c) {
    keyboard_modifier_keys = c;
}

void logitech::set_key1(uint8_t c) {
    keyboard_keys[0] = c;
}

void logitech::set_key2(uint8_t c) {
    keyboard_keys[1] = c;
}

void logitech::set_key3(uint8_t c) {
    keyboard_keys[2] = c;
}

void logitech::set_key4(uint8_t c) {
    keyboard_keys[3] = c;
}

void logitech::set_key5(uint8_t c) {
    keyboard_keys[4] = c;
}

void logitech::set_key6(uint8_t c) {
    keyboard_keys[5] = c;
}

void logitech::send_now() {
    uint8_t p1 = keyboard_toggle ? 0x45 : 0x41;

    if (keyboard_keys[0] == 0x00) {
        sendPacket(2, p1, 0x00);
    } else if (keyboard_modifier_keys != 0x00) {
        sendPacket(3, p1, keyboard_keys[0], keyboard_modifier_keys);
    } else if (keyboard_keys[0] != 0x00) {
        sendPacket(2, p1, keyboard_keys[0]);
    }        

    keyboard_toggle = keyboard_toggle ? 0 : 1;
}

void logitech::set_buttons(uint8_t left, uint8_t middle, uint8_t right) {
    uint8_t mask=0;
    if (left) mask |= 0x20;
    if (middle) mask |= 0x40;
    if (right) mask |= 0x80;
    mouse_buttons = mask;
    move(0, 0, 0);
}

void logitech::move(uint8_t x, uint8_t y) {
    move(x, y, 0);
}

void logitech::move(uint8_t x, uint8_t y, uint8_t z) {
    uint8_t p1 = mouse_toggle ? 0x47 : 0x43;
    sendPacket(4, p1, x, y, (z & 0x1f) | mouse_buttons);
    mouse_toggle = mouse_toggle ? 0 : 1;
}

void logitech::scroll(uint8_t s) {
    move(0, 0, s);
}
