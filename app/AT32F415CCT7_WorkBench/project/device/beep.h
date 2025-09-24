#ifndef _BEEP_H
#define _BEEP_H

#include <stdio.h>
#include "at32f415.h"

/* output */
#define BEEP_PORT GPIOC
#define BEEP_PIN GPIO_PINS_15

#define BEEP_LONG_TIME 15
#define BEEP_SHORT_TIME 3
typedef enum
{
    BEEP_IDLE,
    BEEP_SHORT,
    BEEP_LONG,
} BEEP_RUN;

typedef struct
{
    char *name;
    BEEP_RUN status;
    void (*on)(void);
    void (*off)(void);
} beep_t;

void BeepProc(beep_t *beep);
#endif
