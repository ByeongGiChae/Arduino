#include "Max6675.h"

Max6675 ts(8, 9, 10);
// Max6675 module: SO on pin #8, SS on pin #9, CSK on pin #10 of Arduino UNO
// Other pins are capable to run this library, as long as digitalRead works on SO,
// and digitalWrite works on SS and CSK

void setup()
{
	ts.setOffset(0);
	// set offset for temperature measurement.
	// 1 stannds for 0.25 Celsius

	Serial.begin(9600);
}


void loop()
{
	Serial.print(ts.getCelsius(), 2);
	Serial.print(" C / ");
	Serial.print(ts.getFahrenheit(), 2);
	Serial.print(" F / ");
	Serial.print(ts.getKelvin(), 2);
	Serial.print(" K\n");
	delay(300);
}
