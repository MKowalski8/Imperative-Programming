#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
    int *data;
    size_t array_size;
    struct Node *next;
    struct Node *prev;
} Node;

// doubly linked list
typedef struct List {
    Node *head;
    Node *tail;
} List;

// iterator
typedef struct iterator {
    struct Node *node_ptr;
    size_t position;
} iterator;

// forward initialization
iterator begin(Node *head) {
    iterator it = {head, 0};
    return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node *tail) {
    iterator it = {tail, tail->array_size};
    return it;
}

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) return ptr;
    exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
    void *ptr = realloc(old_ptr, size);
    if (ptr) return ptr;
    free(old_ptr);
    exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
    Node *node = safe_malloc(sizeof(Node));
    node->data = data;
    node->array_size = array_size;
    node->next = next;
    node->prev = prev;
    return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
    list->head = create_node(NULL, 0, NULL, NULL);
    list->tail = create_node(NULL, 0, NULL, list->head);
    list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
    Node *node = create_node(data, array_size, list->tail, list->tail->prev);
    list->tail->prev->next = node;
    list->tail->prev = node;
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator *itr, size_t n) {
    itr->node_ptr = itr->node_ptr->next;

    for (Node *node = itr->node_ptr; itr->position < n; node = node->next) {
        itr->position += node->array_size;
        itr->node_ptr = node;
    }
    itr->position -= itr->node_ptr->array_size - 1;


//    printf("position = %d, n = %d\n", itr->position, n);

}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
    iterator it = begin(list->head);
    skip_forward(&it, n);
//    printf("begin = %d\n", *(it.node_ptr -> data));
    int add = (n - it.position);
    return it.node_ptr->data[add];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator *itr, size_t n) {
//    printf("\npostion = %d\n", itr -> position);

    for (Node *node = itr->node_ptr->prev; itr->position < n; node = node->prev) {
        itr->position += node->array_size;
        itr->node_ptr = node;
//        printf("\nsth = %d ", *itr -> node_ptr -> data);
//        printf("\npostion = %d\n", itr -> position);
    }

//    itr -> position -= itr -> node_ptr -> array_size - 1;

}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
    iterator it = end(list->tail->prev);
    skip_backward(&it, n);
//    printf("begin = %d\n", *(it.node_ptr -> data));
    int add = (it.position - n);
    return (it.node_ptr->data[add]);
}


void remove_node(Node *node_ptr) {
    node_ptr->prev->next = node_ptr->next;
    node_ptr->next->prev = node_ptr->prev;
    free(node_ptr);
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
    iterator it = begin(list->head);
    skip_forward(&it, n);
    int add = (n - it.position);

    if (it.node_ptr->array_size == 1) {
        remove_node(it.node_ptr);
    } else {
        char *ptr = (char *) it.node_ptr->data;
        if (add < it.node_ptr->array_size - 1)
            memmove(ptr + (add) * sizeof(int), ptr + (add + 1) * sizeof(int),
                    ((it.node_ptr->array_size) - add) * sizeof(int));
//        printf("sth = %d\n", *(it.node_ptr -> data));
        it.node_ptr->array_size -= 1;
        it.node_ptr->data = safe_realloc(it.node_ptr->data, (it.node_ptr->array_size) * sizeof(int));
    }
}

// return the number of digits of number n
size_t digits(int n) {
    int ctr = 0;
    do {
        n /= 10;
        ctr++;
    } while (n != 0);

    return ctr;
}
int cmp (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}
// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void put_in_order(List *list, int value) {
    int flag = 1;
    Node *node = list->head->next;

    int how_many_digits = digits((value));

    while (node != list->tail) {
        int comperator = digits(node->data[0]);
        if (comperator >= how_many_digits) {
            flag = 0;
            if (comperator > how_many_digits) {
                flag = 2;
            }
            break;
        }

        node = node->next;
    }

    if (flag == 2) {
        int *value_to_give = (int *) safe_malloc(sizeof(int));
        *value_to_give = value;
        Node *new_node = create_node(value_to_give, 1, node, node->prev);
        node->prev->next = new_node;
        node -> prev = new_node;
    } else if (flag == 1) {
        int *value_to_give = (int *) safe_malloc(sizeof(int));
        *value_to_give = value;
        push_back(list, value_to_give, 1);
    }
    else {
        node -> data = safe_realloc(node -> data, (node -> array_size + 1)*sizeof(int));
        node -> array_size++;
        node -> data[node -> array_size - 1] = value;
        qsort(node -> data, node -> array_size, sizeof(int), cmp);
    }
}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
    for (Node *node = list->head->next; node != list->tail; node = node->next) {
        printf("-> ");
        for (int k = 0; k < node->array_size; k++) {
            printf("%d ", node->data[k]);
        }
        printf("\n");
    }
}

// free list
void freeList(List *list) {
    Node *to_delete = list->head->next, *next;
    while (to_delete != list->tail) {
        next = to_delete->next;
        remove_node(to_delete);
        to_delete = next;
    }
}

// read int vector
void read_vector(int tab[], size_t n) {
    for (size_t i = 0; i < n; ++i) {
        scanf("%d", tab + i);
    }
}

// initialize the list and push data
void read_list(List *list) {
    int n;
    size_t size;
    scanf("%d", &n); // number of nodes
    for (int i = 0; i < n; i++) {
        scanf("%zu", &size); // length of the array in i-th node
        int *tab = (int *) safe_malloc(size * sizeof(int));
        read_vector(tab, size);
        push_back(list, tab, size);
    }
}

int main() {
    int to_do, value;
    size_t size, m;
    List list;
    init(&list);

    scanf("%d", &to_do);
    switch (to_do) {
        case 1:
            read_list(&list);
            dumpList(&list);
            break;
        case 2:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                printf("%d ", get_forward(&list, m));
            }
            printf("\n");
            break;
        case 3:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                printf("%d ", get_backward(&list, m));
            }
            printf("\n");
            break;
        case 4:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                remove_at(&list, m);
            }
            dumpList(&list);
            break;
        case 5:
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &value);
                put_in_order(&list, value);
            }
            dumpList(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    freeList(&list);

    return 0;
}

