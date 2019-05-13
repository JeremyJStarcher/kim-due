#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 64 * 1024

int main(int argc, char *argv[])
{
    char fname[255];
    char tmp[255];

    sprintf(fname, "%s",argv[2]);

    unsigned char buf[BUFFER_SIZE];

    printf("Reading: %s\n", argv[1]);
    printf("Writing : %s\n", fname);

    FILE *ifp = fopen(argv[1], "rb");
    FILE *ofp = fopen(argv[2], "w");

    if (ifp == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        fflush(stderr);
        exit(1);
    }

    size_t size = fread(&buf, 1, BUFFER_SIZE, ifp);
    printf("The size is: %lu\n", size);

    fprintf(ofp, "const uint8_t %s[%lu] PROGMEM = {", argv[3], size);
    for (size_t i = 0; i < size; i++) {
        if (i != 0) {
           fprintf(ofp, ", ");
        }

        if (i % 12 == 0) {
           fprintf(ofp, "\n    ");
        }
        fprintf(ofp, "0x%02X", buf[i]);
    }
    fprintf(ofp, "};\n");
}
