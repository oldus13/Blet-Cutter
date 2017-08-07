/*
 Name:		ServoContrlerFW.ino
 Created:	7/31/2017 11:03:23 PM
 Author:	oldus13

 Boards:	Arduino Mini
*/



// the setup function runs once when you press reset or power the board
#include <Servo.h>

Servo cutServo;


#define Vout 2

#define inTl 5

boolean incom = false;
boolean outcom = false;


void setup()
{
	Serial.begin(9600);

	pinMode(Vout, OUTPUT);
	digitalWrite(Vout, HIGH);

	pinMode(inTl, INPUT);


	// cutter servo
	cutServo.attach(9);
	cutServo.write(110);			// open cutter
	

	delay(200);


	outcom = true;
	Serial.println(outcom);

	
}

// the loop function runs over and over again until power down or reset
void loop()
{
	int value = digitalRead(inTl);
	int pos =100;
	if(value == 1)
	{
		for (pos = 110; pos <= 220; pos += 1) //je od úhlu 0 do úhlu 180
		{
			cutServo.write(pos);		//natočení motoru na aktuální úhel
			delay(5);					//chvilka čekání než se motor natočí
		
		}
		delay(1000);
		Serial.println("Cut");
		for (pos = 220; pos >= 100; pos -= 10) //je od úhlu 180 zpět do úhlu 0
		{
			cutServo.write(pos);  //natočení motoru na aktuální úhel
			delay(15);           //chvilka čekání než se motor natočí

		}
	}
	else 
	{
		value = 0;
	}


	

	if (Serial.available() > 0)			// read the incoming byte:
	{

		incom = Serial.read();

		if (incom == true);
		{

			for (pos = 110; pos <= 220; pos += 1) //je od úhlu 0 do úhlu 180
			{
				cutServo.write(pos);		//natočení motoru na aktuální úhel
				delay(5);					//chvilka čekání než se motor natočí

			}
			delay(1000);
			Serial.println("Cut");
			for (pos = 220; pos >= 100; pos -= 10) //je od úhlu 180 zpět do úhlu 0
			{
				cutServo.write(pos);  //natočení motoru na aktuální úhel
				delay(15);           //chvilka čekání než se motor natočí

			}
			outcom = true;
			Serial.println(outcom);

			

		}
		
	}
}
