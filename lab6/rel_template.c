#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
    int first;
    int second;
} pair;

// Add pair to existing relation if not already there
int add_relation(pair *, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *relation, int n) {
    int flag1 = 1, flag2 = 0;


    int elem[n];
    for (int i = 0; i < n; i++) {
        elem[i] = -1;
    }

    int in = 0;

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < in; k++) {
            if (relation[i].first == elem[k]) {
                flag2 = 1;
                flag1 = 1;
                break;
            }
        }
        if (flag2 == 0) {
            elem[in] = relation[i].first;
            in++;
            for (int k = 0; k < n; k++) {
                if ((elem[in - 1] == relation[k].first) && (elem[in - 1]) == relation[k].second) {
                    flag1 = 0;
                    break;
                }
            }

            if (flag1) return 0;
        }
        for (int k = 0; k < in; k++) {
            if (relation[i].second == elem[k]) {
                flag2 = 1;
                flag1 = 1;
                break;
            }
        }
        if (flag2 == 0) {
            elem[in] = relation[i].second;
            in++;
            for (int k = 0; k < n; k++) {
                if ((elem[in - 1] == relation[k].first) && (elem[in - 1]) == relation[k].second) {
                    flag1 = 0;
                    break;
                }
            }

            if (flag1) return 0;
        }
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *relation, int n) {
    int flag = 0;

    for (int i = 0; i < n; i++) {
        if (relation[i].first == relation[i].second) return 0;
        for (int k = n - 1; k >= 0; k--) {
            if ((relation[i].first == relation[k].second) && (relation[i].second == relation[k].first)) flag = 1;
        }
        if (flag) return 0;
        flag = 0;
    }
    return 1;

}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *relation, int n) {
    int flag = 1;

    for (int i = 0; i < n; i++) {
        if (relation[i].first != relation[i].second) {
            for (int k = n - 1; k >= 0; k--) {
                if ((relation[i].first == relation[k].second) && (relation[i].second == relation[k].first)) {
                    flag = 0;
                    break;
                }
            }
        }
        if (flag) return 0;
        flag = 1;
    }

    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *relation, int n) {
    int flag = 0;

    for (int i = 0; i < n; i++) {
        if (relation[i].first != relation[i].second) {
            for (int k = n - 1; k >= 0; k--) {
                if ((relation[i].first == relation[k].second) && (relation[i].second == relation[k].first)) {
                    flag = 1;
                    break;
                }
            }
        }
        if (flag) return 0;
    }

    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *relation, int n) {
    int flag1 = 1, flag2 = 0;

    int elem[n];
    for (int i = 0; i < n; i++) {
        elem[i] = -1;
    }

    int in = 0;

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < in; k++) {
            if (relation[i].first == elem[k]) {
                flag2 = 1;
                break;
            }
        }
        if (flag2 == 0) {
            elem[in] = relation[i].first;
            in++;
        }

        flag2 = 0;

        for (int k = 0; k < in; k++) {
            if (relation[i].second == elem[k]) {
                flag2 = 1;
                break;
            }
        }
        if (flag2 == 0) {
            elem[in] = relation[i].second;
            in++;
        }

        flag2 = 0;
    }
//    printf("\n");
//    for (int i = 0; i < in; i++) {
//        printf("X = %d, ", elem[i]);
//    }
//    printf("\n");

    for (int i = 0; i < in; i++) {

        for (int j = 0; j < n; j++) {
            if ((elem[i] == relation[j].first) && (elem[i] == relation[j].second)) {
                flag1 = 0;
//                printf("elem[i] = %d, \n", elem[i]);
                break;
            }
        }
        if (flag1) {
//            printf("Here\n");
//            printf("elem[i] = %d, \n", elem[i]);
            return 1;
        }

        flag1 = 1;

        for (int k = i + 1; k < in; k++) {
            for (int j = 0; j < n; j++) {
                if (((elem[i] == relation[j].first) && (elem[k] == relation[j].second)) ||
                    ((elem[i] == relation[j].second) && (elem[k] == relation[j].first))) {
//                    printf("elem[i] = %d, elem[k] = %d, relation[j].first = %d, relation[j].second == %d\n", elem[i],
//                           elem[k], relation[j].first, relation[j].second);
                    flag1 = 0;
                    break;
                }
            }
            if (flag1) {
//                printf("Down Here");
                return 1;
            }
            flag1 = 1;
        }

    }
    return 0;
}


// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *relation, int n) {
    int flag = 1;

    for (int i = 0; i < n; i++) {
        if (relation[i].first != relation[i].second) {
            for (int k = 0; k < n; k++) {
                if (relation[i].second == relation[i].first) {
                    for (int j = 0; k < n; k++) {
                        if ((relation[i].first == relation[j].first) && (relation[k].second == relation[j].second)) {
                            flag = 0;
                            break;
                        }
                    }
                    if (flag) return 0;
                }
            }
        }
    }
    return 1;
}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *relation, int n) {
    int a, b, c;

    a = is_reflexive(relation, n);
    b = is_antisymmetric(relation, n);
    c = is_transitive(relation, n);

    if (a == 1 && b == 1 && c == 1) return 1;
    return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *relation, int n) {
    int b;
    b = is_asymmetric(relation, n);

    if (b == 0) return 1;
    return 0;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair *relation, int n) {
    int a, b;
    a = is_partial_order(relation, n);
    b = is_connected(relation, n);

    if (a == 1 && b == 1) return 1;
    return 0;
}

int get_domain(pair *relation, int n, int *elem);

int find_max_elements(pair *relation, int n, int *max_elements) {
    int elem[n], flag = 1, in = 0;

    int size = get_domain(relation, n, elem);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < n; j++) {
            if (elem[i] != relation[j].second) {
//                printf("elem[i] = %d, relation.first = %d\n", elem[i], relation[j].first);
                if (elem[i] == relation[j].first) {
                    flag = 0;
                    break;
                }
            }
        }
        if (flag) {
            max_elements[in] = elem[i];
            in++;
        }
        flag = 1;
    }
    return in;
}


int find_min_elements(pair *relation, int n, int *min_elements) {
    int elem[n], flag = 1, in = 0;

    int size = get_domain(relation, n, elem);


    for (int i = 0; i < size; i++) {
        for (int j = 0; j < n; j++) {
            if (elem[i] != relation[j].first) {
                if (elem[i] == relation[j].second) flag = 0;
            }
        }
        if (flag) {
            min_elements[in] = elem[i];
            in++;
        }
        flag = 1;
    }
    return in;
}

int get_domain(pair *relation, int n, int *elem) {
    int flag2 = 0;

    for (int i = 0; i < n; i++) {
        elem[i] = -1;
    }

    int in = 0;

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < in; k++) {
            if (relation[i].first == elem[k]) {
                flag2 = 1;
                break;
            }
        }
        if (flag2 == 0) {
            elem[in] = relation[i].first;
            in++;
        }

        flag2 = 0;

        for (int k = 0; k < in; k++) {
            if (relation[i].second == elem[k]) {
                flag2 = 1;
                break;
            }
        }
        if (flag2 == 0) {
            elem[in] = relation[i].second;
            in++;
        }

        flag2 = 0;
    }

    return in;
}

// // Case 3:

int composition(pair *relation, int n, pair *relation2, int m, pair *comp_relation) {
    int flag = 0, ctr = 0;

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            if (relation[i].second == relation2[k].first) {
                comp_relation[ctr].first = relation[i].first;
                comp_relation[ctr].second = relation2[k].second;
                ctr++;
//                break;
            }
        }
    }
    return ctr;
}

// Comparator for pair
int cmp_pair(const void *a, const void *b) {
}

int insert_int(int *tab, int n, int new_element) {
}

// Add pair to existing relation if not already there
int add_relation(pair *tab, int n, pair new_pair) {
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n, a, b;

    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d %d", &relation[i].first, &relation[i].second);
        // printf("First = %d, Second = %d\n", relation[i].first, relation[i].second);
    }

    return n;
}


void print_int_array(int *array, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (array[i] < array[j]) {
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
    printf("%d\n", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];

    scanf("%d", &to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);
            printf("%d %d\n", ordered, is_total_order(relation, size));
            print_int_array(domain, n_domain);
            if (!ordered) break;
            int no_max_elements = find_max_elements(relation, size, max_elements);
            int no_min_elements = find_min_elements(relation, size, min_elements);
            print_int_array(max_elements, no_max_elements);
            print_int_array(min_elements, no_min_elements);
            break;
        case 3:
            size_2 = read_relation(relation_2);
            printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
