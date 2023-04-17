#pragma once

#include "list.h"
#include "str.h"
#include "render.h"
#include "node.h"

typedef enum function_node_flags_t {
	FUNCTION_NODE_NONE = 0,
	FUNCTION_NODE_EXTERN = 1
} function_node_flags;

typedef struct function_node_t {
	node_type type;
	void* parent;

	function_node_flags flags;
	node_field name;
	node_field returnType;
	list* arguments;
	list* body;
} function_node;

function_node* function_node_new(void* parent);
void function_node_destroy(function_node* node);

string function_node_render(function_node* node, render_info* info);
void function_node_get_fields(function_node* node, list* fields, format_options* format);
