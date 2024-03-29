#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union {
    int int_data;
    char char_data;
    // ... other primitive types used
    void *ptr_data;
} data_union;

typedef struct ht_element {
    struct ht_element *next;
    data_union data;
} ht_element;

typedef void (*DataFp)(data_union);

typedef void (*DataPFp)(data_union *);

typedef int (*CompareDataFp)(data_union, data_union);

typedef size_t (*HashFp)(data_union, size_t);

typedef data_union (*CreateDataFp)(void *);

typedef struct {
    size_t size;
    size_t no_elements;
    ht_element *ht;
    DataFp dump_data;
    CreateDataFp create_data;
    DataFp free_data;
    CompareDataFp compare_data;
    HashFp hash_function;
    DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
             DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {

    p_table->ht = malloc(sizeof(ht_element) * size);
    for (size_t i = 0; i < size; i++) {
        p_table->ht[i].next = NULL;
    }

    p_table->size = size;
    p_table->no_elements = 0;
    p_table->dump_data = dump_data;
    p_table->create_data = create_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->hash_function = hash_function;
    p_table->modify_data = modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table *p_table, size_t n) {
    for (ht_element *ht = p_table->ht[n].next; ht != NULL; ht = ht->next) {
        p_table->dump_data(ht->data);
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    free_data(to_delete->data);
}

// free all elements from the table (and the table itself)
void free_table(hash_table *p_table) {
    if (p_table -> free_data){
        for (size_t i = 0; i < p_table -> size; i++) {
            ht_element *hti = p_table -> ht[i].next;
            ht_element *to_delete = hti;
            while (hti != NULL) {
                hti = hti->next;
                p_table -> free_data(to_delete -> data);
                free(to_delete);
                to_delete = hti;
            }
        }
    }
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
    static const double c = 0.618033988; // (sqrt(5.) â€“ 1) / 2.;
    double tmp = k * c;
    return (size_t) floor((double) size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table) {
    size_t old_size = p_table->size;
    ht_element *old_ht = p_table->ht;
    p_table->size *= 2;

    p_table->ht = malloc(sizeof(ht_element) * p_table->size);
    for (size_t i = 0; i < p_table->size; i++) {
        p_table->ht[i].next = NULL;
    }

    for (size_t i = 0; i < old_size; i++) {
        ht_element *hti = old_ht[i].next;
        ht_element *to_delete = hti;

        while (hti != NULL) {
            int index = p_table->hash_function(hti->data, p_table->size);
            ht_element *new_ht = malloc(sizeof(ht_element));
            new_ht->next = p_table->ht[index].next;
            p_table->ht[index].next = new_ht;
            new_ht->data = hti->data;
            hti = hti->next;
            free(to_delete);
            to_delete = hti;
        }
    }
    free(old_ht);
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data) {
    int index = p_table->hash_function(data, p_table->size);
    for (ht_element *ht = p_table->ht[index].next; ht->next != NULL; ht = ht->next)
        if (p_table->compare_data(data, ht->next->data) == 0)return ht;
    return p_table->ht + index * sizeof(ht_element);
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    int index = p_table->hash_function(*data, p_table->size);
    for (ht_element *ht = p_table->ht[index].next; ht != NULL; ht = ht->next)
        if (p_table->compare_data(*data, ht->data) == 0)
            return ht;
    return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
    if (p_table->no_elements / p_table->size > MAX_RATE) {
        rehash(p_table);
    }

    ht_element *ret = get_element(p_table, data);


    if (ret != NULL) {
        if(p_table -> modify_data) p_table -> modify_data(&(ret -> data));
//        printf("RET ");
//        p_table -> dump_data((ret -> data));
//        printf("\n");
//        if (p_table -> free_data) p_table -> free_data(*data);
        return;
    }

    ht_element *ht_new = malloc(sizeof(ht_element));
    ht_new->data = *data;
//    p_table -> dump_data(*data);

    int index = p_table->hash_function(*data, p_table->size);
    ht_new->next = p_table->ht[index].next;
    p_table->ht[index].next = ht_new;
    p_table->no_elements++;
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    ht_element *to_delete = get_element(p_table, &data);
    ht_element *previous = find_previous(p_table, data);

    if (to_delete == NULL) return;

    previous->next = to_delete->next;
//    free_element(p_table -> free_data, to_delete);
    p_table->no_elements--;
}

// type-specific definitions

// int element

size_t hash_int(data_union data, size_t size) {
    return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data - b.int_data;
}

data_union create_int(void *value) {
    int number;
    scanf("%d", &number);
    if (!value) {
        data_union data;
        data.int_data = number;
        return data;
    } else {
        ((data_union *) value)->int_data = number;
        return *((data_union *) value);
    }
}

// char element

size_t hash_char(data_union data, size_t size) {
    return hash_base((int) data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
    return a.char_data - b.char_data;
}

data_union create_char(void *value) {
    char character;
    scanf(" %c", &character);
    if (!value) {
        data_union data;
        data.char_data = character;
        return data;
    } else {
        ((data_union *) value)->char_data = character;
        return *((data_union *) value);
    }
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word(data_union data) {
    DataWord *dw = (DataWord *) data.ptr_data;
    printf("%s %d", dw->word, dw -> counter);
}

void free_word(data_union data) {
    free(data.ptr_data);
}

int cmp_word(data_union a, data_union b) {
    DataWord *dw = (DataWord *) a.ptr_data;
    DataWord *dw2 = (DataWord *) b.ptr_data;
//    printf("dw = %s (dw counter = %d), dw2 = %s\n", dw -> word, dw -> counter, dw2 -> word);
    return strcasecmp(dw->word, dw2->word);
}

size_t hash_word(data_union data, size_t size) {
    int s = 0;
    DataWord *dw = (DataWord *) data.ptr_data;
    char *p = dw->word;
    while (*p) {
        s += tolower(*p++);
    }
    return hash_base(s, size);
}

void modify_word(data_union *data) {
    DataWord *dw = (DataWord *) data->ptr_data;
    dw->counter++;
}

data_union create_data_word(void *value) {
//    DataWord *dw = malloc(sizeof(DataWord));
//    dw->word = malloc(BUFFER_SIZE*sizeof(char));
//    dw->word = ((char *)value);
//    dw->counter = 1;
    data_union data;
    DataWord *dw = malloc(sizeof(DataWord));
    char *word = malloc(BUFFER_SIZE*sizeof(char));
    dw -> word = strcpy(word, (char *)value);
    dw -> counter = 1;
    data.ptr_data = dw;
    return data;
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    char input[BUFFER_SIZE];
    const char sep[] = " .,;\n";
    void *ptr = malloc(BUFFER_SIZE);

    while (fgets(input, BUFFER_SIZE, stream) != NULL) {
        for (ptr = strtok(input, sep); ptr; ptr = strtok(NULL, sep)) {
            data_union data = p_table -> create_data((void *)ptr);
            insert_element(p_table, &data);
        }
    }
    free(ptr);
}



// test primitive type list
void test_ht(hash_table *p_table, int n) {
    char op;
    data_union data;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
//		p_table->create_data(&data);
        data = p_table->create_data(NULL); // should give the same result as the line above
        switch (op) {
            case 'r':
                remove_element(p_table, data);
                break;
            case 'i':
                insert_element(p_table, &data);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    size_t index;
    hash_table table;
    char buffer[BUFFER_SIZE];
    data_union data;

    scanf("%d", &to_do);
    switch (to_do) {
        case 1: // test integer hash table
            scanf("%d %zu", &n, &index);
            init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
            test_ht(&table, n);
            printf("%zu\n", table.size);
            dump_list(&table, index);
            break;
        case 2: // test char hash table
            scanf("%d %zu", &n, &index);
            init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
            test_ht(&table, n);
            printf("%zu\n", table.size);
            dump_list(&table, index);
            break;
        case 3: // read words from text, insert into hash table, and print
            scanf("%s", buffer);
            init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
            stream_to_ht(&table, stdin);
            printf("%zu\n", table.size);
            data = table.create_data(buffer);
            ht_element *e = get_element(&table, &data);
            if (e) table.dump_data(e->data);
            if (table.free_data) table.free_data(data);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    free_table(&table);

    return 0;
}


