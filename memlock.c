#include "vmem_access.h"

int main(int argc, char* argv[]){
      // TODO: check for invalid/no root with mem_rgn_warn 
      bool ps = argc >= 2;
      pid_t pid;
      if(ps)pid = atoi(argv[1]);
      void* addr = 0x0; void* pa = 0x0;
      int val = 0, pv = 0;
      struct lock_container lc;
      lock_container_init(&lc, 5); 
      puts("enter q at any time to kill all locks or Q to exit without killing locks");
      char ch = 0;
      do{
            if(!ps){
                  puts("enter pid, addr, val");
                  fscanf(stdin, "%i %p %i", &pid, &addr, &val);
            }
            else{
                  puts("enter addr, val");
                  fscanf(stdin, "%p %i", &addr, &val);
            }
            if(pa != addr || pv != val){
                  create_lock(&lc, pid, &addr, &val, NULL, 1, false, true, NULL);
                  printf("address %p in proccess %i locked to %i\n", addr, pid, val);
            }
            pa = addr; pv = val;
            ch = getchar();
      }while(ch != 'q' && ch != 'Q');
      if(ch == 'Q'){
            fputs("to remove locks, enter the following:\nkill -9 ", stdout);
            // can use n because removal is not possible
            for(int i = 0; i < lc.n; ++i)printf(" %i", lc.locks[i].pid);
            puts("");
      }
      else printf("%i locks have been removed\n", free_locks(&lc));
      return 0;
}
