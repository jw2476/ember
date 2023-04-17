#include "function.h"

#include "call.h"
#include "ember_config.h"

function_node* function_node_new(void* parent) {
	function_node* node = calloc(1, sizeof(function_node));
	node->type = NODE_FUNCTION;
	node->parent = parent;
	node->name = FIELD(string_new(""));
	node->returnType = FIELD(string_new("void"));
	node->arguments = list_new();
	node->body = list_new();

	return node;
}

void function_node_destroy(function_node* fn) {
	string_destroy(fn->name.data);
	string_destroy(fn->returnType.data);
	list_destroy(fn->arguments);
	list_destroy(fn->body);

	free(fn);
}

typedef struct argument_node_t argument_node;
string argument_node_render(argument_node* arg, render_info* info);

string function_node_render(function_node* fn, render_info* info) {
	string output = string_new("");
	if (fn->flags & FUNCTION_NODE_EXTERN) string_append(&output, string_new("extern "));
	string_append(&output, string_new("fn "));

	string_append(&output, node_field_render(&fn->name, info)); 
	string_append(&output, string_new("("));

	for (list* arg = fn->arguments; arg != NULL; arg = arg->next) {
		if (arg->data == NULL) continue;

		string_append(&output, argument_node_render(arg->data, info));
		if (arg->next != NULL) {
			string_append(&output, string_new(", "));
		}
	}

	string_append(&output, string_new(") -> "));
	string_append(&output, node_field_render(&fn->returnType, info));
	
	if (fn->body->data == NULL || info->lod < LOD_IMPLEMENTATIONS) {
		string_append(&output, string_new(" {}\n"));
	} else {
		string_append(&output, string_new(" {\n"));

		for (list* item = fn->body; item != NULL; item = item->next) {
			if (item->data == NULL) continue;

			node_generic* node = item->data;
			if (node->type == NODE_CALL) {
				string_append(&output, string_new(TAB));
				string_append(&output, call_node_render(node, info));
				string_append(&output, string_new("\n"));
			}
		}

		string_append(&output, string_new("}\n"));
	}

	return output;
}

void function_node_get_fields(function_node* fn, list* fields, format_options* format) {
	list_append(fields, &fn->name);

	for (list* arg = fn->arguments; arg != NULL; arg = arg->next) {
		if (arg->data == NULL) continue;

		argument_node_get_fields(arg->data, fields, format);
	}

	list_append(fields, &fn->returnType);

	for (list* item = fn->body; item != NULL; item = item->next) {
		if (item->data == NULL) continue;

		node_generic* node = item->data;
		if (node->type == NODE_CALL) {
			call_node_get_fields(node, fields, format);
		}
	}
}
