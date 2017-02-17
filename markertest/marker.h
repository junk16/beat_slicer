#include <stdlib.h>

typedef struct struct_markers {
  float start_frame;
  struct struct_markers *next;
} struct_markers;

struct struct_markers *add_marker(float frame, struct struct_markers *head);
void free_marker(struct struct_markers *markers);
