#pragma once

#include "node.h"
#include "string.h"
#include "list.h"
#include "function.h"

typedef struct call_node_t {
	node_type type;
	node_generic* parent;
	node_field name;
	list* parameters;
} call_node;

call_node* call_node_new(node_generic* parent);
void call_node_destroy(call_node* node);

string call_node_render(call_node* node, render_info* info);
void call_node_get_fields(call_node* node, list* fields, format_options* format);

