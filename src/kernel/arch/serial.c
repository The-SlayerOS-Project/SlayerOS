#include <arch/serial.h>

void outb(u16 port, u8 value) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(value), "Nd"(port));
}

u8 inb(u16 port) {
    u8 ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}


void serial_init() {
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
    outb(COM1 + 4, 0x1E);
    outb(COM1 + 0, 0xAE);
    if(inb(COM1 + 0) != 0xAE) {
        for(;;);
   }
   outb(COM1 + 4, 0x0F);
}

bit b_serial_active() {
    return inb(COM1 + 5) & 1;
}

bit b_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}

char read_serial() {
    while(!b_serial_active());
    return inb(COM1);
}

void write_serial(char data) {
    while (!b_transmit_empty());
    outb(COM1,data);
}

void io_wait(void) {
    __asm__ __volatile__ ("outb %%al, $0x80" :: "a"(0));
}
