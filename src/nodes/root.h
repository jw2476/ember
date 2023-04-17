#pragma once

#include "list.h"
#include "node.h"
#include "string.h"
#include "render.h"

typedef struct root_node_t {
	node_type type;
	list* functions;
} root_node;

root_node* root_node_new();
string root_node_render(root_node* root, render_info* info);
void root_node_get_fields(root_node* root, list* fields, format_options* format);
