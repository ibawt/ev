#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *in;
    FILE *out;
    char buff[256];
    int len;
    int i;

    if( argc < 3 ) {
        fprintf(stderr, "usage: %s {out} {in}\n", argv[0]);
        return 1;
    }

    in = fopen(argv[1], "r");
    if( !in )
        return 1;

    snprintf(buff, sizeof(buff), "%s.c", argv[2]);

    out = fopen(buff, "w");
    if(!out)
        return 1;

    fprintf(out, "const char %s[] = {\n", argv[2]);

    for(;;) {
        len = fread(buff, 1, sizeof(buff), in);

        for( i = 0 ; i < len ; ++i ) {
            fprintf(out, "0x%02x, ", buff[i] );
        }
        if( len <= 0 )
            break;
    }

    fprintf(out, "};\n");
    fclose(out);
    fclose(in);

    return 0;
}
