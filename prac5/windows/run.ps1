gcc windows_parent.c error.h mem.h scan.h -o parent.exe -lm;
gcc windows_child.c error.h mem.h scan.h -o child.exe -lm;

./parent.exe $args[0] $args[1];