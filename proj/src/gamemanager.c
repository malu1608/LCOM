#include "gamemanager.h"


struct Manager {
    struct Player *player;

    //struct Platform *platforms[];

    uint16_t x_limit;
    uint16_t y_limit;
    int16_t level;
};

struct Manager* GameManagerCreate(struct Player *player, uint16_t x_res, uint16_t y_res) {
    struct Manager* result = (struct Manager*) malloc(sizeof(struct Manager));
    ManagerInit(result, player, x_res, y_res);
    return result;
}

void ManagerInit(struct Manager *self, struct Player *player, uint16_t x_res, uint16_t y_res) {
    self->player = player;
    self->x_limit = x_res;
    self->y_limit = y_res;
    self->level = 0;
}


bool checkCollisions(struct Player *player) {
    return false;
}

void PhysicsLoop(struct Manager *self) {

    PlayerMove(self->player, 0, PlayerGetIntent(self->player) * 5);

    PlayerResetIntent(self->player);
    
    
    /*
    Check for collisions;
    */

    //GRAVITY
    if (PlayerGetX(self->player) > self->x_limit - (int16_t)PlayerGetWidth(self->player)/2) {
        PlayerSetPosition(self->player, self->x_limit - (int16_t)PlayerGetWidth(self->player)/2, PlayerGetY(self->player));
    } else if (PlayerGetX(self->player) < 0) {
        PlayerSetPosition(self->player, 0, PlayerGetY(self->player));
    }

    if ((PlayerGetY(self->player) > self->y_limit)) {
        PlayerSetPosition(self->player, PlayerGetX(self->player), PlayerGetY(self->player) - self->y_limit);
        self->level--;
    }
    PlayerMove(self->player, 0, GRAVITY);
}
