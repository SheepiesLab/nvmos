#include <kernel/int/pic.h>
#include <kernel/io/io.h>


#define PIC_EOI        0x20        /* End-of-interrupt command code */

void PIC_sendEOI(unsigned char irq) {
    if (irq >= 8)
        outb(PIC_EOI, PIC2_COMMAND);

    outb(PIC_EOI, PIC1_COMMAND);
}

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4    0x01        /* ICW4 (not) needed */
#define ICW1_SINGLE    0x02        /* Single (cascade) mode */
#define ICW1_INTERVAL4    0x04        /* Call address interval 4 (8) */
#define ICW1_LEVEL    0x08        /* Level triggered (edge) mode */
#define ICW1_INIT    0x10        /* Initialization - required! */

#define ICW4_8086    0x01        /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO    0x02        /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE    0x08        /* Buffered mode/slave */
#define ICW4_BUF_MASTER    0x0C        /* Buffered mode/master */
#define ICW4_SFNM    0x10        /* Special fully nested (not) */

/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2) {
    unsigned char a1, a2;

    a1 = inb(PIC1_DATA); // save masks
    a2 = inb(PIC2_DATA);

    outb(ICW1_INIT + ICW1_ICW4, PIC1_COMMAND);// starts the initialization sequence (in cascade mode)
    io_wait();
    outb(ICW1_INIT + ICW1_ICW4, PIC2_COMMAND);
    io_wait();
    outb(offset1, PIC1_DATA);// ICW2: Master PIC vector offset
    io_wait();
    outb(offset2, PIC2_DATA);// ICW2: Slave PIC vector offset
    io_wait();
    outb(4, PIC1_DATA);// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(2, PIC2_DATA);// ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    outb(ICW4_8086, PIC1_DATA);
    io_wait();
    outb(ICW4_8086, PIC2_DATA);
    io_wait();

    outb(a1, PIC1_DATA);// restore saved masks.
    outb(a2, PIC2_DATA);
}

void IRQ_set_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(value, port);
}

void IRQ_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(value, port);
}

#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */

static uint16_t __pic_get_irq_reg(int ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t pic_get_irr(void)
{
    return __pic_get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register */
uint16_t pic_get_isr(void)
{
    return __pic_get_irq_reg(PIC_READ_ISR);
}