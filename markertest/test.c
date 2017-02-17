#include "./marker.h"

#include <stdio.h>
int main(){

  struct_markers *markers;
  struct_markers *m;
  int i = 0;
  markers = NULL;

  for(i = 0; i < 100.0; i++){
    markers = add_marker(i, markers);
  }

  while(markers != NULL){
    printf("%f\n", markers->start_frame);
    m = markers->next;
    markers = m;
  }
  free_marker(markers);
}
