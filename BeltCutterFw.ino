/*
 Name:		BeltCutterFw.ino
 Created:	7/31/2017 8:43:01 PM
 Author:	oldus13

*/
#include <LiquidCrystal.h>



// Lcd - ogiginalPrusa i3
#define LCD_PINS_RS 82
#define LCD_PINS_ENABLE 18
#define LCD_PINS_D4 19
#define LCD_PINS_D5 70
#define LCD_PINS_D6 85
#define LCD_PINS_D7 71

#define BTN_EN1 72
#define BTN_EN2 14
#define BTN_ENC 9

#define stepper_step 35
#define stepper_dir 47

#define dir_in 1
#define dir_out 0

/* Slot - pin Rambo 1.3a

dir		step	en		ref-pwm

x	48		37		29		46

y	49		36		28		46

z	47		35		27		45

e0	43		34		26		44

*/

#define servo_Cut 16

#define cut_servo_open 1700
#define cut_servo_close 900

				
#define in A2								//opto

LiquidCrystal lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5, LCD_PINS_D6, LCD_PINS_D7);

int cut_servo_pos = 0;
bool cut_servo_delay = false;
long cut_delay_micros = 0;
bool cut_servo_state = false;

float steps_per_mm = 12.5;

long total = 0;
long lcd_micros = 0;

long step_micros = 100;
int step_speed = 3000;
bool step_state = true;

long steps_length = 0;
long set_length = 0;


long piece = 0;


// Opto #1

int valIn ;

int i = 0;

long startMillis = 0;
long endMillis = 0;

boolean incom = false;
boolean outcom = false;

// the setup function runs once when you press reset or power the board
void setup()
{
	Serial2.begin(9600);
	Serial.begin(9600);

	// LCD control buton
	pinMode(BTN_EN1, INPUT_PULLUP);
	pinMode(BTN_EN2, INPUT_PULLUP);
	pinMode(BTN_ENC, INPUT_PULLUP);


	// stepper
	pinMode(stepper_step, OUTPUT); // step
	pinMode(stepper_dir, OUTPUT); // dir

	// opto
							
	// cutter servo
	pinMode(16, OUTPUT);



	pinMode(27, OUTPUT); // enable 
	digitalWrite(27, LOW);

	pinMode(45, OUTPUT); // current
	analogWrite(45, 50);

	
	lcd.begin(20, 4);
	lcd.clear();
	menu();

}

// the loop function runs over and over again until power down or reset
void loop()
{   
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(" Automacic cutting");
		
	lcd.setCursor(0, 1);
	lcd.print("Lenght:         mm");
	lcd.setCursor(12, 1);
	lcd.print(set_length);

	lcd.setCursor(0, 2);
	lcd.print("Pieces:    /    pc");
	lcd.setCursor(12, 2);
	lcd.print(piece);
	lcd.setCursor(0, 3);
	lcd.print("Start");
	delay(1500);
	steps_length = set_length * steps_per_mm;

	long step_micros = 0;
	int step_speed = 3000;
	digitalWrite(stepper_dir, dir_out);
	valIn = analogRead(in);
	
	if (i <= piece-1)
	{
		lcd.setCursor(8, 2);
		lcd.print(i);
		lcd.setCursor(0, 3);
		lcd.print("Moving");
		
		
		// move 
		do
		{
			if (micros() - step_micros > step_speed)
			{
				if (step_state)
				{
					digitalWrite(35, HIGH);
					step_state = false;
					step_micros = micros();
					steps_length--;
				}
				if (!step_state)
				{
					digitalWrite(35, LOW);
					step_state = true;
					step_micros = micros();
					steps_length--;
				}
			}
		} while (steps_length > 0);
		
		i = i + 1;
		Serial.print(i);
		Serial.print(" ");
		Serial.println(millis());
		outcom = true;
		lcd.setCursor(0, 3);
		lcd.print("  Cutting !   ");
		Serial2.write(outcom);
		delay(3000);
		outcom = false;
		lcd.setCursor(0, 3);
		lcd.print("            ");


		delay(500);
		
		loop();
		
	}
	else
	{
		lcd.setCursor(0, 3);
		lcd.print("  !DONE!      pc ");
		lcd.setCursor(10, 3);
		lcd.print(i);
		endMillis = millis();
		Serial.print(  "DONE Pices: ");
		Serial.print(i);
		Serial.print("pc Time: ");
		Serial.print((endMillis-startMillis)/60000);
		Serial.println("min");
		delay(60*1000);
		lcd.clear();
		menu();

	}
		
		
}

void menu()
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("  Belt cutter V0.3 ");
	Serial.println("    ");
	Serial.print("  Belt cutter V0.3  ");
	Serial.println(millis());
	delay(1000);

	int item = 0;
	bool done = false;

	do
	{
		switch (item)
		{
		case 0:
			lcd.setCursor(0, 1);
			lcd.print("AutoCut X-Axis 850mm ");
			lcd.setCursor(0, 2);
			lcd.print("> start            ");
			delay(100);
			break;
		case 1:
			lcd.setCursor(0, 1);
			lcd.print("AutoCut Y-Axis 650mm ");
			lcd.setCursor(0, 2);
			lcd.print("> start            ");
			delay(100);
			break;
		case 2:
			lcd.setCursor(0, 1);
			lcd.print("Servis Menu:        ");
			lcd.setCursor(0, 2);
			lcd.print("> load Belt        ");
			delay(100);
			break;
		case 3:
			lcd.setCursor(0, 1);
			lcd.print("Servis Menu:        ");
			lcd.setCursor(0, 2);
			lcd.print("> unload Belt        ");
			delay(100);
			break;
		case 4:
			lcd.setCursor(0, 1);
			lcd.print("Servis Menu:        ");
			lcd.setCursor(0, 2);
			lcd.print("> cut              ");
			delay(100);
			break;
		case 5:
			lcd.setCursor(0, 1);
			lcd.print("Servis Menu:        ");
			lcd.setCursor(0, 2);
			lcd.print("> belt check       ");
			delay(100);
			break;
		case 6:
			lcd.setCursor(0, 1);
			lcd.print("Servis Menu:        ");
			lcd.setCursor(0, 2);
			lcd.print("> RESET            ");
			delay(100);
			break;

		}


		if (digitalRead(BTN_EN1) == 0)
		{
			item = item + 1;
			if (item > 7) { item = 0; }
			delay(100);
		}

		if (digitalRead(BTN_ENC) == 0)
		{
			delay(100);

			switch (item)
			{
			case 0:
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("AutoCut X-Axis 850mm");
				Serial.print("  AutoCut X-Axis 850mm  ");
				Serial.println(millis());
				delay(500);
				set_length = 100;						// x-axis 850mm
				steps_length = 0;
				done = true;
				break;
			case 1:
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("AutoCut Y-Axis 650mm");
				Serial.print("  AutoCut X-Axis 850mm  ");
				Serial.println(millis());
				delay(500);
				set_length = 650;						// z-axis 650mm
				steps_length = 0;
				done = true;

				break;
			case 2:
				loadCheck();
				break;
			case 3:
				unload();
				break;
			case 4:
				cut();
				break;
			case 5:
				belt_check();
				break;
			case 6:
				total = 0;
				lcd.setCursor(0, 3);
				lcd.print("reset DONE ! ");
				delay(500);
				lcd.setCursor(0, 3);
				lcd.print("             ");
				menu();
				break;
			default:
				done = false;
				break;
			}
		}



	} while (!done);
	
	autoCutStart();

}

void autoCutStart()
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(" Start automatically");
	lcd.setCursor(0, 1);
	lcd.print("                    ");
	Serial.print("  Start automatically  ");
	Serial.println(millis());
	incom = Serial2.read();
	valIn = analogRead(in);
	
	// srevo check
	

	if (incom == true)
	{
		lcd.setCursor(0, 1);
		lcd.print("Servo Check     ");
		lcd.setCursor(0, 2);
		lcd.print("Servo Open      ");

		delay(1000);
	}
	else
	{
		lcd.setCursor(0, 1);
		lcd.print("Servo Check     ");
		lcd.setCursor(0, 2);
		lcd.print("!!!CHECK SERVO!!!");
		delay(3000);
		menu();
	}
	

	//belt check
	

	if (valIn > 100)
	{
		lcd.setCursor(0, 1);
		lcd.print("Belt Check     ");
		lcd.setCursor(0, 2);
		lcd.print("Belt  IN       ");
		delay(1000);
		menu_setPeaces();

	}

	else
	{
		lcd.setCursor(0, 1);
		lcd.print("Belt Check     ");
		lcd.setCursor(0, 2);
		lcd.print("!!!MISING BELT!!!");
		lcd.setCursor(0, 3);
		lcd.print("Hold butom for load");
		delay(5000);
		if (digitalRead(BTN_ENC) == 0)
		{
			delay(100);
			loadCheck();
			delay(1000);
			lcd.setCursor(0, 3);
			lcd.print("                    ");
			autoCutStart();
			
		}

	}
	menu();
}

void cut()
{
	outcom = true;
	Serial2.write(outcom);
	lcd.setCursor(0, 3);
	lcd.print("  Cutting !   ");
	delay(100);
	outcom = false;
	Serial2.write(outcom);
	delay(10);
	lcd.setCursor(0, 3);
	lcd.print("            ");
	delay(500);

}

void menu_setPeaces()
{
	int item = 0;
	bool done = false;
		
	//set pieces
	do
	{
		switch (item)
		{
		case 0:
			lcd.setCursor(0, 2);
			lcd.print("Set peces:  10pc ");
			break;
		case 1:
			lcd.setCursor(0, 2);
			lcd.print("Set peces:  20pc ");
			break;
		case 2:
			lcd.setCursor(0, 2);
			lcd.print("Set peces:  50pc ");
			break;
		case 3:
			lcd.setCursor(0, 2);
			lcd.print("Set peces: 100pc ");
			break;
		case 4:
			lcd.setCursor(0, 2);
			lcd.print("Set peces: 200pc ");
			break;
		}

		if (digitalRead(BTN_EN1) == 0)
		{
			item = item + 1;
			if (item >4) { item = 0; }
			delay(100);
		}

		if (digitalRead(BTN_ENC) == 0)
		{
			delay(100);

			switch (item)
			{
			case 0:
				piece = 2;
				done = true;
				break;
			case 1:
				piece = 20;
				done = true;
				break;
			case 2:
				piece = 50;
				done = true;
				break;
			case 3:
				piece = 100;
				done = true;
				break;
			case 4:
				piece = 200;
				done = true;
				break;
			default:
				done = false;
				break;
			}
		}



	} while (!done);
	Serial.print("  Pieces: ");
	Serial.println(piece);
	startMillis = millis();
	Serial.print("Start time [millis] : ");
	Serial.println(startMillis);
	loop();
	
}

void loadCheck()
{
	steps_length = 375;   //100mm=125step  1 mm = 1.25steps 
	long step_micros = 0;
	int step_speed = 1500;
	
	digitalWrite(stepper_dir, dir_out);
	
	valIn = analogRead(in);
	lcd.clear();
	lcd.setCursor(0, 2);
	lcd.print("  .. loading ..  ");
	
	do
	{



		if (micros() - step_micros > step_speed)
		{
			if (step_state)
			{
				digitalWrite(35, HIGH);
				step_state = false;
				step_micros = micros();
				valIn = analogRead(in);
			}
			if (!step_state)
			{
				digitalWrite(35, LOW);
				step_state = true;
				step_micros = micros();
				valIn = analogRead(in);
			}
		}
	} while (valIn<800); 

	do
	{



		if (micros() - step_micros > step_speed)
		{
			if (step_state)
			{
				digitalWrite(35, HIGH);
				step_state = false;
				step_micros = micros();
				steps_length--;
			}
			if (!step_state)
			{
				digitalWrite(35, LOW);
				step_state = true;
				step_micros = micros();
				steps_length--;
			}
		}
	} while (steps_length > 0);


	delay(500);
	
}

void unload()
{
	steps_length = 500; //  1 mm = 1.25 steps
	long step_micros = 0;
	int step_speed = 1000;

	digitalWrite(stepper_dir, dir_out);

	lcd.setCursor(0, 2);
	lcd.print("  ..unloading..  ");
	do
	{

		if (micros() - step_micros > step_speed)
		{
			if (step_state)
			{
				digitalWrite(35, HIGH);
				step_state = false;
				step_micros = micros();
				steps_length--;
			}
			if (!step_state)
			{
				digitalWrite(37, LOW);
				step_state = true;
				step_micros = micros();
				steps_length--;
			}
		}

	} while (steps_length > 0);

	delay(500);
	menu();
}

void belt_check()
{
	
	valIn = analogRead(in);

	if (valIn > 700)
	{
		lcd.setCursor(0, 2);
		lcd.print("S remenem      ");
		lcd.setCursor(0, 3);
		lcd.print("ValIn:");
		lcd.setCursor(6, 3);
		lcd.print(valIn);
		delay(10000);
		menu();
	}
	if (valIn < 700);
	{
		lcd.setCursor(0, 2);
		lcd.print("Bez remen      ");
		lcd.setCursor(0, 3);
		lcd.print("ValIn:");
		lcd.setCursor(6, 3);
		lcd.print(valIn);
		delay(10000);
		menu();
	}
}

