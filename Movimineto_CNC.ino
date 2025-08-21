// ===== Pines del CNC Shield =====

// Eje X
const int stepX = 2;
const int dirX  = 5;

// Eje Y
const int stepY = 3;
const int dirY  = 6;

// Eje Z (comentado por ahora)
 const int stepZ = 4;
 const int dirZ  = 7;

// Pin de habilitación compartido
const int enPin = 8;

// Finales de carrera
const int xMinPin = 9; //
const int xMaxPin = A1;
const int yMinPin = 10; //
const int yMaxPin = A3;
 const int zMinPin = 11; //
 const int zMaxPin = A2;

// Límites máximos por eje (ajusta según pruebas)
const int maxX = 1100;
const int maxY = 1600;
 const int maxZ = 3000;

// Posiciones actuales
int posX = 0;
int posY = 0;
int posZ = 0;

void setup() {
  // Pines motor X
  pinMode(stepX, OUTPUT);
  pinMode(dirX, OUTPUT);

  // Pines motor Y
  pinMode(stepY, OUTPUT);
  pinMode(dirY, OUTPUT);

  // Pines motor Z (comentado)
  pinMode(stepZ, OUTPUT);
  pinMode(dirZ, OUTPUT);

  // Pin de habilitación
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW); // Activar drivers

  // Finales de carrera con pull-up
  pinMode(xMinPin, INPUT_PULLUP);
  pinMode(xMaxPin, INPUT_PULLUP);
  pinMode(yMinPin, INPUT_PULLUP);
  pinMode(yMaxPin, INPUT_PULLUP);
  pinMode(zMinPin, INPUT_PULLUP);
  pinMode(zMaxPin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Iniciando homing...");

  homingX();
  homingY();
  homingZ();

  posX = 0;
  posY = 0;
  posZ = 0;

  Serial.println("Listo. Usa comandos tipo X:100 Y:200");
}

void loop() {
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando.startsWith("X:")) {
      int newX = comando.substring(2).toInt();
      newX = constrain(newX, 0, maxX);
      moverEje(stepX, dirX, xMaxPin, posX, newX, true);
      posX = newX;
      Serial.print("X = "); Serial.println(posX);
    }

    if (comando.startsWith("Y:")) {
      int newY = comando.substring(2).toInt();
      newY = constrain(newY, 0, maxY);
      moverEje(stepY, dirY, yMaxPin, posY, newY, true);
      posY = newY;
      Serial.print("Y = "); Serial.println(posY);
    }

    if (comando.startsWith("Z:")) {
      int newZ = comando.substring(2).toInt();
      newZ = constrain(newZ, 0, maxZ);
      moverEje(stepZ, dirZ, zMaxPin, posZ, newZ, true);
      posZ = newZ;
    Serial.print("Z = "); Serial.println(posZ);
    }
  }
}

// ========== HOMING ==========

void homingX() {
  digitalWrite(dirX, LOW);
  Serial.println("Buscando X−...");
  while (digitalRead(xMinPin) == HIGH) {
    step(stepX);
  }
  delay(100);
  // Retraer
  digitalWrite(dirX, HIGH);
  for (int i = 0; i < 50; i++) step(stepX);
  Serial.println("X listo.");
}

void homingY() {
  digitalWrite(dirY, LOW);
  Serial.println("Buscando Y−...");
  while (digitalRead(yMinPin) == HIGH) {
    step(stepY);
  }
  delay(100);
  digitalWrite(dirY, HIGH);
  for (int i = 0; i < 50; i++) step(stepY);
  Serial.println("Y listo.");
}

 void homingZ() {
   digitalWrite(dirZ, LOW);
   Serial.println("Buscando Z−...");
   while (digitalRead(zMinPin) == HIGH) {
     step(stepZ);
   }
   delay(100);
   digitalWrite(dirZ, HIGH);
   for (int i = 0; i < 50; i++) step(stepZ);
   Serial.println("Z listo.");
 }

// ========== MOVER EJE ==========

void moverEje(int stepPin, int dirPin, int maxPin, int actual, int destino, bool usaLimitePositivo) {
  int pasos = abs(destino - actual);
  bool direccion = (destino > actual);
  digitalWrite(dirPin, direccion ? HIGH : LOW);

  for (int i = 0; i < pasos; i++) {
    if (direccion && usaLimitePositivo && digitalRead(maxPin) == LOW) {
      Serial.println("¡Límite alcanzado!");
      break;
    }
    step(stepPin);
  }
}

// ========== PULSO ==========

void step(int pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(pin, LOW);
  delayMicroseconds(1000);
}