#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
// Feito por Marcus H. // O codigo funciona baseado na tabela ASCII 
#define BOTAO_SOS     4
#define SAIDA_MORSE   18

#define UNIT_TIME_MS    200  // A base do tempo
#define TEMPO_PONTO_MS  UNIT_TIME_MS
#define TEMPO_TRACO_MS  (UNIT_TIME_MS * 3)
#define ESPACO_SINAL    UNIT_TIME_MS       
#define ESPACO_LETRA    (UNIT_TIME_MS * 3) 
#define ESPACO_PALAVRA  (UNIT_TIME_MS * 7) 

// Tabelas de Tradução Morse 
const char* ALFABETO_MORSE[] = {
    ".-",   "-...", "-.-.", "-..",  ".",    "..-.", "--.",  "....", "..", 
    ".---", "-.-",  ".-..", "--",   "-.",   "---",  ".--.", "--.-", ".-.", 
    "...",   "-",    "..-",   "...-", ".--",  "-..-", "-.--", "--.."}; //65 a 90

const char* NUMEROS_MORSE[] = {
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."}; // 48 a 57 

void pulsar_morse(const char* codigo) { //Controla a saida da mensagem 
    int tamanho = strlen(codigo);
    for (int i = 0; i < tamanho; i++) {
        int tempo = (codigo[i] == '.') ? TEMPO_PONTO_MS : TEMPO_TRACO_MS;
        
        gpio_set_level(SAIDA_MORSE, 1);
        vTaskDelay(pdMS_TO_TICKS(tempo));
        
        gpio_set_level(SAIDA_MORSE, 0);
        
        vTaskDelay(pdMS_TO_TICKS(ESPACO_SINAL));
        }}

void transmitir_mensagem(const char* mensagem) {
    for (int i = 0; i < strlen(mensagem); i++) {
        char c = toupper((unsigned char)mensagem[i]); // Transforma em maiúsculas
        
        if (c >= 'A' && c <= 'Z') {
            printf("Enviando letra: %c\n", c); 
            pulsar_morse(ALFABETO_MORSE[c - 'A']); //faz a conta da letra 
            vTaskDelay(pdMS_TO_TICKS(ESPACO_LETRA)); } 
        else if (c >= '0' && c <= '9') {
            printf("Enviando numero: %c\n", c);
            pulsar_morse(NUMEROS_MORSE[c - '0']); // Faz a conta dos numeros 
            vTaskDelay(pdMS_TO_TICKS(ESPACO_LETRA));}
            
        else if (c == ' ') {
            printf("Espaço entre palavras...\n");
            vTaskDelay(pdMS_TO_TICKS(ESPACO_PALAVRA)); }}}


void app_main() {
    gpio_config_t io_conf = {  // configuração da Saída (representada por um LED)
        .pin_bit_mask = (1ULL << SAIDA_MORSE),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE};
    gpio_config(&io_conf);

    gpio_config_t btn_conf = { // Configuração do Botão (com pull-up interno)
        .pin_bit_mask = (1ULL << BOTAO_SOS),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1, 
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE};
    gpio_config(&btn_conf);

    gpio_set_level(SAIDA_MORSE, 0); // para que o led não inicie ligado 
    printf("Aguardando pressionar o botao no GPIO %d...\n", BOTAO_SOS);

    while (1) {
        if (gpio_get_level(BOTAO_SOS) == 0) { // Se o botão for pressionado (Nível 0 devido ao Pull-up)
            transmitir_mensagem("SOS 1");  // A mensagem sera traduzida para morse 
            
            while(gpio_get_level(BOTAO_SOS) == 0) { // faz com que o codigo só reinicia ao soltar o botão 
                vTaskDelay(pdMS_TO_TICKS(50)); }}
        vTaskDelay(pdMS_TO_TICKS(100)); // pequena pausa para otimizar a CPU
    }}