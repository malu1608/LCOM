#include "gamemanager.h"


struct Manager {
    struct Player *player;

    struct Platform *platforms[];

};

bool checkCollisions(struct Player *player) {
    return false;
}
