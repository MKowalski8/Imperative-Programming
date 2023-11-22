#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEST 0

typedef struct {
    int day;
    int month;
    int year;
} Date;

/////////////////////////////////////////////////////////////////
// 8.1 funkcja bsearch2

#define FOOD_MAX  30   // max. liczba artykułów
#define ART_MAX   15   // max. długość nazwy artykułu +1
#define RECORD_MAX 40  // max. długość rekordu (linii) w pliku

typedef struct {
    char art[ART_MAX];
    float price;
    float amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

int compare(const void *a, const void *b) {
    const Food *food_a = (Food *) a;
    const Food *food_b = (Food *) b;

    int str_cmp;

    if (strcmp(food_a->art, food_b->art) == 0) str_cmp = 0;
    else {
        for (int i = 0; i < ART_MAX; i++) {
//            printf("%d, %d\n", (int)food_a -> art[i], (int)food_b -> art[i]);
            if ((int) food_a->art[i] > (int) food_b->art[i]) str_cmp = 2;
            else if ((int) food_a->art[i] < (int) food_b->art[i]) str_cmp = 1;
            if (str_cmp != 0) break;
        }
    }

//    printf("\nfood_key = %s, food_arr = %s, compare = %d\n", food_a->art, food_b->art, str_cmp);

    if (str_cmp == 0 && food_a->price == food_b->price &&
        food_a->valid_date.day == food_b->valid_date.day
        && food_a->valid_date.month == food_b->valid_date.month && food_a->valid_date.year == food_b->valid_date.year)
        return 0;

    if (str_cmp == 0) {
        if (food_a->price == food_b->price) {
            if (food_a->valid_date.year == food_b->valid_date.year) {
                if (food_a->valid_date.month == food_b->valid_date.month) {
                    if (food_a->valid_date.day < food_b->valid_date.day) return 1;
                    return 2;
                } else {
                    if (food_a->valid_date.month < food_b->valid_date.month) return 1;
                    return 2;
                }
            } else {
                if (food_a->valid_date.year < food_b->valid_date.year) return 1;
                return 2;
            }
        } else {
            if (food_a->price < food_b->price) return 1;
            return 2;
        }
    } else {
        if (str_cmp == 1) return 1;
        return 2;
    }
}


void *bsearch2(const void *key, void *base, size_t nitems, size_t size, ComparFp compare, char *result) {
    int res;
    int l = 0, r = nitems - 1, m;
    Food *ptr_arr = (Food *) base;
    const Food *ptr_key = (Food *) key;

//    printf("\nart = %s \n", ptr_key -> art);

    Food *temp_ptr = ptr_arr;

//    if (ptr_key -> art[0] == 'm'){
//        flag += 1;
//       if (flag == 2) printf("START");
//    }

    while (l <= r) {
        if ((l + r) % 2 != 0) m = (l + r) / 2 + 1;
        else m = (l + r) / 2;
//        m = (r-l)/2 + l;
        ptr_arr = temp_ptr;
        ptr_arr += m;
//        printf("last_name = %s, ", last_name);
//        printf("%s \n", ptr_arr -> art);
        res = compare(ptr_key, ptr_arr);

        if (res == 0) {
            *result = 1;
            return ptr_arr;
        } else if (res == 2) {
            l = m + 1;
            ptr_arr = temp_ptr + l;

        } else {
            r = m - 1;
        }
    }
//    printf("DONE\n");
    return ptr_arr;
}

//This is function written for debugging
void print_tab(Food *tab, int number) {
    printf("\nSTART\n");
    for (int i = 0; i < number; i++) {
        printf("%s %.2f %.2f %d %d %d\n", tab[i].art, tab[i].price, tab[i].amount, tab[i].valid_date.day,
               tab[i].valid_date.month, tab[i].valid_date.year);
    }
    printf("END\n");

}

void print_art(Food *p, size_t n, char *art) {
//     int number = n;
//     print_tab(p, number);
    while (strcmp(p->art, art) != 0) p += 1;

    while (strcmp(p->art, art) == 0) {
        printf("%.2f %.2f %d %d %d\n", p->price, p->amount, p->valid_date.day,
               p->valid_date.month, p->valid_date.year);
        p += 1;
    }
}


Food *add_record(Food *tab, size_t tab_size, int *np, ComparFp compare, Food *new_food) {
    char result = 0;
    size_t nitems = *np;
    Food temp;
//    printf("%s\n", new_food -> art);
//    printf("\nBEFORE: ");
//    print_tab(tab, *np);
    Food *adr = bsearch2(new_food, tab, nitems, tab_size, compare, &result);
    if (result == 0) {
        if (tab_size != *np) {
            for (int i = *np; i > 0; i--) {
                tab[i] = tab[i - 1];
                if (adr == &tab[i]) break;
            }
            *adr = *new_food;

        }
        *np += 1;

    } else adr->amount += new_food->amount;
////    printf("\nAFTER: ");
//    print_tab(tab, *np);
//    printf("*np = %d\n", *np);
    return 0;
}

int read_stream(Food *tab, size_t size, int no, FILE *stream) {
    Food new_food;
    int np = 2;

    fscanf(stream, "%s %f %f %d %d %d", &new_food.art, &new_food.price, &new_food.amount,
           &new_food.valid_date.day, &new_food.valid_date.month, &new_food.valid_date.year);

    tab[0] = new_food;

    if (no == 1) return 1;

    fscanf(stream, "%s %f %f %d %d %d", &new_food.art, &new_food.price, &new_food.amount,
           &new_food.valid_date.day, &new_food.valid_date.month, &new_food.valid_date.year);

    tab[1] = new_food;

    for (int i = 0; i < no - 2; i++) {
        fscanf(stream, "%s %f %f %d %d %d", &new_food.art, &new_food.price, &new_food.amount,
               &new_food.valid_date.day, &new_food.valid_date.month, &new_food.valid_date.year);
//         printf("\n%s\n", new_food.art);
//         printf("%f ", new_food.price);
//         printf("%f ", new_food.amount);
//         printf("%d\n\n", new_food.valid_date.day);

        add_record(tab, size, &np, compare, &new_food);
    }

    return np;
}

int compare2(const void *a, const void *b) {
    const Food *food_a = (Food *) a;
    const Food *food_b = (Food *) b;

    if (food_a->valid_date.year == food_b->valid_date.year) {
        if (food_a->valid_date.month == food_b->valid_date.month) {
            if (food_a->valid_date.day < food_b->valid_date.day) return -1;
            return 1;
        } else {
            if (food_a->valid_date.month < food_b->valid_date.month) return -1;
            return 1;
        }
    } else {
        if (food_a->valid_date.year < food_b->valid_date.year) return -1;
        return 1;
    }
}


int compare3(Date date_a, const void *b) {
    const Food *date_b = (Food *) b;

    if (date_a.year == date_b->valid_date.year && date_a.month == date_b->valid_date.month &&
        date_a.day == date_b->valid_date.day) {
//        printf("WYNIK = %d %d %d %d\n" ,date_b -> valid_date.day, date_a.day,  date_b -> valid_date.month, date_b ->valid_date.year);
        return 0;
    }


    if (date_a.year == date_b->valid_date.year) {
        if (date_a.month == date_b->valid_date.month) {
            if (date_a.day > date_b->valid_date.day) return -1;
            return 1;
        } else {
            if (date_a.month > date_b->valid_date.month) return -1;
            return 1;
        }
    } else {
        if (date_a.year > date_b->valid_date.year) return -1;
        return 1;
    }
}

void *bsearch_my(Date curr_date, void *base, size_t elements) {
    int res;
    int l = 0, r = elements - 1, m;
    Food *ptr_arr = (Food *) base;
    Food *temp_ptr = ptr_arr;
    while (l <= r) {
        if ((l + r) % 2 != 0) m = (l + r) / 2 + 1;
        else m = (l + r) / 2;
        ptr_arr = temp_ptr;
        ptr_arr += m;
        res = compare3(curr_date, ptr_arr);

        if (res == 0) {
            return ptr_arr;
        } else if (res == -1) {
            l = m + 1;
            ptr_arr = temp_ptr + l;

        } else {
            r = m - 1;
        }
    }
    return ptr_arr;
}

Date add_date(Date date, int anticip) {
    if (date.month == 1 || date.month == 3 || date.month == 5 || date.month == 7 || date.month == 8 ||
        date.month == 10) {
        int new_day = date.day + anticip;
        if (new_day > 31) {
            int to_add = new_day - 31;
            date.day = to_add;
            date.month += 1;
        } else date.day = new_day;
    } else if (date.month == 2 || date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11) {
        int new_day = date.day + anticip;
        if (new_day > 30) {
            int to_add = new_day - 30;
            date.day = to_add;
            date.month += 1;
        } else date.day = new_day;
    } else {
        int new_day = date.day + anticip;
        if (new_day > 31) {
            int to_add = new_day - 31;
            date.day = to_add;
            date.month = 1;
            date.year += 1;
        } else date.day = new_day;
    }

    return date;
}

int read_stream0(Food *tab, size_t size, int no, FILE *stream) {
    int np = read_stream(tab, size, no, stream);
    qsort(tab, np, sizeof(Food), compare2);


    return np;

}

float value(Food *food_tab, size_t n, Date curr_date, int anticip) {
//    print_tab(food_tab, n);


    curr_date = add_date(curr_date, anticip);
//    printf("day = %d\n", curr_date.day);

    float value = 0;
    Food *adr = bsearch_my(curr_date, food_tab, n);

//    printf("%s %f \n", adr->art, adr->amount);

    value += adr->amount * adr->price;
//    printf("\nValue = %f ", value);

    Food *temp_adr = adr;

    for (int i = 1; i < n; i++) {
        adr += 1;
        if (adr->valid_date.day == curr_date.day && adr->valid_date.month == curr_date.month &&
            adr->valid_date.year == curr_date.year)
            value += adr->amount * adr->price;
        else break;
//        printf("%s ", adr -> art);

    }

    adr = temp_adr;
//    printf("\nValue = %f ", value);
//    printf("\n");
    for (int i = 1; i < n; i++) {
        adr -= 1;
        if (adr->valid_date.day == curr_date.day && adr->valid_date.month == curr_date.month &&
            adr->valid_date.year == curr_date.year)
            value += adr->amount * adr->price;
        else break;
//        printf("%s ", adr -> art);

    }

    return value;
}

/////////////////////////////////////////////////////////////////
// 8.3 Sukcesja

#define CHILD_MAX  20   // maksymalna liczba przesuwanej w tablicy grupy osób (elementów tablicy)

enum Sex {
    F, M
};
enum BOOL {
    no, yes
};

struct Bit_data {
    enum Sex sex: 1;
    enum BOOL pretendent: 1;   // =no - nie pretenduje (panuje albo nie żyje) ale jest w zbiorze dla spójności relacji.
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *par_name;
    int index;
    int children;
} Parent;    // strukturę można rozbudować o pole liczby dzieci

const Date primo_date = {28, 10, 2011}; // Data zmiany kolejności sukcesji

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {

}

void persons_shiftings(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab, int n) {
//    printf("Hello\n");

    for (int i = 0; i < n; i++) {
        if (person_tab[i].bits.pretendent == no) {
            for (int j = i; j < n; j++) {
                person_tab[j] = person_tab[j + 1];
            }
            n--;
            i--;
        }
    }
    return n;
}

void print_person(const Person *p) {
    printf("%s\n", p->name);
}

void print_persons(const Person *person_tab, int n) {
    for (int i = 1; i <= n; ++i, ++person_tab) printf("%2d %12s %s\n", i, person_tab->name, person_tab->parent);
}

int compare4(const void *a, const void *b) {
    const Person *person_a = (Person *) a;
    const Person *person_b = (Person *) b;


    // osoby o tym samym rodzicu powinny być blisko siebie

    if (person_a->parent == NULL) return -1;
    if (person_b->parent == NULL) return 1;


//    printf("%s, %s, cos = %d \n", person_a->parent, person_b->parent, strcmp(person_a->parent, person_b->parent));

    if (strcmp(person_a->parent, person_b->parent) == 0){
        if (person_a -> born.year < 2011 && person_b -> born.year < 2011){
            if (person_a -> bits.sex == F && person_b -> bits.sex == M) return -1;
            else if (person_a -> bits.sex == M && person_b -> bits.sex == F) return 1;
        }
        if (person_a->born.year == person_b->born.year) {
            if (person_a->born.month == person_b->born.month) {
                if (person_a->born.day < person_b->born.day) return 1;
                return -1;
            } else if (person_a->born.month < person_b->born.month) return 1;
            return -1;
        } else if (person_a->born.year < person_b->born.year) return 1;
        return -1;
    }

    if (person_a->born.year == person_b->born.year) {
        if (person_a->born.month == person_b->born.month) {
            if (person_a->born.day < person_b->born.day) return 1;
            return -1;
        } else if (person_a->born.month < person_b->born.month) return 1;
        return -1;
    } else if (person_a->born.year < person_b->born.year) return 1;
    return -1;

    // w przeciwnym przypadku sortujemy alfabetycznie po rodzicu
}


int create_list(Person *person_tab, int n) {

    qsort(person_tab, n, sizeof(Person), compare4);
//    print_persons(person_tab, n);

    Person temp_person;


    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (person_tab[j].parent == person_tab[i].name) {
                temp_person = person_tab[j];
                for (int k = j; k > i; k--) {
                    person_tab[k] = person_tab[k - 1];
                }
                person_tab[i + 1] = temp_person;
            }
        }
    }

    int temp_n = n;

    n = cleaning(person_tab,n);
//    print_persons(person_tab, temp_n);
    return n;

}

////////////////////////////////////////////////////////////////

int main(void) {
    // Wszyscy potomkowie Jerzego VI (w kolejności przypadkowej):
    Person person_tab[33] = {
            {"Charles III",  M, no,  14, 11, 1948, "Elizabeth II"},
            {"Anne",         F, yes, 15, 8,  1950, "Elizabeth II"},
            {"Andrew",       M, yes, 19, 2,  1960, "Elizabeth II"},
            {"Edward",       M, yes, 10, 3,  1964, "Elizabeth II"},
            {"David",        M, yes, 3,  11, 1961, "Margaret"},
            {"Sarah",        F, yes, 1,  5,  1964, "Margaret"},
            {"William",      M, yes, 21, 6,  1982, "Charles III"},
            {"Henry",        M, yes, 15, 9,  1984, "Charles III"},
            {"Peter",        M, yes, 15, 11, 1977, "Anne"},
            {"Zara",         F, yes, 15, 5,  1981, "Anne"},
            {"Beatrice",     F, yes, 8,  8,  1988, "Andrew"},
            {"Eugenie",      F, yes, 23, 3,  1990, "Andrew"},
            {"James",        M, yes, 17, 12, 2007, "Edward"},
            {"Louise",       F, yes, 8,  11, 2003, "Edward"},
            {"Charles",      M, yes, 1,  7,  1999, "David"},
            {"Margarita",    F, yes, 14, 5,  2002, "David"},
            {"Samuel",       M, yes, 28, 7,  1996, "Sarah"},
            {"Arthur",       M, yes, 6,  5,  2019, "Sarah"},
            {"Georg",        M, yes, 22, 7,  2013, "William"},
            {"George VI",    M, no,  14, 12, 1895, NULL},
            {"Charlotte",    F, yes, 22, 5,  2015, "William"},
            {"Louis",        M, yes, 23, 4,  2018, "William"},
            {"Archie",       M, yes, 6,  5,  2019, "Henry"},
            {"Lilibet",      F, yes, 4,  6,  2021, "Henry"},
            {"Savannah",     F, yes, 29, 12, 2010, "Peter"},
            {"Isla",         F, yes, 29, 3,  2012, "Peter"},
            {"Mia",          F, yes, 17, 1,  2014, "Zara"},
            {"Lena",         F, yes, 18, 6,  2018, "Zara"},
            {"Elizabeth II", F, no,  21, 4,  1925, "George VI"},
            {"Margaret",     F, no,  21, 8,  1930, "George VI"},
            {"Lucas",        M, yes, 21, 3,  2021, "Zara"},
            {"Sienna",       F, yes, 18, 9,  2021, "Beatrice"},
            {"August",       M, yes, 9,  2,  2021, "Eugenie"}
    };

    int to_do, no;
    size_t size, n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    FILE *file;
    if (TEST) printf("Wpisz nr zadania (1 - 3) ");
    scanf("%d", &to_do);

    switch (to_do) {
        case 1:  // bsearch2
            if (TEST) printf("Wpisz liczbe linii danych: ");
            scanf("%d", &no);
            getchar();
            if (TEST) file = stdin;
            else {
                scanf("%s", buff);
                file = fopen(buff, "r");
//            file = fopen("foods0.txt","r");
                if (file == NULL) {
                    printf("Error 1\n");
                    break;
                }
            }
            if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n", no);
            n = read_stream(food_tab, FOOD_MAX, no, file);
            if (TEST) printf("Wpisz nazwe artykulu: ");
            scanf("%s", buff);
            print_art(food_tab, n, buff);
            break;
        case 2: // proste sortowanie struktur
            if (TEST) printf("Wpisz liczbe linii danych: ");
            scanf("%d", &no);
            getchar();
            if (TEST) file = stdin;
            else {
                scanf("%s", buff);
                file = fopen(buff, "r");
//            file = fopen("foods0.txt","r");
                if (file == NULL) {
                    printf("Error 1\n");
                    break;
                }
            }
            if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n", no);
            n = read_stream0(food_tab, FOOD_MAX, no, file);
            Date curr_date;
            int anticip;
            if (TEST) printf("Wpisz date odniesienia dd mm yyyy: ");
            scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
            if (TEST) printf("Wpisz roznice dni: ");
            scanf("%d", &anticip);
            printf("%.2f\n", value(food_tab, n, curr_date, anticip));
            break;
        case 3: // sukcesja
            if (TEST == 1) printf("Wpisz pozycję w kolejce (liczona od 1): ");
            scanf("%d", &no);
            int no_persons = sizeof(person_tab) / sizeof(Person);
            no_persons = create_list(person_tab, no_persons);
            if (TEST) print_persons(person_tab, no_persons);
            else print_person(person_tab + no - 1);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}

