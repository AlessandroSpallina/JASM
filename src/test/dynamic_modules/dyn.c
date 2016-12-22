#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char **argv) {
        void *handle;
        char *mod_name[3] = {"init", "routine", "end"};
        void (*pointer[3])();
        char *error;

        handle = dlopen ("./module.so", RTLD_LAZY);
        if (!handle) {
                fputs (dlerror(), stderr);
                exit(1);
        }

        for(int i=0; i<3; i++) {
          pointer[i] = dlsym(handle, mod_name[i]);
          if ((error = dlerror()) != NULL)  {
                  fputs(error, stderr);
                  exit(1);
          }
        }

        for(int i=0; i<3; i++)
          (*pointer[i])();

        printf ("Done\n");
        dlclose(handle);
}
