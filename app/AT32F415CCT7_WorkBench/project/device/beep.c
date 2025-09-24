#include "beep.h"

static void beep_on(void)
{
    gpio_bits_set(BEEP_PORT, BEEP_PIN);
}

static void beep_off(void)
{
    gpio_bits_reset(BEEP_PORT, BEEP_PIN);
}

beep_t sbeep =
    {
        .name = "beep",
        .status = BEEP_IDLE,
        .on = beep_on,
        .off = beep_off,
};

void BeepProc(beep_t *beep)
{
    static uint8_t s_BeepStartTime = 0;

    switch (beep->status)
    {
    case BEEP_IDLE:
        beep->off();
        break;

    case BEEP_SHORT:
        beep->on();
        if (++s_BeepStartTime > BEEP_SHORT_TIME)
        {
            beep->status = BEEP_IDLE;
            s_BeepStartTime = 0;
            break;
        }

    case BEEP_LONG:
        beep->on();

        if (++s_BeepStartTime >= BEEP_LONG_TIME)
        {
            beep->status = BEEP_IDLE;
            s_BeepStartTime = 0;
        }

        break;
    }
}
