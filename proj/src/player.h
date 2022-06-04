#include <lcom/lcf.h>


struct Player;

void PlayerInit(struct Player* self, uint16_t x, uint16_t y);

struct Player* PlayerCreate(uint16_t x, uint16_t y);

void PlayerReset(struct Player* self);

void PlayerDestroy(struct Player* player);

void PlayerMove(struct Player* self, uint16_t diff_x, uint16_t diff_y);

void PlayerIntent(struct Player* self, int x);

void PlayerResetIntent(struct Player* self);

int PlayerGetWidth(struct Player* self);

int PlayerGetHeight(struct Player* self);

int PlayerGetX(struct Player* self);

int PlayerGetY(struct Player* self);

void PlayerChargeJump(struct Player* self);

void PlayerJump(struct Player* self);

xpm_image_t PlayerGetSprite(struct Player* self);
