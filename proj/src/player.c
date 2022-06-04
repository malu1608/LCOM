#include "player.h"
#include "sprites.h"


struct Vector {
    uint16_t x;
    uint16_t y;
};

struct Player {
    struct Vector position;
    uint16_t width;
    uint16_t height;

    struct Vector momentum;

    bool jumping;

    int charge;

    xpm_image_t frames[13];

    int frame;
};

// Constructor (without allocation)
void PlayerInit(struct Player *self, uint16_t x, uint16_t y) {
    self->position.x = 50;
    self->position.y = 50;
    self->width = 200;
    self->height = 200;
    self->charge = 0;
    self->jumping = false;

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
    if (!self->jumping) {
        self->position.x += diff_x;
        self->position.y += diff_y;
    }
}

void PlayerChargeJump(struct Player* self) {
    self->jumping = true;
    self->charge++;
}

void PlayerJump(struct Player* self) {
    self->jumping = true;
    self->charge = 0;
}


int PlayerGetWidth(struct Player* self) {
    return self->width;
}

int PlayerGetHeight(struct Player* self) {
    return self->height;
}

int PlayerGetX(struct Player* self) {
    return self->position.x;
}

int PlayerGetY(struct Player* self) {
    return self->position.y;
}

xpm_image_t PlayerGetSprite(struct Player* self) {
    return self->frames[0];
}
