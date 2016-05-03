#ifndef __HAL_H__
#define __HAL_H__

#define __IO volatile

#define FGPIOA_BASE             ((uint32_t)0xF8000000)
#define FGPIOB_BASE             ((uint32_t)0xF8000040)
#define FGPIOC_BASE             ((uint32_t)0xF8000080)
#define FGPIOD_BASE             ((uint32_t)0xF80000C0)
#define FGPIOE_BASE             ((uint32_t)0xF8000100)

typedef struct
{
  __IO uint32_t PDOR;
  __IO uint32_t PSOR;
  __IO uint32_t PCOR;
  __IO uint32_t PTOR;
  __IO uint32_t PDIR;
  __IO uint32_t PDDR;
} GPIO_TypeDef;

#define FGPIOA                  ((GPIO_TypeDef  *)   FGPIOA_BASE)
#define FGPIOB                  ((GPIO_TypeDef  *)   FGPIOB_BASE)
#define FGPIOC                  ((GPIO_TypeDef  *)   FGPIOC_BASE)
#define FGPIOD                  ((GPIO_TypeDef  *)   FGPIOD_BASE)
#define FGPIOE                  ((GPIO_TypeDef  *)   FGPIOE_BASE)


#define PORTA_BASE              ((uint32_t)0x40049000)
#define PORTB_BASE              ((uint32_t)0x4004A000)
#define PORTC_BASE              ((uint32_t)0x4004B000)
#define PORTD_BASE              ((uint32_t)0x4004C000)
#define PORTE_BASE              ((uint32_t)0x4004D000)

typedef struct
{
  __IO uint32_t PCR[32];
  __IO uint32_t GPCLR;
  __IO uint32_t GPCHR;
  uint32_t RESERVED0[6];
  __IO uint32_t ISFR;
} PORT_TypeDef;

#define PORTA                   ((PORT_TypeDef  *)   PORTA_BASE)
#define PORTB                   ((PORT_TypeDef  *)   PORTB_BASE)
#define PORTC                   ((PORT_TypeDef  *)   PORTC_BASE)
#define PORTD                   ((PORT_TypeDef  *)   PORTD_BASE)
#define PORTE                   ((PORT_TypeDef  *)   PORTE_BASE)

#endif /* __HAL_H__ */
