#include <lcom/lcf.h>
#include <stdio.h>

#include "timer.h"


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
    int timer_hook = TIMER0_IRQ;

    int timer_mask = BIT(timer_hook);

    if (timer_subscribe(&timer_hook)) {
        printf("ERROR: Failed to subscribe to Timer0 \n");
        return 1;
    }

    int r;
    int ipc;
    message msg;

    //Por si, o timer já está a 60Hz, portanto, num segundo vai interromper 60 vezes;
    int time = 400; //variável temporária!

    bool playing = true;

    //main loop
    while(playing) {

        if ( (r = driver_receive(ANY, &msg, &ipc)) != 0) {
            printf("Function driver_receive fails with: %d \n", r);
            continue;
        }
        if (is_ipc_notify(ipc)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                if (msg.m_notify.interrupts & timer_mask) {
                    
                    //timer_int_handler();
                    
                    --time;

                    printf("%d \n", time);

                    if (time <= 0) playing = false;

                    //graphics_handling();
                    
                }
                break;
                default:
                break;
            }
        }
    }

    if (timer_unsubscribe(&timer_hook)) {
        printf("ERROR: Failed to unsubscribe to Timer0 \n");
        return 1;
    }


    printf("Program terminated");
    return 0;
}
