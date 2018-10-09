#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define LDRPin A0
#define PushButton 2
#define GreenLed 3
#define YelowwLed 4
#define RedLed 5
#define MONTHS 12
#define HOURS 15
#define INTERVAL 500

unsigned long lastTime;
unsigned long millis_held;    
unsigned long secs_held;
//(millis() - firstTime);

typedef struct 
{
   int humidity;
   int temp;
   int lumens;
 }  record_type;
 
record_type matrix[MONTHS][HOURS];

int i = 0;
int j = 0;


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
	randomSeed(millis());
	
	lastTime = 0 ;
}

void loop() {
	
	if (!dataComplete()){
		readData();
	}
	
	printData();
	
	buttonListener();
}

void readData(){
	// TODO read temp & humidity


	if ((lastTime + INTERVAL) > millis()){
		lastTime = millis();
		
		//int lumens = mapLumens();
		int lumens = random(1, 101);
		
		if (j < (HOURS - 1) ){
			j++;
		} else {
			j = 0;
			i++;		
		}
		Serial.print("Se leyeron :");
		Serial.print(lumens);
		Serial.print(" Se grabaran en :");
		Serial.print(i);
		Serial.print(" ");
		Serial.println(j);
		
		matrix[i][j].lumens = lumens;
	}
}

void printData(){
	//TODO 
	//if end of month print stats -> do math
	
	//just a test
	if (dataComplete()){
		for (int k = 0 ; k < MONTHS ; k++){
			Serial.print("|");			
			for(int p = 0 ; p < HOURS ; p++){
				Serial.print(",");			
				Serial.print(matrix[i][j].lumens);			
			}
			Serial.println("|");			
		}
	}
	
	//TODO if end of year -> do more math......
}

void buttonListener(){
	//TODO 
	//fancy stuff on display on button push
}

int mapLumens(){
	//TODO Map lumens to 1-100 int 
	return analogRead(LDRPin);
}

bool dataComplete(){
	return (j == HOURS && i == MONTHS);
}