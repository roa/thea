#include <helper.h>

uint32_t 
inverse_exit(uint32_t exit)
{
    if (exit == UPPER_EXIT)
        return LOWER_EXIT;
    else if (exit == LOWER_EXIT)
        return UPPER_EXIT;
    else if (exit == LEFT_EXIT)
        return RIGHT_EXIT;
    else if (exit == RIGHT_EXIT)
        return LEFT_EXIT;
    else
        return exit;
}
