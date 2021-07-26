#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
   unsigned char r;
   unsigned char g;
   unsigned char b;
} Color;

typedef struct
{
   Color* data;
   int width;
   int height;
} Image;

#define imgCreate(img, w, h)           {\
                                          int length;\
                                          (img).width = (w);\
                                          (img).height = (h);\
                                          length = (img).width * (img).height * sizeof(Color);\
                                          (img).data = malloc(length);\
                                          memset((img).data, 0, length);\
                                       }

#define imgDestroy(img)                {\
                                          free((img).data);\
                                          (img).width = 0;\
                                          (img).height = 0;\
                                       }

#define imgGetP(img, x, y)             ((img).data[(int)(x) + (int)(y) * (img).width])

#define imgSetP(img, x, y, c)          {\
                                          (img).data[(int)(x) + (int)(y) * (img).width] = c;\
                                       }

#define imgLine(img, x, y, xx, yy, c)  {\
                                          int x0 = (x), y0 = (y), x1 = (xx), y1 = (yy);\
                                          int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;\
                                          int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;\
                                          int err = dx + dy, e2;\
                                          \
                                          for(;;)\
                                          {\
                                             imgSetP((img), x0, y0, c);\
                                             if (x0 == x1 && y0 == y1) break;\
                                             e2 = 2 * err;\
                                             if (e2 >= dy) {err += dy; x0 += sx;}\
                                             if (e2 <= dx) {err += dx; y0 += sy;}\
                                          }\
                                       }

#define imgSave(img, fname)            {\
                                          FILE* f = fopen((fname), "wb");\
                                          fprintf(f, "P6\n%d %d\n255\n", (img).width, (img).height);\
                                          fwrite((img).data, sizeof(Color), (img).width * (img).height, f);\
                                          fclose(f);\
                                       }

#define imgLoad(img, fname)            {\
                                          FILE* f = fopen((fname), "rb");\
                                          char buffer[16];\
                                          int index = 0;\
                                          int field = 0;\
                                          int isP5 = 0;\
                                          unsigned char c = ' ';\
                                          while(field < 4)\
                                          {\
                                             do\
                                             {\
                                                if(c == '#') while(c = fgetc(f), c != '\n');\
                                             } while(c = fgetc(f), isspace(c) || c == '#');\
                                             index = 0;\
                                             do\
                                             {\
                                                buffer[index++] = c;\
                                             } while(c = fgetc(f), !isspace(c) && c != '#' && index < 16);\
                                             buffer[index] = 0;\
                                             switch(field)\
                                             {\
                                                case 0:\
                                                   if (strcmp(buffer, "P5") == 0) isP5 = 1;\
                                                   else if (strcmp(buffer, "P6") == 0) isP5 = 0;\
                                                   else fprintf(stderr, "image format \"%s\" unsupported (not P5 or P6)\n", buffer), exit(1);\
                                                   break;\
                                                case 1:\
                                                   (img).width = atoi(buffer);\
                                                   break;\
                                                case 2:\
                                                   (img).height = atoi(buffer);\
                                                   break;\
                                                case 3:\
                                                   index = atoi(buffer);\
                                                   if (index != 255) fprintf(stderr, "image format unsupported (not 255 values per channel)\n"), exit(1);\
                                                   break;\
                                             }\
                                             field++;\
                                          }\
                                          imgCreate((img), (img).width, (img).height);\
                                          if (isP5)\
                                          {\
                                             int length = (img).width * (img).height;\
                                             for(index = 0; index < length; ++index)\
                                             {\
                                                (img).data[index].r = (img).data[index].g = (img).data[index].b = fgetc(f);\
                                             }\
                                          }\
                                          else\
                                          {\
                                             fread((img).data, sizeof(Color), (img).width * (img).height, f);\
                                          }\
                                          fclose(f);\
                                       }
