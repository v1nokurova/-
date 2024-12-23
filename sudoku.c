#include <stdio.h>

#define USER_MODE // USER_MODE OR BRUTEFORCE_MODE

#ifdef USER_MODE
    #include "user.h"
#else
    #include "bruteforce.h"
#endif