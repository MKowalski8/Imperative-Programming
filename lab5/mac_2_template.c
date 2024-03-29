#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 40

#define TEST 0    // 1 - dla testow,  0 - dla oceny automatycznej

void read_vec(double x[], size_t n) {
    for (size_t i = 0; i < n; ++i) scanf("%lf", x++);
}

void print_vec(const double x[], size_t n) {
    for (size_t i = 0; i < n; ++i) printf("%.4f ", x[i]);
    printf("\n");
}

void read_mat(double A[][SIZE], size_t m, size_t n) {
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) scanf("%lf", &A[i][j]);
    }
}

void print_mat(const double A[][SIZE], size_t m, size_t n) {
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) printf("%.4f ", A[i][j]);
        printf("\n");
    }
}

void mac_vec_product(const double A[][SIZE], const int index[SIZE], size_t n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) { printf("%.4f ", A[index[i]][j]); }
        printf(" index = %d\n", index[i]);
    }
}

void mac_vec_product_b(const double B[SIZE], const int index[SIZE], size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%.4f ", B[index[i]]);
        printf(" index = %d\n", index[i]);
    }
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

// 4.383 -4.114 -2.223 1.915 2.793 3.335 0.386 -4.508 1.649 -3.579 -2.638 -4.973 3.690 -4.9412.763 -2.638 -4.973 3.690 -4.941 2.000 0.211 0.368 -2.433 1.429 0.782

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
    int index[n], max, temp_k, temp, changes = 0;
    double a, det = 1;


    for (int i = 0; i < n; i++) {
        index[i] = i;
        x[i] = b[i];
    }
//  mac_vec_product_b(x, index, n);



    for (int i = 0; i < n - 1; i++) {
        max = index[i];
        // index[i] = i;
        for (int k = i; k < n; k++) {
            if (abs(A[index[k]][i]) > abs(A[max][i])) {
                max = index[k];
                temp_k = k;
            }
        }

        if (abs(A[max][i]) < eps) { return 0; }

        if (max != index[i]) {
            temp = index[i];
            index[i] = max;
            index[temp_k] = temp;
            changes += 1;
        }


        for (int l = i + 1; l < n; l++) {
            a = A[index[l]][i] / A[index[i]][i];
            if (a != 0) {
                for (int k = i; k < n; k++) {
                    A[index[l]][k] -= A[index[i]][k] * a;
                }
                x[index[l]] -= x[index[i]] * a;
//         mac_vec_product_b(x, index, n);
//         printf("\n\n");
            }
        }
//    mac_vec_product(A, index, n);

    }


    for (size_t i = 0; i < n; i++) {
        det *= A[index[i]][i];
    }

    if (changes % 2 != 0) det *= -1;

    double variable;

    for (int i = n - 1; i >= 0; i--) {
        a = 1 / A[index[i]][i];
        A[index[i]][i] *= a;
        x[index[i]] *= a;
        for (int l = i - 1; l >= 0; l--) {
            variable = A[index[l]][i];
            x[index[l]] -= x[index[i]] * variable;
            A[index[l]][i] = 0;

        }
    }
//    mac_vec_product_b(x, index, n);
//    printf("\n\n");
//
    double temp_tab[SIZE];

    for (int i = 0; i < n; i++) {
        temp_tab[i] = x[index[i]];
    }
    for (int i = 0; i < n; i++) {
        x[i] = temp_tab[i];
    }

//    mac_vec_product_b(x, index, n);

//    printf("\n\n");
//  mac_vec_product(A, index, n);
    return det;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
    int index[n], max, temp_k, temp, changes = 0;
    double a, det = 1;

//DET
    for (int i = 0; i < n; i++) {
        index[i] = i;
        B[i][i] = 1;
    }

    for (int i = 0; i < n - 1; i++) {
        max = index[i];
        for (int k = i; k < n; k++) {
            if (abs(A[index[k]][i]) > abs(A[max][i])) {
                max = index[k];
                temp_k = k;
            }
        }

        if (abs(A[max][i]) < eps) { return 0; }

        if (max != index[i]) {
            temp = index[i];
            index[i] = max;
            index[temp_k] = temp;
            changes += 1;
        }


        for (int l = i + 1; l < n; l++) {
            a = A[index[l]][i] / A[index[i]][i];
            if (a != 0) {
                for (int k = i; k < n; k++) { A[index[l]][k] -= A[index[i]][k] * a; }
                for (int k = 0; k < n; k++) { B[index[l]][k] -= B[index[i]][k] * a; }
            }
        }
//
    }
//    printf("\n B = \n");
//    mac_vec_product(B, index, n);
//    printf("\n A = \n");
//    mac_vec_product(A, index, n);
    for (size_t i = 0; i < n; i++) {
        det *= A[index[i]][i];
    }

    if (changes % 2 != 0) det *= -1;

//ODWRACANIE

    double variable;
    for (int i = n - 1; i >= 0; i--) {
//        printf("\n B = \n");
//        mac_vec_product(B, index, n);
//        printf("\n A = \n");
//        mac_vec_product(A, index, n);

        a = 1 / A[index[i]][i];
        A[index[i]][i] *= a;
        for (int m = 0; m < n; m++) { B[index[i]][m] *= a; }
        for (int l = i - 1; l >= 0; l--) {
            if (A[index[l]][i] != 0) {
                for (int k = 0; k < n; k++) {
                    variable = B[index[i]][k] * A[index[l]][i];
                    B[index[l]][k] -= variable;

//                printf("\na = %f B = \n", a);
//                mac_vec_product(B, index, n);
                }
                A[index[l]][i] = 0;
            }
        }

    }
//    printf("\n B = \n");
//    mac_vec_product(B, index, n);
//    printf("\n A = \n");
//    mac_vec_product(A, index, n);
    double B_temp[n][n];

    for (int i = 0; i < n; i++) {
        if (index[i] != i) {
            for (int j = 0; j < n; j++) {
                B_temp[i][j] = B[index[i]][j];
            }
        }

    }
    for (int i = 0; i < n; i++) {
        if (index[i] != i) {
            for (int j = 0; j < n; j++) {
                B[i][j] = B_temp[i][j];
            }
        }
    }

//    mac_vec_product(B, index, n);
    return det;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;
    int to_do;
    size_t m, n, p, q;
    if (TEST) printf("Wpisz nr zadania ");
    scanf("%d", &to_do);
    switch (to_do) {
        case 1:
            if (TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
            scanf("%d", &n);
            if (TEST) printf("Wpisz macierz A (wierszami): ");
            read_mat(A, n, n);
            if (TEST) printf("Wpisz wektor b: ");
            read_vec(b, n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n", det);
            if (det) print_vec(x, n);
            break;
        case 2:
            if (TEST) printf("Wpisz rozmiar macierzy n = ");
            scanf("%d", &n);
            if (TEST) printf("Wpisz elementy macierzy (wierszami): ");
            read_mat(A, n, n);
            det = matrix_inv(A, B, n, eps);
            printf("%.4f\n", det);
            if (det) print_mat(B, n, n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
