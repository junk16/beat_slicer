#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

#define WINDOW_SIZE 1024
#define OVERLAP_RATIO 1
#define LOCAL_ENERGY_WINDOW_SIZE 43
#define SENSITIVITY 130.0
typedef struct slicer_energy_history{
  int   size;
  float *energies;

} t_energies;

void calc_energy_history(t_energies *energies, PCM *pcm, int window_size);
float calc_local_energy(int energy_history_index, t_energies energies);

int main(void)
{
  PCM pcm;
  int n, i;
  int   step = WINDOW_SIZE / OVERLAP_RATIO;
  int location;
  int locaton_size = 0;
  int flag = 0;
  float local = 0;
  float c = SENSITIVITY / 100.0;
  read_wave(&pcm, "01.wav"); /* WAVEファイルからステレオの音データを入力する */
  t_energies energies; // = {NULL, 0};


  printf("%d\n", pcm.length);
  //printf("%d\n", pcm.buffer);

/*
  for(n = 0; n < pcm.length ; n++){

   printf("%d:%f\n", n, pcm.buffer[n]);
  }
*/


  calc_energy_history(&energies, &pcm, WINDOW_SIZE);

  for(i = 0; i < energies.size; i++){
    //printf("%d : %f\n", i, energies.energies[i]);
    local = calc_local_energy(i, energies);

    if(energies.energies[i] > c * local){
      location = i * step;
      if(flag != 1){
          printf("localtion : %d\n", location);
          locaton_size++;
          flag = 1;
      }

    }else{
      flag = 0;
    }
    //printf("energy index : %d > %f\n", i, local);
  }
  printf("%d\n", energies.size);
  printf("%d\n", locaton_size);
  printf("%d\n", pcm.length / 2);
  free(pcm.buffer); /* メモリの解放 */
  free(energies.energies);
  return 0;
}

float calc_local_energy(int energy_history_index, t_energies energies){
  int enerygies_size = energies.size;
  int from = 0;
  int to   = LOCAL_ENERGY_WINDOW_SIZE;
  int i = 0;
  int real_local_energy_window_size = 0;
  float sum = 0.0;


  if(energy_history_index  == 0){
      from = 0;
      to = LOCAL_ENERGY_WINDOW_SIZE;

  } else if(energy_history_index < LOCAL_ENERGY_WINDOW_SIZE){
      from = energy_history_index;
      to   = energy_history_index + LOCAL_ENERGY_WINDOW_SIZE;
    //  printf("%d", energy_history_index);
  } else if(energy_history_index + LOCAL_ENERGY_WINDOW_SIZE < energies.size) {
      from = energy_history_index;
      to = energy_history_index + LOCAL_ENERGY_WINDOW_SIZE;
  }else {
      from = energy_history_index; //energies.size - LOCAL_ENERGY_WINDOW_SIZE;

      if(energy_history_index + LOCAL_ENERGY_WINDOW_SIZE < energies.size){
        to = energy_history_index + LOCAL_ENERGY_WINDOW_SIZE; //energies.size;
      }else{
        to = energies.size;
      }

  }

  for(i = from; i < to; i++){
    sum = sum + pow(energies.energies[i], 1.0);
    real_local_energy_window_size++;
  }
//  printf("energy index : %d > from :%d > to :%d > sum:%f\n", energy_history_index, from, to, sum / real_local_energy_window_size);

  return sum / real_local_energy_window_size;
}


void calc_energy_history(t_energies *energies, PCM *pcm, int window_size){
  int   step = window_size / OVERLAP_RATIO;
  int   energy_frame_num = pcm->length / 2 / step;
  int   window_index = 0;
  int   i, j; //counter
  //float  *energy;
  float  sum;
  //assert(); //framenumber < 1

  energies->energies = calloc(energy_frame_num, sizeof(float));
  //printf("-----------------\n");

  for(i = 0; i + window_size < pcm->length / 2; i += step){
    sum = 0;
    printf("-----------------\n");
    for(j = 0; j < window_size; j += 2){

      printf("index %d : l > %f : r > %f\n", i + j, pcm->buffer[i + j], pcm->buffer[i + j + 1]);
      sum = sum + pow(pcm->buffer[i + j], 2) + pow(pcm->buffer[i + j + 1], 2);
    }
    energies->energies[window_index++] = sum;
    printf("%f\n", sum);
  }
  energies->size = window_index;
  for(i = 0; i < window_index; i++){
    printf("%d : %f\n", i, energies->energies[i]);
  }

  printf("FINISH WINDOW\n");

}
