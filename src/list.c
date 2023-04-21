#include "list.h"

#include <stdlib.h>

list* list_new() { return calloc(1, sizeof(list)); }

void list_destroy(list* root) {
    if (root->next)
        list_destroy(root->next);
    free(root);
}

list* list_end(list* root) {
    list* item = root;
    while (item->next != NULL) {
        item = item->next;
    }

    return item;
}

void list_append(list* root, void* data) {
    if (root->data == NULL) {
        root->data = data;
    } else {
        list* end = list_end(root);
        list* new = list_new();
        new->data = data;
        end->next = new;
    }
}

list* list_reverse(list* root) {
    list* current = root;
    list* previous = NULL;
    list* next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    return previous;
}
