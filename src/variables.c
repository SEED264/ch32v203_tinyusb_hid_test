#include <stdint.h>
#include "variables.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

volatile uint16_t *FLASHCAP = (volatile uint16_t*)0x1FFFF7E0;
volatile uint32_t *UID0 = (volatile uint32_t*)0x1FFFF7E8;
volatile uint32_t *UID1 = (volatile uint32_t*)0x1FFFF7EC;
volatile uint32_t *UID2 = (volatile uint32_t*)0x1FFFF7F0;

#if __cplusplus
}
#endif //__cplusplus
