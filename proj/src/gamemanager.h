#pragma once
#include <lcom/lcf.h>

#include "player.h"
#include "physicObj.h"


#define GRAVITY 2

struct Manager* GameManagerCreate(struct Player *player);

void ManagerInit(struct Manager *self, struct Player *player);

bool checkCollisions(struct Player *player);

void PhysicsLoop(struct Manager *self);

