#pragma once

#include "str.h"

typedef enum argument_format_t {
	ARGUMENT_FORMAT_DEFAULT = 0,
	ARGUMENT_FORMAT_C = 1,
	ARGUMENT_FORMAT_RUST = 2,
} argument_format;

typedef enum lods_t {
	LOD_LOWEST = 0,
	LOD_SIGNATURES_ONLY,
	LOD_IMPLEMENTATIONS,
	LOD_HIGHEST
} lods;

typedef struct format_options_t {
	argument_format argFormat;
} format_options;

typedef struct node_field_t node_field;
typedef struct root_node_t root_node;
typedef struct render_info_t {
	format_options format;
	node_field* currentField;
	root_node* root;
	lods lod;
} render_info;

render_info render_info_new();
