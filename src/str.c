#include "str.h"

#include <string.h>

string string_new_with_size(const char* data, size_t size) {
	char* newData = calloc(1, size);
	memcpy(newData, data, size);

	return (string){ newData, size }; 
}

string string_new(const char* data) {
	size_t size = strlen(data);
	return string_new_with_size(data, size);
}

void string_destroy(string str) {
	free(str.data);	
}

string string_clone(string* base) {
	char* data = calloc(1, base->size);
	memcpy(data, base->data, base->size);

	return (string){ data, base->size };
}

void string_append(string* base, string new) {
	if (new.size != 0) {	
		base->data = realloc(base->data, base->size + new.size);
		memcpy(&base->data[base->size], new.data, new.size);

		base->size += new.size;
	}

	string_destroy(new);
}

void string_addchar(string* base, char c) {
	base->data = realloc(base->data, base->size + 1);
	base->data[base->size] = c;
	base->size++;
}

void string_remove(string* base, size_t at) {
	memcpy(&base->data[at], &base->data[at + 1], base->size - at);
	base->size--;
}

char* string_charptr(string* str) {
	char* buffer = calloc(1, str->size + 1);
	memcpy(buffer, str->data, str->size);

	return buffer;
}

string string_substr(string* base, size_t from, size_t to) {
	return string_new_with_size(&base->data[from], to - from);
}

void string_clear(string* str) {
	memset(str->data, 0, str->size);
	str->size = 0;
}
