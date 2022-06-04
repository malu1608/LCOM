#include "physicObj.h"


struct Platform {
    uint16_t x;
    uint16_t y;

    uint16_t width;
    uint16_t height;
};

struct Platform* PlatformCreate(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    struct Platform* result = (struct Platform*) malloc(sizeof(struct Platform));
    PlatformInit(result, x, y, width, height);
    return result;
}


void PlatformInit(struct Platform *self, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    self->x = x;
    self->y = y;
    self->width = width;
    self->height = height;
}
