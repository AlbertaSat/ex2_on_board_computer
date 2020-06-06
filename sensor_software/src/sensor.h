#ifndef SENSOR_H
#define SENSOR_H

#include "adc_handler.h"

#include <stdint.h>

enum {
    CHANNEL_TEMP_1 = 0,
    CHANNEL_TEMP_2,
    CHANNEL_TEMP_3,
    CHANNEL_VOLT,
    CHANNEL_CURR,
    CHANNEL_NUM         // Number of ADC channels
}channel_type_t;

enum {
    PANEL_1 = 0,
    PANEL_2,
    PANEL_3,
    PANEL_4,
    PANEL_5,
    PANEL_6,
    PANEL_NUM          // Number of solar panels
}panel_t;

typedef struct hyp_panel_t hyperion_panel;

struct hyp_panel_t{
    //
};

//returns temperature value from specified temp sensor(1...3)
unsigned int get_temp();

//returns voltage value from sensor
unsigned int get_volt();

//return current value from sensor
unsigned int get_curr();

unsigned int get_val(enum channel_type_t);


#endif