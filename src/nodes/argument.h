#pragma once

#include "string.h"
#include "node.h"
#include "render.h"
#include "list.h"
#include "function.h"

typedef struct argument_node_t {
	node_type type;
	function_node* parent;
	node_field name;
	node_field typename;
} argument_node;

argument_node* argument_node_new(function_node* parent);
string argument_node_render(argument_node* arg, render_info* info);
void argument_node_get_fields(argument_node* arg, list* fields, format_options* format);
