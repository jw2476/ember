#include "string_literal.h"

string_literal_node* string_literal_node_new(node_generic* parent) {
    string_literal_node* node = calloc(1, sizeof(string_literal_node));
    node->type = NODE_STRING_LITERAL;
    node->parent = parent;
    node->data = FIELD(string_new(""));

    return node;
}

void string_literal_node_destroy(string_literal_node* node) {
    string_destroy(node->data.data);
    free(node);
}

string string_literal_node_render(string_literal_node* node,
                                  render_info* info) {
    string output = string_new("\"");
    string_append(&output, node_field_render(&node->data, info));
    string_append(&output, string_new("\""));

    return output;
}

void string_literal_node_get_fields(string_literal_node* node, list* fields,
                                    render_info* info) {
    list_append(fields, &node->data);
}
