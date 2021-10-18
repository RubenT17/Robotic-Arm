 /*
        PROYECTO PARA UN BRAZO ROBÓTICO CON 3 GRADOS DE LIBERTAD
 * ************************************************************************
 *
 *  Created on:   06.06.2021
 *  Author:       Rubén Torres Bermúdez <rubentorresbermudez@gmail.com>
 *
 *  Description:
 *    Este archivo .ino contiene el código para el control de un brazo
 *    robótico de tres grados de libertad con lecura de datos a través de
 *    un monitoreo por puerto serial. Además tiene un modo reposo, el cual
 *    se activa con una interrupción externa en el pin digital 2.
 *  
 *   2021 Rubén Torres Bermúdez.
 *
 *   This project is licensed under the GNU General Public License v3.0
 *
 * ************************************************************************
*/


#include <Servo.h> 
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


  //***INICIALIZACIÓN INTERFAZ***
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
      lectura.remove(lectura.length()-1);
      Serial.print ("Comando \"");
      Serial.print (lectura);
      Serial.print ("\" no reconocido\n");
    }
}


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
    - Pin 7:                           
    - Pin 8:                           
    - Pin 9 (PWM): PWM Giro            
    - Pin 10 (PWM):                    
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
