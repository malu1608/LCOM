#include "gamemanager.h"
#include "graphics.h"

struct Platform {
    int16_t x;
    int16_t y;

    uint16_t width;
    uint16_t height;
};


struct Level {
    struct Platform *platforms[5];
};


struct Manager {
    struct Player *player;

    struct Level *levels[5];

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

    self->levels[0] = (struct Level*) malloc(sizeof(struct Level));
    self->levels[1] = (struct Level*) malloc(sizeof(struct Level));
    self->levels[2] = (struct Level*) malloc(sizeof(struct Level));
    self->levels[3] = (struct Level*) malloc(sizeof(struct Level));
    self->levels[4] = (struct Level*) malloc(sizeof(struct Level));

    loadLevel1(self->levels[0], self);
    // Level[1] = loadLevel2();
    // Level[2] = loadLevel3();
    // Level[3] = loadLevel4();
    // Level[4] = loadLevel5();
}


bool checkCollisions(struct Player *player) {
    return false;
}

void PhysicsLoop(struct Manager *self) {

    PlayerMove(self->player, PlayerGetIntent(self->player) * 5, 0);

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

    if ((PlayerGetY(self->player) > self->y_limit + 20)) {
        PlayerSetPosition(self->player, PlayerGetX(self->player), 0-PlayerGetHeight(self->player));
        if (self->level == 0){
            PlayerSetPosition(self->player, PlayerGetX(self->player), 700);
        }
        self->level--;
    }
    else if ((PlayerGetY(self->player) + PlayerGetHeight(self->player) < -20)) {
        PlayerSetPosition(self->player, PlayerGetX(self->player), self->y_limit);
        self->level++;
    }

    PlayerMove(self->player, 0, GRAVITY);
}

void DrawLevel(struct Manager *self) {
    for (int i = 0; i < 5; i++) {
        draw_rectangle(self->levels[0]->platforms[i]->x, self->levels[0]->platforms[i]->y, self->levels[0]->platforms[i]->width, self->levels[0]->platforms[i]->height, 0xff00ff);
    }
}


void loadLevel1(struct Level *level, struct Manager *manager) {
    level->platforms[0] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[1] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[2] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[3] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[4] = (struct Platform*) malloc(sizeof(struct Platform));

    level->platforms[0]->x = 0;
    level->platforms[0]->y = 800;
    level->platforms[0]->width = 1152;
    level->platforms[0]->height = 64;

    level->platforms[1]->x = 600;
    level->platforms[1]->y = 60;
    level->platforms[1]->width = 80;
    level->platforms[1]->height = 40;

    level->platforms[2]->x = 200;
    level->platforms[2]->y = 600;
    level->platforms[2]->width = 80;
    level->platforms[2]->height = 40;

    level->platforms[3]->x = 10;
    level->platforms[3]->y = 60;
    level->platforms[3]->width = 10;
    level->platforms[3]->height = 40;

    level->platforms[4]->x = 30;
    level->platforms[4]->y = 200;
    level->platforms[4]->width = 40;
    level->platforms[4]->height = 40;
}

