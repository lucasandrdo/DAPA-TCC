# DAPA — Desligamento Automático por Porta Aberta

Sistema IoT desenvolvido como projeto de Trabalho de Conclusão de Curso (TCC), com o objetivo de contribuir para a redução do desperdício de energia elétrica causado pela permanência da porta aberta durante o funcionamento de aparelhos de ar-condicionado em ambientes escolares.

## Sobre o projeto

O DAPA utiliza dois módulos baseados em ESP32 que se comunicam por meio do protocolo ESP-NOW.

O primeiro módulo é instalado próximo à porta da sala e utiliza um sensor magnético do tipo Reed Switch para identificar o estado da porta. Quando a porta permanece aberta durante o período configurado, o módulo envia um comando ao segundo ESP32.

O segundo módulo fica próximo ao aparelho de ar-condicionado e recebe o comando por ESP-NOW. Em seguida, utiliza comunicação infravermelha para enviar ao aparelho o comando correspondente.

Dessa forma, o sistema permite automatizar o desligamento do ar-condicionado quando a porta permanece aberta por um período prolongado e realizar o religamento de acordo com a lógica definida no sistema.

## Arquitetura

```text
┌───────────────────────┐
│   Módulo da Porta     │
│                       │
│ ESP32                 │
│ Reed Switch           │
└───────────┬───────────┘
            │
            │ ESP-NOW
            ▼
┌───────────────────────┐
│ Módulo de Controle    │
│                       │
│ ESP32                 │
│ Comunicação IR        │
└───────────┬───────────┘
            │
            │ Infravermelho
            ▼
┌───────────────────────┐
│ Ar-condicionado       │
└───────────────────────┘
```

## Funcionamento

O sistema possui dois módulos independentes.

### Módulo da Porta

Responsável por:

* monitorar o estado da porta;
* identificar quando a porta é aberta;
* contabilizar o tempo em que a porta permanece aberta;
* enviar comandos utilizando ESP-NOW.

O sistema está configurado para considerar que a porta permaneceu aberta por tempo excessivo após **5 minutos (300 segundos)**.

Quando esse período é atingido, o Módulo da Porta envia um comando ao Módulo de Controle solicitando o desligamento do ar-condicionado.

Código:

[`codigo/modulo-porta/modulo-porta.ino`](codigo/modulo-porta/modulo-porta.ino)

### Módulo de Controle

Responsável por:

* receber os comandos enviados pelo Módulo da Porta;
* interpretar o comando recebido;
* controlar o aparelho de ar-condicionado por infravermelho.

Após o envio do comando de desligamento, o sistema aguarda **3 minutos (180 segundos)** antes de permitir o religamento do aparelho.

Caso a porta esteja fechada após esse período, o Módulo de Controle recebe o comando para ligar novamente o ar-condicionado.

Código:

[`codigo/modulo-controle/modulo-controle.ino`](codigo/modulo-controle/modulo-controle.ino)

## Tecnologias e componentes

### Hardware

* ESP32
* Reed Switch
* LED infravermelho
* Transistor
* Componentes eletrônicos auxiliares
* Aparelho de ar-condicionado compatível com o controle infravermelho utilizado

### Software e tecnologias

* Arduino IDE
* C++
* ESP-NOW
* Comunicação infravermelha
* Biblioteca IRremoteESP8266

## Estrutura do repositório

```text
DAPA-TCC/
├── README.md
├── LICENSE
│
├── codigo/
│   ├── modulo-porta/
│   │   └── modulo-porta.ino
│   │
│   └── modulo-controle/
│       └── modulo-controle.ino
│
└── documentacao/
    ├── arquitetura/
    │   └── arquitetura-sistema.png
    │
    └── imagens/
        ├── prototipo-modulo-porta.jpg
        └── prototipo-modulo-controle.jpg
```

## Configuração

Antes de carregar os códigos nos ESP32, é necessário verificar as configurações presentes nos arquivos, especialmente:

* endereço MAC utilizado na comunicação ESP-NOW;
* pinos utilizados pelos componentes;
* tempo de detecção da porta aberta;
* tempo de espera para o religamento;
* configurações de comunicação infravermelha;
* configurações específicas do aparelho de ar-condicionado.

Atualmente, o sistema está configurado com:

| Parâmetro                        |     Valor |
| -------------------------------- | --------: |
| Tempo para desligamento          | 5 minutos |
| Tempo de espera para religamento | 3 minutos |
| Pino do Reed Switch              |   GPIO 21 |
| Pino do LED infravermelho        |    GPIO 4 |
| Pino do LED indicador            |    GPIO 2 |

## Código-fonte

O código-fonte completo do sistema DAPA está disponibilizado neste repositório para consulta e reprodução do projeto.

O código é composto principalmente pelos arquivos:

* `modulo-porta.ino`
* `modulo-controle.ino`

## Trabalho de Conclusão de Curso

Este repositório está relacionado ao Trabalho de Conclusão de Curso desenvolvido no Instituto Federal de Educação, Ciência e Tecnologia da Paraíba (IFPB), Campus Itabaiana.

**Projeto:** DAPA: Uma solução IoT para eficiência energética no controle de ar-condicionado escolar.

## Autores

* Jamily Emily Alves Barbosa
* Lavínya de Andrade Nascimento Albuquerque Souza
* Lucas Andrade de Oliveira

**Orientador:** Prof. Dr. Bruno Neiva Moreno

## Licença

Este projeto é disponibilizado sob a [MIT License](LICENSE), para fins acadêmicos, educacionais e de desenvolvimento.
