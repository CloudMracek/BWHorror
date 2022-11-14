#include <psxgte.h>
#include "../src/display.h"

int cube_faces = 6;

SVECTOR cube_vertices[] = {
    {75,75,-75},
    {75,-75,-75},
    {75,75,75},
    {75,-75,75},
    {-75,75,-75},
    {-75,-75,-75},
    {-75,75,75},
    {-75,-75,75}
};

INDEX cube_vertex_indices[] = {
    {0,2,4,6},
    {3,7,2,6},
    {7,5,6,4},
    {5,7,1,3},
    {1,3,0,2},
    {5,1,4,0}
};

SVECTOR cube_normals[] = {
    {0,-4096,0},
    {0,0,-4096},
    {4096,0,0},
    {0,4096,0},
    {-4096,0,0},
    {0,0,4096}
};

INDEX cube_normal_indices[] = {
    {0,0,0,0},
    {1,1,1,1},
    {2,2,2,2},
    {3,3,3,3},
    {4,4,4,4},
    {5,5,5,5}
};

SVECTOR cube_uvs[] = {
    {0,0},
    {0,64},
    {64,64},
    {64,0},
    {0,0},
    {0,64},
    {64,0},
    {0,0},
    {0,64},
    {64,64},
    {64,0},
    {0,0},
    {64,64},
    {64,0},
    {0,0},
    {0,64},
    {64,64},
    {64,0},
    {0,64},
    {64,64}
};

INDEX cube_uv_indices[] = {
    {0,1,3,2},
    {4,5,6,2},
    {7,8,10,9},
    {11,5,13,12},
    {14,15,17,16},
    {11,18,3,19}
};



void fillMesh_cube(MESH* mesh) {
  mesh->faces_num = cube_faces;

  mesh->normal_data = &cube_normals;
  mesh->normal_indices = &cube_normal_indices;
  mesh->uv_data = &cube_uvs;
  mesh->uv_indices = &cube_uv_indices;
  mesh->vertex_data = &cube_vertices;
  mesh->vertex_indices = &cube_vertex_indices;
}
