#include <lcom/lcf.h>
#include <stdio.h>
#include "graphics.h"
#include "timer.h"
#include "player.h"
#include "physicObj.h"
#include "sprites.h"
#include "keyboard.h"


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

    
    if (initialize_graphics() == 0) {
        vg_exit();
    }
    
    

    int kbd_hook = KBD_IRQ;

    int kbd_mask = BIT(kbd_hook);

    if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook) != OK) {
        printf("Failed to subscribe to keyboard. \n");
        return 1;
    }


    int r;
    int ipc;
    message msg;


    //bool playing = true;


    struct Player* player = PlayerCreate(200, 50);



    //main loop
    while(scancode != ESC_BREAKCODE) {
        //printf("ON LOOP");
        if (is_scancode_complete)
            kbc_print_scancode_wrapper();

        if ( (r = driver_receive(ANY, &msg, &ipc)) != 0) {
            printf("Function driver_receive fails with: %d \n", r);
            continue;
        }
        if (is_ipc_notify(ipc)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                if (msg.m_notify.interrupts & timer_mask) {
                    if (get_scancode()) {
                        if (tickdelay(micros_to_ticks(KBC_WAIT))) {
                            return 1;
                        }
                        continue;
                    }

                    analyse_scancode();
                    


                    graphics_handling();

                    //draw_rectangle((uint16_t) time + 201, 200, 40, 80, 0x000000);

                    //draw_rectangle((uint16_t) time + 200, 200, 40, 80, 0x000003);

                    //drawing a temporary player
                    
                    draw_rectangle(PlayerGetX(player), PlayerGetY(player), 96, 96, 0x000002);

                    if (scancode == SPACE_MAKECODE) {
                        PlayerChargeJump(player);
                    }
                    if (scancode == SPACE_BREAKCODE) {
                        PlayerJump(player);
                    }

                    if (scancode == A_MAKECODE || scancode == A_MAKECODE)
                        PlayerMove(player, -5, 0);
                    if (scancode == D_MAKECODE || scancode == D_MAKECODE)
                        PlayerMove(player, 5, 0);
                    
                    //draw_rectangle(PlayerGetX(player), PlayerGetY(player), PlayerGetWidth(player), PlayerGetHeight(player), 0x000005);

                    //draw_xpm(PlayerGetSprite(player), PlayerGetX(player), PlayerGetY(player)); //NOT WORKING YET, CANT LOAD XPM

                    //drawPlayer(player);

                    
                    
                }
                if (msg.m_notify.interrupts & kbd_mask) {
                    get_scancode();
                    analyse_scancode();
                    if (is_scancode_complete) {
                        if (kbc_print_scancode_wrapper()) {
                            return 1;
                        }
                    }
                }
                break;
                default:
                break;
            }
        }
    }

    vg_exit();

    if (timer_unsubscribe(&timer_hook)) {
        printf("ERROR: Failed to unsubscribe to Timer0 \n");
        return 1;
    }

    sys_irqrmpolicy(&kbd_hook);

    printf("Program terminated");
    return 0;
}
