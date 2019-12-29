//2018.12.6

extern const unsigned char H8W6[][6];
extern const unsigned char H16W8[][16];


#define ASCII_H8W6 ((const unsigned char (*)[6])&(H8W6[-32]))
#define ASCII_H16W8 ((const unsigned char (*)[16])&(H16W8[-32]))

