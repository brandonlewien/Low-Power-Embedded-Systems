#include "captouch.h"

void captouch_init(void) {
  /* Use the default STK capacative sensing setup */
  ACMP_CapsenseInit_TypeDef capsenseInit = ACMP_CAPSENSE_INIT_DEFAULT;
  ACMP_CapsenseInit(ACMP_CAPSENSE, &capsenseInit);
}
