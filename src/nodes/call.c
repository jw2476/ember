#include "call.h"

#include "string_literal.h"

call_node* call_node_new(node_generic* parent) {
    call_node* node = calloc(1, sizeof(call_node));
    node->type = NODE_CALL;
    node->parent = parent;
    node->name = FIELD(string_new(""));
    node->parameters = list_new();

    return node;
}
void call_node_destroy(call_node* node) {
    string_destroy(node->name.data);
    list_destroy(node->parameters);
    free(node);
}

string call_node_render(call_node* node, render_info* info) {
    string output = string_new("");
    string_append(&output, node_field_render(&node->name, info));
    string_append(&output, string_new("("));

    for (list* item = node->parameters; item != NULL; item = item->next) {
        if (item->data == NULL)
            continue;

        node_generic* parameter = item->data;
        if (parameter->type == NODE_CALL) {
            string_append(&output,
                          call_node_render((call_node*)parameter, info));
        } else if (parameter->type == NODE_STRING_LITERAL) {
            string_append(&output, string_literal_node_render(
                                       (string_literal_node*)parameter, info));
        }

        if (item->next != NULL) {
            string_append(&output, string_new(", "));
        }
    }

    string_append(&output, string_new(")"));

    return output;
}

void call_node_get_fields(call_node* node, list* fields,
                          format_options* format) {
    list_append(fields, &node->name);

    for (list* item = node->parameters; item != NULL; item = item->next) {
        if (item->data == NULL)
            continue;

        node_generic* parameter = item->data;
        if (parameter->type == NODE_CALL) {
            call_node_get_fields((call_node*)parameter, fields, format);
        } else if (parameter->type == NODE_STRING_LITERAL) {
            string_literal_node_get_fields((string_literal_node*)parameter,
                                           fields, format);
        }
    }
}
