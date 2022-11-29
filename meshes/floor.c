#include <psxgte.h>
#include "../src/display.h"
int floor_faces = 3;

SVECTOR floor_vertices[] = {
    {-500,0,-500},
    {500,0,-500},
    {-500,0,500},
    {500,0,500},
    {1500,0,-500},
    {1500,0,500},
    {500,0,1500},
    {1500,0,1500}
};

INDEX floor_vertex_indices[] = {
    {0,1,2,3},
    {3,1,5,4},
    {3,5,6,7}
};

SVECTOR floor_normals[] = {
    {0,-4096,0}
};

INDEX floor_normal_indices[] = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

SVECTOR floor_uvs[] = {
    {0,0},
    {63,0},
    {63,63},
    {0,63},
    {63,0},
    {63,63},
    {63,63},
    {63,63}
};

INDEX floor_uv_indices[] = {
    {0,1,3,2},
    {2,1,5,4},
    {2,5,7,6}
};







void fillMesh_floor(MESH* mesh) {
  mesh->faces_num = floor_faces;

  mesh->normal_data = &floor_normals;
  mesh->normal_indices = &floor_normal_indices;
  mesh->uv_data = &floor_uvs;
  mesh->uv_indices = &floor_uv_indices;
  mesh->vertex_data = &floor_vertices;
  mesh->vertex_indices = &floor_vertex_indices;
}
