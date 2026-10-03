#include <WiFi.h>
#include <esp_now.h>

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Tcl.h>

// =========================
// CONFIGURAÇÕES
// =========================

// Pino do LED infravermelho
#define IR_PIN 4

// LED da placa (opcional)
#define LED_PIN 2

// Objeto do ar-condicionado TCL
IRTcl112Ac ac(IR_PIN);

// =========================
// ESTRUTURA DA MENSAGEM
// =========================
struct Mensagem {
  bool ligar;
};

Mensagem msg;

// =========================
// RECEBIMENTO ESP-NOW
// =========================
void OnDataRecv(
  const esp_now_recv_info_t *info,
  const uint8_t *incomingData,
  int len
) {

  memcpy(&msg, incomingData, sizeof(msg));

  // =========================
  // LIGAR AR
  // =========================
  if (msg.ligar) {

    Serial.println("LIGAR TCL");

    digitalWrite(LED_PIN, LOW);

    ac.on();

    // Temperatura padrão
    // Altere se desejar
    ac.setTemp(24);

    ac.send();
  }

  // =========================
  // DESLIGAR AR
  // =========================
  else {

    Serial.println("DESLIGAR TCL");

    digitalWrite(LED_PIN, HIGH);

    ac.off();

    ac.send();
  }
}

void setup() {

  // Velocidade do Monitor Serial
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  // Inicializa o transmissor IR
  ac.begin();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {

    Serial.println("Erro ESP-NOW");

    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("ESP TCL PRONTO");

  // Mostra o MAC do ESP
  // Útil para configurar no ESP da porta
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}