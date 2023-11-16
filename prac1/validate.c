#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "validate.h"

bool isValidWeekday(char *weekday)
{
    for (int i = 0; i < 7; i++)
    {
        if (strcmp(weekday, WEEKDAYS[i]) == 0)
            return true;
    }

    return false;
}

bool isValidMonth(char *month)
{
    for (int i = 0; i < 12; i++)
    {
        if (strcmp(month, MONTHS[i]) == 0)
            return true;
    }

    return false;
}

bool isValidDate(int day, char *month)
{
    if (day > 0 && day <= 31)
    {
        return false;
    }
    return true;
}