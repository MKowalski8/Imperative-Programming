#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define N  100

#define TEST  1  // 1 - dla testowania
                 // 0 - dla oceny automatycznej


//// Losuje jedna liczbe calkowita z przedzialu [a,b] stosujac funkcje rand() i operator %
//// Jezeli a>b to zwraca INT_MIN
//// Jezeli b-a > RAND_MAX to zwraca INT_MAX
//// Jezeli a=b to zwraca a (bez wywolywania funkcji rand)
//// Else zwraca liczbe z przedzialu [a,b]
int rand_from_interval(int a, int b) {
    if(a > b){
      return INT_MIN;
    }

    if(b-a > RAND_MAX){
        return INT_MAX;
    }

    if(a==b){
        return a;
    }
    else{
        int num = (rand() % (b - a + 1)) + a;
        return num;
    }

}

//// Losowa permutacja elementow zbioru liczb {0, 1, 2,..., n-1} (z rozkladem rownomiernym)
//// wg algorytmu przedstawionego w opisie zadania
//// jest zapisywana w tablicy tab.
//// 0 < n <= 100, jezeli nie to elementy tablicy tab maja wartosci nieokreslone.
void rand_permutation(int n, int tab[]) {
    for(int i = 0; i < n; i++){
        tab[i] = i;
    }

    for(int i = 0; i < n-1; i++){
        int k = rand_from_interval(i,n-1);
        int temp = tab[i];
        tab[i] = tab[k];
        tab[k] = temp;
    }
}

//// Metoda babelkowa sortowania n elementow tablicy tab w porzadku od wartosci najmniejszej do najwiekszej.
//// Zwraca numer iteracji petli zewnetrznej (liczony od 1), po ktorej tablica byla uporzadkowana,
//// np. dla { 0 1 2 3 7 4 5 6 } -> 1,
////     dla { 1 2 3 7 4 5 6 0 } -> 7,
////     dla { 0 1 2 3 4 5 6 7 } -> 0.
int bubble_sort(int n,int tab[]) {
    int temp;
    int counter = 0;
    int flag = 0;

    for (int i = 0; i < n - 1; i++) {
        for (int k = 0; k < n - i - 1; k++) {
            if (tab[k] > tab[k + 1]) {
                temp = tab[k];
                tab[k] = tab[k + 1];
                tab[k + 1] = temp;
                flag = 1;
            }
        }
        if(flag == 1){
            counter++;
        }
        flag = 0;
    }

////    for (int i = 0; i < n; i++) { printf("%d, ", tab[i]); }
////    printf("\n");
    return counter;
}



int main(void) {
    int nr_testu, seed;
    int a,b,n,i;
    int tab[N];
    if(TEST) printf("Wpisz nr testu i seed = ");
    scanf("%d %d",&nr_testu, &seed);
    srand(seed);        // ustawienie ziarna generatora (dla powterzalnosci wynikow)
    switch(nr_testu) {
case 1:
        if(TEST) printf("a b = ");
        scanf("%d %d",&a, &b);
        for(int i=0; i<3; ++i) printf("%d ",rand_from_interval(a, b));
        break;
case 2:
        if(TEST) printf("n = ");
        scanf("%d",&n);
        rand_permutation(n,tab);
        for(i=0; i<n; ++i) printf("%d ",tab[i]);
        printf("\n");
        break;
case 3:
        if(TEST) printf("n = ");
        scanf("%d",&n);
        rand_permutation(n,tab);
        printf("%d\n",bubble_sort(n,tab));
//        int tab1[8] = {0, 1, 2, 3, 7, 4, 5, 6};
//        int tab1[8] = {1, 2, 3, 7, 4, 5, 6, 0};
//        0 2 3 7 4 5 6 1
//        0 1 3 7 4 5 6 2
//        0 1 2 7 4 5 6 3
//        0 1 2 4 5 6 3 7
//        0 1 2
//        int tab1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
//        printf("%d", bubble_sort(8, tab1));
        break;
default:
        if(TEST) printf("NOTHING TO DO!\n");
    }
    return 0;
}


