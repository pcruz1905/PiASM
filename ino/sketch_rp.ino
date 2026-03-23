#include <DHT.h>

#define DHTPIN 2     // Pino onde o sensor está ligado
#define DHTTYPE DHT11 // Tipo do sensor DHT11 

DHT dht(DHTPIN, DHTTYPE);

int ledTempPin = 3; // Pino do LED da temperatura
int ledHumPin = 4;  // Pino do LED da humidade

float initialTemp;
float initialHum;
  
unsigned long startMillis;  // Armazena o tempo desde o início do programa

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(ledTempPin, OUTPUT);
  pinMode(ledHumPin, OUTPUT);

  // Leitura inicial
  initialTemp = dht.readTemperature();
  initialHum = dht.readHumidity();

  // Inicia o contador de tempo
  startMillis = millis();
}

void loop() {
  float currentTemp = dht.readTemperature();
  float currentHum = dht.readHumidity();

  // Verifica se a leitura é válida
  if (isnan(currentTemp) || isnan(currentHum)) {
    Serial.println("Falha ao ler do sensor DHT!");
    return;
  }

  // Calcula o tempo decorrido
  unsigned long elapsedMillis = millis() - startMillis;
  unsigned long seconds = (elapsedMillis / 1000) % 60;
  unsigned long minutes = (elapsedMillis / 60000) % 60;
  unsigned long hours = (elapsedMillis / 3600000);

  // Marcar flags
  if (currentTemp >= initialTemp + 5) {
    Serial.print("Flag da Temperatura acionada às ");
    printTime(hours, minutes, seconds);
  }
  if (currentHum >= initialHum + 5) {
    Serial.print("Flag da Humidade acionada às ");
    printTime(hours, minutes, seconds);
  }

  // Controlo do LED de temperatura
  if (currentTemp >= initialTemp + 5) {
    digitalWrite(ledTempPin, LOW); // Acende o LED
  } else {
    digitalWrite(ledTempPin, HIGH); // Apaga o LED
  }

  // Controle do LED de humidade
  if (currentHum >= initialHum + 5) {
    digitalWrite(ledHumPin, LOW); // Acende o LED
  } else {
    digitalWrite(ledHumPin, HIGH); // Apaga o LED
  }

  // Exibe os valores no Monitor
  Serial.print("Temperatura: ");
  Serial.print(currentTemp);
  Serial.print(" *C\t");
  Serial.print("Humidade: ");
  Serial.print(currentHum);
  Serial.println(" %");

  delay(2000); // Espera 2 segundos antes da próxima leitura
}

// Função para exibir o tempo formatado
void printTime(unsigned long hours, unsigned long minutes, unsigned long seconds) {
  if (hours < 10) Serial.print("0");
  Serial.print(hours);
  Serial.print(":");
  if (minutes < 10) Serial.print("0");
  Serial.print(minutes);
  Serial.print(":");
  if (seconds < 10) Serial.print("0");
  Serial.println(seconds);
}
