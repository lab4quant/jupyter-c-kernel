#include <stdlib.h>
#include <stdio.h>
//#include <dlfcn.h>
#include <Windows.h>

typedef int (*main_t)(int, char **, char **);

int main(int argc, char **argv, char **envp)
{
    char *error = NULL;

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s PROGRAM\nWhere PROGRAM is the user's program to supervise\n", argv[0]);
        return EXIT_FAILURE;
    }
    //void *userhandle = dlopen(argv[1], RTLD_LAZY);
    void *userhandle = LoadLibrary(argv[1]);
    if (userhandle == NULL) {
        //fprintf(stderr, "%s: %s\n", argv[0], dlerror());
        fprintf(stderr, "%s: %s\n", argv[0], GetLastError());
        return EXIT_FAILURE;
    }
    //dlerror();
    //main_t usermain = dlsym(userhandle, "main");
    main_t usermain = GetProcAddress(userhandle, "main");
    if ((error = GetLastError()) != NULL) {
        fprintf(stderr, "%s: %s\n", argv[0], error);
        return EXIT_FAILURE;
    }

    /* Call Users main, but make master.c invisible by removing first argument */
    return usermain(argc-1, argv+1, envp);
}
