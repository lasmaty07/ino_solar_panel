#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define LDRPin A0
#define PushButton 2
#define GreenLed 3
#define YelowwLed 4
#define RedLed 5
#define MONTHS 12
#define HOURS 15

typedef struct 
{
   int humidity;
   int temp;
   int lumens;
 }  record_type;
 
record_type matrix[MONTHS][HOURS];

int i = MONTHS;
int j = HOURS;


void setup() {
	Serial.begin(9600); 
	Serial.println("INIT");
	pinMode(LDRPin, INPUT);
	pinMode(PushButton, INPUT);
	pinMode(GreenLed, OUTPUT);
	pinMode(YelowwLed, OUTPUT);
	pinMode(RedLed, OUTPUT);
	lcd.begin(16, 2);	
    lcd.print("");	
}

void loop() {
	
	// < OR <=    ----> check
	if (i < MONTHS && j < HOURS){
		readData();
	}
	
	printData();
}

void readData(){
	//TODO time delay 
	
	int lumens = mapLumens();
	
	//TODO read temp & humidity

	if (j = HOURS ){
		j = 0;
		i++;
	} else {
		j++;
	}
	matrix[i][j].lumens = lumens;
}

void printData(){
	//TODO 
	//if end of month print stats
	
	//TODO if end of year -> do math......
}


int mapLumens(){
	//TODO Map lumens to 1-100 int 
	return analogRead(LDRPin);
}
