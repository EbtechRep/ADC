#include "display.h"

// Função de inicialização do display
void display_init(display_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c) {
    ssd->width = width;
    ssd->height = height;
    ssd->pages = height / 8U;
    ssd->address = address;
    ssd->i2c_port = i2c;
    ssd->bufsize = ssd->pages * ssd->width + 1;
    ssd->ram_buffer = calloc(ssd->bufsize, sizeof(uint8_t));
    ssd->ram_buffer[0] = 0x40;  // Inicia com valor de controle para dados
    ssd->port_buffer[0] = 0x80; // Inicializa o buffer do comando
}

// Função que configura o display com os parâmetros padrões
void display_config(display_t *ssd) {
    display_command(ssd, SET_DISP | 0x00);          // Desliga o display
    display_command(ssd, SET_MEM_ADDR);             // Define o modo de endereço da memória
    display_command(ssd, 0x01);                     // Ativa a memória
    display_command(ssd, SET_DISP_START_LINE | 0x00); // Configura a linha inicial para exibição
    display_command(ssd, SET_SEG_REMAP | 0x01);     // Inverte o mapeamento dos segmentos
    display_command(ssd, SET_MUX_RATIO);            // Define a razão do MUX
    display_command(ssd, HEIGHT - 1);               // Ajuste do número de linhas
    display_command(ssd, SET_COM_OUT_DIR | 0x08);   // Configura direção de saída da COM
    display_command(ssd, SET_DISP_OFFSET);          // Configura o deslocamento do display
    display_command(ssd, 0x00);                     // Sem deslocamento
    display_command(ssd, SET_COM_PIN_CFG);          // Configura os pinos de COM
    display_command(ssd, 0x12);                     // Configuração padrão
    display_command(ssd, SET_DISP_CLK_DIV);         // Ajusta a divisão do clock
    display_command(ssd, 0x80);                     // Clock ajustado
    display_command(ssd, SET_PRECHARGE);            // Pré-carga
    display_command(ssd, 0xF1);                     // Valor do pré-chargue
    display_command(ssd, SET_VCOM_DESEL);           // Desseleciona VCOM
    display_command(ssd, 0x30);                     // Configura VCOM
    display_command(ssd, SET_CONTRAST);             // Ajusta o contraste
    display_command(ssd, 0xFF);                     // Contraste máximo
    display_command(ssd, SET_ENTIRE_ON);            // Exibe toda a tela
    display_command(ssd, SET_NORM_INV);             // Exibição normal
    display_command(ssd, SET_CHARGE_PUMP);          // Configura a bomba de carga
    display_command(ssd, 0x14);                     // Valor da bomba
    display_command(ssd, SET_DISP | 0x01);          // Liga o display
}

// Função para enviar um comando ao display via I2C
void display_command(display_t *ssd, uint8_t command) {
    ssd->port_buffer[1] = command;
    i2c_write_blocking(ssd->i2c_port, ssd->address, ssd->port_buffer, 2, false);
}

// Função para enviar os dados da RAM para o display via I2C
void display_send_data(display_t *ssd) {
    display_command(ssd, SET_COL_ADDR);  // Define o endereço da coluna
    display_command(ssd, 0);              // Inicia na primeira coluna
    display_command(ssd, ssd->width - 1); // Finaliza na última coluna
    display_command(ssd, SET_PAGE_ADDR);  // Define o endereço da página
    display_command(ssd, 0);              // Começa na primeira página
    display_command(ssd, ssd->pages - 1); // Finaliza na última página
    i2c_write_blocking(ssd->i2c_port, ssd->address, ssd->ram_buffer, ssd->bufsize, false);
}

// Função para desenhar ou apagar um pixel
void display_pixel(display_t *ssd, uint8_t x, uint8_t y, bool value) {
    uint16_t index = (y >> 3) + (x << 3) + 1;
    uint8_t pixel = (y & 0b111);

    if (value)
        ssd->ram_buffer[index] |= (1 << pixel);  // Define o pixel
    else
        ssd->ram_buffer[index] &= ~(1 << pixel); // Apaga o pixel
}

// Função para preencher toda a tela com pixels definidos (ligados ou desligados)
void display_fill(display_t *ssd, bool value) {
    for (uint8_t y = 0; y < ssd->height; ++y) {
        for (uint8_t x = 0; x < ssd->width; ++x) {
            display_pixel(ssd, x, y, value); // Preenche cada pixel
        }
    }
}

// Função para desenhar um retângulo na tela
void display_rect(display_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value) {
    // Desenha as bordas superior e inferior
    for (uint8_t x = left; x < left + width; ++x) {
        display_pixel(ssd, x, top, value);                   // Borda superior
        display_pixel(ssd, x, top + height - 1, value);       // Borda inferior
    }

    // Desenha as bordas esquerda e direita
    for (uint8_t y = top; y < top + height; ++y) {
        display_pixel(ssd, left, y, value);                  // Borda esquerda
        display_pixel(ssd, left + width - 1, y, value);      // Borda direita
    }
}

// Funções auxiliares para desenhar formas estilizadas (estrela, quadrado, coração)
void display_rect_stars(display_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value) {
    const uint8_t spacing = 5; // Distância entre as estrelas
    for (uint8_t x = left; x < left + width; x += spacing) {
        display_draw_star(ssd, x, top, value);                   // Borda superior
        display_draw_star(ssd, x, top + height - 1, value);       // Borda inferior
    }
    for (uint8_t y = top; y < top + height; y += spacing) {
        display_draw_star(ssd, left, y, value);                   // Borda esquerda
        display_draw_star(ssd, left + width - 1, y, value);       // Borda direita
    }
}

void display_draw_star(display_t *ssd, uint8_t x, uint8_t y, bool value) {
    display_pixel(ssd, x, y, value);
    display_pixel(ssd, x - 1, y, value);
    display_pixel(ssd, x + 1, y, value);
    display_pixel(ssd, x, y - 1, value);
    display_pixel(ssd, x, y + 1, value);
}

// Função para desenhar um quadrado dentro da borda
void display_rect_squares(display_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value) {
    const uint8_t square_size = 3;  // Tamanho de cada quadrado (3x3 pixels)
    const uint8_t spacing = 4;      // Espaçamento entre os quadrados

    // Desenha a borda superior
    for (uint8_t x = left; x < left + width; x += spacing) {
        display_rect(ssd, x, top, square_size, square_size, value);
    }

    // Desenha a borda inferior
    for (uint8_t x = left; x < left + width; x += spacing) {
        display_rect(ssd, x, top + height - square_size, square_size, square_size, value);
    }

    // Desenha a borda esquerda
    for (uint8_t y = top; y < top + height; y += spacing) {
        display_rect(ssd, left, y, square_size, square_size, value);
    }

    // Desenha a borda direita
    for (uint8_t y = top; y < top + height; y += spacing) {
        display_rect(ssd, left + width - square_size, y, square_size, square_size, value);
    }
}

// Funções para desenhar formas como coração, linha, e bordas
void display_draw_heart(display_t *ssd, uint8_t x, uint8_t y, bool value) {
    display_pixel(ssd, x, y, value);
    display_pixel(ssd, x - 1, y - 1, value);
    display_pixel(ssd, x + 1, y - 1, value);
    display_pixel(ssd, x - 2, y, value);
    display_pixel(ssd, x + 2, y, value);
    display_pixel(ssd, x - 1, y + 1, value);
    display_pixel(ssd, x + 1, y + 1, value);
    display_pixel(ssd, x, y + 2, value);
}

void display_rect_hearts(display_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value) {
    const uint8_t spacing = 6;
    for (uint8_t x = left; x < left + width; x += spacing) {
        display_draw_heart(ssd, x, top, value);
        display_draw_heart(ssd, x, top + height - 1, value);
    }
    for (uint8_t y = top; y < top + height; y += spacing) {
        display_draw_heart(ssd, left, y, value);
        display_draw_heart(ssd, left + width - 1, y, value);
    }
}

// Função para desenhar uma linha reta
void display_line(display_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        display_pixel(ssd, x0, y0, value); // Desenha o pixel atual
        if (x0 == x1 && y0 == y1) break;  // Termina quando chega ao ponto final

        int e2 = err * 2;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

// Funções auxiliares para desenhar linhas horizontais e verticais
void display_hline(display_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value) {
    for (uint8_t x = x0; x <= x1; ++x)
        display_pixel(ssd, x, y, value);
}

void display_vline(display_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value) {
    for (uint8_t y = y0; y <= y1; ++y)
        display_pixel(ssd, x, y, value);
}

// Função que atualiza a posição de um quadrado 8x8 com base nos valores do joystick
void update_square_position(int *square_x, int *square_y, uint16_t vrx_value, uint16_t vry_value) {
    int delta_y = vrx_value / 32;
    int delta_x = (4095 - vry_value) / 64;

    *square_x = delta_x;
    *square_y = delta_y;

    // Limites da tela para o movimento do quadrado
    if (*square_x < 8) { *square_x = 8; }
    if (*square_x > 56) { *square_x = 56; }
    if (*square_y < 12) { *square_y = 12; }
    if (*square_y > 124) { *square_y = 124; }
}
