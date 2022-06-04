#include "player.h"
#include "sprites.h"


struct Vector {
    int16_t x;
    int16_t y;
};

struct Player {
    struct Vector position;
    uint16_t width;
    uint16_t height;
    int16_t intent;
    struct Vector momentum;

    bool jumping;

    int charge;

    xpm_image_t frames[13];

    int frame;
};

// Constructor (without allocation)
void PlayerInit(struct Player *self, int16_t x, int16_t y) {
    self->position.x = 50;
    self->position.y = 50;
    self->width = 200;
    self->height = 200;
    self->charge = 0;
    self->jumping = false;
    self->intent = 0;

    xpm_load(playerSprite1, 0, &self->frames[0]);
    self->frame = 0;

}

// Allocation + initialization (equivalent to "new Point(x, y)")
struct Player* PlayerCreate(int16_t x, int16_t y) {
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

void PlayerSetPosition(struct Player* self, int16_t x, int16_t y) {
    self->position.x = x;
    self->position.y = y;
}

void PlayerMove(struct Player* self, int16_t diff_x, int16_t diff_y) {
    self->position.x += diff_x;
    self->position.y += diff_y;
}

void PlayerChargeJump(struct Player* self) {
    self->jumping = true;
    self->charge++;
}

void PlayerJump(struct Player* self) {
    self->jumping = true;
    self->charge = 0;
}

void PlayerIntent(struct Player* self, int16_t x) {
    self->intent = x;
}

void PlayerResetIntent(struct Player* self) {
    self->intent = 0;
}

int16_t PlayerGetIntent(struct Player* self) {
    return self->intent;
}

int16_t PlayerGetWidth(struct Player* self) {
    return self->width;
}

int16_t PlayerGetHeight(struct Player* self) {
    return self->height;
}

int16_t PlayerGetX(struct Player* self) {
    return self->position.x;
}

int16_t PlayerGetY(struct Player* self) {
    return self->position.y;
}

xpm_image_t PlayerGetSprite(struct Player* self) {
    return self->frames[0];
}
