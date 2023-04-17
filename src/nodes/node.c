#include "node.h"

string node_field_render(node_field* field, render_info* info) {
	string output = string_new("");
	if (field == info->currentField) {
		string_append(&output, string_new("|"));
		string_append(&output, string_clone(&field->data));
		string_append(&output, string_new("|"));
	} else {
		string_append(&output, string_clone(&field->data));
	}

	return output;
}
