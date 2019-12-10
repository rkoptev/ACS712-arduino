#ifndef ACS712_h
#define ACS712_h

#include <Arduino.h>


#define DEFAULT_FREQUENCY 50

enum ACS712_type {ACS712_05B, ACS712_20A, ACS712_30A};

class ACS712 {
public:
	ACS712(ACS712_type type, uint8_t _pin);
	int calibrate();
	void setZeroPoint(int _zero);
	void setSensitivity(float sens);
  void setVoltageReference(float volts);
  void setAdcResolution(float adcResolution);

  float getVoltage();
	float getCurrentDC();
	float getCurrentAC(uint16_t frequency = 50);

private:
	int zero = 512;
	float sensitivity;
	uint8_t pin;
  float voltageReference = 5;      // Vcc of the MCU
  float adcScale = 1024.0;           // ADC resolution
};

#endif