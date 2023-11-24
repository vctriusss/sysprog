#include "types.h"
#include "io.h"
#include "encode.h"

char DecodeByte(char c) {
    return c ^ 0b01010101;
}

int main(int argc, char *argv[]) {
    int err;

    char filePath[MAX_SIZE];

    err = GetFilePath(filePath, argc, argv);
    if (err == ERR_INVALID_FLAG) {
        RaiseError("Run program like: bin/deserialize -f <file_path_for_deserialization>");
        return err;
    }

    Union *uPtr;
    uPtr = (Union*) malloc(1 * sizeof(Union));

    err = ReadFromFile(filePath, uPtr->Buffer, sizeof(Union));

    if (err != 0) {
        switch (err) {
        case ERR_BROKEN_FILE:
            RaiseError("There's something wrong with serialized file");
            break;
        case ERR_OPENING_FILE:
            RaiseError("File can't be opened");
        }
        
        free(uPtr);
        return err;
    }

    DecodeBuffer(uPtr->Buffer, uPtr->Buffer, sizeof(Union), DecodeByte);

    printf("Airport name: %s\nAirport code: %s\n", uPtr->airport.Name, uPtr->airport.Code);
    
    return 0;
}