#include <lcom/lcf.h>
#include "math.h"
#include "video_gr.h"
#include <stdint.h>
#include "../lab3/i8042.h"
#include "../lab2/i8254.h"
#include "../lab3/keyboard.h"
#include "../lab2/timer.h"



static char *video_mem;	
//static vbe_mode_info_t info;	 //Process (virtual) address to which VRAM is mapped 

//static phys_bytes video_phys;
static unsigned int video_phys;

static uint16_t h_res;	        // Horizontal resolution in pixels 
static uint16_t v_res;	        // Vertical resolution in pixels 
static int bytes_per_pixel;  //Number of VRAM bits per pixel 

/* Global variables for patterns */
static uint16_t RED_MASK_SIZE;
static uint16_t GREEN_MASK_SIZE;
static uint16_t BLUE_MASK_SIZE;
static uint16_t RED_FIELD_POS;
static uint16_t GREEN_FIELD_POS;
static uint16_t BLUE_FIELD_POS;


extern uint8_t scancode;
extern uint8_t stat;
extern uint32_t counter;

unsigned get_hres(){
    return h_res;
}
unsigned get_vres(){
    return v_res;
}
unsigned get_bpp(){
    return bytes_per_pixel;
}

int (vg_init_vals)(uint16_t mode, vbe_mode_info_t info){

    //inicializa o modo
    if(vbe_get_mode_info(mode,&info)) return 1;

    //Load das Globais
    h_res = info.XResolution;
    v_res = info.YResolution;
    bytes_per_pixel = ceil(info.BitsPerPixel/(double)8);

    RED_MASK_SIZE = info.RedMaskSize;
    GREEN_MASK_SIZE = info.GreenMaskSize;
    BLUE_MASK_SIZE = info.BlueMaskSize;
    RED_FIELD_POS = info.RedFieldPosition;
    GREEN_FIELD_POS = info.GreenFieldPosition;
    BLUE_FIELD_POS = info.BlueFieldPosition;

    //buf
    video_phys = info.PhysBasePtr;

    return 0;
}

void *(init)(uint16_t mode, vbe_mode_info_t *info){

    //Nos SLIDES
    struct reg86 r;
    //inicialização das Vars Glovais
    vg_init_vals(mode, *info);
    //alocar mem

    int n;
    static struct minix_mem_range mr;
    //memset(&mr, 0, sizeof(mr));  physical memory range 
    unsigned int vram_base = video_phys;   //VRAM’s physical addresss 
    //unsigned int vram_size;   VRAM’s size, but you can use the frame-buffer size, instead 
    unsigned int vram_size =  h_res * v_res * bytes_per_pixel;
     //Allow memory mapping 
    mr.mr_base = (phys_bytes) vram_base;
    //mr.mr_base = video_phys;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (n = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
        printf("Hey");
        panic("sys_privctl (ADD_MEM) failed: %d\n", n);
        return NULL;
    }
         //Map memory 
   video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
   //vg_init_vals(0x4F01);
   if(video_mem == MAP_FAILED){
       printf("No");
       panic("couldn’t map video memory");
       return NULL;
   }
    memset(&r, 0, sizeof(r));
    //printf("K");

    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    
    if( sys_int86(&r) != OK ) {  //call BIOS  
        printf("vg_init(): sys_int86() failed \n");
        return NULL;
    }


    return video_mem;
}
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    char *init = ((char*)video_mem + (y*h_res+x)*bytes_per_pixel);
    //printf("hey");
    uint8_t cl = (uint8_t) color;
    *init = cl;
    for(int i = 0; i < bytes_per_pixel;i++){
        *init = color;
        init++;
        color = color >> 8;
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    //uint16_t x_max = min(info.XResolution, x+len);
    for (int i = 0; i < len; i++){
        vg_draw_pixel(x+i, y, color);
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    //draw top line
    for (int i = 0; i < height; i++){
        if(vg_draw_hline(x,y+i,width,color)) return 1;
    }
    // draw bottom line
    //if(vg_draw_hline(x,y+height,width,color)) return 1;

    return 0;
}

int R_first(int first){
    return first >> RED_FIELD_POS & (BIT(RED_MASK_SIZE)-1);
}

int G_first(int first){
    return first >> GREEN_FIELD_POS & (BIT(GREEN_MASK_SIZE)-1);
}

int B_first(int first){
    return first >> BLUE_FIELD_POS & (BIT(BLUE_MASK_SIZE)-1);
}

void (vg_pattern)(vbe_mode_info_t *info, uint16_t mode,  uint8_t no_rectangles, int first, uint8_t step){
    int width = get_hres() / no_rectangles;
    int height = get_vres() / no_rectangles;

    int color = first;

    for(int i = 0; i < no_rectangles; i++){
        for(int j = 0; j < no_rectangles; j++){
            /* Indexed Mode */
            if(mode == 0x105){
                color = (first + (j * no_rectangles + i) * step) % (1 << (bytes_per_pixel*8));
            }
            /* Direct Mode */
            else{
                int red;
                int green;
                int blue;
                if(i == 0 && j == 0){
                    red = R_first(first);
                    green = G_first(first);
                    blue = B_first(first);
                }
                else{
                    red = (R_first(first) + i * step) % (1 << RED_MASK_SIZE);
                    green = (G_first(first) + j * step) % (1 << GREEN_MASK_SIZE);
                    blue = (B_first(first) + (i + j) * step) % (1 << BLUE_MASK_SIZE);
                }
                color = (red << RED_FIELD_POS | green << GREEN_FIELD_POS | blue << BLUE_FIELD_POS);
            }

            vg_draw_rectangle(i * width, j * height, width, height, color);
        }
    }
}

void draw_xpm(xpm_image_t img, uint16_t x, uint16_t y){
    int color_index = 0;

    for(uint16_t i = y; i < y + img.height; i++){
        for(uint16_t j = x; j < x + img.width; j++){
            
            color_index++;
        }
    }
}

void erase_xpm(xpm_image_t img, uint16_t x, uint16_t y){
    int color = xpm_transparency_color(XPM_INDEXED);

    for(uint16_t i = y; i < y + img.height; i++){
        for(uint16_t j = x; j < x + img.width; j++){
            vg_draw_pixel(j, i, color);
        }
    }
}

void move_xpm(xpm_image_t img, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate){
    
    /* Timer and Keyboard variables */ 
    uint8_t irq_set;
    uint8_t bit_no;
    int ipc_status;
    message msg;
    int r;

    int frame_count = 0; //variable to count the number of frames needed no displace 1 pixel

    timer_subscribe_int(&bit_no); //Subscribe timer
    keyboard_subscribe_int(&irq_set); //Subscribe keyboard
    
    while(scancode != ESC_BEAKCODE ) {
    /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & BIT(bit_no)){
                        util_sys_inb(KBC_ST_REG, &stat); //assuming it returns OK
                        //loop while 8042 output buffer is empty
                    }
                    kbc_ih();
                    if (msg.m_notify.interrupts &BIT(bit_no) && (xf != xi || yf != yi)) { /* subscribed interrupt */
                        timer_int_handler();
                        if(counter % (sys_hz() / fr_rate) == 0){
                            if(speed >= 0){
                                erase_xpm(img, xi, yi);

                                xi += speed;
                                yi += speed;

                                if(xi > xf){
                                    xi = xf;
                                }
                                if(yi > yf){
                                    yi = yf;
                                }

                                draw_xpm(img, xi, yi);
                            }else{
                                frame_count++;

                                if(frame_count == -1*speed){
                                    erase_xpm(img, xi, yi);

                                    xi += 1;
                                    yi += 1;

                                    if(xi > xf){
                                        xi = xf;
                                    }
                                    if(yi > yf){
                                        yi = yf;
                                    }

                                    draw_xpm(img, xi, yi);
                                    frame_count = 0;
                                }
                            }
                        }
                        
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else {
            /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    erase_xpm(img, xi, yi);

    sys_outb(KBC_OUT_BUF,0x01);
    keyboard_unsubscribe_int();
    timer_unsubscribe_int();
}


///Funções já implementadas
// int (vg_exit)(void);

// int (vg_display_vbe_contr_info)(struct vg_vbe_contr_info *info_p)

