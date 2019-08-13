#include <stdbool.h>
#include "cards.h"
#include "house.h"

/*----global variables--------------------------------------------------------*/
card playerHand[11];
int playerHandSize; //index to first free slot

card dealerHand[11];
int dealerHandSize;

bool playerStand;
bool dealerStand;

int playerBet;
int playerMoney = 100;

/*----private functions-------------------------------------------------------*/
int calculate_value (card* hand, int size) {
    int result = 0;
    int aceCount = 0;
    
    for (int i=0; i<size; i++) {
        result += hand[i].value;
        if (hand[i].value == 11) ++aceCount;
    }
    
    while (aceCount > 0 && result > 21) {
        result -= 10;
        --aceCount;
    }
    
    return result;
}

void make_dealer_decision () {
    int value = calculate_value (dealerHand, dealerHandSize);
    if (value < 17) {
        dealerHand[dealerHandSize] = pickup();
        ++dealerHandSize;
    }
    else {
        dealerStand = true;
    }
}

/*----public functions--------------------------------------------------------*/
int player_balance () {
    return playerMoney;
}

card new_round (int bet) {
    playerBet = bet;
    playerStand = dealerStand = false;
    
    shuffle();
    for (int i=0; i<2; i++) {
        playerHand[i] = pickup();
        dealerHand[i] = pickup();
    }
    playerHandSize = dealerHandSize = 2;
    
    return dealerHand[1]; //dealer's upcard
}

card hit () {
    if (!playerStand) {
        playerHand[playerHandSize] = pickup();
        ++playerHandSize;
        make_dealer_decision();
    }
    return playerHand[playerHandSize-1];
}

card double_down () {
    playerBet <<= 1;
    card result = hit();
    player_stand();
    return result;
}

void player_stand () {
    playerStand = true;
    while (!dealerStand) make_dealer_decision();
}

int payout () {
    if (!playerStand) player_stand();
    int result = 0;
    int playerValue = calculate_value(playerHand, playerHandSize);
    int dealerValue = calculate_value(dealerHand, dealerHandSize);
    
    if (playerValue == 21 && playerHandSize == 2)
        result = playerBet + (playerBet >> 1);
    else if (dealerValue == 21 && dealerHandSize == 2)
        result = (playerBet + (playerBet >> 1)) * (-1);
    else if (playerValue > 21)
        result = -playerBet;
    else if (dealerValue > 21)
        result = playerBet;
    else if (dealerValue > playerValue)
        result = -playerBet;
    else if (dealerValue < playerValue)
        result = playerBet;
    else
        result = 0;
    
    playerMoney += result;
    return result;
}

int get_hand (card* buf, int bufsize) {
    int i = 0;
    while (i < playerHandSize && i < bufsize) {
        buf[i] = playerHand[i];
        ++i;
    }
    return playerHandSize;
}

int get_dealer_hand (card* buf, int bufsize) {
    int i = 0;
    while (i < dealerHandSize && i < bufsize) {
        buf[i] = dealerHand[i];
        ++i;
    }
    return dealerHandSize;
}
