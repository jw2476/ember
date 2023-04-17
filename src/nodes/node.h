#pragma once

#include "str.h"
#include "render.h"

#define FIELD(data) (node_field){ data, (node_generic*)node }

typedef enum node_type_t {
	NODE_UNDEFINED = 0,
	NODE_ROOT,
	NODE_FUNCTION,
	NODE_ARGUMENT,
	NODE_CALL,
	NODE_STRING_LITERAL
} node_type;

typedef struct node_generic_t {
	node_type type;
	void* parent;
} node_generic;

typedef struct node_field_t {
	string data;
	node_generic* parent;
} node_field;

string node_field_render(node_field* field, render_info* info);
