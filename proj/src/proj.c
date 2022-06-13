#include <lcom/lcf.h>
#include <stdio.h>
#include "timer.h"
#include "img/sprites.h"
#include "keyboard.h"
#include "gameManager.h"
#include "graphics.h"
#include "mouse_func.h"
#include "i8254.h"
#include "menu.h"
#include "img/mouse.xpm"
#include "img/pause.xpm"

enum GAME_STATE {MAIN_MENU, PLAYING, PAUSE, WON};

enum GAME_STATE game_state = MAIN_MENU;

xpm_image_t mouse_loaded_xpm;

xpm_image_t pause_loaded_xpm;


int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

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
    uint8_t timer_hook = TIMER0_IRQ;

    int timer_mask = BIT(timer_hook);


    if (timer_subscribe_int(&timer_hook)) {
        printf("ERROR: Failed to subscribe to Timer0 \n");
        return 1;
    }
    
    if (initialize_graphics() == 0) {
       vg_exit();
    }
    
    loadMenu();
    loadWinMenu();

    int kbd_hook = KBD_IRQ;

    int kbd_mask = BIT(kbd_hook);

    if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook) != OK) {
        printf("Failed to subscribe to keyboard. \n");
        return 1;
    }

    /* MOUSE */

    xpm_load((xpm_map_t) mouse_xpm, XPM_8_8_8_8, &mouse_loaded_xpm);
    xpm_load((xpm_map_t) paused_xpm, XPM_8_8_8_8, &pause_loaded_xpm);

    int mouse_hook = MOUSE_IRQ;
    int mouse_mask = BIT(mouse_hook);

    mouse_x = 600;
    mouse_y = 420;
    mouse_pressed = false;


    sys_outb(WRITE_CMD_CTRLREG, MOUSE_WRITE_BYTE);
    sys_outb(DATA_PORT, MOUSE_SET_STREAM_MODE);

    sys_outb(WRITE_CMD_CTRLREG, MOUSE_WRITE_BYTE);
    sys_outb(DATA_PORT, MOUSE_STATUS_REQUEST);

    uint32_t response;

    sys_inb(DATA_PORT, &response);

    uint8_t cmd = 0;

    cmd |= CMD_EN_INT_MOUSE;

    if (mouse_enable_data_reporting() != 0) return 1;

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook) != OK) {
        printf("Failed to subscribe to mouse. \n");
        return 1;
    }

    /* MOUSE */


    int r;
    int ipc;
    message msg;

    struct Player* player = PlayerCreate(600, 400);
    bool pressed_space = false;

    struct Manager* manager = GameManagerCreate(player, vg_info.x_res, vg_info.y_res);

    bool looping = true;

    //main loop
    while(looping) {
        if ( (r = driver_receive(ANY, &msg, &ipc)) != 0) {
            printf("Function driver_receive fails with: %d \n", r);
            continue;
        }
        if (is_ipc_notify(ipc)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                if (msg.m_notify.interrupts & timer_mask) {
                    if (game_state == MAIN_MENU) {
                        Menu(vg_info.x_res, vg_info.y_res);
                        checkOptions(mouse_x, mouse_y);
                        draw_loaded_xpm(mouse_loaded_xpm, mouse_x, mouse_y);
                        if (mouse_pressed) {
                            switch(clickOption(mouse_x, mouse_y)) {
                                case 0: 
                                    game_state = PLAYING;
                                    break;
                                case 1:
                                    looping = false;
                                    break;
                            }
                            mouse_pressed = false;
                        }
                    } else if (game_state == WON) {
                        WinMenu(vg_info.x_res, vg_info.y_res);
                    } 
                    else {

                        if (pressed_space)
                            if (game_state != PAUSE) 
                                PlayerChargeJump(player);

                        DrawLevel(manager);
                        if (game_state != PAUSE) {
                            if (PhysicsLoop(manager)) {
                                game_state = WON;
                                player = PlayerCreate(600, 400);
                                manager = GameManagerCreate(player, vg_info.x_res, vg_info.y_res);
                            }
                        }

                        if (game_state == PAUSE) draw_loaded_xpm(pause_loaded_xpm, 112, 94);
                        //draw_xpm(img, PlayerGetSpriteX(player), PlayerGetSpriteY(player), map);

                        draw_loaded_xpm(PlayerGetSprite(player), PlayerGetSpriteX(player), PlayerGetSpriteY(player));

                        // draw_rectangle(100, 25, 10, 10, 0xff0000); //DEBUG:

                        // draw_rectangle(115, 25, 10, 10, 0xff0000); //DEBUG:

                        // draw_rectangle(115, 10, 10, 10, 0xff0000); //DEBUG:

                        // draw_rectangle(130, 25, 10, 10, 0xff0000); //DEBUG:

                        // draw_rectangle(160, 25, 40, 10, 0xff0000); //DEBUG:

                        // if (PlayerGetLeftKey(player)) {
                        //     draw_rectangle(100, 25, 10, 10, 0x00ff00); //DEBUG:
                        // }

                        // if (PlayerGetRightKey(player)) {
                        //     draw_rectangle(130, 25, 10, 10, 0x00ff00); //DEBUG:
                        // }

                        // if (pressed_space) {
                        //     draw_rectangle(160, 25, 40, 10, 0x00ff00); //DEBUG:
                        // }
                    }
                        
                    refresh_screen();
                }
                if (msg.m_notify.interrupts & kbd_mask) {
                    get_scancode();
                    // Handle Input
                    analyse_scancode();
                    if (is_scancode_complete) {

                        if (scancode == A_MAKECODE || scancode == LEFT_ARROW_MAKECODE) {
                            PlayerLeftPressed(player);
                        } else if (scancode == A_BREAKCODE || scancode == LEFT_ARROW_BREAKCODE) {
                            PlayerLeftReleased(player);
                        } else if (scancode == D_MAKECODE || scancode == RIGHT_ARROW_MAKECODE) {
                            PlayerRightPressed(player);
                        } else if (scancode == D_BREAKCODE || scancode == RIGHT_ARROW_BREAKCODE) {
                            PlayerRightReleased(player);
                        }

                        if (scancode == SPACE_MAKECODE) {
                            pressed_space = true;
                        }

                        if (scancode == ESC_BREAKCODE) {
                            if (game_state == MAIN_MENU)
                                looping = false;
                            else if (game_state == PAUSE)
                                game_state = MAIN_MENU;
                            else if (game_state == PLAYING)
                                game_state = MAIN_MENU;
                            else if (game_state == WON) {
                                game_state = MAIN_MENU;
                            }
                        }

                        if (scancode == P_BREAKCODE) {
                            if (game_state == PLAYING) {
                                game_state = PAUSE;
                            } else if (game_state == PAUSE) {
                                game_state = PLAYING;
                            }
                        }
                        
                        if (scancode == SPACE_BREAKCODE && pressed_space && game_state != PAUSE) {
                            PlayerJump(player);
                            //draw_rectangle(40, 30, 10, 10, 0x0000ff); //DEBUG:
                            pressed_space = false;
                        }
                    }
                }
                if (msg.m_notify.interrupts & mouse_mask) {
                    //handler
                    mouse_ih();

                    if (mouse_ih_error)
                        return 1;
        
                    if (mouse_data_handler())
                        return 1;

                    // if (is_mouse_packet_complete) {
                    //     mouse_print_packet(&mouse_parsed_packet);
                    // }
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

    sys_irqrmpolicy(&kbd_hook);

    sys_irqrmpolicy(&mouse_hook);

    sys_outb(WRITE_CMD_CTRLREG, MOUSE_WRITE_BYTE);
    sys_outb(DATA_PORT, MOUSE_DATA_REP_DISABLE);
    uint32_t value;
    //This sys_inb to the DATA_PORT will make sure our program doesn't leave anything there making it never finish.
    sys_inb(DATA_PORT, &value);

    sys_outb(WRITE_CMD_CTRLREG, MOUSE_WRITE_BYTE);
    sys_outb(DATA_PORT, MOUSE_RESET);

    printf("Program terminated");
    return 0;
}
