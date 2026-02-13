#ifndef TX_API_H
#define TX_API_H

#include <stdint.h>

/* --- 1. ThreadX Primitives --- */
/* CRITICAL FIX: Use #define for VOID so CMock doesn't try to instantiate it */
#define VOID void

typedef unsigned int    UINT;
typedef unsigned long   ULONG;
typedef char            CHAR;

/* --- 2. ThreadX Macros --- */
#define TX_WAIT_FOREVER 0xFFFFFFFFUL
#define TX_SUCCESS      0x00
#define TX_NO_WAIT      0x00
#define TX_OR           0x00
#define TX_TIMER_TICKS_PER_SECOND 1000

/* --- 3. ThreadX Object Handles --- */
typedef struct { uint32_t id; } TX_THREAD;
typedef struct { uint32_t id; } TX_QUEUE;
typedef struct { uint32_t id; } TX_EVENT_FLAGS_GROUP;
typedef struct { uint32_t id; } TX_MUTEX;

/* --- 4. ThreadX Function Prototypes --- */
/* These are the functions your driver actually calls */
UINT  tx_mutex_get(TX_MUTEX *mutex_ptr, ULONG wait_option);
UINT  tx_mutex_put(TX_MUTEX *mutex_ptr);
UINT  tx_event_flags_set(TX_EVENT_FLAGS_GROUP *group_ptr, ULONG flags_to_set, UINT set_option);
ULONG tx_time_get(void);
UINT  tx_thread_sleep(ULONG timer_ticks);

/* Add this if your driver uses it */
UINT App_ThreadX_Init(VOID *memory_ptr);

#endif // TX_API_H