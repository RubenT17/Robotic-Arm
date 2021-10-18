
#include <Servo.h>    //Importa la librería Servo del Arduino
#include <string.h>

Servo brazo;
  int pinbrazo = 3;
  int angulobrazo = 0;
Servo antebrazo;
  int pinantebrazo = 5;
  int anguloantebrazo = 180;
Servo pinza;
  int pinpinza = 6;
  int angulopinza = 100;
  int cerrarpinza = 11;
  int abrirpinza = 12;
Servo giro;
  int pingiro = 9;
  int angulogiro = 90;
  int giroder = 11;
  int giroizq = 12;
  
int x1;   //   |
int y1;   //   |    => JoyStick
int y2;   //   |
int pulsomin = 1530;        //Valor de tiempo del pulso mínimo en us (por la función attach) para 0 grados
int pulsomax = 2470;        //Valor de tiempo del pulso máximo en us (por la función attach) para 180 grados

//***Giro horizontal***
/*int der;
int izq;
int ppA = 7;
int ppB = 8;
int ppC = 9;
int ppD = 10;
int tiempo = 1;      //Por fabricante, no debe ser menor a 10 ms
int i=0;
int a=0;
int paso [4] [4] =        //Matriz 4x4 StepbyStep
{
    {1,1,0,0},
    {0,1,1,0},
    {0,0,1,1},
    {1,0,0,1},
};
*/
//***Otros***
int presion; //Analógico??
int pinreposo = 2;  //CONECTAR EN PULL-DOWN. NA --> 0
int flag;
int LEDreposo = 13;
int a=0;
unsigned int tiempo = 0;

////////////////////////////////////////////////////////////////////////////////////

void setup(){

     //***INICIALIZACIÓN SERIAL***
  Serial.begin (115200);
  Serial.print ("Inicializando...\n");
  
  //***INTERRUPCIÓN REPOSO***
  attachInterrupt (digitalPinToInterrupt(pinreposo), reposo, RISING); //Se activa la función "reposo" mientras "pinreposo" está en LOW
  flag = 1; 
  digitalWrite (LEDreposo, 1);

  //***MODOS DE PIN***
  pinMode (cerrarpinza, INPUT);
  pinMode (abrirpinza, INPUT);
  pinMode (pinreposo, INPUT);
  pinMode (LEDreposo, OUTPUT);
/*
  pinMode (ppA, OUTPUT);
  pinMode (ppB, OUTPUT);
  pinMode (ppC, OUTPUT);
  pinMode (ppD, OUTPUT);
*/ 
  //***BRAZO*** 
  brazo.attach (pinbrazo);
  brazo.write (angulobrazo);
  
  //***ANTEBRAZO***
  antebrazo.attach (pinantebrazo);
  antebrazo.write (anguloantebrazo);

  //***PINZA***
  pinza.attach (pinpinza, pulsomin, pulsomax);
  pinza.write (angulopinza);

  //***GIRO***
  giro.attach (pingiro, pulsomin, pulsomax);
  giro.write (angulogiro);

/*
  //***INICIALIZACIÓN GIRO HORIZONTAL***
  while (a<2048){
    digitalWrite (ppA, paso [i][0]);     // En la matriz "paso", filas son las "i" 
    delay (tiempo); 
    digitalWrite (ppB, paso [i][1]);     // los 0,1,2,3 son las columnas. Por cada ciclo se suma un "i"
    delay (tiempo);
    digitalWrite (ppC, paso [i][2]);     // entonces recorrerá la matriz de izq a der y de arriba a abajo
    delay (tiempo);
    digitalWrite (ppD, paso [i][3]);
    delay (tiempo);
    i++;
    a++;
    if (i==4)
      i=0;
  }
  while (a!=0){
    digitalWrite (ppA, paso [i][3]); 
    delay (tiempo); 
    digitalWrite (ppB, paso [i][2]);
    delay (tiempo);
    digitalWrite (ppC, paso [i][1]);
    delay (tiempo);
    digitalWrite (ppD, paso [i][0]);
    delay (tiempo);
    i++;
    a--;
    if (i==4)
      i=0;  
    }
  digitalWrite (ppA, LOW);      //Desactiva las bobinas para que no se queden alimentadas 
  digitalWrite (ppB, LOW);      //  al iniciar un nuevo ciclo
  digitalWrite (ppC, LOW);
  digitalWrite (ppD, LOW); 
*/ 
  //***INICIALIZACIÓN SERIAL***
  Serial.print ("Pulse el botón de reposo o escriba \"reposo\" para comenzar\n");
}

////////////////////////////////////////////////////////////////////////////////////

void loop() {
  while (flag==1){
    if (a<1){
    angulobrazo = 0;
    anguloantebrazo = 180;
    angulopinza = 100;
    angulogiro = 90;
    brazo.write (angulobrazo);
    antebrazo.write (anguloantebrazo);
    pinza.write(angulopinza);
    giro.write(angulogiro);
    digitalWrite (LEDreposo, 1);
    a++;
    }
    if (Serial.available()>0)
      serial(); 
    }
  a=0;  
  digitalWrite (LEDreposo, 0);
  
  Servos();
//  Giro();

 //***INFO LECTURAS***
  if (Serial.available()>0)
    serial();
}

////////////////////////////////////////////////////////////////////////////////////
void Servos (){
  x1 = analogRead (A0);
  y1 = analogRead (A1);
  y2 = analogRead (A2);
  angulobrazo = map (y1, 0, 1023, 0, 80);
  anguloantebrazo = map (y2, 0, 1023, 180, 70);

  if ((digitalRead(abrirpinza)==LOW) & (digitalRead(cerrarpinza)==HIGH) & (angulopinza<100))
    angulopinza++;
  
  if ((digitalRead(abrirpinza)==HIGH) & (digitalRead(cerrarpinza)==LOW) & (angulopinza>0))
    angulopinza--;

  if (x1>600)
    angulogiro++;

  if (x1<400)
    angulogiro--;

  if (millis()>tiempo){
  pinza.write (angulopinza);
  brazo.write (angulobrazo);
  antebrazo.write (anguloantebrazo);
  giro.write (angulogiro);

  tiempo = millis() + 200;
  }
}


////////////////////////////////////////////////////////////////////////////////////
void reposo() {
  flag = !flag;
  if (flag==1)
    Serial.print ("Modo REPOSO activado\n-------------------------\n");
  else
    Serial.print ("Modo REPOSO desactivado\n-------------------------\n");
}


////////////////////////////////////////////////////////////////////////////////////
void serial () {
  String lectura = Serial.readString();
  if (lectura == "reposo\n"){
    flag = !flag;
    if (flag == 1)
      Serial.print ("Modo REPOSO activado\n-------------------------\n");
    else
      Serial.print ("Modo REPOSO desactivado\n-------------------------\n");
    }

  else if (lectura == "info\n") {
    x1 = analogRead (A0);
    y1 = analogRead (A1);
    y2 = analogRead (A2);
    Serial.print ("Modo Reposo: ");
      if (flag)
        Serial.println ("ON");
      if (!flag)
        Serial.println ("OFF");    
    Serial.print ("Valor x1: ");
    Serial.println (x1);
    Serial.print ("Valor y1: ");
    Serial.println (y1);  
    Serial.print ("Valor y2: ");
    Serial.println (y2);
    
/*  Serial.print ("Valor der: ");
    Serial.println (der);
    Serial.print ("Valor izq: ");
    Serial.println (izq);
    Serial.print ("Valor angulo horizontal: ");
    Serial.println ((a/1024)*180); // Grados = Nºde pasos * 180grados / 1024 pasos
*/
    Serial.print ("Valor angulo brazo: ");
    Serial.println (angulobrazo);
    Serial.print ("Valor angulo antebrazo: ");
    Serial.println (anguloantebrazo);
    Serial.print ("Valor angulo pinza: ");
    Serial.println (angulopinza);
    Serial.print ("Estado pinreposo: ");
    Serial.println (digitalRead (pinreposo));
    Serial.print ("Estado cerrarpinza: ");
    Serial.println (digitalRead (cerrarpinza));
    Serial.print ("Estado abrirpinza: ");
    Serial.println (digitalRead (abrirpinza));
    Serial.println ("-------------------------");
    }
    
    else {
      //unsigned int index = lectura.length();
      lectura.remove(lectura.length()-1);
      Serial.print ("Comando \"");
      Serial.print (lectura);
      Serial.print ("\" no reconocido\n");
    }
}


////////////////////////////////////////////////////////////////////////////////////
/*void Giro() {
  x1 = analogRead (A0);
  
  if (x1>500 & (a<854 || a==-854)) {
    digitalWrite (ppA, paso [i][0]);     // En la matriz "paso", filas son las "i" 
    delay (tiempo); 
    digitalWrite (ppB, paso [i][1]);     // los 0,1,2,3 son las columnas. Por cada ciclo se suma un "i"
    delay (tiempo);
    digitalWrite (ppC, paso [i][2]);     // entonces recorrerá la matriz de izq a der y de arriba a abajo
    delay (tiempo);
    digitalWrite (ppD, paso [i][3]);
    delay (tiempo);
    i++;
    a++;
    if (i==4)
      i=0;
    }

  if (x1<480 & (a>-854 || a==854)) {
    digitalWrite (ppA, paso [i][3]); 
    delay (tiempo); 
    digitalWrite (ppB, paso [i][2]);
    delay (tiempo);
    digitalWrite (ppC, paso [i][1]);
    delay (tiempo);
    digitalWrite (ppD, paso [i][0]);
    delay (tiempo);
    i++;
    a--;
    if (i==4)
      i=0;
    }
    
  else {
    digitalWrite (ppA, LOW);
    digitalWrite (ppB, LOW);
    digitalWrite (ppC, LOW);
    digitalWrite (ppD, LOW);
    }
}
*/


/* 
*******************************CONEXIÓN DE PINES******************************* 

    DIGITAL:
    - Pin 0: Vacío.
    - Pin 1: Vacío.
    - Pin 2: Pulsador Reposo.
    - Pin 3 (PWM): PWM Brazo
    - Pin 4:
    - Pin 5 (PWM): PWM Antebrazo
    - Pin 6 (PWM): PWM Pinza
    - Pin 7:                           Bobina ppA
    - Pin 8:                           Bobina ppB
    - Pin 9 (PWM): PWM Giro            Bobina ppC
    - Pin 10 (PWM):                    Bobina ppD
    - Pin 11 (PWM): Cerrar Pinza
    - Pin 12: Abrir Pinza
    - Pin 13: 

    ANALÓGICO:
    - A0: Eje X.1
    - A1: Eje Y.1
    - A2: Eje Y.2
    - A3:
    - A4:
    - A5: 
*/
