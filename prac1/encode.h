#include <stdlib.h>

void EncodeBuffer(char src[], char dest[], size_t size, char (*encodeByte)(char));

void DecodeBuffer(char src[], char dest[], size_t size, char (*decodeByte)(char));