#ifndef __brici_h
#define __brici_h
void playBrici();
// Image definitions for brici
#define DBLU COLOUR(0,0,10)
#define YLBK COLOUR(255, 255, 48)

#define BGND COLOUR(0,0,0)
#define BLACK COLOUR(0,0,0)
#define RED COLOUR(0xff,0,0)
#define GRN COLOUR(0,0xff,0)
#define BLUE COLOUR(0x1f,0x1f,0xff)
#define WHI COLOUR(0xff,0xff,0xff)
#define YEL COLOUR(0xff,0xff,0x00)

const uint16_t YELLBLK[] = { 
    DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,
    DBLU,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,DBLU,
    DBLU,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,DBLU,
    DBLU,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,DBLU,
    DBLU,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,DBLU,
    DBLU,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,DBLU,
    DBLU,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,DBLU,
    DBLU,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,DBLU,
    DBLU,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,YLBK,DBLU,
    DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU 
};

#define GNBK RGBToWord(47, 249, 24)
const uint16_t GRENBLK[] = { 
    DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,
    DBLU,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,DBLU,
    DBLU,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,DBLU,
    DBLU,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,DBLU,
    DBLU,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,DBLU,
    DBLU,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,DBLU,
    DBLU,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,DBLU,
    DBLU,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,DBLU,
    DBLU,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,GNBK,DBLU,
    DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU 
};

#define BLBK RGBToWord(24, 201, 249)
const uint16_t BLUEBLK[] = { 
    DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,
    DBLU,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,DBLU,
    DBLU,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,DBLU,
    DBLU,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,DBLU,
    DBLU,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,DBLU,
    DBLU,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,DBLU,
    DBLU,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,DBLU,
    DBLU,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,DBLU,
    DBLU,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,BLBK,DBLU,
    DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU,DBLU 
};

const uint16_t BAT[] = { 
    WHI, WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI , WHI, WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,
    WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI , WHI, WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,
    WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI , WHI, WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI  
};
const uint16_t BALL[] = { 
    BGND, WHI,  BGND,
    WHI , WHI , WHI,
    BGND ,WHI,  BGND
};
#endif
