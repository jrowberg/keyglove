#ifndef Accelerometer_h
#define Accelerometer_h

#ifdef LUFA
    #include "ArduinoWrapper.h"
#endif

#ifndef LUFA
    #include "WProgram.h"
#endif

/* ------- Register names ------- */
#define ADXL345_DEVID 0x00
#define ADXL345_RESERVED1 0x01
#define ADXL345_THRESH_TAP 0x1d
#define ADXL345_OFSX 0x1e
#define ADXL345_OFSY 0x1f
#define ADXL345_OFSZ 0x20
#define ADXL345_DUR 0x21
#define ADXL345_LATENT 0x22
#define ADXL345_WINDOW 0x23
#define ADXL345_THRESH_ACT 0x24
#define ADXL345_THRESH_INACT 0x25
#define ADXL345_TIME_INACT 0x26
#define ADXL345_ACT_INACT_CTL 0x27
#define ADXL345_THRESH_FF 0x28
#define ADXL345_TIME_FF 0x29
#define ADXL345_TAP_AXES 0x2a
#define ADXL345_ACT_TAP_STATUS 0x2b
#define ADXL345_BW_RATE 0x2c
#define ADXL345_POWER_CTL 0x2d
#define ADXL345_INT_ENABLE 0x2e
#define ADXL345_INT_MAP 0x2f
#define ADXL345_INT_SOURCE 0x30
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37
#define ADXL345_FIFO_CTL 0x38
#define ADXL345_FIFO_STATUS 0x39

/* 
 Interrupt PINs
 INT1: 0
 INT2: 1
 */
#define ADXL345_INT1_PIN 0x00
#define ADXL345_INT2_PIN 0x01

/* 
 Interrupt bit position
 */
#define ADXL345_INT_DATA_READY_BIT 0x07
#define ADXL345_INT_SINGLE_TAP_BIT 0x06
#define ADXL345_INT_DOUBLE_TAP_BIT 0x05
#define ADXL345_INT_ACTIVITY_BIT   0x04
#define ADXL345_INT_INACTIVITY_BIT 0x03
#define ADXL345_INT_FREE_FALL_BIT  0x02
#define ADXL345_INT_WATERMARK_BIT  0x01
#define ADXL345_INT_OVERRUNY_BIT   0x00

class ADXL345
{
public:
  ADXL345();
  void powerOn();
  void readAccel(int16_t* x, int16_t* y, int16_t* z);

  void setTapThreshold(int16_t tapThreshold);
  int16_t getTapThreshold();
  void setAxisOffset(int16_t x, int16_t y, int16_t z);
  void getAxisOffset(int16_t* x, int16_t* y, int16_t*z);
  void setTapDuration(int16_t tapDuration);
  int16_t getTapDuration();
  void setDoubleTapLatency(int16_t doubleTapLatency);
  int16_t getDoubleTapLatency();
  void setDoubleTapWindow(int16_t doubleTapWindow);
  int16_t getDoubleTapWindow();
  void setActivityThreshold(int16_t activityThreshold);
  int16_t getActivityThreshold();
  void setInactivityThreshold(int16_t inactivityThreshold);
  int16_t getInactivityThreshold();
  void setTimeInactivity(int16_t timeInactivity);
  int16_t getTimeInactivity();
  void setFreeFallThreshold(int16_t freeFallthreshold);
  int16_t getFreeFallThreshold();
  void setFreeFallDuration(int16_t freeFallDuration);
  int16_t getFreeFallDuration();

  bool isActivityXEnabled();
  bool isActivityYEnabled();
  bool isActivityZEnabled();
  bool isInactivityXEnabled();
  bool isInactivityYEnabled();
  bool isInactivityZEnabled();
  bool isActivityAc();
  bool isInactivityAc();
  void setActivityAc(bool state);
  void setInactivityAc(bool state);

  bool getSuppressBit();
  void setSuppressBit(bool state);
  bool isTapDetectionOnX();
  void setTapDetectionOnX(bool state);
  bool isTapDetectionOnY();
  void setTapDetectionOnY(bool state);
  bool isTapDetectionOnZ();
  void setTapDetectionOnZ(bool state);

  void setActivityX(bool state);
  void setActivityY(bool state);
  void setActivityZ(bool state);
  void setInactivityX(bool state);
  void setInactivityY(bool state);
  void setInactivityZ(bool state);

  bool isActivitySourceOnX();
  bool isActivitySourceOnY();
  bool isActivitySourceOnZ();
  bool isTapSourceOnX();
  bool isTapSourceOnY();
  bool isTapSourceOnZ();
  bool isAsleep();

  bool isLowPower();
  void setLowPower(bool state);
  float getRate();
  void setRate(float rate);

  uint8_t getInterruptSource();
  bool getInterruptSource(uint8_t interruptBit);
  bool getInterruptMapping(uint8_t interruptBit);
  void setInterruptMapping(uint8_t interruptBit, bool interruptPin);
  bool isInterruptEnabled(uint8_t interruptBit);
  void setInterrupt(uint8_t interruptBit, bool state);

  void getRangeSetting(uint8_t* rangeSetting);
  void setRangeSetting(int16_t val);
  bool getSelfTestBit();
  void setSelfTestBit(bool selfTestBit);
  bool getSpiBit();
  void setSpiBit(bool spiBit);
  bool getInterruptLevelBit();
  void setInterruptLevelBit(bool interruptLevelBit);
  bool getFullResBit();
  void setFullResBit(bool fullResBit);
  bool getJustifyBit();
  void setJustifyBit(bool justifyBit);
  void printAllRegister();

private:
  void writeTo(int16_t device, uint8_t address, uint8_t val);
  void readFrom(int16_t device, uint8_t address, uint8_t num, uint8_t buff[]);
  void setRegisterBit(uint8_t regAdress, int16_t bitPos, bool state);
  bool getRegisterBit(uint8_t regAdress, int16_t bitPos);
  uint8_t _buff[6] ;    // 6 bytes buffer for saving data read from the device
};

#endif

