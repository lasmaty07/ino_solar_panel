#include <LiquidCrystal.h>
#include <string.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define LDRPin A0
#define enterButton 6
#define moveButton 13
#define GreenLed 7
#define YelowLed 8
#define RedLed 9
#define MONTHS 5
#define HOURS 5
#define INTERVAL 100
#define INTERVAL2 500
#define ROW_SET 3
#define LOW_LIGHT 33
#define MID_LIGHT 66
#define MENU_TIMEOUT 1000

unsigned long lastTime;
unsigned long lastHour;
unsigned long enterMenu;

int enterButtonState;
int moveButtonState;
int buttonStateAnt;

int numberOptionsMenu = 3;

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

	pinMode(LDRPin, INPUT);
	pinMode(enterButton, INPUT_PULLUP);
	pinMode(moveButton, INPUT_PULLUP);
	pinMode(GreenLed, OUTPUT);
	pinMode(YelowLed, OUTPUT);
	pinMode(RedLed, OUTPUT);
	
	lcd.begin(16,2);
    lcd.print("Iniciando");	
	
	randomSeed(millis());
	
	lastTime = 0 ;
	lastHour = 0 ;

}

void loop() {
	
	if (!dataComplete()){
		printInit();
		//readData();
		completeMatrixTest();
	}
	
	printData();
	
	buttonListener();
}

void readData(){
	// TODO read temp & humidity

	if ((lastTime + INTERVAL) < millis()){
		mediciones++;
		totLumens += mapLumens();
		
		lastTime = millis();
	}

	if ((lastHour + INTERVAL2) < millis()){
		
		lastHour = millis();
		
		int lumens = totLumens / mediciones ; //al ser int, solo guarda int
		
		//encience los leds segun la cantidad de luz
		showLED(lumens);
		
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
		lcd.clear();
		lcd.print("Finalizo la carga");
		for (int k = 0 ; k < MONTHS ; k++){
			Serial.print("|");			
			for(int p = 0 ; p < HOURS ; p++){
				if	(p != 0 ) Serial.print(",");			
				Serial.print(matrix[k][p].lumens);			
			}
			Serial.println("|");			
		}
	
		buscarMaxOpt1();
		buscarMaxOpt2();
		shown = true;
	}
}

void buttonListener(){
	//TODO 
	//fancy stuff on display on button push
	enterButtonState = digitalRead(enterButton);

	if (dataComplete() && enterButtonState == LOW ){
		//enters menu
		delay(50); //debug bounce effect
		enterMenu = millis();
		enterButtonState = digitalRead(enterButton);
		int option = 0 ;
		while ( option < numberOptionsMenu && (enterMenu + MENU_TIMEOUT) < millis()) {

			if (i == 0){
				bool reset = false;

				delay(200);
				enterButtonState = digitalRead(enterButton);
				
				lcd.clear();

				lcd.setCursor(0,0);
				lcd.print("Reiniciar programa");
				lcd.setCursor(0,1);		

				while ( enterButtonState == HIGH && (enterMenu + MENU_TIMEOUT) < millis()){
					moveButtonState = digitalRead(moveButton);	
					lcd.setCursor(0,1);
					if (moveButtonState == LOW ){
						lcd.print(reset ? "YES" : "NO");
						reset = true;
						//faltaaaaa
					}					

					delay(100);
					enterButtonState = digitalRead(enterButton);
				}		
			}

		}
		//reset
		i = 0;
		j = 0;
		shown = false;
		lcd.clear();
	}
}

int mapLumens(){
	int lum =  analogRead(LDRPin);
	return map(lum, 0, 1000, 0, 100);
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
		Serial.print("|");
		for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
			if	(l != 0 ) Serial.print(",");
			Serial.print(arrAux[k][l]);
		}
		Serial.println("|");
	}

	//busco el maximo	
	Serial.println("Maximo de la matriz: ");	
	for (int k = 0 ; k < MONTHS ; k++){		
		int max1 = 0 ;
		for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
			if (max1 < arrAux[k][l]){
				max1 = arrAux[k][l];
			}			
		}
		Serial.print(getMonth(k));
		Serial.print("\t");
		Serial.println(max1);
	}
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
	Serial.print("|");	
	for(int l = 0 ; l < (HOURS-ROW_SET+1) ; l++){
		if	(l != 0 ) Serial.print(",");
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
	Serial.println("\t");
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

void showLED(int lum){
	if (lum > MID_LIGHT){
		digitalWrite(GreenLed, HIGH);
		digitalWrite(YelowLed, HIGH);
		digitalWrite(RedLed, HIGH);
	} else {
		if (lum > LOW_LIGHT){
			digitalWrite(GreenLed, HIGH);
			digitalWrite(YelowLed, HIGH);
		} else {
			digitalWrite(GreenLed, HIGH);
		}
	}
}

String getMonth(int mm){
	mm++;
	switch (mm) {
  		case 1:
    		return "Enero";
    	break;
		case 2:
    		return "Febrero";
    	break;
		case 3:
    		return "Marzo";
    	break;
		case 4:
    		return "Abril";
    	break;
		case 5:
    		return "Mayo";
    	break;
		case 6:
    		return "Junio";
    	break;
		case 7:
    		return "julio";
    	break;
		case 8:
    		return "Agosto";
    	break;
		case 9:
    		return "Septiembre";
    	break;
		case 10:
    		return "Octubre";
    	break;
		case 11:
    		return "Noviembre";
    	break;
		case 12:
    		return "Diciembre";
    	break;
	}
}

void printInit(){
	Serial.println("Se inicia la lectura de datos");
	lcd.setCursor(0, 0);
	lcd.print("Leyendo datos");
	lcd.setCursor(0, 1);
	lcd.print("Horas: ");
	lcd.print(HOURS);
	lcd.print(" Set: ");
	lcd.print(ROW_SET);
}