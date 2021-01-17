#include <stdlib.h>
#include <stdio.h>
#include <time.h>

clock_t start;

typedef char card_t;
typedef unsigned __int128 uint128_t;

int otherdim[3][3] = { {0, 2, 1}, { 2, 1, 0}, {1, 0, 2} };
card_t third_card[81][81];


card_t find_3rd_card(card_t card1, card_t card2) {

    int dim[4];
    int i;
    card_t third_card = 0;

    for (i = 3; i >= 0; i--) {
        int dim1 = card1 % 3, dim2 = card2 % 3;
        card1 /= 3; card2 /= 3;
        dim[i] = otherdim[dim1][dim2];
    }
    for (i = 0; i < 4; i++) {
        third_card *= 3;
        third_card += dim[i];
    }
    return third_card;
}


void fill_third_card() {

    for (card_t i=0; i<81; i++) {
        for (card_t j=0; j<81; j++) {
            third_card[i][j] = find_3rd_card(i, j);
        }
    }
}


card_t find_3rd_card_opt(card_t card1, card_t card2) {
    return third_card[card1][card2];
}


void print_board(card_t *board, int boardsize) {

    printf("%2i: ", boardsize);
    for (int i=0; i<boardsize; i++) {
        card_t card = board[i];
        for (int l=0; l<4; l++) {
            printf("%i", card % 3);
            card /= 3;
        }
        putchar(' ');
    }
    putchar('\n');
}


void generate_board(card_t *board, int boardsize, int cardnum, uint128_t validcards) {

    static int max_boardsize = 0;

    if (boardsize >= max_boardsize) {
        printf("%f\n", (double) (clock() - start) / CLOCKS_PER_SEC);
        print_board(board, boardsize);
        max_boardsize = boardsize;
    }

    for (int i = cardnum; i < 81; i++) {
        uint128_t cardmask = 1; cardmask <<= i;
        if (validcards & cardmask) {
            uint128_t invalidcards = 0;

            board[boardsize] = i;

            for (int j = 0; j < boardsize; j++) {
                uint128_t third_card_mask = 1; third_card_mask <<= third_card[i][board[j]];
                invalidcards |= third_card_mask;
            }

            generate_board(board, boardsize + 1, i+1, validcards & ~invalidcards);
        }
    }
}


void generate_max_nonset() {
    card_t board[81];
    uint128_t invalidcards = 0;

    board[0] = 0;
    fill_third_card();
    generate_board(board, 1, 1, ~invalidcards);
}


int main(int argc, char **argv) {

    start = clock();

    generate_max_nonset();

    return EXIT_SUCCESS;
}