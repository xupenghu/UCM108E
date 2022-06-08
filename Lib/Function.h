#ifndef	__FUNCTION_H_
#define	__FUNCTION_H_

#include "Type.h"

#ifndef	__FUNCTION_C_
#define	EXT_FUN			extern
#else
#define	EXT_FUN
#endif

EXT_FUN void MemoryCopy(OUT U08 *pDst, IN const U08 *pSrc, IN U32 nLen);

#endif
