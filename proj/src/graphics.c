#include "graphics.h"


void *buffer_base = NULL;

static uint8_t *second_buffer;

vbe_mode_info_summary_t vg_info;


void *initialize_graphics() {
    vbe_mode_info_t vbe_info;
    if (vbe_get_mode_info(0x14C, &vbe_info) != 0) {
        printf("Failed to obtain vbe_mode_info \n");
        return NULL;
    }

	if ((vbe_info.ModeAttributes & BIT(0)) == 0)
		return NULL;

	vg_info.x_res = vbe_info.XResolution;
	vg_info.y_res = vbe_info.YResolution;
	vg_info.vg_mode = vbe_info.MemoryModel;

	// If in direct mode, calculate the masks
	if (vbe_info.MemoryModel == VG_MODE_DIRECT) {

		vg_info.red_mask = 0;
		vg_info.red_field_position = vbe_info.RedFieldPosition;
		vg_info.red_mask_size = vbe_info.RedMaskSize;
		for (uint32_t i = vbe_info.RedFieldPosition; i < vbe_info.RedFieldPosition + vbe_info.RedMaskSize; ++i)
			vg_info.red_mask |= BIT(i); 
		
		vg_info.green_mask = 0;
		vg_info.green_field_position = vbe_info.GreenFieldPosition;
		vg_info.green_mask_size = vbe_info.GreenMaskSize;
		for (uint32_t j = vbe_info.GreenFieldPosition; j < vbe_info.GreenFieldPosition + vbe_info.GreenMaskSize; ++j)	
			vg_info.green_mask |= BIT(j); 
		
		vg_info.blue_mask = 0;
		vg_info.blue_field_position = vbe_info.BlueFieldPosition;
		vg_info.blue_mask_size = vbe_info.BlueMaskSize;
		for (uint32_t k = vbe_info.BlueFieldPosition; k < vbe_info.BlueFieldPosition + vbe_info.BlueMaskSize; ++k)
			vg_info.blue_mask |= BIT(k); 
	}

	vg_info.bits_per_pixel = vbe_info.BitsPerPixel;
	vg_info.bytes_per_pixel = (vg_info.bits_per_pixel + 7) >> 3;
    size_t space_size = vg_info.x_res * vg_info.y_res * vg_info.bytes_per_pixel;

    int ans;
	struct minix_mem_range mr;

	mr.mr_base = (phys_bytes) vbe_info.PhysBasePtr;	
	mr.mr_limit = (phys_bytes) vbe_info.PhysBasePtr + space_size;  

	if (0 != (ans = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", ans);
		

	buffer_base = vm_map_phys(SELF, (void *) vbe_info.PhysBasePtr, space_size);

	second_buffer = malloc(space_size);
	
	if (buffer_base == MAP_FAILED)
		printf("Mapping failed\n");

	reg86_t reg;
	memset(&reg, 0, sizeof(reg86_t));	

	reg.ah = VBE_AH_FUNCTION_INDICATOR;
	reg.al = VBE_AL_SET_MODE;
	reg.bx = VBE_BX_SET_MODE_ARGUMENT | 0x14C; 
	reg.intno = VBE_INTERRUPT_NUMBER;

	if(sys_int86(&reg) != 0 ) {
		printf("sys_int86() failed \n");
		return NULL;
	}

	if (reg.ah != VBE_FUNCTION_RET_SUCCESS || reg.al != VBE_AH_FUNCTION_INDICATOR) {
		printf("reg function return value different from success: %x\n", reg.al);
		return NULL;
	}

	return buffer_base;
}


int draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
	uint8_t *base = (uint8_t *) second_buffer + (y * vg_info.x_res + x) * vg_info.bytes_per_pixel;
	for (uint8_t j = 0; j < vg_info.bytes_per_pixel; ++j) {
		*base = color >> (j * 8);
		++base;
	}

	return 0;
}

int draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
	for (uint16_t i = x; i < x + len; ++i) {
		draw_pixel(i, y, color);
	}

	return 0;
}

int draw_rectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color) {
	if (x < 0 || y < 0) {
		return 0;
	}
	if (x > vg_info.x_res || y > vg_info.y_res) {
		return 0;
	}


	if (x + width >= vg_info.x_res) {
		width = vg_info.x_res - x;
	}

	if (y + height >= vg_info.y_res) {
		height = vg_info.y_res - y;
	}

	for (uint16_t i = y; i < y + height; ++i) {
		draw_hline(x, i, width, color);
	}

  	return 0;
}

void draw_xpm(xpm_image_t img, int16_t x, int16_t y, uint8_t *map){

	if (x < 0 && x + img.width < 0) {
		return;
	}

	if (y < 0 && y + img.height < 0) {
		return;
	}
	if (x > vg_info.x_res || y > vg_info.y_res) {
		return;
	}

	uint16_t offsetY = 0;
	uint16_t offsetX = 0;

	if (y < 0 ) {
		offsetY = -y;
	}

	if (x < 0) {
		offsetX = -x;
	}

	uint16_t height = img.height;
	uint16_t width = img.width;
	if (y + height >= vg_info.y_res) {
		height = vg_info.y_res - y;
	}
	if (x + width >= vg_info.x_res) {
		width = vg_info.x_res - x;
	}

    for(uint16_t i = y + offsetY; i < y + height; i++){
        for(uint16_t j = x + offsetX; j < x + width; j++){
            uint8_t *ptr = (uint8_t*) second_buffer + j*vg_info.bytes_per_pixel + i*vg_info.x_res*vg_info.bytes_per_pixel;
			if (((uint32_t)*(img.bytes + (j-x)*vg_info.bytes_per_pixel + (i-y)*img.width*vg_info.bytes_per_pixel)) != 254)
				memcpy(ptr, img.bytes+(j-x)*vg_info.bytes_per_pixel + (i-y)*img.width*vg_info.bytes_per_pixel, vg_info.bytes_per_pixel);
        }
    }
}


void (destroy_second_buffer)(){
  free(second_buffer);
}

void (refresh_screen)(){
  memcpy(buffer_base, second_buffer, vg_info.x_res * vg_info.y_res * vg_info.bytes_per_pixel);
}

void erase_xpm(xpm_image_t img, uint16_t x, uint16_t y){
    int color = xpm_transparency_color(XPM_8_8_8_8);

    for(uint16_t i = y; i < y + img.height; i++){
        for(uint16_t j = x; j < x + img.width; j++){
            draw_pixel(j, i, color);
        }
    }
}

void graphics_handling() {
	draw_rectangle(0, 0, vg_info.x_res, vg_info.y_res, 0x000000);
    return;
}
