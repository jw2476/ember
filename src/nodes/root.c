#include "root.h"

#include "function.h"

root_node* root_node_new() {
	root_node* root = calloc(1, sizeof(root_node));
	root->type = NODE_ROOT;
	root->functions = list_new();

	return root;
}

string root_node_render(root_node* root, render_info* info) {
	string output = string_new("");

	for (list* fn = root->functions; fn != NULL; fn = fn->next) {
		if (fn->data == NULL) continue;

		string_append(&output, function_node_render(fn->data, info));
	}

	return output;
}

void root_node_get_fields(root_node* root, list* fields, format_options* format) {
	for (list* fn = root->functions; fn != NULL; fn = fn->next) {
		if (fn->data == NULL) continue;

		function_node_get_fields(fn->data, fields, format);
	}
}
