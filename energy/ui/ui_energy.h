#ifndef UI_ENERGY_H
#define UI_ENERGY_H

#include "system_state.h"

/* Splash screen */
void UI_Energy_ShowSplash(void);

/* Inicializa layout fixo */
void UI_Energy_Init(void);

/* Atualiza dados dinâmicos de energia */
void UI_Energy_Update(const system_state_t *state);

#endif /* UI_ENERGY_H */
