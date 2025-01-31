#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2818b.pio.h"

// Definindo os pinos dos componentes
#define PINO_MATRIZ 7  // Pino de controle da matriz de LEDs
#define NUM_LEDS 25    // Número total de LEDs na matriz
#define BOTAO_A 5      // Pino do Botão A
#define BOTAO_B 6      // Pino do Botão B
#define BLUE 11    // Pino do LED azul
#define GREEN 12   // Pino do LED verde
#define RED 13 // Pino do LED vermelho

int num_eventos = 11; // Número de eventos para alternar entre os botões
int estado_atual = 0; // Estado atual do sistema (0 - Desliga, 1 - Chuva)

// Tempo mínimo entre interrupções para debouncing
#define DEBOUNCE_DELAY 300 // Milissegundos

// Variáveis para controlar o debouncing
volatile uint32_t last_irq_time_A = 0;
volatile uint32_t last_irq_time_B = 0;

// Função para converter as posições (x, y) da matriz para um índice do vetor de LEDs
int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return 24 - (y * 5 + x);
    } else {
        return 24 - (y * 5 + (4 - x));
    }
}

//Inicializar os pinos do led RGB
void iniciar_rgb() {
  gpio_init(RED);
  gpio_init(GREEN);
  gpio_init(BLUE);

  gpio_set_dir(RED, GPIO_OUT);
  gpio_set_dir(GREEN, GPIO_OUT);
  gpio_set_dir(BLUE, GPIO_OUT);
}

// Função para iniciar os leds RGB dos pinos 11, 12 e 13 e configurar o tempo ligado
void state(bool r, bool g, bool b, int tempo) {
 iniciar_rgb();
 gpio_put(RED, r);
 gpio_put(GREEN, g);
 gpio_put(BLUE, b);
 sleep_ms(tempo);
} 


// Definição da estrutura de cor para cada LED
struct pixel_t {
    uint8_t R, G, B;
};
typedef struct pixel_t npLED_t;
npLED_t leds[NUM_LEDS];

// PIO e state machine para controle dos LEDs
PIO np_pio;
uint sm;

// Função para atualizar os LEDs da matriz
void bf() {
    for (uint i = 0; i < NUM_LEDS; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

// Função de controle inicial da matriz de LEDs
void controle(uint pino) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);
    ws2818b_program_init(np_pio, sm, offset, pino, 800000.f);

    for (uint i = 0; i < NUM_LEDS; ++i) {
        leds[i].R = leds[i].G = leds[i].B = 0;
    }
    bf();
}

// Função para configurar a cor de um LED específico
void cor(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[indice].R = r;
    leds[indice].G = g;
    leds[indice].B = b;
}

// Função para desligar todos os LEDs
void desliga() {
    for (uint i = 0; i < NUM_LEDS; ++i) {
        cor(i, 0, 0, 0);
    }
    bf();
}

// Função de callback para o botão A (avançar estado)
void botao_callback_A(uint gpio, uint32_t eventos) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_irq_time_A > DEBOUNCE_DELAY) {
        estado_atual = (estado_atual - 1 num_eventos) % num_eventos; // Avança para o próximo estado
        last_irq_time_A = current_time; // Atualiza o tempo do último evento
    }
}

// Função de callback para o botão B (retroceder estado)
void botao_callback_B(uint gpio, uint32_t eventos) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_irq_time_B > DEBOUNCE_DELAY) {
        estado_atual = (estado_atual + 1) % num_eventos; // Retrocede para o estado anterior
        last_irq_time_B = current_time; // Atualiza o tempo do último evento
    }
}

// Funções para os diferentes efeitos
void number0() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0},{0, 0, 0}},
        {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0},  {0, 0, 0}},
        {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {55, 0, 0},  { 0, 0, 0}},
        {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0},  {0, 0, 0}},
        {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0},{0, 0, 0}}
    };
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number1() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}}
    };

    // Exibir a primeira matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number2() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}}
    };

    // Exibir a primeira matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number3() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {100, 255, 0}, {100, 255, 0}, {100, 255, 0}, {0, 0, 0  }},
        {{0, 0, 0}, {0, 0, 0},     {0, 0,     0}, {100, 255, 0}, {0, 0,   0}},
        {{0, 0, 0}, {100, 255, 0}, {100, 255, 0}, {100, 255, 0}, {0, 0,   0}},
        {{0, 0, 0}, {0, 0, 0},     {0, 0,     0}, {100, 255, 0}, {0, 0,   0}},
        {{0, 0, 0}, {100, 255, 0}, {100, 255, 0}, {100, 255, 0}, {0, 0,   0}}
    };

    // Exibir a primeira matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number4() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}}
    };

    // Exibir a primeira matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number5() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}}
    
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number6() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}}
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number7() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {0, 255, 255}, {0, 255, 255}, {0, 255, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 255}, {0, 0, 0}}
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number8() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {255, 0, 255}, {255, 0, 255}, {255, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {255, 0, 255}, {0, 0, 0}, {255, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {255, 0, 255}, {255, 0, 255}, {255, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {255, 0, 255}, {0, 0, 0}, {255, 0, 255}, {0, 0, 0}},
        {{0, 0, 0}, {255, 0, 255}, {255, 0, 255}, {255, 0, 255}, {0, 0, 0}}
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number9() {
    int mat1[5][5][3] = {
        {{0, 0, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {0, 0, 0}},
        {{0, 0, 0}, {255, 255, 255}, {0, 0, 0}, {255, 255, 255}, {0, 0, 0}},
        {{0, 0, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 255, 255}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 255, 255}, {0, 0, 0}}
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void number10() {
    int mat1[5][5][3] = {
        {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
        {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
        {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
        {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
        {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}
    };

    // Exibir a matriz
    for (int linha = 0; linha < 5; linha++) {
        for (int cols = 0; cols < 5; cols++) {
            int posicao = getIndex(linha, cols);
            cor(posicao, mat1[linha][cols][0], mat1[linha][cols][1], mat1[linha][cols][2]);
        }
    }
    bf();
}

void atualizar_estado() {
    switch (estado_atual) {
        case 0:
            number0();
            break;
        case 1:
            number1();
            break;
        case 2:
            number2();
            break;
        case 3:
            number3();
            break;
        case 4:
            number4();
            break;
        case 5:
            number5();
            break;
        case 6:
            number6();
            break;
        case 7:
            number7();
            break;
        case 8:
            number8();
            break;
        case 9:
            number9();
            break;
        case 10:
            number10();
            break;
    }
}

void debounce_botao(uint pino, volatile uint32_t *last_irq_time, int direcao) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    if (tempo_atual - *last_irq_time > DEBOUNCE_DELAY) {
        *last_irq_time = tempo_atual;
        estado_atual += direcao;
        if (estado_atual >= num_eventos) estado_atual = 0;
        if (estado_atual < 0) estado_atual = num_eventos - 1;
        atualizar_estado();
    }
}
// Função main
int main() {
    controle(PINO_MATRIZ);
    gpio_init(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_pull_up(BOTAO_B);
    number0();

    while (1) {
        state(1, 0, 0, 200);
        state(0, 0, 0, 1);
        if (!gpio_get(BOTAO_B)) debounce_botao(BOTAO_B, &last_irq_time_B, 1);
        if (!gpio_get(BOTAO_A)) debounce_botao(BOTAO_A, &last_irq_time_A, -1);
        sleep_ms(10);
    }
}
