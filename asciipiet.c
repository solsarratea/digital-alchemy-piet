#include "img.h"

Color piet[6][3] = {
    {{0xff,0xc0,0xc0},{0xff,0x00,0x00},{0xc0,0x00,0x00}},
    {{0xff,0xff,0xc0},{0xff,0xff,0x00},{0xc0,0xc0,0x00}},
    {{0xc0,0xff,0xc0},{0x00,0xff,0x00},{0x00,0xc0,0x00}},
    {{0xc0,0xff,0xff},{0x00,0xff,0xff},{0x00,0xc0,0xc0}},
    {{0xc0,0xc0,0xff},{0x00,0x00,0xff},{0x00,0x00,0xc0}},
    {{0xff,0xc0,0xff},{0xff,0x00,0xff},{0xc0,0x00,0xc0}}
};

Color white = {0xff,0xff,0xff};

int main(int argc, char* argv[])
{
    char* str;
    int len, i, hue, dark;
    Image out;

    if(argc != 2)
    {
        printf("Usage: %s \"string to print\"\n", argv[0]);
        return -1;
    }

    str = argv[1];
    len = strlen(str);

    imgCreate(out, len * 2 + 3, 128);

    hue = 0;
    dark = 1;
    for(i = 0; i < len; i++)
    {
        imgLine(out, i * 2, 0, i * 2, str[i] - 1, piet[hue][dark]);
        dark = (dark + 1) % 3;
        imgSetP(out, i * 2 + 1, 0, piet[hue][dark]);
        dark = (dark + 2) % 3;
        hue = (hue + 5) % 6;
    }
    imgSetP(out, len * 2, 0, piet[hue][dark]);
    imgSetP(out, len * 2 + 1, 0, white);
    imgSetP(out, len * 2 + 2, 0, white);
    imgSetP(out, len * 2 + 2, 1, white);
    imgSetP(out, len * 2 + 2, 2, white);
    imgSetP(out, len * 2 + 2, 3, white);
    imgSetP(out, len * 2 + 1, 3, white);
    imgSetP(out, len * 2, 2, piet[0][4]);
    imgSetP(out, len * 2, 3, piet[0][5]);
    imgSetP(out, len * 2, 4, piet[0][6]);

    imgSave(out, "piet.pnm");

    return 0;
}
