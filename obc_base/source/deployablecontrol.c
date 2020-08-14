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
 * @file    deployablecontrol.c
 * @author  Josh Lazaruk
 * @date    2020-08-13
 */

#include "HL_gio.h"
#include "HL_het.h"
#include "deployablescontrol.h"

void activateknife(HotKnife_TypeDef knife){
    switch(knife){
        case PortKnife:
            gioSetBit(hetPORT1, 23, 1);
            //add a delay for x seconds
            gioSetBit(hetPORT1, 23, 0);
            break;
        case UHFKnife1:
            gioSetBit(hetPORT2, 5, 1);
            //add a delay for x seconds
            gioSetBit(hetPORT2, 5, 0);
            break;
        case UHFKnife2:
            gioSetBit(hetPORT1, 26, 1);
            //add a delay for x seconds
            gioSetBit(hetPORT1, 26, 0);
            break;
        case UHFKnife3:
            gioSetBit(hetPORT1, 14, 1);
            //add a delay for x seconds
            gioSetBit(hetPORT1, 14, 0);
            break;
        case UHFKnife4:
            gioSetBit(gioPORTA, 0, 1);
            //add a delay for x seconds
            gioSetBit(gioPORTA, 0, 0);
            break;
        case DFGMKnife:
            gioSetBit(hetPORT1, 22, 1);
            //add a delay for x seconds
            gioSetBit(hetPORT1, 22, 0);
            break;
        case StarboardKnife:
            gioSetBit(hetPORT2, 1, 1);
            //add a delay for x seconds
            gioSetBit(hetPORT2, 1, 0);
            break;
        case PayloadKnife:
            gioSetBit(hetPORT1, 16, 1);
            //add a delay for x seconds
            gioSetBit(hetPORT1, 16, 0);
            break;
        default:
            return -1;
    }
    return 0;
}
