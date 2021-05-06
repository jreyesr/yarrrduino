//#include "pic16f1455.h"

/**
 * @brief Value indicating a bit to be OFF.
 */
#define OFF 0

/**
 * @brief Value indicating a bit to be ON.
 */
#define ON 1

/**
 * @brief Value indicating a pin direction to be OUTPUT.
 */
#define OUTPUT 0x1

/**
 * @brief Value indicating a pin direction to be INPUT.
 */
#define INPUT 0x0

/**
 * @brief Value indicating a pin state to be LOW.
 */
#define LOW 0x0

/**
 * @brief Value indicating a pin state to be HIGH.
 */
#define HIGH 0x1

/**
 * @brief Value indicating a negative statement.
 */
#define NO 0

/**
 * @brief Value indicating a positive statement.
 */
#define YES 1

/**
 * @brief Value indicating a push-pull pin.
 */
#define PUSH_PULL 0

/**
 * @brief Value indicating an open drain pin.
 */
#define OPEN_DRAIN 1

/**
 * @brief Returns the most significant 16 bits of the given 32-bits integer.
 *
 * @param[in] value a 32-bits integer whose MSW is required.
 */
#define HI16(value) (((uint32_t)(value) >> 16) & 0xFFFF)

/**
 * @brief Returns the least significant 16 bits of the given 32-bits integer.
 *
 * @param[in] value a 32-bits integer whose LSW is required.
 */
#define LO16(value) ((uint32_t)(value)&0xFFFF)

/**
 * @brief Returns the most significant 8 bits of the given 16-bits integer.
 *
 * @param[in] value a 16-bits integer whose MSB is required.
 */
#define HI8(value) (((uint16_t)(value) >> 8) & 0xFF)

/**
 * @brief Returns the least significant 8 bits of the given 16-bits integer.
 *
 * @param[in] value a 16-bits integer whose LSB is required.
 */
#define LO8(value) ((uint16_t)(value)&0xFF)

/**
 * @brief Returns the given value with only the requested bottom bits masked in.
 *
 * @param[in] value a 8-bits integer to mask.
 * @param[in] bits how many bits to leave, starting from LSB.
 */
#define MASKBOTTOM8(value, bits) (((uint8_t)value) & ((1 << (uint8_t)bits) - 1))


/**
 * @brief Returns the given value with only the requested bottom bits masked in.
 *
 * @param[in] value a 16-bits integer to mask.
 * @param[in] bits how many bits to leave, starting from LSB.
 */
#define MASKBOTTOM16(value, bits)                                              \
  (((uint16_t)value) & ((1 << (uint16_t)bits) - 1))
