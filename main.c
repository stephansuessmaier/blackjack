#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "cards.h"
#include "house.h"

#define BUFSIZE 71
#define MAX_HANDSIZE 11

/*----constants---------------------------------------------------------------*/
const char* helpMessage =
    "    Blackjack for ncurses   \n"
    " - by Stephan Suessmaier -  \n"
    "\n"
    "Usage:\n"
    "  UP:    hit\n"
    "  DOWN:  stand\n"
    "  SPACE: double down\n"
    "  RIGHT: increase bet\n"
    "  LEFT:  decrease bet\n"
    "  h:     show this help page\n"
    "  q:     quit\n"
    "\n"
    "press any key to continue...\n";

/*----variables---------------------------------------------------------------*/
char buf[BUFSIZE];
card hand[MAX_HANDSIZE];
card dealerhand[MAX_HANDSIZE]; //upcard == dealerhand[0]
int handsize;
int dealerhandsize;
int bet = 10;

/*----function declaration----------------------------------------------------*/
void quit ();
void display_multiline (const char* text, int size, int y, int x);
void display_card (card c, int y, int x);
int main_interaction ();
void round_finished ();
void help_message ();

/*----main program------------------------------------------------------------*/
int main () {
    handsize = 0;
    int inpt;

    // ncurses setup
    initscr();
    atexit(quit);
    curs_set(0);
    raw();
    keypad(stdscr, TRUE); // allows usage of F-keys, arrows, etc.
    noecho();

    // display help message at startup
    help_message();

    init_deck();
    dealerhand[0] = new_round(bet);
    handsize = get_hand(hand, MAX_HANDSIZE);

    // game loop
    while (1) {
        inpt = main_interaction();
        switch (inpt) {
        case 'q': // quit
            return 0;
        case 'h': // help
            help_message();
            break;
        case ' ': // double down
            if (handsize == 2) { // only allowed at start
                hand[handsize] = double_down();
                ++handsize;
                round_finished();
                break;
            }
            break;
        case KEY_UP: // hit
            if (handsize < MAX_HANDSIZE) {
                hand[handsize] = hit();
                ++handsize;
                break;
            }
            // otherwise stand (==next case)
        case KEY_DOWN: // stand
            round_finished();
            break;
        case KEY_LEFT: // decrease bet
            if (bet > 5) bet -= 5;
            break;
        case KEY_RIGHT: // increase bet
            bet += 5;
            break;
        default:
            break;
        }

        refresh();
    }

    return 0;
}

/*----function definitions----------------------------------------------------*/
void quit () {
    endwin();
}

void display_multiline (const char* text, int size, int y, int x) {
    int i = 0;
    int xStart = x;
    while (i < size) {
        if (text[i] == '\n' || text[i] == '\0') {
            x = xStart;
            ++y;
        }
        else {
            mvaddch(y, x, text[i]);
            ++x;
        }
        ++i;
    }
}

void display_card (card c, int y, int x) {
    makestr(c, buf, BUFSIZE);
    display_multiline(buf, BUFSIZE, y, x);
}

int main_interaction () {
    mvaddstr(0, 0, "Dealer's upcard:");
    display_card(dealerhand[0], 1, 2);

    mvaddstr(9, 0, "Player hand:");
    for (int i=0; i<handsize; i++) {
        display_card(hand[i], 10, 9*i);
    }

    move(LINES-2, 0);
    clrtoeol();
    move(LINES-1, 0);
    clrtoeol();
    mvprintw(LINES-2, 0, "current bet:  %d", bet);
    mvprintw(LINES-1, 0, "player money: %d", player_balance());

    return getch();
}

void round_finished () {
    player_stand();
    int gain = payout();

    clear();

    dealerhandsize = get_dealer_hand(dealerhand, MAX_HANDSIZE);
    mvaddstr(0, 0, "Dealer's hand was:");
    for (int i=0; i<dealerhandsize; i++) {
        display_card(dealerhand[i], 1, 9*i);
    }
    mvaddstr(9, 0, "Player hand was:");
    for (int i=0; i<handsize; i++) {
        display_card(hand[i], 10, 9*i);
    }
    if (gain < 0) {
        mvprintw(LINES-2, 0, "You lost %d to the house.", gain*(-1));
    }
    else {
        mvprintw(LINES-2, 0, "You won %d.", gain);
    }
    mvaddstr(LINES-1, 0, "press any key to continue...");

    getch();
    clear();
    
    dealerhand[0] = new_round(bet);
    handsize = get_hand(hand, MAX_HANDSIZE);    
}

void help_message () {
    // helpmessage block witdth=28 height=13
    clear();
    display_multiline(helpMessage, strlen(helpMessage), (LINES-13)>>1,
        (COLS-28)>>1);
    refresh();
    getch(); // wait for any key pressed
    clear();
}
