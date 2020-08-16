/*
 * Copyright (C) 2015  University of Alberta
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/**
 * @file    deployablecontrol.h
 * @author  Josh Lazaruk
 * @date    2020-08-13
 */

#include "HL_gio.h"


typedef enum
{
    PortKnife = 0,
    UHFKnife_P = 1,
    UHFKnife_Z = 2,
    PayloadKnife = 3,
    UHFKnife_S = 4,
    UHFKnife_N = 5,
    StarboardKnife = 6,
    DFGMKnife = 7,
} HotKnife_TypeDef;

void activateknife(HotKnife_TypeDef knife);
