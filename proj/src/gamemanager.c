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

    //PlayerMove(self->player, PlayerGetIntent(self->player) * 5, 0);


    int16_t p_x = PlayerGetX(self->player);
    int16_t p_y = PlayerGetY(self->player);
    uint16_t p_width = PlayerGetWidth(self->player);
    uint16_t p_height = PlayerGetHeight(self->player);
    int16_t p_momentum_x = PlayerGetMomentumX(self->player);
    int16_t p_momentum_y = PlayerGetMomentumY(self->player);
    int16_t p_movement = PlayerGetMovement(self->player);
    //int16_t p_momentum_y = PlayerGetMomentumY(self->player);


    int16_t diff_y = GRAVITY + p_momentum_y;
    int16_t diff_x = p_momentum_x + p_movement;
    PlayerSetFloored(self->player, false);

    draw_rectangle(30, 30, 10, 10, 0xff0000);
    draw_rectangle(10, 30, 10, 10, 0xff0000);
    draw_rectangle(20, 40, 10, 10, 0xff0000);
    draw_rectangle(20, 20, 10, 10, 0xff0000);
    if (diff_x > 0) {
        draw_rectangle(30, 30, 10, 10, 0x00ff00);
    } else if (diff_x < 0) {
        draw_rectangle(10, 30, 10, 10, 0x00ff00);
    }
    if (diff_y > 0) {
        draw_rectangle(20, 40, 10, 10, 0x00ff00);
    } else if(diff_y < 0){
        draw_rectangle(20, 20, 10, 10, 0x00ff00);
    }

    if (diff_y > 0) { //going down
        for (int i = 0; i < 5; i++) {
            if (p_x > self->levels[0]->platforms[i]->x + self->levels[0]->platforms[i]->width || self->levels[0]->platforms[i]->x > p_x + p_width)
                continue;
            if (p_y + p_height == self->levels[0]->platforms[i]->y) {
                PlayerSetFloored(self->player, true);
                break;
            }
            if (p_y + p_height + GRAVITY + p_momentum_y + 1 > self->levels[0]->platforms[i]->y && p_y + p_height  <= self->levels[0]->platforms[i]->y){
                if (self->levels[0]->platforms[i]->y - (p_y + p_height)< diff_y)
                    diff_y = self->levels[0]->platforms[i]->y - (p_y + p_height);
            } 
        }
    } else if (diff_y < 0) { //going up
        for (int i = 0; i < 5; i++) {
            if (p_x > self->levels[0]->platforms[i]->x + self->levels[0]->platforms[i]->width || self->levels[0]->platforms[i]->x > p_x + p_width)
                continue;
            if (p_y == self->levels[0]->platforms[i]->y + self->levels[0]->platforms[i]->height) {
                PlayerSetMomentumY(self->player, -PlayerGetMomentumY(self->player));
                diff_y = 0;
                break;
            }
            if (p_y + GRAVITY + p_momentum_y - 1 < self->levels[0]->platforms[i]->y + self->levels[0]->platforms[i]->height && p_y >= self->levels[0]->platforms[i]->y + self->levels[0]->platforms[i]->height){
                if (self->levels[0]->platforms[i]->y + self->levels[0]->platforms[i]->height - p_y > diff_y)
                    diff_y = self->levels[0]->platforms[i]->y + self->levels[0]->platforms[i]->height - p_y;
            } 
        }
    }

    if (diff_x != 0) {
        if (diff_x > 0) {

        } else {

        }
    }

    PlayerMove(self->player, PlayerGetMovement(self->player), 0);
    PlayerMove(self->player, PlayerGetMomentumX(self->player), 0);

    //if not floored move down
    if (!PlayerGetFloored(self->player)){
        //if there is movement turn it to momentum
        if (PlayerGetMovement(self->player) != 0) {
            PlayerSetMomentumX(self->player, PlayerGetMovement(self->player) / 1.5);
            PlayerSetMovement(self->player, 0);
        }
        PlayerMove(self->player, 0, diff_y);
        draw_rectangle(10, 10, 10, 10, 0xff0000); // A little signal to tell if its floored (GREEN is floored, RED isnt)
        //PRECESS MOMENTUM
    }
    else{
        draw_rectangle(10, 10, 10, 10, 0x00ff00); // A little signal to tell if its floored (GREEN is floored, RED isnt)
        //PlayerMove(self->player, PlayerGetIntent(self->player) * 5, 0);
        //When floored the player shall not move;
        PlayerResetMomentum(self->player);
        //turn intent into movement
        //TODO: only if not jumping
        PlayerSetMovement(self->player, PlayerGetIntent(self->player) * PlayerGetSpeed(self->player));
        PlayerResetIntent(self->player);
    }
    
    
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
            PlayerSetPosition(self->player, PlayerGetX(self->player), 600);
        }
        else {
            self->level--;
        }
    }
    else if ((PlayerGetY(self->player) + PlayerGetHeight(self->player) < -20)) {
        PlayerSetPosition(self->player, PlayerGetX(self->player), self->y_limit);
        self->level++;
    }

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

    level->platforms[1]->x = 800;
    level->platforms[1]->y = 0;
    level->platforms[1]->width = 80;
    level->platforms[1]->height = 800;

    level->platforms[2]->x = 200;
    level->platforms[2]->y = 600;
    level->platforms[2]->width = 80;
    level->platforms[2]->height = 40;

    level->platforms[3]->x = 10;
    level->platforms[3]->y = 60;
    level->platforms[3]->width = 10;
    level->platforms[3]->height = 40;

    level->platforms[4]->x = 0;
    level->platforms[4]->y = 0;
    level->platforms[4]->width = 1152;
    level->platforms[4]->height = 20;
}
