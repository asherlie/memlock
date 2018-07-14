#include "vmem_access.h"

int main(int argc, char* argv[]){
      if(argc < 4)return -1;
      pid_t pid = atoi(argv[1]);
      void* addr = (void*)strtoul(argv[2], NULL, 16);
      int v = atoi(argv[3]);
      struct lock_container lc;
      lock_container_init(&lc, 5);
      create_lock(&lc, pid, &addr, &v, NULL, 1, false, true, NULL);
      printf("address %p in proccess %i locked to %i\nlock has pid: %i\npress enter to remove locks", addr, pid, v, lc.locks[0].pid);
      while(getchar() != '\n');
      free_locks(&lc);
}
