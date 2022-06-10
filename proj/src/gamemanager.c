#include "gamemanager.h"
#include "graphics.h"

struct Platform {
    int16_t x;
    int16_t y;

    uint16_t width;
    uint16_t height;
};


struct Level {
    struct Platform *platforms[19];
};


struct Manager {
    struct Player *player;

    struct Level *levels[5];

    uint16_t x_limit;
    uint16_t y_limit;
    int16_t level;
    uint16_t num_platforms;
    uint16_t level_num;
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

    loadLevel5(self->levels[4], self);
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
        for (int i = 0; i < self->num_platforms; i++) {
            if (p_x > self->levels[self->level]->platforms[i]->x + self->levels[self->level]->platforms[i]->width || self->levels[self->level]->platforms[i]->x > p_x + p_width)
                continue;
            if (p_y + p_height == self->levels[self->level]->platforms[i]->y) {
                PlayerSetFloored(self->player, true);
                break;
            }
            if (p_y + p_height + GRAVITY + p_momentum_y + 1 > self->levels[self->level]->platforms[i]->y && p_y + p_height  <= self->levels[self->level]->platforms[i]->y){
                if (self->levels[self->level]->platforms[i]->y - (p_y + p_height)< diff_y)
                    diff_y = self->levels[self->level]->platforms[i]->y - (p_y + p_height);
            } 
        }
    } else if (diff_y < 0) { //going up
        for (int i = 0; i < self->num_platforms; i++) {
            if (p_x > self->levels[self->level]->platforms[i]->x + self->levels[self->level]->platforms[i]->width || self->levels[self->level]->platforms[i]->x > p_x + p_width)
                continue;
            if (p_y == self->levels[self->level]->platforms[i]->y + self->levels[self->level]->platforms[i]->height) {
                PlayerSetMomentumY(self->player, -PlayerGetMomentumY(self->player));
                diff_y = 0;
                break;
            }
            if (p_y + GRAVITY + p_momentum_y - 1 < self->levels[self->level]->platforms[i]->y + self->levels[self->level]->platforms[i]->height && p_y >= self->levels[self->level]->platforms[i]->y + self->levels[self->level]->platforms[i]->height){
                if (self->levels[self->level]->platforms[i]->y + self->levels[self->level]->platforms[i]->height - p_y > diff_y)
                    diff_y = self->levels[self->level]->platforms[i]->y + self->levels[self->level]->platforms[i]->height - p_y;
            } 
        }
    }

    if (diff_x != 0) {
        if (diff_x > 0) {
            for (int i = 0; i < self->num_platforms; i++) {
                if (p_y > self->levels[self->level]->platforms[i]->y + self->levels[self->level]->platforms[i]->height || self->levels[self->level]->platforms[i]->y > p_y + p_height)
                    continue;
                if (p_x + p_width == self->levels[self->level]->platforms[i]->x) {
                    PlayerSetMomentumX(self->player, -PlayerGetMomentumX(self->player));
                    diff_x = 0;
                    break;
                }
                if (p_x + p_width + p_movement + p_momentum_x + 1 > self->levels[self->level]->platforms[i]->x && p_x + p_width <= self->levels[self->level]->platforms[i]->x){
                    if (self->levels[self->level]->platforms[i]->x - p_x - p_width < diff_x)
                        diff_x = self->levels[self->level]->platforms[i]->x - p_x - p_width;
                } 
            }
        } else {
            for (int i = 0; i < self->num_platforms; i++) {
                if (p_y > self->levels[self->level]->platforms[i]->y + self->levels[self->level]->platforms[i]->height || self->levels[self->level]->platforms[i]->y > p_y + p_height)
                    continue;
                if (p_x == self->levels[self->level]->platforms[i]->x + self->levels[self->level]->platforms[i]->width) {
                    PlayerSetMomentumX(self->player, -PlayerGetMomentumX(self->player));
                    diff_x = 0;
                    break;
                }
                if (p_x + p_movement + p_momentum_x - 1 < self->levels[self->level]->platforms[i]->x + self->levels[self->level]->platforms[i]->width && p_x >= self->levels[self->level]->platforms[i]->x + self->levels[self->level]->platforms[i]->width){
                    if (p_x - self->levels[self->level]->platforms[i]->x - self->levels[self->level]->platforms[i]->width > diff_x)
                        diff_x = self->levels[self->level]->platforms[i]->x + self->levels[self->level]->platforms[i]->width - p_x;
                } 
            }
        }
    }

    // PlayerMove(self->player, PlayerGetMovement(self->player), 0);
    // PlayerMove(self->player, PlayerGetMomentumX(self->player), 0);
    PlayerMove(self->player, diff_x, 0);

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
    for (int i = 0; i < self->num_platforms; i++) {
        draw_rectangle(self->levels[self->level]->platforms[i]->x, self->levels[self->level]->platforms[i]->y, self->levels[self->level]->platforms[i]->width, self->levels[self->level]->platforms[i]->height, 0xff00ff);
    }
}


void loadLevel1(struct Level *level, struct Manager *manager) {
    manager->level = 0;
    manager->num_platforms = 4;

    level->platforms[0] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[1] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[2] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[3] = (struct Platform*) malloc(sizeof(struct Platform));

    level->platforms[0]->x = 0;
    level->platforms[0]->y = 800;
    level->platforms[0]->width = 1152;
    level->platforms[0]->height = 64;

    level->platforms[1]->x = 955;
    level->platforms[1]->y = 500;
    level->platforms[1]->width = 200;
    level->platforms[1]->height = 300;

    level->platforms[2]->x = 425;
    level->platforms[2]->y = 220;
    level->platforms[2]->width = 300;
    level->platforms[2]->height = 150;

    level->platforms[3]->x = 0;
    level->platforms[3]->y = 500;
    level->platforms[3]->width = 200;
    level->platforms[3]->height = 300;
}

void loadLevel2(struct Level *level, struct Manager *manager) {
    manager->level = 1;
    manager->num_platforms = 5;

    level->platforms[0] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[1] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[2] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[3] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[4] = (struct Platform*) malloc(sizeof(struct Platform));

    level->platforms[0]->x = 600;
    level->platforms[0]->y = 750;
    level->platforms[0]->width = 150;
    level->platforms[0]->height = 64;

    level->platforms[1]->x = 450;
    level->platforms[1]->y = 500;
    level->platforms[1]->width = 200;
    level->platforms[1]->height = 70;

    level->platforms[2]->x = 955;
    level->platforms[2]->y = 550;
    level->platforms[2]->width = 200;
    level->platforms[2]->height = 150;

    level->platforms[3]->x = 350;
    level->platforms[3]->y = 260;
    level->platforms[3]->width = 200;
    level->platforms[3]->height = 100;

    level->platforms[4]->x = 0;
    level->platforms[4]->y = 200;
    level->platforms[4]->width = 200;
    level->platforms[4]->height = 100;
}

void loadLevel3(struct Level *level, struct Manager *manager) {
        manager->level = 2;
    manager->num_platforms = 8;

    level->platforms[0] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[1] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[2] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[3] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[4] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[5] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[6] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[7] = (struct Platform*) malloc(sizeof(struct Platform));

    level->platforms[0]->x = 200;
    level->platforms[0]->y = 750;
    level->platforms[0]->width = 150;
    level->platforms[0]->height = 64;

    level->platforms[1]->x = 600;
    level->platforms[1]->y = 750;
    level->platforms[1]->width = 150;
    level->platforms[1]->height = 70;

    level->platforms[2]->x = 300;
    level->platforms[2]->y = 500;
    level->platforms[2]->width = 300;
    level->platforms[2]->height = 50;

    level->platforms[3]->x = 600;
    level->platforms[3]->y = 450;
    level->platforms[3]->width = 100;
    level->platforms[3]->height = 100;

    level->platforms[4]->x = 900;
    level->platforms[4]->y = 550;
    level->platforms[4]->width = 150;
    level->platforms[4]->height = 100;

    level->platforms[5]->x = 950;
    level->platforms[5]->y = 250;
    level->platforms[5]->width = 100;
    level->platforms[5]->height = 70;

    level->platforms[6]->x = 50;
    level->platforms[6]->y = 250;
    level->platforms[6]->width = 200;
    level->platforms[6]->height = 70;

    level->platforms[7]->x = 1050;
    level->platforms[7]->y = 150;
    level->platforms[7]->width = 100;
    level->platforms[7]->height = 170;
}

void loadLevel4(struct Level *level, struct Manager *manager) {
        manager->level = 3;
    manager->num_platforms = 9;

    level->platforms[0] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[1] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[2] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[3] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[4] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[5] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[6] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[7] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[8] = (struct Platform*) malloc(sizeof(struct Platform));

    level->platforms[0]->x = 800;
    level->platforms[0]->y = 750;
    level->platforms[0]->width = 150;
    level->platforms[0]->height = 64;

    level->platforms[1]->x = 200;
    level->platforms[1]->y = 0;
    level->platforms[1]->width = 70;
    level->platforms[1]->height = 600;

    level->platforms[2]->x = 900;
    level->platforms[2]->y = 0;
    level->platforms[2]->width = 70;
    level->platforms[2]->height = 500;

    level->platforms[3]->x = 500;
    level->platforms[3]->y = 650;
    level->platforms[3]->width = 100;
    level->platforms[3]->height = 70;

    level->platforms[4]->x = 270;
    level->platforms[4]->y = 550;
    level->platforms[4]->width = 50;
    level->platforms[4]->height = 50;

    level->platforms[5]->x = 750;
    level->platforms[5]->y = 430;
    level->platforms[5]->width = 150;
    level->platforms[5]->height = 70;

    level->platforms[6]->x = 850;
    level->platforms[6]->y = 250;
    level->platforms[6]->width = 50;
    level->platforms[6]->height = 70;

    level->platforms[7]->x = 500;
    level->platforms[7]->y = 230;
    level->platforms[7]->width = 30;
    level->platforms[7]->height = 30;

    level->platforms[8]->x = 270;
    level->platforms[8]->y = 120;
    level->platforms[8]->width = 30;
    level->platforms[8]->height = 30;
}

void loadLevel5(struct Level *level, struct Manager *manager) {
        manager->level = 4;
    manager->num_platforms = 19;

    level->platforms[0] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[1] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[2] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[3] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[4] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[5] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[6] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[7] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[8] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[9] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[10] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[11] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[12] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[13] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[14] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[15] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[16] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[17] = (struct Platform*) malloc(sizeof(struct Platform));
    level->platforms[18] = (struct Platform*) malloc(sizeof(struct Platform));

    level->platforms[0]->x = 0;
    level->platforms[0]->y = 0;
    level->platforms[0]->width = 30;
    level->platforms[0]->height = 865;

    level->platforms[1]->x = 1125;
    level->platforms[1]->y = 0;
    level->platforms[1]->width = 30;
    level->platforms[1]->height = 865;

    level->platforms[2]->x = 30;
    level->platforms[2]->y = 815;
    level->platforms[2]->width = 50;
    level->platforms[2]->height = 50;

    level->platforms[3]->x = 230;
    level->platforms[3]->y = 740;
    level->platforms[3]->width = 100;
    level->platforms[3]->height = 20;

    level->platforms[4]->x = 450;
    level->platforms[4]->y = 760;
    level->platforms[4]->width = 50;
    level->platforms[4]->height = 20;

    level->platforms[5]->x = 630;
    level->platforms[5]->y = 740;
    level->platforms[5]->width = 70;
    level->platforms[5]->height = 20;

    level->platforms[6]->x = 850;
    level->platforms[6]->y = 800;
    level->platforms[6]->width = 50;
    level->platforms[6]->height = 20;

    level->platforms[7]->x = 1050;
    level->platforms[7]->y = 700;
    level->platforms[7]->width = 75;
    level->platforms[7]->height = 30;

    level->platforms[8]->x = 300;
    level->platforms[8]->y = 550;
    level->platforms[8]->width = 570;
    level->platforms[8]->height = 50;

    level->platforms[9]->x = 820;
    level->platforms[9]->y = 350;
    level->platforms[9]->width = 50;
    level->platforms[9]->height = 200;

    level->platforms[10]->x = 725;
    level->platforms[10]->y = 150;
    level->platforms[10]->width = 400;
    level->platforms[10]->height = 30;
    
    level->platforms[11]->x = 300;
    level->platforms[11]->y = 450;
    level->platforms[11]->width = 50;
    level->platforms[11]->height = 100;

    level->platforms[12]->x = 870;
    level->platforms[12]->y = 540;
    level->platforms[12]->width = 50;
    level->platforms[12]->height = 20;

    level->platforms[13]->x = 1050;
    level->platforms[13]->y = 420;
    level->platforms[13]->width = 75;
    level->platforms[13]->height = 30;

    level->platforms[14]->x = 30;
    level->platforms[14]->y = 380;
    level->platforms[14]->width = 30;
    level->platforms[14]->height = 30;

    level->platforms[15]->x = 180;
    level->platforms[15]->y = 220;
    level->platforms[15]->width = 30;
    level->platforms[15]->height = 30;

    level->platforms[16]->x = 300;
    level->platforms[16]->y = 200;
    level->platforms[16]->width = 50;
    level->platforms[16]->height = 100;

    level->platforms[17]->x = 530;
    level->platforms[17]->y = 300;
    level->platforms[17]->width = 30;
    level->platforms[17]->height = 30;

    level->platforms[18]->x = 620;
    level->platforms[18]->y = 220;
    level->platforms[18]->width = 30;
    level->platforms[18]->height = 30;
}
