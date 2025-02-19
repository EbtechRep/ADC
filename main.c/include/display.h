#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define WIDTH 128
#define HEIGHT 64

typedef enum {
  SET_CONTRAST = 0x81,
  SET_ENTIRE_ON = 0xA4,
  SET_NORM_INV = 0xA6,
  SET_DISP = 0xAE,
  SET_MEM_ADDR = 0x20,
  SET_COL_ADDR = 0x21,
  SET_PAGE_ADDR = 0x22,
  SET_DISP_START_LINE = 0x40,
  SET_SEG_REMAP = 0xA0,
  SET_MUX_RATIO = 0xA8,
  SET_COM_OUT_DIR = 0xC0,
  SET_DISP_OFFSET = 0xD3,
  SET_COM_PIN_CFG = 0xDA,
  SET_DISP_CLK_DIV = 0xD5,
  SET_PRECHARGE = 0xD9,
  SET_VCOM_DESEL = 0xDB,
  SET_CHARGE_PUMP = 0x8D
} display_command_t;

typedef struct {
  uint8_t width, height, pages, address;
  i2c_inst_t *i2c_port;
  bool external_vcc;
  uint8_t *ram_buffer;
  size_t bufsize;
  uint8_t port_buffer[2];
} display_t;
void update_square_position(int *square_x, int *square_y, uint16_t vrx_value, uint16_t vry_value);
void display_init(display_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c);
void display_config(display_t *ssd);
void display_command(display_t *ssd, uint8_t command);
void display_send_data(display_t *ssd);
void display_pixel(display_t *ssd, uint8_t x, uint8_t y, bool value);
void display_fill(display_t *ssd, bool value);
void display_rect_stars(display_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value);
void display_draw_star(display_t *ssd, uint8_t x, uint8_t y, bool value);
void display_rect_squares(display_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value);
void display_rect(display_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value);
void display_rect_hearts(display_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value);
void display_draw_heart(display_t *ssd, uint8_t x, uint8_t y, bool value);
void display_line(display_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value);
void display_hline(display_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value);
void display_vline(display_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value);
void display_draw_string(display_t *ssd, const char *str, uint8_t x, uint8_t y);