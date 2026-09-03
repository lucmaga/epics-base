#ifndef INC_registerModules_H
#define INC_registerModules_H

#include "libComAPI.h"

LIBCOM_API void registerPrintModules();
LIBCOM_API int registerModule(char *name, char *version);

#endif /* INC_registerModules_H */
