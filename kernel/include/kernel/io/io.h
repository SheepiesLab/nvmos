//
// Created by benng on 8/26/17.
//

#ifndef NVMOS_IO_H
#define NVMOS_IO_H

unsigned char inb(unsigned short int port);

unsigned char inb_p(unsigned short int port);

unsigned short int inw(unsigned short int port);

unsigned short int inw_p(unsigned short int port);

unsigned int inl(unsigned short int port);

unsigned int inl_p(unsigned short int port);

void outb(unsigned char value, unsigned  short int port);

void outb_p(unsigned char value, unsigned short int port);

void outw(unsigned short int value, unsigned short int port);

void outw_p(unsigned short int value, unsigned short int port);

void outl(unsigned int value, unsigned short int port);

void outl_p(unsigned int value, unsigned short int port);

void insb(
        unsigned short int port,
        void *addr,
        unsigned long int count);

void insw(
        unsigned short int port,
        void *addr,
        unsigned long int count);

void insl(
        unsigned short int port,
        void *addr,
        unsigned long int count);

void outsb(
        unsigned short int port,
        const void *addr,
        unsigned long int count);

void outsw(
        unsigned short int port,
        const void *addr,
        unsigned long int count);

void outsl(
        unsigned short int port,
        const void *addr,
        unsigned long int count);

#endif //NVMOS_IO_H