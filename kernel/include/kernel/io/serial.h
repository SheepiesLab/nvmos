//
// Created by benng on 8/29/17.
//

#ifndef NVMOS_SERIAL_H
#define NVMOS_SERIAL_H

void init_serial();

int serial_received();

char read_serial();

int is_transmit_empty();

void write_serial(char a);

#endif //NVMOS_SERIAL_H
