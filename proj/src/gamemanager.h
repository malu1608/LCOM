#pragma once
#include <lcom/lcf.h>

#include "player.h"
#include "physicObj.h"


#define GRAVITY 2

struct Manager* GameManagerCreate(struct Player *player, uint16_t x_res, uint16_t y_res);

void ManagerInit(struct Manager *self, struct Player *player, uint16_t x_res, uint16_t y_res);

bool checkCollisions(struct Player *player);

void PhysicsLoop(struct Manager *self);

