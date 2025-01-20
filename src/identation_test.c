#include <stdbool.h>
#include <stdlib.h>

#define dynarr_init(type)                                                      \
  struct dynarr_type_t {                                                       \
    type __dummy;                                                              \
    type *data;                                                                \
    size_t length;                                                             \
  }

typedef dynarr_init(int) dynarr_int_t;

dynarr_init(float);

typedef struct vec3_t {
  float x, y, z;
} vec3_t;

void get_lines_intersection(float x1, float y1, float z1, float x2, float y2,
                            float z2, vec3_t *intersection_point,
                            bool *has_intersection) {}
