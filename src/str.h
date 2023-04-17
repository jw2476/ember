#pragma once

#include <stdlib.h>

typedef struct string_t {
	char* data;
	size_t size;
} string;

string string_new_with_size(const char* data, size_t size);
string string_new(const char* data);
void string_destroy(string str);
string string_clone(string* base);
void string_append(string* base, string new);
void string_addchar(string* base, char c);
void string_remove(string* base, size_t at);
char* string_charptr(string* str);
string string_substr(string* base, size_t from, size_t to);
void string_clear(string* str);
