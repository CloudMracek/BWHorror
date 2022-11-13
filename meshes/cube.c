#include <psxgte.h>
#include "../src/display.h"

int cube_num_faces = 6;

const SVECTOR cube_verts[] = {
  {75,-75,75},
  {75,-75,-75},
  {75,75,75},
  {75,75,-75},
  {-75,-75,75},
  {-75,-75,-75},
  {-75,75,75},
  {-75,75,-75}
};

const SVECTOR cube_norms[] = {
  {0.0,-0.0,1.0},
  {0.0,-1.0,0.0},
  {-1.0,-0.0,0.0},
  {0.0,-0.0,-1.0},
  {1.0,-0.0,0.0},
  {0.0,1.0,0.0}
};

const INDEX cube_vertex_indices[] = {
  {0,4,2,6},
  {3,2,7,6},
  {7,6,5,4},
  {5,1,7,3},
  {1,0,3,2},
  {5,4,1,0}
};

const INDEX cube_uv_indices[] = {
  {0,1,3,2},
  {4,5,6,2},
  {7,8,10,9},
  {11,12,6,13},
  {14,15,17,16},
  {11,1,19,18}
};

const int cube_normal_indices[] = {
2,
7,
5,
7,
3,
1};

const SVECTOR cube_uv[] = {
  {0,0},
  {127,0},
  {127,127},
  {0,127},
  {0,0},
  {127,0},
  {0,127},
  {0,0},
  {127,0},
  {127,127},
  {0,127},
  {0,0},
  {127,0},
  {127,127},
  {0,0},
  {127,0},
  {127,127},
  {0,127},
  {127,127},
  {0,127}
};

void fillMesh_cube(MESH* mesh) {
  mesh->faces_num = cube_num_faces;

  mesh->normal_data = &cube_norms;
  mesh->normal_indices = &cube_normal_indices;
  mesh->uv_data = &cube_uv;
  mesh->uv_indices = &cube_uv_indices;
  mesh->vertex_data = &cube_verts;
  mesh->vertex_indices = &cube_vertex_indices;
}
