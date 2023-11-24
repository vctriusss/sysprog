#include "encode.h"

void EncodeBuffer(char src[], char dest[], size_t size, char (*encodeByte)(char)) {
    for (size_t i = 0; i < size; i++) {
        dest[i] = encodeByte(src[i]);
    }
}

void DecodeBuffer(char src[], char dest[], size_t size, char (*encodeByte)(char)) {
    for (size_t i = 0; i < size; i++) {
        dest[i] = encodeByte(src[i]);
    }
}