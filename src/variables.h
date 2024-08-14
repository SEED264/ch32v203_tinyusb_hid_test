#if !defined(__VARIABLES_H__)
#define __VARIABLES_H__

#include <stdint.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

extern uint32_t SystemCoreClock;
extern volatile uint16_t *FLASHCAP;
extern volatile uint32_t *UID0;
extern volatile uint32_t *UID1;
extern volatile uint32_t *UID2;

#if __cplusplus
}
#endif //__cplusplus

#endif // __VARIABLES_H__