#include <lcom/lcf.h>

#include "player.h"


#define GRAVITY 12

/** @defgroup GameManager GameManager
 * @{
 *
 * Functions for the GameManager
 */


/**
 * @brief A struct that stores the position and dimensions of a platform. Utilized specially for collisions.
 * 
 */
struct Platform;

/**
 * @brief Creates a platform with the position (x, y) and dimensions width and height. Returns a pointer to a Platform struct.
 * 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @return struct Platform* 
 */
struct Platform* PlatformCreate(int16_t x, int16_t y, uint16_t width, uint16_t height);

/**
 * @brief Initializes the platform, giving itself the values provided.
 * 
 * @param self 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 */
void PlatformInit(struct Platform *self, int16_t x, int16_t y, uint16_t width, uint16_t height);

/**
 * @brief A struct to store information about each level, the background drawing information, an array of Platforms and the size of the Platforms array.
 * 
 */
struct Level;

/**
 * @brief Creates a Manager, storing its player and screen dimensions.
 * 
 * @param player 
 * @param x_res 
 * @param y_res 
 * @return struct Manager* 
 */
struct Manager* GameManagerCreate(struct Player *player, uint16_t x_res, uint16_t y_res);

/**
 * @brief Initializes the Manager, giving itself the provided values.
 * 
 * @param self 
 * @param player 
 * @param x_res 
 * @param y_res 
 */
void ManagerInit(struct Manager *self, struct Player *player, uint16_t x_res, uint16_t y_res);

/**
 * @brief Processes collisions and some game logic.
 * 
 * @param self 
 * @return int 
 */
int PhysicsLoop(struct Manager *self);

/**
 * @brief Loads the first level.
 * 
 * @param level 
 * @param manager 
 */
void loadLevel1(struct Level *level, struct Manager *manager);

/**
 * @brief Loads the second level.
 * 
 * @param level 
 * @param manager 
 */
void loadLevel2(struct Level *level, struct Manager *manager);

/**
 * @brief Loads the third level.
 * 
 * @param level 
 * @param manager 
 */
void loadLevel3(struct Level *level, struct Manager *manager);

/**
 * @brief Loads the forth level.
 * 
 * @param level 
 * @param manager 
 */
void loadLevel4(struct Level *level, struct Manager *manager);

/**
 * @brief Loads the fifth level.
 * 
 * @param level 
 * @param manager 
 */
void loadLevel5(struct Level *level, struct Manager *manager);

/**
 * @brief Draws the current level (background and the platforms).
 * 
 * @param self 
 */
void DrawLevel(struct Manager *self);


