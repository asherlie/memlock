#include <vmem_access.h>

bool strtoi(const char* str, int* i){
      char* res;
      if(i)*i = (int)strtol(str, &res, 10);
      else strtol(str, &res, 10);
      return !*res;
}

bool strtop(const char* str, void** p){
      char* res;
      if(p)*p = (void*)strtoul(str, &res, 16);
      else strtoul(str, &res, 16);
      return !*res;
}

int main(int argc, char* argv[]){
      // TODO: check for invalid/no root with mem_rgn_warn 
      pid_t pid;
      bool ps = argc >= 2 && strtoi(argv[1], &pid);
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
      if(ch == 'q')printf("%i locks have been removed\n", free_locks(&lc));
      else if(lc.n > 0){
            printf("%i locks in place\nto remove locks, enter the following:\nkill -9 ", lc.n);
            // can use n because removal is not possible in memlock
            for(int i = 0; i < lc.n; ++i)printf(" %i", lc.locks[i].pid);
            puts("");
      }
      return 0;
}
