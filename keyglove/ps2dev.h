/*
 * ps2.h - a library to interface with ps2 hosts.
 * Written by Chris J. Kiick, January 2008.
 * modified by Gene E. Scogin, August 2008.
 * Release into public domain.
 *
 * limitations:
 *      we do not handle parity errors.
 *      The timing constants are hard coded from the spec. Data rate is
 *      not impressive. Probably lots of room for optimization.
 */

#ifndef ps2dev_h
#define ps2dev_h

#ifdef USE_LUFA
    #include "ArduinoWrapper.h"
#endif

#ifndef USE_LUFA
    #include "WProgram.h"
    #include "wiring.h"
#endif

#ifdef round
    #undef round
#endif

class PS2dev {
    public:
        PS2dev(int clockPin, int dataPin);
        int write(unsigned char data);
        int read(unsigned char * data);
    private:
        int _ps2clk;
        int _ps2data;
        void golo(int pin);
        void gohi(int pin);
};

#endif /* ps2dev_h */
