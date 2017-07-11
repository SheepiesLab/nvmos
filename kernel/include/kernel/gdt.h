#include <stdint.h>
#include <stddef.h>

#define GDT_GD_TYPE_PR 0x80
#define GDT_GD_TYPE_PRIVI_0 0x00   //0 is highest
#define GDT_GD_TYPE_PRIVI_1 0x20
#define GDT_GD_TYPE_PRIVI_2 0x40
#define GDT_GD_TYPE_PRIVI_3 0x60
#define GDT_GD_TYPE_BIT4 0x10 //Bit 4 of access byte must be set
#define GDT_GD_TYPE_EX 0x8
#define GDT_GD_TYPE_DC 0x4
#define GDT_GD_TYPE_RW 0x2
#define GDT_GD_TYPE_AC 0x1

#define GDT_GD_FLAG_GR 0x80
#define GDT_GD_FLAG_SZ 0x40


struct GD{
  uint32_t base;
  uint32_t limit; //Limit only has 20bits. Check for it.
  uint8_t type;
};

typedef struct GD GlobalDescriptor;

int commitGDT(uint8_t *buffer, GlobalDescriptor *GDT, size_t GDTSize);
int encodeGD(uint8_t *buffer, GlobalDescriptor GD);
extern void setGDT(uint32_t base, uint16_t size);
