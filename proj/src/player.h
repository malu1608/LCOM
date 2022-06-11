#include <lcom/lcf.h>

#define MAX_CHARGE 40

struct Player;

void PlayerInit(struct Player* self, int16_t x, int16_t y);

struct Player* PlayerCreate(int16_t x, int16_t y);

void PlayerReset(struct Player* self);

void PlayerDestroy(struct Player* player);

void PlayerSetPosition(struct Player* self, int16_t x, int16_t y);

void PlayerMove(struct Player* self, int16_t diff_x, int16_t diff_y);

void PlayerLeftPressed(struct Player* self);

void PlayerLeftReleased(struct Player* self);

void PlayerRightPressed(struct Player* self);

void PlayerRightReleased(struct Player* self);

bool PlayerGetLeftKey(struct Player* self);

bool PlayerGetRightKey(struct Player* self);

int16_t PlayerGetIntent(struct Player* self);

int16_t PlayerGetWidth(struct Player* self);

int16_t PlayerGetHeight(struct Player* self);

int16_t PlayerGetX(struct Player* self);

int16_t PlayerGetY(struct Player* self);

uint16_t PlayerGetColliderWidth(struct Player* self);

uint16_t PlayerGetColliderHeight(struct Player* self);

int16_t PlayerGetMomentumX(struct Player* self);

int16_t PlayerGetMomentumY(struct Player* self);

void PlayerSetMomentumX(struct Player* self, int16_t new_mom);

void PlayerSetMomentumY(struct Player* self, int16_t new_mom);

void PlayerResetMomentum(struct Player* self);

int16_t PlayerGetMovement(struct Player* self);

void PlayerSetMovement(struct Player* self, int16_t new_move);

void PlayerChargeJump(struct Player* self);

void PlayerJump(struct Player* self);

bool PlayerGetFloored(struct Player* self);

void PlayerSetFloored(struct Player* self, bool floored);

bool PlayerGetLaid(struct Player* self);

void PlayerSetLaid(struct Player* self, bool _laid);

bool PlayerGetJumping(struct Player* self);

int PlayerGetCharge(struct Player* self);

void PlayerSetBounce(struct Player* self, bool _bounce);

bool PlayerGetBounce(struct Player* self);

uint16_t PlayerGetSpeed(struct Player* self);

void PlayerSetAnimation(struct Player* self, int newAnim);

xpm_map_t PlayerGetSprite(struct Player* self);
