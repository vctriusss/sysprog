HEADERS = encode.h io.h error.h types.h

DEFAULT = encode.c io.c error.c

DIR = bin

all: serialize deserialize

serialize: serialize.c $(DEFAULT) $(HEADERS)
	gcc serialize.c $(DEFAULT) $(HEADERS) -o $(DIR)/serialize -I.

deserialize: deserialize.c $(DEFAULT) $(HEADERS)
	gcc deserialize.c $(DEFAULT) $(HEADERS) -o $(DIR)/deserialize -I.

clean:
	-rm -f $(DIR)/serialize $(DIR)/deserialize