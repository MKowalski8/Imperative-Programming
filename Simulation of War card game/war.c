#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#define TEST  1

#define DECK_SIZE 52

int rand_from_interval(int a, int b);

void shuffling_cards(int size, int array[]);

void print_all_deck(const int array[], int size);

void giving_cards(const int array[], int arrayA[], int arrayB[]);

//It's a circular buffer
double card_push(int card, int *deck, int *len, int *out);

int card_pop(int *deck, int *len, int *out);

void card_print(int *deck, int *len, int *out);

int
symulation_simplified(int *arrayA, int *arrayB, int number_of_conflicts, int *lenA, int *outA, int *lenB, int *outB);

int symulation_normal(int *arrayA, int *arrayB, int number_of_conflicts, int *lenA, int *outA, int *lenB, int *outB);

int if_war(int a, int b);

int give_maxidx(int *len, int *out);

int give_idx(int i);

int conflicts;


int main(void) {
    start:;

    conflicts = 0;

    int to_do, number_of_possible_conflicts, seed;
    int arrayA[DECK_SIZE], arrayB[DECK_SIZE];
    int array[DECK_SIZE];

    int outA_ = 0;
    int *outA = &outA_;
    int lenA_ = 26;
    int *lenA = &lenA_;
    int outB_ = 0;
    int *outB = &outB_;
    int lenB_ = 26;
    int *lenB = &lenB_;

    int r;



    if (TEST) printf("Write seed, version (simplify or normal) and number of possible conflicts = ");
    scanf("%d %d %d", &seed, &to_do, &number_of_possible_conflicts);
    srand(seed);

    shuffling_cards(DECK_SIZE, array);
    // print_all_deck(array, DECK_SIZE);
    giving_cards(array, arrayA, arrayB);
//    print_all_deck(arrayA, DECK_SIZE);
//    print_all_deck(arrayB, DECK_SIZE);

    switch (to_do) {
        case 0:
            r = symulation_normal(arrayA, arrayB, number_of_possible_conflicts, lenA, outA, lenB, outB);

            printf("\nOutput: %d \n", r);
//          End of cards or number of possible conflicts has been achieved
            if (r == 0 || r == 1) {
                printf("Number of Cards (Player A) = %d, Number of Cards (PlayerB) =  %d\n", *lenA, *lenB);
                printf("Number of conflicts that occur =  %d", conflicts);
            }
//          Player A has won
            if (r == 2) {
                printf("Player A deck: \n");
                card_print(arrayA, lenA, outA);
                printf("Number of conflicts that occur =  %d", conflicts);
            }
//          Player B has won
            if (r == 3) {
                printf("Player B deck: \n");
                card_print(arrayB, lenB, outB);
                printf("Number of conflicts that occur =  %d", conflicts);
            }
            break;
        case 1:
            r = symulation_simplified(arrayA, arrayB, number_of_possible_conflicts, lenA, outA, lenB, outB);


            printf("\nOutput: %d \n", r);
//          End of cards or number of possible conflicts has been achieved
            if (r == 0 || r == 1) {
                printf("Number of Cards (Player A) = %d, Number of Cards (PlayerB) =  %d\n", *lenA, *lenB);
                printf("Number of conflicts that occur =  %d", conflicts);
            }
//          Player A has won
            if (r == 2) {
                printf("Player A deck: \n");
                card_print(arrayA, lenA, outA);
                printf("Number of conflicts that occur =  %d", conflicts);
            }
//          Player B has won
            if (r == 3) {
                printf("Player B deck: \n");
                card_print(arrayB, lenB, outB);
                printf("Number of conflicts that occur =  %d", conflicts);
            }
            break;
    }

    printf("\n\nPRESS 0 TO END OR PRESS SOMETHING ELSE TO DO IT AGAIN\n");
    int what_to_do;
    scanf("\n %d", &what_to_do);
    if(what_to_do == 0){
        return 0;
    }
    else{
        goto start;
    }

//    sleep(5);

}

int
symulation_simplified(int arrayA[], int arrayB[], int number_of_conflicts, int *lenA, int *outA, int *lenB, int *outB) {
    int a, b;

    while (1) {


        a = card_pop(arrayA, lenA, outA);
        b = card_pop(arrayB, lenB, outB);

        // printf("Player A: ");
        // print_all_deck(arrayA, DECK_SIZE);
        // printf("Player B: ");
        // print_all_deck(arrayB, DECK_SIZE);

        conflicts++;

        if (a == -1 || b == -1) {
            return 1;
        }

        if (if_war(a, b) == 1) {
            // printf("PASS \n");
            //     printf("Card A = %d\n", a);
            //     printf("Card B = %d\n", b);

            // printf("Player A card: %d\n\n", a);
            card_push(a, arrayA, lenA, outA);
            // print_all_deck(arrayA, DECK_SIZE);
            // printf("Player B card: %d\n\n", b);
            card_push(b, arrayB, lenB, outB);
            // printf("%d\n", conflicts);
            // printf("Player A out: %d\n\n", arrayA[*outA]);
            // print_all_deck(arrayA, DECK_SIZE);
            //   printf("Player B out: %d\n\n", arrayB[*outB]);
            //  print_all_deck(arrayB, DECK_SIZE);
        } else {
            if (a > b) {
                card_push(a, arrayA, lenA, outA);
                card_push(b, arrayA, lenA, outA);
            } else {
                card_push(b, arrayB, lenB, outB);
                card_push(a, arrayB, lenB, outB);
            }
        }

        number_of_conflicts--;

        if (number_of_conflicts == 0) {
            return 0;
        }


        if (*lenA == 52) { return 2; }
        if (*lenB == 52) { return 3; }

        // printf("%d\n", number_of_conflicts);


    }


}

int symulation_normal(int *arrayA, int *arrayB, int number_of_conflicts, int *lenA, int *outA, int *lenB, int *outB) {
//    int a = arrayA[*outA];
//    int b = arrayB[*outB];

    int counter;
    int i, k, a, b;
    int maxidxA, maxidxB;


    while (1) {
        counter = 2;
        i = *outA;
        k = *outB;
        a = arrayA[i];
        b = arrayB[k];
        maxidxA = give_maxidx(lenA, outA);
        maxidxB = give_maxidx(lenB, outB);
        conflicts += 1;

//        printf("Card A= %d\n", a);
//        printf("Card B= %d\n", b);
        // printf("Player A: \n");
        // print_all_deck(arrayA, DECK_SIZE);
        // printf("Player B: \n");
        // print_all_deck(arrayB, DECK_SIZE);
        // printf("\n");

        while (if_war(a, b)) {
            // printf("Card A= %d\n", a);
            // printf("Card B= %d\n", b);
            // printf("i = %d \n", i);
            // printf("k = %d \n", k);
            counter += 2;
            i = give_idx(i + 1);
//            printf("i = %d \n", i);
            k = give_idx(k + 1);
//            printf("k = %d \n", k);

            number_of_conflicts--;

            if (i == maxidxA || k == maxidxB) { return 1; }
            a = arrayA[i];
            b = arrayB[k];

            // printf("Card A= %d\n", a);
            // printf("Card B= %d\n", b);

        }
        if (counter != 2) {
//            printf("Before: \n");
//            printf("Player A: \n");
//            print_all_deck(arrayA, DECK_SIZE);
//            printf("Player B: \n");
//            print_all_deck(arrayB, DECK_SIZE);
//            Adding cards to player who won the war
            if (arrayA[i] > arrayB[k]) {
//                Adding cards played by player who won
                for (int m = 0; m != counter - 1; m++) {
                    a = card_pop(arrayA, lenA, outA);
                    card_push(a, arrayA, lenA, outA);
                }
//                Adding cards played by player who lose
                for (int m = 0; m != counter - 1; m++) {
                    b = card_pop(arrayB, lenB, outB);
                    card_push(b, arrayA, lenA, outA);
                }
            } else {
                for (int m = 0; m != counter - 1; m++) {
                    b = card_pop(arrayB, lenB, outB);
//                    printf("Giving card B= %d\n", b);
                    card_push(b, arrayB, lenB, outB);
                }
//                Adding cards played by player who lose
                for (int m = 0; m != counter - 1; m++) {
                    a = card_pop(arrayA, lenA, outA);
//                    printf("Giving card from A= %d\n", a);
                    card_push(a, arrayB, lenB, outB);
                }
            }
//            printf("After: \n");
//            printf("Player A: \n");
//            print_all_deck(arrayA, DECK_SIZE);
//            printf("Player B: \n");
//            print_all_deck(arrayB, DECK_SIZE);
//            printf("counter = %d\n", counter);
            conflicts += counter / 2 - 1;
        } else {
            if (a > b) {
                a = card_pop(arrayA, lenA, outA);
                b = card_pop(arrayB, lenB, outB);
                card_push(a, arrayA, lenA, outA);
                card_push(b, arrayA, lenA, outA);

//                printf("Giving card B= %d\n", b);
//                printf("Giving card from A= %d\n", a);
            } else {
                a = card_pop(arrayA, lenA, outA);
                b = card_pop(arrayB, lenB, outB);
                card_push(b, arrayB, lenB, outB);
                card_push(a, arrayB, lenB, outB);
//                printf("Giving card A= %d\n", b);
//                printf("Giving card from B= %d\n", a);
            }
        }

        number_of_conflicts--;

        if (*lenA == 52) { return 2; }
        if (*lenB == 52) { return 3; }

        if (number_of_conflicts == 0) {
            return 0;
        }


        // printf("Conflicts = %d \n", conflicts);

//        printf("Card A= %d\n", a);
//        printf("Card B= %d\n", b);

    }
}

int if_war(int a, int b) {
    // printf("Card A = %d\n", a);
    // printf("Card B = %d\n", b);

    if (a / 4 == b / 4) {
        return 1;
    }

    return 0;
}

int give_maxidx(int *len, int *out) {
    return (*len + *out) % 52;
}

int give_idx(int i) {
    if (i % 52 != 0) { i++; }
    else { i = 0; }
    return i;
}

double card_push(int card, int *deck, int *len, int *out) {
    int idx = give_maxidx(len, out);
    deck[idx] = card;
    *len += 1;

    return 0;
}

int card_pop(int *deck, int *len, int *out) {
    if (*len == 0) {
        return -1;
    }
    if (*out == 52) {
        *out = 0;
    }
//    printf("\n%d 'position'\n", cbuff[out]);
    // printf("OUT and LEN: %d %d\n", *out, *len);

    int to_return = deck[*out];
    deck[*out] = -1;
    *out += 1;
    *len -= 1;

    return to_return;
}

void card_print(int *deck, int *len, int *out) {
    // printf("out = %d \n", *out);
    // printf("len = %d \n", *len);
    if (*out != 0) {
        for (int i = *out; i < 52; i++) {
            printf("%d ", deck[i]);
        }
        for (int i = 0; i < *out; i++) {
            printf("%d ", deck[i]);
        }
    } else {
        for (int i = 0; i < *len; i++) {
            printf("%d ", deck[i]);
        }
    }
    printf("\n");
}


int rand_from_interval(int a, int b) {
    if (a > b) {
        return INT_MIN;
    }

    if (b - a > RAND_MAX) {
        return INT_MAX;
    }

    if (a == b) {
        return a;
    } else {
        int num = (rand() % (b - a + 1)) + a;
        return num;
    }

}

void shuffling_cards(int size, int array[]) {
    for (int i = 0; i < size; i++) {
        array[i] = i;
    }

    for (int i = 0; i < size - 1; i++) {
        int k = rand_from_interval(i, size - 1);
        int temp = array[i];
        array[i] = array[k];
        array[k] = temp;
    }
}


void giving_cards(const int *array, int *arrayA, int *arrayB) {
    for (int i = 0; i < DECK_SIZE / 2; i++) {
        arrayA[i] = array[i];
        arrayB[i] = array[i + DECK_SIZE / 2];
    }
    for (int i = DECK_SIZE / 2; i < DECK_SIZE; i++) {
        arrayA[i] = -1;
        arrayB[i] = -1;
    }
}

//This function has been written for debugging
void print_all_deck(const int array[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d, ", array[i]);
    }
    printf("FINISHED \n\n");
}