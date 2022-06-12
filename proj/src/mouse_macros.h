#ifndef _MOUSE_MACROS_H_
#define _MOUSE_MACROS_H_


#define MOUSE_IRQ 12
#define KBD_IRQ 1

#define DATA_PORT 0x60
#define READ_ST_REG 0x64
#define WRITE_CMD_CTRLREG 0x64



#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60
#define MOUSE_DISABLE 0xA7
#define MOUSE_ENABLE 0xA8
#define MOUSE_CHECK_INT 0xA9
#define MOUSE_WRITE_BYTE 0xD4

//STATUS REGISTER

#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define AUX_ST_REG BIT(5)
#define INH_FLAG BIT(4)
#define A2_ST_REG BIT(3)
#define SYS_FLAG BIT(2)
#define INPUT_BUF_FULL BIT(1)
#define OUTPUT_BUF_FULL BIT(0)


//COMMAND BYTE

#define CMD_DIS_MOUSE BIT(5)
#define CMD_DIS_KBD BIT(4)
#define CMD_EN_INT_MOUSE BIT(1)
#define CMD_EN_INT_KBD BIT(0)

//MOUSE COMMANDS

#define MOUSE_RESET 0xFF
#define MOUSE_RESEND 0xFE //For communication errors.
#define MOUSE_SET_DEFAULTS 0XF6
#define MOUSE_DATA_REP_DISABLE 0XF5 //In stream mode only
#define MOUSE_DATA_REP_ENABLE 0XF4 //In stream mode only
#define MOUSE_SET_SAMPLE_RATE 0XF3 //Args: Sample rate
#define MOUSE_SET_REMOTE_MODE 0XF0
#define MOUSE_READ_DATA 0XEB //When in remote mode, request packet.
#define MOUSE_SET_STREAM_MODE 0XEA
#define MOUSE_STATUS_REQUEST 0XE9 //Get mouse configuration
#define MOUSE_SET_RESOLUTION 0XF6 //Args: Resolution
#define MOUSE_SET_SCALING_2_1 0XE7
#define MOUSE_SET_SCALING_1_1 0XE6

//RESPONSES

#define RES_ACK 0xFA //Everything OK
#define RES_NACK 0xFE //invalid byte
#define RES_ERROR 0xFC //second consecutive invalid byte


/* MOUSE PACKET PARSING */

#define MOUSE_PARSE_LB BIT(0)
#define MOUSE_PARSE_RB BIT(1)
#define MOUSE_PARSE_MB BIT(2)
#define MOUSE_PARSE_FIRST_PACKET_IDENTIFIER BIT(3)
#define MOUSE_PARSE_X_DELTA_MSB BIT(4)
#define MOUSE_PARSE_Y_DELTA_MSB BIT(5)
#define MOUSE_PARSE_X_OVERFLOW BIT(6)
#define MOUSE_PARSE_Y_OVERFLOW BIT(7)
#define EXTEND_SIGN 0xFF00


#endif
