#ifndef SCREEN_H
#define SCREEN_H

#include "../include/types.h"

#define MAX_SCREENS 5

void screen_init(void);

void screen_switch(uint8_t screen_num);

uint8_t screen_get_active(void);

void screen_save(uint8_t screen_num);

void screen_restore(uint8_t screen_num);

void screen_sync_current(void);

#endif
