// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/pp.h>

#include <stdint.h>

/* 
1º - Configure the video card to operate in the mode specified in its
first argument
2º map its frame-buffer
3º Then draw a line (either horizontal or vertical)
4º Sleep for delay seconds
5º Switch back to Minix’s text mode

*/

// Any header files included below this line should have been created by you


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/pp/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/pp/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

static char *video_mem;

static uint16_t h_res;
static uint16_t v_res;
static int bytes_per_pixel;

static uint16_t RED_MASK_SIZE;
static uint16_t RED_FIELD_POS;
static uint16_t GREEN_MASK_SIZE;
static uint16_t GREEN_FIELD_POS;
static uint16_t BLUE_MASK_SIZE;
static uint16_t BLUE_FIELD_POS;

static unsigned int video_phys;

lpv_mode_info_t  lmi_p;


void draw_pixel(uint16_t x, uint16_t y, uint32_t color){
    char * init = ((char *)video_mem +(y*h_res+x)*bytes_per_pixel);

    uint8_t cl = (uint8_t) color;
    *init = cl;
    for(int i = 0; i < bytes_per_pixel; i++){
        *init = color;
        init++;
        color = color >> 8;
    }
}

void vg_init(uint16_t mode, lpv_mode_info_t * lmi_p){
    struct reg86 r86;

    lpv_get_mode_info(mode, lmi_p);

    h_res = lmi_p.x_res;
    v_res = lmi_p.y_res;
    bytes_per_pixel = ceil(lmi_p.bpp/(double)8);
    RED_MASK_SIZE = lmi_p.r_sz;
    RED_FIELD_POS = lmi_p.r_pos;
    GREEN_MASK_SIZE = lmi_p.g_sz;
    GREEN_FIELD_POS = lmi_p.g_pos;
    BLUE_MASK_SIZE = lmi_p.b_sz;
    BLUE_FIELD_POS = lmi_p.b_pos;

    video_phys = lmi_p.phys_addr;

    /* Map memory */

    video_mem = video_map_phys(video_phys, h_res * v_res * ((lmi_p.bpp+7)/8));
    
}

int pp_test_line(uint8_t mode, enum lpv_dir_t dir, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay){
    vg_init(mode, lmi_t);
    if(dir == lpv_hor){
        for(int i = 0; i < len; i++){
            draw_pixel(x+i, y, color);
        }
    }
    if(dir == lpv_vert){
        for(int i = 0; i < len; i++){
            draw_pixel(x, y+i, color);
        }
    }
    sleep(delay);
    lpv_set_mode(0);
}
