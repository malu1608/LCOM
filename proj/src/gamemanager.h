#include <lcom/lcf.h>

#include "player.h"


#define GRAVITY 12

struct Platform;

struct Platform* PlatformCreate(int16_t x, int16_t y, uint16_t width, uint16_t height);

void PlatformInit(struct Platform *self, int16_t x, int16_t y, uint16_t width, uint16_t height);

struct Level;

struct Manager* GameManagerCreate(struct Player *player, uint16_t x_res, uint16_t y_res);

void ManagerInit(struct Manager *self, struct Player *player, uint16_t x_res, uint16_t y_res);

bool checkCollisions(struct Player *player);

void PhysicsLoop(struct Manager *self);

void loadLevel1(struct Level *level, struct Manager *manager);

void loadLevel2(struct Level *level, struct Manager *manager);

void DrawLevel(struct Manager *self);


