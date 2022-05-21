#include <lcom/lcf.h>
#include "math.h"
#include <stdint.h>



static char *video_mem;	
static vbe_mode_info_t info;	 //Process (virtual) address to which VRAM is mapped 

//static phys_bytes video_phys;
static unsigned int video_phys;

static uint16_t h_res;	        // Horizontal resolution in pixels 
static uint16_t v_res;	        // Vertical resolution in pixels 
static int bytes_per_pixel;  //Number of VRAM bits per pixel 


unsigned get_hres(){
    return h_res;
}
unsigned get_vres(){
    return v_res;
}
unsigned get_bpp(){
    return bytes_per_pixel;
}

int (vg_init_vals)(uint16_t mode){

    //inicializa o modo
    if(vbe_get_mode_info(mode,&info)) return 1;

    //Load das Globais
    h_res = info.XResolution;
    v_res = info.YResolution;
    bytes_per_pixel = ceil(info.BitsPerPixel/(double)8);

    //buf
    video_phys = info.PhysBasePtr;

    return 0;
}

void *(vg_init)(uint16_t mode){

    //Nos SLIDES
    struct reg86 r;
    //inicialização das Vars Glovais
    vg_init_vals(mode);
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

int (min)(int x, int y){
    return x < y ? x:y;
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

///Funções já implementadas
// int (vg_exit)(void);

// int (vg_display_vbe_contr_info)(struct vg_vbe_contr_info *info_p)

