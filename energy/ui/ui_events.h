#pragma once
#include "FreeRTOS.h"
#include "event_groups.h"

#define UI_EVT_BUTTON_PRESS   (1 << 0)
#define UI_REFRESH_MS         500

extern EventGroupHandle_t ui_event_group;
