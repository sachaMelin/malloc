#include <stdio.h>
#include <stdlib.h>

#include "malloc.h"

int main(void)
{
    char *s1 = my_malloc(6);
    s1[0] = 'h';
    s1[1] = 'e';
    s1[2] = 'l';
    s1[3] = 'l';
    s1[4] = 'o';
    s1[5] = '\0';
    fprintf(stdout, "%s\n", s1);

    char *s2 = my_malloc(4000);
    s2[0] = 'c';
    s2[1] = 'a';
    s2[2] = 'c';
    s2[3] = 'a';
    s2[4] = '\0';

    fprintf(stdout, "s2 %p\n", (void *)s2);
    fprintf(stdout, "%s\n", s2);

    char *s3 = my_malloc(3000);
    s3[0] = 'b';
    s3[1] = 'i';
    s3[2] = 't';
    s3[3] = 'e';
    s3[4] = '\0';

    fprintf(stdout, "s3 %p\n", (void *)s3);
    fprintf(stdout, "%s\n", s3);

    // char *s4 = my_malloc(5);
    // s4[0] = 'z';
    // s4[1] = 'i';
    // s4[2] = 'z';
    // s4[3] = 'i';
    // s4[4] = '\0';

    // fprintf(stdout, "s4 %p\n", (void *)s4);
    // fprintf(stdout, "%s\n", s4);

    my_free(s2);
    my_free(s3);
    my_free(s1);
}
