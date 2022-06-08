#ifndef	__ADDR_SHARE_MEMORY_H_
#define	__ADDR_SHARE_MEMORY_H_

#include "Type.h"

#define int_disable()   asm ( "csrci mstatus, 0x08")
#define int_enable()    asm ( "csrsi mstatus, 0x08") 

#define REG_INT_ENABLE 			*((volatile U32 *)0x1A104000)
#define REG_INT_PEND			*((volatile U32 *)0x1A104004)
#define REG_INT_CLEAR			*((volatile U32 *)0x1A10400C)
#endif
