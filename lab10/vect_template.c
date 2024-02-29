#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 64

typedef struct Vector {
    void *data;
    size_t element_size;
    size_t size;
    size_t capacity;
} Vector;

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void *, const void *);

typedef int(*predicate_ptr)(void *);

typedef void(*read_ptr)(void *);

typedef void(*print_ptr)(const void *);


// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
    vector->data = malloc(block_size * element_size);
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = block_size;

}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
//    printf("I'm in reserve\n");

    if (vector->capacity <= new_capacity) {
        vector->capacity *= 2;

        vector->data = realloc(vector->data, vector->capacity * vector->element_size);

        memset((char *) vector->data + vector->element_size * vector->size, 0,
               (vector->capacity - vector->size) * vector->element_size);
//        for (int i = vector->size - 1; i <= vector->capacity; i++) {
//            for (int j = 0; j < vector->element_size; j++) {
//                ((char *) vector->data)[i * vector->element_size + j] = 0;
//            }
//        }

//        printf("I change capacity\n");
    }
//    printf("I'm out of reserve\n");


}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.
// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
    if (vector->size < new_size) {
        if (vector->capacity < new_size) vector->capacity = new_size;
        vector->data = realloc(vector->data, vector->capacity * vector->element_size);
        memset((char *) vector->data + vector->element_size * vector->size, 0,
               (new_size - vector->size) * vector->element_size);
        vector->size = new_size;

//        for (int i = vector->size - 1; i <= vector->capacity; i++) {
//            for (int j = 0; j < vector->element_size; j++) {
//                ((char *) vector->data)[i * vector->element_size + j] = 0;
//            }
//        }
    } else if (vector->size > new_size) {
        vector->size = new_size;
        vector->capacity = new_size;
        vector->data = realloc(vector->data, new_size * vector->element_size);
    }

}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
    vector->size++;
    reserve(vector, vector->size);
//    printf("\nsize = %d", vector->size);
//    printf("\n%d", *(char *) value);
    memcpy((char *) vector->data + ((vector->size - 1) * vector->element_size), (char *) value, vector->element_size);
}

// Remove all elements from the vector
void clear(Vector *vector) {
    vector->data = calloc(vector->size, vector->element_size);
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
    vector->size += 1;
//    if (index == vector -> size):
    reserve(vector, vector->size);

//    printf("%p\n", vector -> data);
    memmove((char *) vector->data + vector->element_size * (index + 1),
            (char *) vector->data + vector->element_size * index, vector->element_size * (vector->size - index));
    memcpy((char *) vector->data + vector->element_size * index, (char *)value, vector->element_size);
//    printf("%p\n", vector -> data);
}

// Erase element at position index
void erase(Vector *vector, size_t index) {
//    printf("I do it\n");

    memmove((char *)vector->data + (vector->element_size * index),
            (char *)vector->data + (vector->element_size * (index + 1)),
            vector->element_size * (vector->size - index));
    vector->size -= 1;

//    printf("I did it\n");


}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
    for (int i = 0; i < vector->size; i++) {
        if (!cmp(value, vector->data + i * vector->element_size)) {
            memmove((char *) vector->data + vector->element_size * i,
                    (char *) vector->data + vector->element_size * (i + 1),
                    vector->element_size * (vector->size - i));
            vector->size -= 1;
            i--;
        }
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
    for (int i = 0; i < vector->size; i++) {
        if (predicate(vector->data + i * vector->element_size)) {
            memmove((char *) vector->data + vector->element_size * i,
                    (char *) vector->data + vector->element_size * (i + 1),
                    vector->element_size * (vector->size - i));
            vector->size -= 1;
            i--;
        }
    }

}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
    vector->data = realloc(vector->data, vector->size * vector->element_size);
    vector->capacity = vector->size;
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
//    printf("%d, %d\n",  *(int *) v1, *(int *) v2);
//    if (*(int *) v1 == *(int *) v2) return 2;
    return *(int *) v1 - *(int *) v2;
}

// char comparator
int char_cmp(const void *v1, const void *v2) {
    return *(char *) v1 - *(char *) v2;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
    Person *ptr, *ptr2;
    ptr = (Person *) p1;
    ptr2 = (Person *) p2;

    if (ptr->age == ptr2->age) {
        if (strcmp(ptr->first_name, ptr2->first_name) == 0) {
            if (strcmp(ptr->last_name, ptr2->last_name) == 0) return 0;
            if (strcmp(ptr2->last_name, ptr->last_name) > 0) return -1;
        }
        if (strcmp(ptr->first_name, ptr2->first_name) < 0) return -1;
    }
    if (ptr -> age > ptr2 -> age) return -1;
    return 1;
}

// predicate: check if number is even
int is_even(void *value) {
    return *(int *) value % 2;
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
    char letter = *(char *) value;
    if (letter == 'a' || letter == 'e' || letter == 'i' || letter == 'o' || letter == 'u' || letter == 'y' ||
        letter == 'A' ||
        letter == 'E' || letter == 'I' || letter == 'O' || letter == 'U' || letter == 'Y')
        return 1;
    return 0;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
    Person *ptr;
    ptr = (Person *) person;
    if (ptr->age > 25) return 1;
    return 0;

}

// print integer value
void print_int(const void *v) {
    printf("%d ", *(int *) v);
}

// print char value
void print_char(const void *v) {
    printf("%c ", *(char *) v);
}

// print structure Person
void print_person(const void *v) {
    Person *ptr;
    ptr = (Person *) v;
    printf("%d %s %s\n", ptr->age, ptr->first_name, ptr->last_name);
}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print) {
    printf("%d\n", vector->capacity);

    for (int i = 0; i < vector->size; i++) {
        print(vector->data + (i * vector->element_size));
    }
//    printf("\n");
//    for (int i = 0; i < vector->capacity; i++) {
//        print(vector->data + (i * vector->element_size));
//    }
//    printf("\n");
}

// read int value
void read_int(void *value) {
//    printf("%p", value);
    scanf("%d", value);
//    printf("%d", value);
}

// read char value
void read_char(void *value) {
    scanf(" %c", value);
}

// read struct Person
void read_person(void *value) {
    Person *ptr;
//    printf("Hello\n");
    ptr = (Person *) value;
    scanf("%d %s %s", &ptr->age, &ptr->first_name, &ptr->last_name);

//    printf("Hello\n");

//    value = &ptr;
//    printf("%d, %s, %s", ptr.age, ptr.first_name, ptr.last_name);
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
                 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
    init_vector(vector, block_size, elem_size);
    void *v = malloc(vector->element_size);
    size_t index, size;
    for (int i = 0; i < n; ++i) {
        char op;
//        printf("hello");
        scanf(" %c", &op);
        switch (op) {
            case 'p': // push_back
                read(v);
//                printf("%p\n", v);
//                printf("%c\n", *(char*)v);
                push_back(vector, v);
                break;
            case 'i': // insert
                scanf("%zu", &index);
                read(v);
                insert(vector, index, v);
                break;
            case 'e': // erase
                scanf("%zu", &index);
//                printf("\nI try\n");
                erase(vector, index);
                break;
            case 'v': // erase
                read(v);
                erase_value(vector, v, cmp);
                break;
            case 'd': // erase (predicate)
                erase_if(vector, predicate);
                break;
            case 'r': // resize
                scanf("%zu", &size);
                resize(vector, size);
                break;
            case 'c': // clear
                clear(vector);
                break;
            case 'f': // shrink
                shrink_to_fit(vector);
                break;
            case 's': // sort
                qsort(vector->data, vector->size,
                      vector->element_size, cmp);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
//        print_vector(vector, print);
//        printf("\n");
    }
    print_vector(vector, print);
    free(vector->data);
    free(v);
}

int main(void) {
    int to_do, n;
    Vector vector_int, vector_char, vector_person;

    scanf("%d%d", &to_do, &n);
//
//    to_do = 1;
//    n = 12;

    switch (to_do) {
        case 1:
            vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
                        is_even, print_int);
            break;
        case 2:
            vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
                        is_vowel, print_char);
            break;
        case 3:
            vector_test(&vector_person, 2, sizeof(Person), n, read_person,
                        person_cmp, is_older_than_25, print_person);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }

    return 0;
}


