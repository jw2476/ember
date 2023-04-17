#pragma once

typedef struct list_t list;
typedef struct list_t {
	void* data;
	list* next;
} list;

list* list_new();
void list_destroy(list* root);
list* list_end(list* root);
void list_append(list* root, void* data);
list* list_reverse(list* root);
