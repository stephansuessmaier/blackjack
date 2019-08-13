#ifndef HOUSE_H
#define HOUSE_H

/*----functions and procedures------------------------------------------------*/
int player_balance ();
card new_round (int bet);
card hit ();
card double_down ();
void player_stand();
int payout ();
int get_hand (card* buf, int bufsize);
int get_dealer_hand (card* buf, int bufsize);

#endif
