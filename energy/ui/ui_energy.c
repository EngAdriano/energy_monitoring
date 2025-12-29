#include <stdio.h>
#include <stdbool.h>

#include "ui_energy.h"
#include "st7735.h"
#include "fonts.h"

/* ================= Layout ================= */
#define VALUE_W        56
#define VALUE_H        12
#define VALUE_PADDING  6

/* ================= Inicialização única ================= */
static bool display_initialized = false;

static void ui_display_init_once(void)
{
    if (!display_initialized)
    {
        //ST7735_Init();
        //ST7735_SetRotation(1);   /* Paisagem: 160x128 */
        display_initialized = true;
    }
}

/* ================= Header ================= */
static void ui_draw_header(void)
{
    ST7735_FillRectangle(0, 0, 160, 16, ST7735_BLUE);

    ST7735_DrawString(36, 3, "ENERGY MONITOR",
                      Font_7x10, ST7735_WHITE, ST7735_BLUE);

    ST7735_DrawLine(0, 16, 159, 16, ST7735_WHITE);
}

/* ================= Card ================= */
static void ui_draw_card(uint16_t x, uint16_t y, const char *title)
{
    ST7735_DrawRoundRect(x, y, 72, 30, 4, ST7735_WHITE);

    ST7735_DrawString(x + 4, y + 3, title,
                      Font_7x10, ST7735_CYAN, ST7735_BLACK);
}

static void ui_clear_value(uint16_t x, uint16_t y)
{
    ST7735_FillRectangle(x + VALUE_PADDING,
                         y,
                         VALUE_W,
                         VALUE_H,
                         ST7735_BLACK);
}

/* ================= Layout fixo ================= */
static void ui_draw_main_frame(void)
{
    ST7735_FillScreen(ST7735_BLACK);

    ui_draw_header();

    ui_draw_card(4,  22, "Voltage");
    ui_draw_card(82, 22, "Corrente");

    ui_draw_card(4,  56, "Potencia");
    ui_draw_card(82, 56, "Energia");

    ui_draw_card(4,  90, "Freq");
    ui_draw_card(82, 90, "PF");
}

/* ================= Splash ================= */
void UI_Energy_ShowSplash(void)
{
    ui_display_init_once();

    ST7735_FillScreen(ST7735_BLACK);

    ST7735_FillRectangle(0, 0, 160, 22, ST7735_GREEN);
    ST7735_DrawString(22, 6, "EMBARCATECH - IFCE",
                      Font_7x10, ST7735_BLACK, ST7735_GREEN);

    ST7735_DrawRoundRect(10, 40, 140, 50, 6, ST7735_WHITE);

    ST7735_DrawString(18, 50, "Sistema IoT de",
                      Font_7x10, ST7735_CYAN, ST7735_BLACK);

    ST7735_DrawString(18, 65, "Monitoramento de",
                      Font_7x10, ST7735_CYAN, ST7735_BLACK);

    ST7735_DrawString(18, 80, "Energia",
                      Font_7x10, ST7735_CYAN, ST7735_BLACK);

    ST7735_DrawString(20, 110, "Eng. Jose Adriano",
                      Font_7x10, ST7735_YELLOW, ST7735_BLACK);

    /* Tempo da splash screen */
    vTaskDelay(pdMS_TO_TICKS(2500));
}

/* ================= Init ================= */
void UI_Energy_Init(void)
{
    ui_display_init_once();
    ui_draw_main_frame();
}

/* ================= Update ================= */
void UI_Energy_Update(const system_state_t *state)
{
    if (!state || !state->energy.valid)
        return;

    const energy_data_t *e = &state->energy;
    char buf[20];

    ui_clear_value(4, 38);
    snprintf(buf, sizeof(buf), "%.1f V", e->voltage);
    ST7735_DrawString(4 + VALUE_PADDING, 38, buf,
                      Font_7x10, ST7735_GREEN, ST7735_BLACK);

    ui_clear_value(82, 38);
    snprintf(buf, sizeof(buf), "%.3f A", e->current);
    ST7735_DrawString(82 + VALUE_PADDING, 38, buf,
                      Font_7x10, ST7735_GREEN, ST7735_BLACK);

    ui_clear_value(4, 72);
    snprintf(buf, sizeof(buf), "%.1f W", e->power);
    ST7735_DrawString(4 + VALUE_PADDING, 72, buf,
                      Font_7x10, ST7735_GREEN, ST7735_BLACK);

    ui_clear_value(82, 72);
    snprintf(buf, sizeof(buf), "%.3f kWh", e->energy);
    ST7735_DrawString(82 + VALUE_PADDING, 72, buf,
                      Font_7x10, ST7735_YELLOW, ST7735_BLACK);

    ui_clear_value(4, 106);
    snprintf(buf, sizeof(buf), "%.1f Hz", e->frequency);
    ST7735_DrawString(4 + VALUE_PADDING, 106, buf,
                      Font_7x10, ST7735_GREEN, ST7735_BLACK);

    ui_clear_value(82, 106);
    snprintf(buf, sizeof(buf), "%.2f", e->pf);
    ST7735_DrawString(82 + VALUE_PADDING, 106, buf,
                      Font_7x10, ST7735_GREEN, ST7735_BLACK);
}
