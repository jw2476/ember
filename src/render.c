#include "render.h"

#include "nodes/node.h"
#include "nodes/root.h"

render_info render_info_new() {
    render_info info;
    info.format.argFormat = ARGUMENT_FORMAT_DEFAULT;
    info.currentField = NULL;
    info.root = root_node_new();
    info.lod = LOD_IMPLEMENTATIONS;

    return info;
}
