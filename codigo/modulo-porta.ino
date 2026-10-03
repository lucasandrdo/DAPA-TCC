#include <WiFi.h>
#include <esp_now.h>

// =========================
// CONFIGURAÇÕES
// =========================

// Pino do Reed Switch
#define REED_PIN 21

// Tempo que a porta deve ficar aberta
// 300000 ms = 300 segundos = 5 minutos
const unsigned long TEMPO_LIMITE = 300000;

// Tempo de pausa antes de religar o ar
// depois que ele foi desligado
// 180000 ms = 180 segundos = 3 minutos
const unsigned long TEMPO_PAUSA_RELIGAR = 180000;

// MAC do ESP 1
uint8_t MAC_AR1[] = {
  0x68, 0x25, 0xDD, 0xFD, 0x2E, 0xA4
};

// =========================
// ESTRUTURA DA MENSAGEM
// =========================
// ligar = true  -> liga o ar
// ligar = false -> desliga o ar
struct Mensagem {
  bool ligar;
};

Mensagem msg;
esp_now_peer_info_t peerInfo;

// Variáveis de controle
unsigned long tempoAbertura = 0;
unsigned long tempoDesligamento = 0;

bool contando = false;
bool arDesligado = false;
bool aguardandoReligar = false;

// =========================
// ADICIONA UM RECEPTOR
// =========================
void adicionarPeer(uint8_t *mac) {

  memset(&peerInfo, 0, sizeof(peerInfo));

  memcpy(peerInfo.peer_addr, mac, 6);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
}

// =========================
// ENVIA COMANDO
// =========================
void enviarParaTodos(bool ligar) {

  msg.ligar = ligar;

  esp_now_send(
    MAC_AR1,
    (uint8_t *)&msg,
    sizeof(msg)
  );
}

void setup() {

  // Velocidade do Monitor Serial
  Serial.begin(115200);

  pinMode(REED_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {

    Serial.println("Erro ao iniciar ESP-NOW");

    return;
  }

  adicionarPeer(MAC_AR1);

  Serial.println("ESP DA PORTA PRONTO");
}

void loop() {

  // Se a lógica estiver invertida,
  // troque por:
  // bool portaAberta = !digitalRead(REED_PIN);

  bool portaAberta = digitalRead(REED_PIN);

  // =========================
  // PORTA ABERTA
  // =========================
  if (portaAberta) {

    if (!contando) {

      tempoAbertura = millis();

      contando = true;

      Serial.println("Porta aberta");
    }

    // Verifica se atingiu o tempo limite
    if (!arDesligado &&
        millis() - tempoAbertura >= TEMPO_LIMITE) {

      Serial.println("Enviando DESLIGAR");

      enviarParaTodos(false);

      arDesligado = true;

      // Marca o instante do desligamento
      // para contar a pausa antes de religar
      tempoDesligamento = millis();
    }

  }

  // =========================
  // PORTA FECHADA
  // =========================
  else {

    contando = false;

    // Se o ar tinha sido desligado,
    // entra em espera para religar
    // somente após a pausa
    if (arDesligado) {

      aguardandoReligar = true;
    }
  }

  // =========================
  // PAUSA ANTES DE RELIGAR
  // =========================
  // Só religa o ar depois do tempo de pausa
  // e se a porta continuar fechada
  if (aguardandoReligar &&
      !portaAberta &&
      millis() - tempoDesligamento >= TEMPO_PAUSA_RELIGAR) {

    Serial.println("Enviando LIGAR");

    enviarParaTodos(true);

    arDesligado = false;

    aguardandoReligar = false;
  }

  delay(100);
}