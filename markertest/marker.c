#include "marker.h"



struct_markers *add_marker(float frame, struct_markers *head){

  struct_markers *m;

  m = (struct_markers *) malloc(sizeof(struct_markers));

  if(m == NULL) exit(EXIT_FAILURE);

  m->start_frame = frame;
  m->next = head;
  head = m;

  return head;
}


void free_marker(struct struct_markers *markers){

  struct_markers *m;

  while(markers != NULL){
    m = markers->next;
    free(markers);
    markers = m;
  }
}
