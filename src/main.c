#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>

#include "raylib.h"

#include "colors.h"
#include "str.h"
#include "list.h"
#include "nodes/function.h"
#include "nodes/root.h"
#include "nodes/argument.h"
#include "nodes/call.h"
#include "nodes/string_literal.h"

#define DEFAULT_FONT_SIZE 48
#define MAX_FONT_SIZE 256
#define MIN_FONT_SIZE 4

#define FONT_REGULAR_PATH "assets/JetBrainsMono/JetBrainsMono-Regular.ttf"

typedef enum input_mode_t {
	INPUT_MODE_OFF = 0,
	INPUT_MODE_INSERT,
	INPUT_MODE_ADD_MODIFIER
} input_mode;

typedef struct input_buffer_t {
	string data;
	size_t protected;
	input_mode mode;
} input_buffer;

void insert_node(input_buffer* buffer, render_info* info, const char* input) {
	if (strcmp(input, "fn") == 0) {
		function_node* node = function_node_new(info->root);
		list_append(info->root->functions, node);
		info->currentField = &node->name;
	} else if (strcmp(input, "arg") == 0) {
		function_node* parent = NULL;

		if (info->currentField->parent->type == NODE_FUNCTION) {
			parent = (function_node*)info->currentField->parent;
		} else if (info->currentField->parent->type == NODE_ARGUMENT) {
			parent = (function_node*)info->currentField->parent->parent;
		} else {
			string_clear(&buffer->data);
			string_append(&buffer->data, string_new("Arguments can only be inserted to function or argument nodes"));
		}
		
		if (parent != NULL) {
			argument_node* node = argument_node_new(parent);
			list_append(parent->arguments, node);
			info->currentField = &node->name;
		}
	} else if (strcmp(input, "call") == 0 || strcmp(input, "c") == 0) {
		if (info->currentField->parent->type == NODE_FUNCTION) {
			function_node* parent = (function_node*)info->currentField->parent;
			call_node* node = call_node_new((node_generic*)parent);
			list_append(parent->body, node);
			info->currentField = &node->name;
		} else if (info->currentField->parent->type == NODE_CALL) {
			call_node* parent = (call_node*)info->currentField->parent;
			call_node* node = call_node_new((node_generic*)parent);
			list_append(parent->parameters, node);
			info->currentField = &node->name;
		}
	} else if (strcmp(input, "str") == 0 || strcmp(input, "s") == 0) {
		if (info->currentField->parent->type == NODE_CALL) {
			call_node* parent = (call_node*)info->currentField->parent;
			string_literal_node* node = string_literal_node_new((node_generic*)parent);
			list_append(parent->parameters, node);
			info->currentField = &node->data;
		}
	}
}

void add_modifier(input_buffer* buffer, render_info* info, const char* input) {
	if (strcmp(input, "extern") == 0) {
		if (info->currentField->parent->type == NODE_FUNCTION) {
			function_node* node = (function_node*)info->currentField->parent;
			node->flags |= FUNCTION_NODE_EXTERN;
		} else {
			string_clear(&buffer->data);
			string_append(&buffer->data, string_new("Only functions can be extern"));
		}
	}
}

void handle_input(input_buffer* buffer, render_info* info) {
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_I)) {
		buffer->mode = INPUT_MODE_INSERT;
		string_clear(&buffer->data);
		string_append(&buffer->data, string_new("Inserting: "));
		buffer->protected = buffer->data.size;
	}

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_M)) {
		buffer->mode = INPUT_MODE_ADD_MODIFIER;
		string_clear(&buffer->data);
		string_append(&buffer->data, string_new("Add Modifer: "));
		buffer->protected = buffer->data.size;
	}

	if (buffer->mode != INPUT_MODE_OFF) {
		char c;
		while ((c = GetCharPressed()) != 0) {
			string_addchar(&buffer->data, c);
		}
		if (IsKeyPressed(KEY_BACKSPACE) && buffer->data.size > buffer->protected) {
			string_remove(&buffer->data, buffer->data.size - 1);
		}

		if (IsKeyPressed(KEY_ENTER)) { 
			string inputStr = string_substr(&buffer->data, buffer->protected, buffer->data.size);
			char* input = string_charptr(&inputStr);
			string_destroy(inputStr);

			if (buffer->mode == INPUT_MODE_INSERT) {
				insert_node(buffer, info, input);
			} else if (buffer->mode == INPUT_MODE_ADD_MODIFIER) {
				add_modifier(buffer, info, input);
			}

			free(input);

			string_clear(&buffer->data);
			buffer->mode = INPUT_MODE_OFF;
		}
	}
}

int main() {
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	InitWindow(width, height, "Ember");
	SetTargetFPS(60);

	unsigned int fontSize = DEFAULT_FONT_SIZE;
	Font regular = LoadFontEx("assets/JetBrainsMono/JetBrainsMono-Regular.ttf", fontSize, NULL, 0);

	render_info info = render_info_new();

	input_buffer inputBuffer = (input_buffer){ string_new(""), 0, INPUT_MODE_OFF };

	while (!WindowShouldClose()) {
		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_EQUAL)) {
			fontSize *= 1.25;
			if (fontSize < MAX_FONT_SIZE) {
				regular = LoadFontEx(FONT_REGULAR_PATH, fontSize, NULL, 0);
			} else {
				regular = LoadFontEx(FONT_REGULAR_PATH, MAX_FONT_SIZE, NULL, 0);
			}
		}

		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_MINUS)) {
			fontSize /= 1.25;
			if (fontSize < MIN_FONT_SIZE) fontSize = MIN_FONT_SIZE;

			if (fontSize < MAX_FONT_SIZE) {
				regular = LoadFontEx(FONT_REGULAR_PATH, fontSize, NULL, 0);
			} else {
				regular = LoadFontEx(FONT_REGULAR_PATH, MAX_FONT_SIZE, NULL, 0);
			}
		}

		if (IsKeyPressed(KEY_UP)) {
			info.lod--;
			if (info.lod == LOD_LOWEST) {
				info.lod = LOD_LOWEST + 1;
			}
		}
		if (IsKeyPressed(KEY_DOWN)) {
			info.lod++;
			if (info.lod == LOD_HIGHEST) {
				info.lod = LOD_HIGHEST - 1;
			} 
		}

		handle_input(&inputBuffer, &info);

		if (inputBuffer.mode == INPUT_MODE_OFF && info.currentField != NULL) {
			if (IsKeyPressed(KEY_TAB)) {
				list* fields = list_new();
				root_node_get_fields(info.root, fields, &info.format);

				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					fields = list_reverse(fields);
				}

				for (list* field = fields; field != NULL; field = field->next) {
					if (info.currentField == field->data) {
						if (field->next != NULL) {
							info.currentField = field->next->data;
						} else {
							info.currentField = fields->data;
						}

						break;
					}
				}
			}

			char c;
			while ((c = GetCharPressed()) != 0) {
				string_addchar(&info.currentField->data, c);
			}

			if (IsKeyPressed(KEY_BACKSPACE) && info.currentField->data.size != 0) {
				string_remove(&info.currentField->data, info.currentField->data.size - 1);
			}

			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_W) && info.currentField != NULL) {
				string_clear(&info.currentField->data);	
			}
		}


		if (IsKeyPressed(KEY_F1)) {
			info.format.argFormat = ARGUMENT_FORMAT_C;
		}
		
		if (IsKeyPressed(KEY_F2)) {
			info.format.argFormat = ARGUMENT_FORMAT_RUST;
		}

		string output = root_node_render(info.root, &info);
		char* outputText = string_charptr(&output);
		char* inputBufferText = string_charptr(&inputBuffer.data);

		BeginDrawing();
		{
			ClearBackground(ZINC900);
			DrawTextEx(regular, outputText, (Vector2){190, 200}, fontSize, 0, RAYWHITE);

			DrawRectangle(0, GetScreenHeight() - (fontSize + 32), GetScreenWidth(), fontSize + 32, ZINC800);
			DrawTextEx(regular, inputBufferText, (Vector2){ 16, GetScreenHeight() - (fontSize + 16) }, fontSize, 0, RAYWHITE);
			
		}
		EndDrawing();
		
		free(inputBufferText);
		free(outputText);
		string_destroy(output);
	}

	CloseWindow();

	return 0;
}
