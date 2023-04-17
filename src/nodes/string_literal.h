#pragma once

#include "node.h"
#include "string.h"
#include "list.h"
#include "render.h"

typedef struct string_literal_node_t {
	node_type type;
	node_generic* parent;

	node_field data;
} string_literal_node;

string_literal_node* string_literal_node_new(node_generic* parent);
void string_literal_node_destroy(string_literal_node* node);

string string_literal_node_render(string_literal_node* node, render_info* info);
void string_literal_node_get_fields(string_literal_node* node, list* fields, render_info* info);
