#include <lcom/lcf.h>
#include <stdio.h>
#include "graphics.h"
#include "timer.h"
#include "sprites.h"
#include "keyboard.h"
#include "gameManager.h"

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
    
    
    //bool draw = true;

    int kbd_hook = KBD_IRQ;

    int kbd_mask = BIT(kbd_hook);

    if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook) != OK) {
        printf("Failed to subscribe to keyboard. \n");
        return 1;
    }


    int r;
    int ipc;
    message msg;

    struct Player* player = PlayerCreate(200, 50);

    struct Manager* manager = GameManagerCreate(player, vg_info.x_res, vg_info.y_res);


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
                    }

                    analyse_scancode();
                    
                    graphics_handling();
                    

                    if (scancode == SPACE_MAKECODE) {
                        PlayerChargeJump(player);
                    }
                    if (scancode == SPACE_BREAKCODE) {
                        PlayerJump(player);
                        draw_rectangle(40, 30, 10, 10, 0x0000ff);
                    }

                    if (scancode == A_MAKECODE || scancode == A_MAKECODE)
                        PlayerIntent(player, -1);
                    if (scancode == D_MAKECODE || scancode == D_MAKECODE)
                        PlayerIntent(player, 1);
                    
                    //draw_rectangle(plat->x, plat->y, plat->width, plat->height, 0x000005);

                    //draw_xpm(PlayerGetSprite(player), PlayerGetX(player), PlayerGetY(player)); //NOT WORKING YET, CANT LOAD XPM

                    //drawPlayer(player);

                    

                    u_int8_t *map;
                    xpm_image_t img;
                    map = xpm_load(PlayerGetSprite(player), XPM_8_8_8_8, &img);
                    
                    //if (draw) {
                    DrawLevel(manager);
                    draw_xpm(img, PlayerGetX(player), PlayerGetY(player), map);
                    //} 


                    PhysicsLoop(manager);

                    
                    //if (draw) {
                    refresh_screen();
                    //     draw = false;
                    // } else {
                    //     draw = true;
                    // }
                    
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

    uint32_t value;
    //This sys_inb to the DATA_PORT will make sure our program doesn't leave anything there making it never finish.
    sys_inb(0x60, &value);

    printf("Program terminated");
    return 0;
}
