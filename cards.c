#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cards.h"

/*----constants---------------------------------------------------------------*/
const char* suites[4] = {
    ".-------.\n"
    "|%s     |\n"
    "|   _   |\n"
    "|  ( )  |\n"
    "| (_x_) |\n"
    "|   Y %s|\n"
    "'-------'\n",

    ".-------.\n"
    "|%s     |\n"
    "|   /\\  |\n"
    "|  /  \\ |\n"
    "|  \\  / |\n"
    "|   \\/%s|\n"
    "'-------'\n",

    ".-------.\n"
    "|%s     |\n"
    "|  _  _ |\n"
    "| ( \\/ )|\n"
    "|  \\  / |\n"
    "|   \\/%s|\n"
    "'-------'\n",

    ".-------.\n"
    "|%s     |\n"
    "|   .   |\n"
    "|  / \\  |\n"
    "| (_,_) |\n"
    "|   I %s|\n"
    "'-------'\n"
};

const char* types[13] = {" 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10",
    " J", " Q", " K", " A"};

/*----variables---------------------------------------------------------------*/
card cards[DECKSIZE];
int next;

/*----functions---------------------------------------------------------------*/
// "private" -> not in header
void swap (int i, int j) {
    card tmp = cards[i];
    cards[i] = cards[j];
    cards[j] = tmp;
}

void init_deck () {
    next = 0;

    for (int i=0; i<DECKSIZE; i++) {
        cards[i].suite = (char)((i / 13) % 4); //modulo 4 for DECKSIZE > 52
        cards[i].type = (char)(i % 13);
        cards[i].value = (short)(cards[i].type + 2);
        if (cards[i].value > 10) cards[i].value = (short)10;
        if (cards[i].type == 12) cards[i].value++; //ace
    }
}

void makestr (card c, char* buf, int bufsize) {
    snprintf(buf, bufsize, suites[c.suite], types[c.type], types[c.type]);
}

void shuffle () {
    next = 0; //reset

    srand(time(NULL));
    int r;
    for (int i=0; i<DECKSIZE; i++) {
        r = rand() % DECKSIZE;
        swap(i, r);
    }
}

card pickup () {
    card c = cards[next];
    if (++next >= DECKSIZE) next = 0;
    return c;
}

