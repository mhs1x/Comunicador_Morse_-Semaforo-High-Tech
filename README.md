📟 Emissor de Código Morse com ESP32

Este projeto utiliza um **ESP32** e o framework **ESP-IDF** para converter mensagens de texto em sinais luminosos (Código Morse).
O sistema é acionado por um botão físico e processa letras (A-Z) e números (0-9) com base na tabela ASCII.

🚀 Funcionalidades

* **Tradução em Tempo Real:** Converte strings estáticas ou dinâmicas para pulsos Morse.
* **Suporte Alfanumérico:** Inclui letras de A a Z e números de 0 a 9.
* **Controle de Tempo Preciso:** Implementa os padrões internacionais de tempo Morse (unidades de tempo).
* **Debounce e Segurança:** Evita disparos múltiplos acidentais e protege a CPU com o uso de RTOS.
* **Interrupção de Fluxo:** O código aguarda a liberação do botão antes de permitir uma nova transmissão.

🛠️ Hardware Necessário

* 1x ESP32 (DevKit v1 ou similar)
* 1x LED (ou o LED interno no GPIO 2, embora o código use o 18)
* 1x Botão (Push-button)
* 1x Resistor de 220Ω (para o LED)
* 1x Resistor de 10kΩ (Pull-up externo para o GPIO 36)
* Protoboard e Jumpers

Pinagem Utilizada
|   Componente   |   Pino ESP32  |     Modo        |
|  **Botão SOS** |  GPIO 36 (VP) | Entrada (Input) |
| **Saída Morse**|     GPIO 18   | Saída (Output)  |

 O GPIO 36 não possui resistor de pull-up interno. Certifique-se de usar um resistor externo de 10kΩ ligado ao 3.3V.

⏱️ Padrão de Temporização

O projeto segue a regra de `UNIT_TIME_MS` (200ms):
Ponto (.): 1 unidade.
Traço (-): 3 unidades.
Espaço entre sinais (mesma letra): 1 unidade.
Espaço entre letras: 3 unidades.
Espaço entre palavras: 7 unidades.

💻 Estrutura do Código

O software está dividido em três camadas lógicas:

1.  app_main: Inicializa os drivers de GPIO e monitora o estado do botão em um loop infinito.
2.  transmitir_mensagem: Percorre a string, trata caracteres minúsculos/maiúsculos e decide o tempo de espera entre letras e palavras.
3.  pulsar_morse: A camada física que traduz os símbolos `.` e `-` em níveis lógicos (High/Low) no pino de saída.

⚙️ Como Compilar e Rodar

1.  Tenha o **ESP-IDF** instalado e configurado no seu ambiente.
2.  Crie um novo projeto:
    ```bash
    idf.py create-project morse_sender
    ```
3.  Substitua o conteúdo do arquivo `main.c` pelo código deste repositório.
4.  Compile o projeto:
    ```bash
    idf.py build
    ```
5.  Grave no ESP32:
    ```bash
    idf.py -p [PORTA_COM] flash monitor
    ```

📊 Fluxo de Funcionamento (Lógica)

```text
[Botão Pressionado] -> [Envia "SOS 1"]
      |
      v
[Analisa cada Letra] -> [Busca na Tabela Morse]
      |
      v
[Gera Pulsos] -> [Liga LED] -> [Espera UNIT] -> [Desliga LED]
      |
      v
[Fim da Mensagem] -> [Aguarda Soltar Botão]

📝 Autor
Desenvolvido por **Marcus H.** como um estudo de sistemas embarcados e temporização com FreeRTOS.
