#include "player.h"
#include "sprites.h"


struct Vector {
    int16_t x;
    int16_t y;
};

enum Anim {
    Idle,
    Charge,
    Run,
    Jumping,
    Falling,
    Bounce,
    Laid
    }; 

struct Player {
    struct Vector position;
    uint16_t width;
    uint16_t height;

    uint16_t colliderWidth;
    uint16_t colliderX;
    uint16_t colliderHeight;
    uint16_t colliderY;

    int16_t leftKey;
    int16_t rightKey;
    struct Vector momentum; // Will be the variable responsible in storing the characters 
    int16_t movement; // Alike momentum, it will store the character on foot x values;

    uint16_t speed;

    bool flipped;

    bool floored;
    bool jumping;
    int jumpDir;
    int charge;
    bool bounce;

    enum Anim animation;
    int frame;
    bool laid;

    int anim_delay;
};

// Constructor (without allocation)
void PlayerInit(struct Player *self, int16_t x, int16_t y) {
    self->position.x = 200;
    self->position.y = 100;
    self->width = 96;
    self->height = 96;
    self->charge = 10;
    self->jumping = false;
    self->jumpDir = 0;
    self->leftKey = 0;
    self->rightKey = 0;
    self->flipped = false;
    self->momentum.x = 0;
    self->momentum.y = 0;
    self->movement = 0;
    self->speed = 7;
    self->anim_delay = 0;
    self->floored = false;
    self->animation = Idle;
    self->laid = false;
    self->bounce = false;
    self->frame = 1;
    self->colliderWidth = 66;
    self->colliderX = 15;
    self->colliderHeight = 80;
    self->colliderY = 16;
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
    if (!self->floored) {
        return;
    }
    if (!self->jumping) {
        self->jumpDir = 0;
    }
    self->laid = false;
    self->jumping = true;
    self->charge++;
    if (PlayerGetIntent(self) > 0) {
        self->jumpDir = 1;
    } else if (PlayerGetIntent(self) < 0) {
        self->jumpDir = -1;
    }
    if (self->charge >= MAX_CHARGE) {
        PlayerJump(self);
    }
}

void PlayerJump(struct Player* self) {
    self->laid = false;
    if (self->jumping) {
        if (self->floored) {
            self->momentum.y = -self->charge;
            if (self->charge > MAX_CHARGE) {
                self->momentum.x = self->charge/3 * self->jumpDir;
            } 
            else
                self->momentum.x = self->charge/3 * self->jumpDir;
        }
        self->jumping = false;
        self->jumpDir = 0;
        self->charge = 10;
    }
}

void PlayerLeftPressed(struct Player* self) {
    self->laid = false;
    self->leftKey = 1;
}

void PlayerLeftReleased(struct Player* self) {
    self->leftKey = 0;
}

void PlayerRightPressed(struct Player* self) {
    self->laid = false;
    self->rightKey = 1;
}

void PlayerRightReleased(struct Player* self) {
    self->rightKey = 0;
}

bool PlayerGetLeftKey(struct Player* self) {
    return self->leftKey;
}

bool PlayerGetRightKey(struct Player* self) {
    return self->rightKey;
}

int16_t PlayerGetIntent(struct Player* self) {
    return self->rightKey - self->leftKey;
}

int16_t PlayerGetWidth(struct Player* self) {
    return self->width;
}

int16_t PlayerGetHeight(struct Player* self) {
    return self->height;
}

int16_t PlayerGetX(struct Player* self) {
    return self->position.x + self->colliderX;
}

int16_t PlayerGetY(struct Player* self) {
    return self->position.y + self->colliderY;
}

uint16_t PlayerGetColliderWidth(struct Player* self) {
    return self->colliderWidth + self->colliderX;
}

uint16_t PlayerGetColliderHeight(struct Player* self) {
    return self->colliderHeight + self->colliderY;
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

bool PlayerGetLaid(struct Player* self) {
    return self->laid;
}

void PlayerSetLaid(struct Player* self, bool _laid) {
    self->laid = _laid;
}

bool PlayerGetJumping(struct Player* self) {
    return self->jumping;
}

int PlayerGetCharge(struct Player* self) {
    return self->charge;
}

void PlayerSetBounce(struct Player* self, bool _bounce) {
    self->bounce = _bounce;
}

bool PlayerGetBounce(struct Player* self) {
    return self->bounce;
}

void PlayerSetAnimation(struct Player* self, int newAnim) {
    self->animation = newAnim;
}
/*
0 - Idle,
1 - Charge,
2 - Run,
3 - Jumping,
4 - Falling,
5 - Bounce,
6 - Laid
*/
xpm_map_t PlayerGetSprite(struct Player* self) {
    switch (self->animation) {
        case 0: {
            self->anim_delay++;
            if (self->anim_delay > 435) {
                self->anim_delay = 0;
            }
            if (self->anim_delay > 400) {
                self->frame = 12;
            }
            else {
                self->frame = 0;
            }
            break;
        }
        case 1: {
            self->frame = 4;
            break;
        }
        case 2: {
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
            break;
        }
        case 3: {
            self->frame = 5;
            break;
        }
        case 4: {
            self->frame = 6;
            break;
        }
        case 5: {
            self->frame = 8;
            break;
        }
        case 6: {
            self->frame = 7;
            break;
        }
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
