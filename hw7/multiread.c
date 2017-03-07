/*
 * Reads president files, outputs initials and ages
 *
 * Will Graham <grahamw0@students.rowan.edu>
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char           inits[5];
    unsigned char  num;
} prezinfo;

int main(int argc, char *argcv[])
{
    prezinfo president;
    FILE *data;

    if (argc < 2) {
      fprintf(stderr, "Usgae: %s filename\n", argcv[0]);
      exit(1);
    }

    if((data = fopen(argcv[1], "r")) == NULL) {
        perror(argcv[1]);
        exit(1);
    }

    printf("Num  Inits\n");

    while (fread(&president, sizeof(prezinfo), 1, data) == 1) {
        printf(" %d   %s\n", president.num, president.inits);
    }

    printf("End of data\n");
    fclose(data);


    return 0;
}
