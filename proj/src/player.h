#include <lcom/lcf.h>


struct Player;

void PlayerInit(struct Player* self, int16_t x, int16_t y);

struct Player* PlayerCreate(int16_t x, int16_t y);

void PlayerReset(struct Player* self);

void PlayerDestroy(struct Player* player);

void PlayerSetPosition(struct Player* self, int16_t x, int16_t y);

void PlayerMove(struct Player* self, int16_t diff_x, int16_t diff_y);

void PlayerIntent(struct Player* self, int16_t x);

void PlayerResetIntent(struct Player* self);

int16_t PlayerGetIntent(struct Player* self);

int16_t PlayerGetWidth(struct Player* self);

int16_t PlayerGetHeight(struct Player* self);

int16_t PlayerGetX(struct Player* self);

int16_t PlayerGetY(struct Player* self);

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

uint16_t PlayerGetSpeed(struct Player* self);

xpm_map_t PlayerGetSprite(struct Player* self);
