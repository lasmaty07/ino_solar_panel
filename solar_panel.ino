#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define LDRPin A0
#define PushButton 6
#define GreenLed 7
#define YelowwLed 8
#define RedLed 9
#define MONTHS 5
#define HOURS 5
#define INTERVAL 100
#define INTERVAL2 500
#define ROW_SET 3

unsigned long lastTime;
unsigned long lastHour;

bool shown = false;

typedef struct 
{
	// comment out to save space 
	//int humidity;
	//int temp;
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
	// pinMode(LDRPin, INPUT);
	// pinMode(PushButton, INPUT);
	// pinMode(GreenLed, OUTPUT);
	// pinMode(YelowwLed, OUTPUT);
	// pinMode(RedLed, OUTPUT);
	lcd.begin(16, 2);
    lcd.print("Iniciando");	
	randomSeed(millis());
	
	lastTime = 0 ;
	lastHour = 0 ;
}

void loop() {
	
	if (!dataComplete()){
		//readData();
		completeMatrixTest();
	}
	
	printData();
	
	//buttonListener();
}

void readData(){
	// TODO read temp & humidity

	if ((lastTime + INTERVAL) < millis()){
		//int lumens = mapLumens();
		mediciones++;
		totLumens += random(10, 101);  //delete after test
		
		//totLumens += mapLumens();
		
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
		Serial.print(mediciones);		
		Serial.print(", Promedio :");
		Serial.print(lumens);
		Serial.print(", Se grabaran en :");
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
	
	if (!shown && dataComplete()){
		for (int k = 0 ; k < MONTHS ; k++){
			Serial.print("|");			
			for(int p = 0 ; p < HOURS ; p++){
				if	(p != 0 ) Serial.print(",");			
				Serial.print(matrix[k][p].lumens);			
			}
			Serial.println("|");			
		}
	
	
	if (!dataComplete()){
		digitalWrite(RedLed,HIGH);
	}else {
		digitalWrite(RedLed,LOW);
		digitalWrite(GreenLed,HIGH);
	}
	
	
		buscarMaxOpt1();
		buscarMaxOpt2();
		shown = true;
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
	return (j == (HOURS -1) && i == (MONTHS -1) );
}

void buscarMaxOpt1(){	
	
	//crea una matriz con los maximos mensuales
	int arrAux[MONTHS][HOURS-ROW_SET+1];
	
	//inicializar la matriz - fix de un bug
	for (int k = 0 ; k < MONTHS ; k++){		
		for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
			arrAux[k][l] = 0;
		}
	}

	for (int k = 0 ; k < MONTHS ; k++){		
		for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
			for(int p = 0 ; p < ROW_SET ; p++){
				arrAux[k][l] += matrix[k][l+p].lumens;
			}
		}
	}
	
	//print por serial
	Serial.println("Busca Max Opt1");
	for (int k = 0 ; k < MONTHS ; k++){		
		Serial.println("|");
		for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
			Serial.print(",");
			Serial.print(arrAux[k][l]);
		}
		Serial.println("|");
	}

	//busco el maximo
	int max1 = 0 ;
	for (int k = 0 ; k < MONTHS ; k++){		
		for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
			if (max1 < arrAux[k][l]){
				max1 = arrAux[k][l];
			}			
		}
	}

	Serial.print("Maximo de la matriz: ");
	Serial.println(max1);
}

void buscarMaxOpt2(){	
	
	//crea un arrar con las mediciones de todos los meses sumadas.
	int arrAux[HOURS-ROW_SET+1];	
	
	//inicializar el vector - fix de un bug
	for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
		arrAux[l] = 0;
	}

	for (int k = 0 ; k < MONTHS ; k++){		
		for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
			for(int p = 0 ; p < ROW_SET ; p++){
				arrAux[l] += matrix[k][l+p].lumens;
			}
		}
	}

	//print por serial
	Serial.println("Busca Max Opt2");
	Serial.println("|");	
	for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
		Serial.print(",");
		Serial.print(arrAux[l]);
	}
	Serial.println("|");
		
	//busco el maximo
	int max2 = 0 ;
	for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
		if (max2 < arrAux[l]){
			max2 = arrAux[l];
		}			
	}

	Serial.print("Maximo del Vector: ");
	Serial.println(max2);
}


//just to speed up the process
void completeMatrixTest(){
	j = HOURS -1;
	i = MONTHS -1;
	Serial.println("Carga AUTO de la matriz");	
	for (int k = 0 ; k < MONTHS ; k++){		
		for(int l = 0 ; l < HOURS ; l++){
			delay(100);
			matrix[l][k].lumens = random(10, 101);
		}
	}	
	Serial.println("FIN Carga AUTO de la matriz");		
}