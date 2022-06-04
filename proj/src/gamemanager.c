#include "gamemanager.h"


struct Manager {
    struct Player *player;

    struct Platform *platforms[];

};

struct Manager* GameManagerCreate(struct Player *player) {
    struct Manager* result = (struct Manager*) malloc(sizeof(struct Manager));
    ManagerInit(result, player);
    return result;
}

void ManagerInit(struct Manager *self, struct Player *player) {
    self->player = player;
}


bool checkCollisions(struct Player *player) {
    return false;
}

void PhysicsLoop(struct Manager *self) {
    PlayerResetIntent(self->player);

    /*
    Check for collisions;
    */
   //GRAVITY
   PlayerMove(self->player, 0, GRAVITY);
}
