const int dirPin = 2; 
const int stepPin = 3;

int xPin = A0;
int xVal;

int ButtonPin = 10;         
int ButtonState = 0;

int MosfetPin = 9;        
int LeftSwitchPin = 7;
int RightSwitchPin = 6;

int LeftSwitchState = 0;
int RightSwitchState = 0;
int dt = 50;

int stepsPerRevolution = 200;
int stepDelay = 2;


void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(xPin, INPUT);
  
  pinMode(ButtonPin, INPUT_PULLUP);     // Botón con resistencia interna pull-up
  pinMode(MosfetPin, OUTPUT);           // Salida para el MOSFET que controla el solenoide
  
  pinMode(LeftSwitchPin, INPUT_PULLUP);
  pinMode(RightSwitchPin, INPUT_PULLUP);

  digitalWrite(MosfetPin, LOW);         // Asegura que el solenoide esté apagado al inicio

  Serial.begin(9600);
}

void loop() {

  ButtonState = digitalRead(ButtonPin);
  
  //  Control del solenoide
  if (ButtonState == LOW) {             // Botón presionado (activo en LOW)
    digitalWrite(MosfetPin, HIGH);      // Activa el solenoide
  } else {
    digitalWrite(MosfetPin, LOW);       // Desactiva el solenoide
  }

  xVal = analogRead(xPin);
  LeftSwitchState = digitalRead(LeftSwitchPin);
  RightSwitchState = digitalRead(RightSwitchPin);

  // Depuración
  Serial.print("X value = ");
  Serial.print(xVal);
  Serial.print(" - Left Switch = ");
  Serial.print(LeftSwitchState);
  Serial.print(" - Right Switch = ");
  Serial.print(RightSwitchState);
  Serial.print(" - Button = ");
  Serial.println(ButtonState);

  //  Control del motor a pasos basado en el joystick
  if (RightSwitchState == HIGH && xVal < 450) {
    digitalWrite(dirPin, HIGH); // Derecha
    for (int i = 0; i < 100; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(650);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(650);
    }
  }
  else if (LeftSwitchState == HIGH && xVal > 650) {
    digitalWrite(dirPin, LOW); // Izquierda
    for (int i = 0; i < 100; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(650);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(650);
    }
  }
}
