#include <lcom/lcf.h>
#include <stdio.h>
#include "graphics.h"
#include "timer.h"
#include "sprites.h"
#include "keyboard.h"
#include "gameManager.h"

int main(int argc, char *argv[]) {

    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/g02/proj/src/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/g02/proj/src/output.txt");

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

    bool pressed_space = false;

    u_int8_t *map;
        xpm_image_t img;
        map = xpm_load((xpm_map_t)testSprite, XPM_8_8_8_8, &img);
    
    printf("%x \n", (unsigned long)TRANSPARENCY_COLOR_8_8_8_8);
    printf("%x \n", xpm_transparency_color(XPM_8_8_8_8));
    printf("%d \n", vg_info.bits_per_pixel);

    printf("1: %ld|", (int32_t)*img.bytes);
    printf("%ld|", (int32_t)*(img.bytes + 1 * 4));
    printf("%ld|", (int32_t)*(img.bytes + 2 * 4));
    printf("%ld|", (int32_t)*(img.bytes + 3 * 4));
    printf("%ld \n", (uint32_t)*img.bytes == 254);
    

    //main loop
    while(scancode != ESC_BREAKCODE) {
    
        //if (is_scancode_complete)
            //kbc_print_scancode_wrapper();

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
                    

                    if (scancode == A_MAKECODE) {
                        PlayerLeftPressed(player);
                    } else if (scancode == A_BREAKCODE) {
                        PlayerLeftReleased(player);
                    } else if (scancode == D_MAKECODE) {
                        PlayerRightPressed(player);
                    } else if (scancode == D_BREAKCODE) {
                        PlayerRightReleased(player);
                    }

                    if (scancode == SPACE_MAKECODE) {
                        pressed_space = true;
                    }
                    if (pressed_space)
                     PlayerChargeJump(player);
                     
                    if (scancode == SPACE_BREAKCODE && pressed_space) {
                        PlayerJump(player);
                        draw_rectangle(40, 30, 10, 10, 0x0000ff);
                        pressed_space = false;
                    }

                    

                    u_int8_t *map;
                    xpm_image_t img;
                    map = xpm_load(PlayerGetSprite(player), XPM_8_8_8_8, &img);
                    
                    //if (draw) {
                    //} 


                    PhysicsLoop(manager);
                    DrawLevel(manager);
                    draw_xpm(img, PlayerGetSpriteX(player), PlayerGetSpriteY(player), map);


                    draw_rectangle(100, 25, 10, 10, 0xff0000);

                    draw_rectangle(115, 25, 10, 10, 0xff0000);

                    draw_rectangle(115, 10, 10, 10, 0xff0000);

                    draw_rectangle(130, 25, 10, 10, 0xff0000);

                    draw_rectangle(160, 25, 40, 10, 0xff0000);


                    if (PlayerGetLeftKey(player)) {
                        draw_rectangle(100, 25, 10, 10, 0x00ff00);
                    }

                    if (PlayerGetRightKey(player)) {
                        draw_rectangle(130, 25, 10, 10, 0x00ff00);
                    }

                    if (pressed_space) {
                        draw_rectangle(160, 25, 40, 10, 0x00ff00);
                    }

                    //draw_rectangle(PlayerGetX(player), PlayerGetY(player), PlayerGetColliderWidth(player), PlayerGetColliderHeight(player), 0x6666ff);
                    
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
