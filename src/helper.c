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

uint64_t
get_micro_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec;
}

char*
create_dyn_string(const char *str)
{
    size_t str_len = strlen(str) + 1; 
    char *dyn_str = calloc(str_len, sizeof(*dyn_str));
    snprintf(dyn_str, str_len, str);
    return dyn_str;
}

char*
create_num_string(const char *str, int32_t num)
{
    size_t str_len = strlen(str) + 1 + width_as_string(num);
    char *num_string = calloc(str_len, sizeof(*num_string));
    snprintf(num_string, str_len, "%s%d", str, num);
    return num_string;
}

uint8_t
get_screen_ratio()
{
    //TODO: find good ratio for animation_move_towards
    double cols = (double) COLS;
    double lines = (double) LINES;
    return (uint8_t) (cols / lines);
}

size_t
width_as_string(int32_t num)
{
    if (num == 0)
        return 1;
    size_t count = 0;
    while (num != 0)
    {
        num /= 10;
        ++count;
    }
    return count;
}
