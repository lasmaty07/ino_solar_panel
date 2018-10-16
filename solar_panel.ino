#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define LDRPin A0
#define PushButton 2
#define GreenLed 3
#define YelowwLed 4
#define RedLed 5
#define MONTHS 12
#define HOURS 15
#define INTERVAL 100
#define INTERVAL2 1000

unsigned long lastTime;
unsigned long lastHour;

typedef struct 
{
   int humidity;
   int temp;
   int lumens;
 }  record_type;
 
record_type matrix[MONTHS][HOURS];

int i = 0;
int j = -1 ; //ugly af
int mediciones = 0;
int totLumens = 0;


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
	lastHour = 0 ;
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

	if ((lastTime + INTERVAL) < millis()){
		//int lumens = mapLumens();
		mediciones++;
		totLumens += random(10, 101);  //delete after test
		
		//totLumens = mapLumens();
		
		lastTime = millis();
	}

	if ((lastHour + INTERVAL2) < millis()){
		
		lastHour = millis();

		int lumens = totLumens / mediciones ; //al ser int, solo guarda int

		
		if (j < (HOURS -1 ) ){
			j++;
		} else {
			j = 0;
			i++;		
		}

		Serial.print("mediciones:");
		Serial.println(mediciones);
		
		Serial.print("Se leyeron :");
		Serial.print(lumens);
		Serial.print(" Se grabaran en :");
		Serial.print(i);
		Serial.print(" ,");
		Serial.println(j);		
	
		matrix[i][j].lumens = lumens;
		
		mediciones = 0;
		totLumens = 0;
	}
}

void printData(){
	//TODO 
	//if end of month print stats -> do math
	
	//TODO if end of year -> do more math......
	
	//show matrix via Serial
	if (dataComplete()){
		delay(1000);
		for (int k = 0 ; k < MONTHS ; k++){
			Serial.print("|");			
			for(int p = 0 ; p < HOURS ; p++){
				if	(p != 0 ) Serial.print(",");			
				Serial.print(matrix[k][p].lumens);			
			}
			Serial.println("|");			
		}
	}
	
	
	if (!dataComplete()){
		digitalWrite(RedLed,HIGH);
	}else {
		digitalWrite(RedLed,LOW);
		digitalWrite(GreenLed,HIGH);
	}
	
	
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
	return (j == HOURS -1 && i == MONTHS -1 );
}