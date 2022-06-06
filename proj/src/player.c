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
    struct Vector momentum; // Will be the variable responsible in storing the characters 
    int16_t movement; // Alike momentum, it will store the character on foot x values;

    uint16_t speed;

    bool flipped;

    bool floored;
    bool jumping;

    int charge;

    int frame;

    int anim_delay;
};

// Constructor (without allocation)
void PlayerInit(struct Player *self, int16_t x, int16_t y) {
    self->position.x = 200;
    self->position.y = 100;
    self->width = 96;
    self->height = 96;
    self->charge = 0;
    self->jumping = false;
    self->intent = 0;
    self->flipped = false;
    self->momentum.x = 0;
    self->momentum.y = 0;
    self->movement = 0;
    self->speed = 7;
    self->anim_delay = 0;
    self->floored = false;
    self->frame = 1;
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
    if (diff_x > 0)
        self->flipped = false;
    if (diff_x < 0)
        self->flipped = true;
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
    if (self->floored)
        self->momentum.y = -25;
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

int16_t PlayerGetMomentumX(struct Player* self) {
    return self->momentum.x;
}

int16_t PlayerGetMomentumY(struct Player* self) {
    return self->momentum.y;
}

void PlayerSetMomentumX(struct Player* self, int16_t new_mom) {
    self->momentum.x = new_mom;
}

void PlayerSetMomentumY(struct Player* self, int16_t new_mom) {
    self->momentum.y = new_mom;
}

void PlayerResetMomentum(struct Player* self) {
    self->momentum.x = 0;
    self->momentum.y = 0;
}

int16_t PlayerGetMovement(struct Player* self) {
    return self->movement;
}

void PlayerSetMovement(struct Player* self, int16_t new_move) {
    self->movement = new_move;
}

uint16_t PlayerGetSpeed(struct Player* self) {
    return self->speed;
}

bool PlayerGetFloored(struct Player* self) {
    return self->floored;
}

void PlayerSetFloored(struct Player* self, bool floored) {
    self->floored = floored;
}

xpm_map_t PlayerGetSprite(struct Player* self) {
    if (self->frame < 3)  {
        self->anim_delay++;
        if (self->anim_delay > 4) {
            self->anim_delay = 0;
            self->frame++;
        }
    }
    else {
        self->frame = 1;
    }
    switch (self->frame) {
        case 0: return (self->flipped)? (xpm_map_t)playerSprite1Flipped : (xpm_map_t)playerSprite1;
        case 1: return (self->flipped)? (xpm_map_t)playerSprite2Flipped : (xpm_map_t)playerSprite2;
        case 2: return (self->flipped)? (xpm_map_t)playerSprite3Flipped : (xpm_map_t)playerSprite3;
        case 3: return (self->flipped)? (xpm_map_t)playerSprite4Flipped : (xpm_map_t)playerSprite4;
        case 4: return (self->flipped)? (xpm_map_t)playerSprite5Flipped : (xpm_map_t)playerSprite5;
        case 5: return (self->flipped)? (xpm_map_t)playerSprite6Flipped : (xpm_map_t)playerSprite6;
        case 6: return (self->flipped)? (xpm_map_t)playerSprite7Flipped : (xpm_map_t)playerSprite7;
        case 7: return (self->flipped)? (xpm_map_t)playerSprite8Flipped : (xpm_map_t)playerSprite8;
        case 8: return (self->flipped)? (xpm_map_t)playerSprite9Flipped : (xpm_map_t)playerSprite9;
        case 9: return (self->flipped)? (xpm_map_t)playerSprite10Flipped : (xpm_map_t)playerSprite10;
        case 10: return (self->flipped)? (xpm_map_t)playerSprite11Flipped : (xpm_map_t)playerSprite11;
        case 11: return (self->flipped)? (xpm_map_t)playerSprite12Flipped : (xpm_map_t)playerSprite12;
        case 12: return (self->flipped)? (xpm_map_t)playerSprite13Flipped : (xpm_map_t)playerSprite13;
    }
    //return self->frames[0];
    return (xpm_map_t)playerSprite1;
}
