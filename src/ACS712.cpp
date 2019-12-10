#include "ACS712.h"

ACS712::ACS712(ACS712_type type, uint8_t _pin) {
	pin = _pin;

	// Different models of the sensor have their sensitivity:
	switch (type) {
		case ACS712_05B:
			sensitivity = 0.185;
			break;
		case ACS712_20A:
			sensitivity = 0.100;
			break;
		case ACS712_30A:
			sensitivity = 0.066;
			break;
	}
}

int ACS712::calibrate() {
	uint16_t acc = 0;
	for (int i = 0; i < 10; i++) {
		acc += analogRead(pin);
	}
	zero = acc / 10;
	return zero;
}

void ACS712::setZeroPoint(int _zero) {
	zero = _zero;
}

void ACS712::setSensitivity(float sens) {
	sensitivity = sens;
}

void ACS712::setVoltageReference(float volts){
  voltageReference = volts;
}

void ACS712::setAdcResolution(float adcResolution){
  adcScale = adcResolution;
}

float ACS712::getVoltage(){
	int16_t acc = 0;
	for (int i = 0; i < 10; i++) {
		acc += analogRead(pin);
	}  
	float V = (float)acc / 10.0 / adcScale * voltageReference;
	return V;
}

float ACS712::getCurrentDC() {
	int16_t acc = 0;
	for (int i = 0; i < 10; i++) {
		acc += analogRead(pin) - zero;
	}
	float I = (float)acc / 10.0 / adcScale * voltageReference / sensitivity;
	return I;
}

float ACS712::getCurrentAC(uint16_t frequency) {
	uint32_t period = 1000000 / frequency;
	uint32_t t_start = micros();

	uint32_t Isum = 0, measurements_count = 0;
	int32_t Inow;

	while (micros() - t_start < period) {
		Inow = analogRead(pin) - zero;
		Isum += Inow*Inow;
		measurements_count++;
	}

	float Irms = sqrt(Isum / measurements_count) / adcScale * voltageReference / sensitivity;
	return Irms;
}