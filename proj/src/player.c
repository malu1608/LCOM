#include "player.h"
#include "sprites.h"


struct Player {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;

    xpm_image_t frames[13];

    int frame;
};

// Constructor (without allocation)
void PlayerInit(struct Player *self, uint16_t x, uint16_t y) {
    self->x = 50;
    self->y = 50;
    self->width = 200;
    self->height = 200;

    xpm_load(playerSprite1, 0, &self->frames[0]);
    self->frame = 0;

}

// Allocation + initialization (equivalent to "new Point(x, y)")
struct Player* PlayerCreate(uint16_t x, uint16_t y) {
    struct Player* result = (struct Player*) malloc(sizeof(struct Player));
    PlayerInit(result, x, y);
    return result;
}

// Destructor (without deallocation)
void PlayerReset(struct Player* self) {
}

// Destructor + deallocation (equivalent to "delete point")
void PlayerDestroy(struct Player* player) {
    if (player) {
        PlayerReset(player);
        free(player);
    }
}

void PlayerMove(struct Player* self, uint16_t diff_x, uint16_t diff_y) {
    self->x += diff_x;
    self->y += diff_y;
}


int PlayerGetWidth(struct Player* self) {
    return self->width;
}

int PlayerGetHeight(struct Player* self) {
    return self->height;
}

int PlayerGetX(struct Player* self) {
    return self->x;
}

int PlayerGetY(struct Player* self) {
    return self->y;
}

xpm_image_t PlayerGetSprite(struct Player* self) {
    return self->frames[0];
}
