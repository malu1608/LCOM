#include <lcom/lcf.h>
#include <stdio.h>


int main(int argc, char *argv[]) {

    if (lcf_start(argc, argv))
    return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(proj_main_loop)(int argc, char* argv[])
{
    /* your main loop code here */
    printf("Hello World!");
    return 0;
}
