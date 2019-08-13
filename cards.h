#ifndef CARDS_H
#define CARDS_H

#define DECKSIZE 52

/*----types-------------------------------------------------------------------*/
typedef struct {
    //all index values 0 <= value <= 127
    //data types selected like this because sizeof(card) == 4
    char type;
    char suite;
    short value;
} card;

/*----functions and procedures------------------------------------------------*/
void makestr (card c, char* buf, int bufsize);
void init_deck ();
void shuffle ();
card pickup ();

#endif
