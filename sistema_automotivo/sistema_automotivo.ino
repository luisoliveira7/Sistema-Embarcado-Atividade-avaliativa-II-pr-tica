const int trig = 10;
const int echo = 11;
const int trigPC = 5;
const int echoPC = 6;
const int buzzer = 12;
 
const int sensorLuz = A3;
 
const int ledBranco1 = 8;
const int ledBranco2 = 7;
const int ledPontoCego = A5;
const int ledVerde = 2;
const int ledAmarelo = 3;
const int ledVermelho = 4;
 
int limiteLuz = 400;
const bool LUZ_INVERTIDA = false;
const int limitePontoCego = 50;
 
long duracao;
int distancia;
unsigned long tempoBip = 0;
bool estadoBuzzer = false;
 
void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trigPC, OUTPUT);
  pinMode(echoPC, INPUT);
  pinMode(sensorLuz, INPUT);
  pinMode(ledBranco1, OUTPUT);
  pinMode(ledBranco2, OUTPUT);
  pinMode(ledPontoCego, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}
 
void loop() {
  controlarLuz();
  controlarPontoCego();
  controlarRe();
  delay(30);
}
 
void controlarLuz() {
  int luminosidade = analogRead(sensorLuz);
 
  bool escuro = LUZ_INVERTIDA ? (luminosidade > limiteLuz)
                              : (luminosidade < limiteLuz);
 
  digitalWrite(ledBranco1, escuro ? HIGH : LOW);
  digitalWrite(ledBranco2, escuro ? HIGH : LOW);
 
  Serial.print("Luz: ");
  Serial.print(luminosidade);
  Serial.print(" | ");
}
 
void controlarPontoCego() {
  digitalWrite(trigPC, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPC, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPC, LOW);
 
  long duracaoPC = pulseIn(echoPC, HIGH, 30000);
  int distanciaPC = (duracaoPC == 0) ? 999 : duracaoPC * 0.034 / 2;
 
  digitalWrite(ledPontoCego, distanciaPC < limitePontoCego ? HIGH : LOW);
 
  Serial.print("PontoCego: ");
  Serial.print(distanciaPC);
  Serial.print(" cm | ");
}
 
void controlarRe() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
 
  duracao = pulseIn(echo, HIGH, 30000);
 
  if (duracao == 0) {
    distancia = 999;
  } else {
    distancia = duracao * 0.034 / 2;
  }
 
  if (distancia > 100) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    pararBuzzer();
  }
  else if (distancia > 50) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    pararBuzzer();
  }
  else if (distancia > 20) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    tocarBuzzer(500);
  }
  else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    tocarBuzzer(100);
  }
 
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}
 
void tocarBuzzer(int intervalo) {
  if (millis() - tempoBip >= (unsigned long)intervalo) {
    tempoBip = millis();
    estadoBuzzer = !estadoBuzzer;
    if (estadoBuzzer) {
      tone(buzzer, 1500);
    } else {
      noTone(buzzer);
    }
  }
}
 
void pararBuzzer() {
  noTone(buzzer);
  estadoBuzzer = false;
}
