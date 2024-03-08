#include "types.h"
#include "io.h"
#include "encode.h"

char EncodeByte(char c) {
    return c ^ 0b01010101;
}

int main(int argc, char *argv[]) {
    int err;

    char filePath[MAX_SIZE];

    err = GetFilePath(filePath, argc, argv);
    if (err == ERR_INVALID_FLAG) {
        RaiseError("Run program like: bin/serialize -f <file_path_for_serialization>");
        return err;
    }

    Union *uPtr;
    uPtr = (Union*) malloc(1 * sizeof(Union));

    char *encodedBuff;
    encodedBuff = (char*) malloc(sizeof(Union) * 1);

    printf("Enter airport name (max %d): ", MAX_SIZE);
    err = Scan(uPtr->airport.Name, MAX_SIZE, false);

    if (err != 0) {
        switch (err) {
        case ERR_TOO_LONG:
            RaiseError("Airport name is too long");
            break;
        case ERR_TOO_SHORT:
            RaiseError("Airport name is too short");
            break;
        case ERR_DEPRECATED_CHAR:
            RaiseError("Airport name contains deprecated chars (possible are [A-Za-z0-9\'\"\\s])");
            break;
        }

        free(uPtr);
        free(encodedBuff);
        return err;
    }


    printf("Enter airport %d-letter code: ", CODE_SIZE);
    err = Scan(uPtr->airport.Code, CODE_SIZE, true);

    if (err != 0) {
        switch (err) {
        case ERR_TOO_LONG:
            RaiseError("Airport code is too long");
            break;
        case ERR_TOO_SHORT:
            RaiseError("Airport code is too short");
            break;
        case ERR_DEPRECATED_CHAR:
            RaiseError("Airport code contains deprecated chars (possible are [A-Za-z])");
            break;
        }

        free(uPtr);
        free(encodedBuff);
        return err;
    }

    EncodeBuffer(uPtr->Buffer, encodedBuff, sizeof(Union), EncodeByte);

    err = WriteToFile(filePath, encodedBuff, sizeof(Union));
        if (err != 0) {
        switch (err) {
        case ERR_OPENING_FILE:
            RaiseError("File can't be opened");
        }
        
        free(uPtr);
        free(encodedBuff);
        return err;
    }

    free(uPtr);
    free(encodedBuff);
    return 0;
}