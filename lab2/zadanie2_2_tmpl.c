#include <stdio.h>
#include <math.h>

#define TEST    0       // 1 dla testowania, 0 dla oceny automatycznej

// Deklaracje (prototypy) funkcji:

double stack_push(double x);

double stack_pop(void);

int stack_state(void);

double queue_push(int how_many);

int queue_pop(int how_many);

int queue_state(void);

void queue_print(void);

double cbuff_push(int cli_nr);

int cbuff_pop(void);

int cbuff_state(void);

void cbuff_print(void);

///////////////////////

int main(void) {
    int to_do, i, n, ans;
    int nr_klienta, kod_operacji;
    double x, f_ans;
    if (TEST) printf("Nr testu = ");
    scanf("%d", &to_do);
    switch (to_do) {
        case 1:
            if (TEST)
                printf("sekwencja operacji (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan stosu i konczy sekwencje operacji
            do {
                scanf("%lf", &x);
                if (x > 0) {
                    f_ans = stack_push(x);
                    if (f_ans == INFINITY) printf("%f ", INFINITY);
                } else {
                    if (x < 0) printf("%.2f ", stack_pop());
                    else printf("\n%d\n", stack_state());
                }
            } while (x != 0.0);
            break;
        case 2:
//  kolejka kolejnych liczb naturlnych > 0 (naplywajacych grupowo) - implementacja w tablicy z przesunieciami
            if (TEST)
                printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
            do {
                scanf("%d", &n);
                if (n > 0) {
                    f_ans = queue_push(n);
                    if (f_ans == INFINITY) printf("%f ", INFINITY);
                } else {
                    if (n < 0) {
                        ans = queue_pop(-n);
                        if (ans < 0) printf("-1 ");
                    } else {
                        printf("\n%d\n", queue_state());
                        queue_print();
                    }
                }
            } while (n != 0);
            break;
        case 3:
//  kolejka kolejnych liczb naturalnych > 0 (naplywajacych pojedynczo - implementacja w buforze cyklicznym
            if (TEST)
                printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
            nr_klienta = 0;
            do {
                scanf("%d", &kod_operacji);
                if (kod_operacji > 0) {
                    f_ans = cbuff_push(++nr_klienta);
                    if (f_ans == INFINITY) printf("%f ", INFINITY);
                } else {
                    if (kod_operacji < 0) {
                        printf("%d ", cbuff_pop());
//                    ans = cbuff_pop();
//                    if(ans < 0) printf("%f ",NAN);
//                    else        printf("%d ",ans);
                    } else {
                        printf("\n%d\n", cbuff_state());
                        cbuff_print();
                    }
                }
            } while (kod_operacji != 0);
            break;
        default:
            if (TEST) printf("NOTHING TO DO!\n");
    }
    return 0;
}

///////////////////////////////////////////////////////

// Definicje funkcji i zmiennych o zasięgu plikowym (file scope):

// Stos

#define STACK_SIZE  10

double stack[STACK_SIZE];
int top = 0;   // jawne zerowanie nie jest konieczne

double stack_push(double x) {
    if (top == 10) {
        return INFINITY;
    }
    stack[top] = x;
    // printf("%.2f ", stack[top]);
    top++;
    return 0;
}

double stack_pop(void) {
    if (top == 0) {
        return NAN;
    }

    double taken;
    taken = stack[top - 1];
    top--;
    return taken;
}

int stack_state(void) {
    return top;
}

//Kolejka z przesuwaniem w tablicy

#define QUEUE_SIZE  10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0;  // 1. klient dostanie nr = 1

double queue_push(int how_many) {
    if (how_many + in > QUEUE_SIZE) {

//        printf("in = %d \n", in);
        for (int i = in; i < 10; i++) {
            ++curr_nr;
            queue[i] = curr_nr;
//            printf("%d'inf'\n", queue[i]);
        }

        curr_nr += how_many-(10-in);
//        printf("%d'after inf'\n", curr_nr);
        in = 10;
        return INFINITY;
    }


    for (int i = in; i < in + how_many; i++) {
        ++curr_nr;
        queue[i] = curr_nr;
//        printf("%d\n", queue[i]);
    }
    in += how_many;

    return 0;
}

int queue_pop(int how_many) {
    if (how_many > in) {
        for (int i = 0; i < in; i++) {
            queue[i] = 0;
        }
        in = 0;
        return -1;
    }

    for (int i = 0; i < in; i++) {
        queue[i] = queue[how_many + i];
    }

    in -= how_many;

    return in;
}

int queue_state(void) {
    return in;
}

void queue_print(void) {
    for (int i = 0; i < in; i++) {
//        printf("%d 'queue[%d]'\n", queue[i], i);
        printf("%d ", queue[i]);
    }
//    printf("\n%d", curr_nr);
}

//Kolejka w buforze cyklicznym

#define CBUFF_SIZE  10

int cbuff[CBUFF_SIZE];
int out = 0, len = 0;

double cbuff_push(int cli_nr) {
    if (len > 9) {
        return INFINITY;
    }
    if (len == 0) {
        out = 0;
    }
    int idx = (len + out) % 10;
    cbuff[idx] = cli_nr;
    len++;
//    printf("\nlen= %d", len);

//    printf("%d", cli_nr);

    return 0;
}

int cbuff_pop(void) {
    if (len == 0) {
        return -1;
    }
//    printf("\n%d 'position'\n", cbuff[out]);
    int to_return = cbuff[out];
    cbuff[out] = 0;
    out++;
    len--;

    return to_return;
}

int cbuff_state(void) {
    return len;
}

void cbuff_print(void) {
    printf("out = %d \n", out);
    printf("len = %d \n", len);
    if (out != 0) {
        int max;
        if (out+len >= 10){
            max = 10;
        }
        else{
            max = len+out;
        }
        for (int i = out; i < max; i++) {
            printf("%d ", cbuff[i]);
        }
        if(len-out>0){
            for (int i = 0; i < out; i++) {
                printf("%d ", cbuff[i]);
            }
        }
    }
    else {

        for (int i = 0; i < len; i++){
            printf("%d ", cbuff[i]);
        }
    }
}

