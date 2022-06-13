#include <lcom/lcf.h>

#define MAX_CHARGE 40

/** @defgroup Player Player
 * @{
 *
 * Functions for the player.
 */

/**
 * @brief The struct containing every information about the player.
 * 
 */
struct Player;

/**
 * @brief Initializes the player on the position (x, y) with the default values, loading the sprites in addition.
 * 
 * @param self 
 * @param x 
 * @param y 
 */
void PlayerInit(struct Player* self, int16_t x, int16_t y);

/**
 * @brief Creates a Player on the position (x, y). Returns the pointer to the created object.
 * 
 * @param x 
 * @param y 
 * @return struct Player* 
 */
struct Player* PlayerCreate(int16_t x, int16_t y);

/**
 * @brief Frees the memory allocated to the player.
 * 
 * @param player 
 */
void PlayerDestroy(struct Player* player);

/**
 * @brief Sets the player (self) position to (x, y);
 * 
 * @param self 
 * @param x 
 * @param y 
 */
void PlayerSetPosition(struct Player* self, int16_t x, int16_t y);

/**
 * @brief Adds the diff_x and diff_y deltas to the player (self) position. Also flips the character depending on the diff_x's sign.
 * 
 * @param self 
 * @param diff_x 
 * @param diff_y 
 */
void PlayerMove(struct Player* self, int16_t diff_x, int16_t diff_y);

/**
 * @brief Sets leftKey value to 1. If the player is laid sets laid to false.
 * 
 * @param self 
 */
void PlayerLeftPressed(struct Player* self);

/**
 * @brief Sets leftKey to 0;
 * 
 * @param self 
 */
void PlayerLeftReleased(struct Player* self);

/**
 * @brief Sets rightKey value to 1. If the player is laid sets laid to false.
 * 
 * @param self 
 */
void PlayerRightPressed(struct Player* self);

/**
 * @brief Sets rightKey to 0;
 * 
 * @param self 
 */
void PlayerRightReleased(struct Player* self);

/**
 * @brief Returns the player's (self) leftKey value.
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool PlayerGetLeftKey(struct Player* self);

/**
 * @brief Returns the player's (self) rightKey value.
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool PlayerGetRightKey(struct Player* self);

/**
 * @brief Returns the player's (self) intent.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetIntent(struct Player* self);

/**
 * @brief Returns the player's (self) width.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetWidth(struct Player* self);

/**
 * @brief Returns the player's (self) height.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetHeight(struct Player* self);

/**
 * @brief Returns the player's (self) sprite x position. For drawing purposes.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetSpriteX(struct Player* self);

/**
 * @brief Returns the player's (self) sprite y position. For drawing purposes.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetSpriteY(struct Player* self);

/**
 * @brief Returns the player's (self) x position. For collision detection purposes.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetX(struct Player* self);

/**
 * @brief Returns the player's (self) y position. For collision detenction purposes.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetY(struct Player* self);

/**
 * @brief Returns the player's (self) collider width.
 * 
 * @param self 
 * @return uint16_t 
 */
uint16_t PlayerGetColliderWidth(struct Player* self);

/**
 * @brief Returns the player's (self) height.
 * 
 * @param self 
 * @return uint16_t 
 */
uint16_t PlayerGetColliderHeight(struct Player* self);

/**
 * @brief Returns the player's (self) momentum x value.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetMomentumX(struct Player* self);

/**
 * @brief Returns the player's (self) momentum y value.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetMomentumY(struct Player* self);

/**
 * @brief Sets the player's (self) momentum x value.
 * 
 * @param self 
 * @param new_mom 
 */
void PlayerSetMomentumX(struct Player* self, int16_t new_mom);

/**
 * @brief Sets the player's (self) momentum y value.
 * 
 * @param self 
 * @param new_mom 
 */
void PlayerSetMomentumY(struct Player* self, int16_t new_mom);

/**
 * @brief Resets the player's (self) momentum value.
 * 
 * @param self 
 */
void PlayerResetMomentum(struct Player* self);

/**
 * @brief Returns the player's (self) movement value.
 * 
 * @param self 
 * @return int16_t 
 */
int16_t PlayerGetMovement(struct Player* self);

/**
 * @brief Sets the player's (self) movement value.
 * 
 * @param self 
 * @param new_move 
 */
void PlayerSetMovement(struct Player* self, int16_t new_move);

/**
 * @brief Calculates jump force and direction, jumping if maximum force is reached.
 * 
 * @param self 
 */
void PlayerChargeJump(struct Player* self);

/**
 * @brief Adds the jump force to the player's (self) momentum.
 * 
 * @param self 
 */
void PlayerJump(struct Player* self);

/**
 * @brief Returns the player's (self) floored value.
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool PlayerGetFloored(struct Player* self);

/**
 * @brief Sets the player's (self) floored value.
 * 
 * @param self 
 * @param floored 
 */
void PlayerSetFloored(struct Player* self, bool floored);

/**
 * @brief Returns the player's (self) laid value.
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool PlayerGetLaid(struct Player* self);

/**
 * @brief Sets the player's (self) laid value.
 * 
 * @param self 
 * @param _laid 
 */
void PlayerSetLaid(struct Player* self, bool _laid);

/**
 * @brief Returns the player's (self) jumping value.
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool PlayerGetJumping(struct Player* self);

/**
 * @brief Returns the player's (self) jump charge value.
 * 
 * @param self 
 * @return int 
 */
int PlayerGetCharge(struct Player* self);

/**
 * @brief Sets the player's (self) bounce value.
 * 
 * @param self 
 * @param _bounce 
 */
void PlayerSetBounce(struct Player* self, bool _bounce);

/**
 * @brief Returns the player's (self) bounce value.
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool PlayerGetBounce(struct Player* self);

/**
 * @brief Returns the player's (self) speed value. 
 * 
 * @param self 
 * @return uint16_t 
 */
uint16_t PlayerGetSpeed(struct Player* self);

/**
 * @brief Sets the player's (self) animation indicator value.
 * 
 * @param self 
 * @param newAnim 
 */
void PlayerSetAnimation(struct Player* self, int newAnim);

/**
 * @brief As it is called every frame if processes the animations changing the player's (self) frame and returning its xpm_image_t value.
 * 
 * @param self 
 * @return xpm_image_t 
 */
xpm_image_t PlayerGetSprite(struct Player* self);
