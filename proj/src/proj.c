#include <lcom/lcf.h>
#include <stdio.h>
#include "graphics.h"
#include "timer.h"
#include "player.h"
#include "physicObj.h"
#include "keyboard.h"
#include "sprites.h"
#include "i8042.h"
#include "i8254.h"
#include "resources/PlayerSprites1.xpm"


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
    uint8_t timer_hook = TIMER0_IRQ;

    int timer_mask = BIT(timer_hook);

    if (timer_subscribe_int(&timer_hook)) {
        printf("ERROR: Failed to subscribe to Timer0 \n");
        return 1;
    }

    if (initialize_graphics() == 0) {
        vg_exit();
    }

    int r;
    int ipc;
    message msg;

    //Por si, o timer já está a 60Hz, portanto, num segundo vai interromper 60 vezes;
    int time = 400; //variável temporária!

    bool playing = true;

    initializeObject();

    struct Player* player = PlayerCreate(200, 50);


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

                    if (time <= 0) playing = false;
                        xpm_image_t img;
                    
                    u_int8_t *map;
                    map = xpm_load((xpm_map_t)PlayerSprites1_xpm, XPM_5_6_5, &img);
                    draw_xpm(img, PlayerGetX(player), PlayerGetY(player), map);
                    keyboard_scan_stop_key(0x81);
                    

                    graphics_handling();

                    //draw_rectangle((uint16_t) time + 201, 200, 40, 80, 0x000000);

                    //draw_rectangle((uint16_t) time + 200, 200, 40, 80, 0x000003);

                    PlayerMove(player, 1, 0);

                    
                    //draw_rectangle(PlayerGetX(player), PlayerGetY(player), PlayerGetWidth(player), PlayerGetHeight(player), 0x000005);

                    //draw_xpm(PlayerGetSprite(player), PlayerGetX(player), PlayerGetY(player)); NOT WORKING YET, CANT LOAD XPM

                    //drawPlayer(player);
                    
                }
                break;
                default:
                break;
            }
        }
    }

    vg_exit();

    if (timer_unsubscribe_int()) {
        printf("ERROR: Failed to unsubscribe to Timer0 \n");
        return 1;
    }

    printf("Program terminated");
    return 0;
}
