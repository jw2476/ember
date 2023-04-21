#include "argument.h"

argument_node* argument_node_new(function_node* parent) {
    argument_node* node = calloc(1, sizeof(argument_node));
    node->parent = parent;
    node->type = NODE_ARGUMENT;
    node->name = FIELD(string_new(""));
    node->typename = FIELD(string_new("void"));

    return node;
}

string argument_node_render(argument_node* arg, render_info* info) {
    string output = string_new("");

    if (info->format.argFormat == ARGUMENT_FORMAT_DEFAULT ||
        info->format.argFormat == ARGUMENT_FORMAT_C) {
        string_append(&output, node_field_render(&arg->typename, info));
        string_append(&output, string_new(" "));
        string_append(&output, node_field_render(&arg->name, info));
    } else if (info->format.argFormat == ARGUMENT_FORMAT_RUST) {
        string_append(&output, node_field_render(&arg->name, info));
        string_append(&output, string_new(": "));
        string_append(&output, node_field_render(&arg->typename, info));
    }

    return output;
}

void argument_node_get_fields(argument_node* arg, list* fields,
                              format_options* format) {
    if (format->argFormat == ARGUMENT_FORMAT_DEFAULT ||
        format->argFormat == ARGUMENT_FORMAT_C) {
        list_append(fields, &arg->typename);
        list_append(fields, &arg->name);
    } else if (format->argFormat == ARGUMENT_FORMAT_RUST) {
        list_append(fields, &arg->name);
        list_append(fields, &arg->typename);
    }
}
