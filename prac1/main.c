#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "validate.h"
#include "scan.h"

#define MAX_SIZE 32

struct Date
{
    int day;

    char *month[MAX_SIZE];
    char *weekday[MAX_SIZE];
};

union Union
{
    struct Date date;

    char buffer[sizeof(struct Date)];
};

void Capitalize(char str[])
{
    for (size_t i = 1; i < strlen(str); i++)
    {
        str[i] = tolower(str[i]);
    }
}

void RaiseError(char *msg)
{
    printf("[ERROR] %s\n", msg);
}

int main(int argc, char *argv[])
{

    bool serialize = false;

    if (argc < 2)
    {
        RaiseError("Please choose program mode: -s (serialize) or -d (deserialize)");
        return -1;
    }
    else if (argc > 2)
    {
        RaiseError("Please choose only one mode: -s (serialize) or -d (deserialize)");
        return -2;
    }
    else
    {
        char *mode = argv[1];
        if (strcmp(mode, "-s") == 0)
        {
            serialize = true;
            printf("MODE - Serializaion\n");
        }
        else if (strcmp(mode, "-d") == 0)
        {
            printf("MODE - Deserializaion\n");
        }
        else
        {
            RaiseError("Incorrect mode, choose one of: -s (serialize) or -d (deserialize)");
            return -3;
        }
    }

    if (serialize)
    {
    }

    int day;
    char weekday[MAX_SIZE], month[MAX_SIZE];

    // Scanning day
    printf("Enter day number: ");
    int ok = ScanInt(&day, MAX_SIZE);

    if (!ok)
    {
        RaiseError("Invalid number format");
        return 1;
    }

    if (day <= 0 && day > 31)
    {
        RaiseError("Day is out of range");
        return 1;
    }

    // Scanning month
    printf("Enter month name: ");
    ScanString(&month, MAX_SIZE);

    Capitalize(month);
    if (!isValidMonth(month))
    {
        RaiseError("Month is incorrect");
        return 2;
    }
    if (!isValidDate(day, month))
    {
        char *err;
        sprintf(err, "There aren't %d days in %s", day, month);
        RaiseError(err);
        return 3;
    }

    printf("Enter weekday name: ");
    scanf("%s", weekday);

    if (!isValidWeekday(weekday))
    {
        RaiseError("Weekday is incorrect");
        return 4;
    }

    // TODO: serialize or deserialize

    return 0;
}