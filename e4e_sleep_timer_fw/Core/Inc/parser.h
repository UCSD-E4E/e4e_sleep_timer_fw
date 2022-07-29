/*
 * parser.h
 *
 *  Created on: July 7, 2022
 *      Author: frankc
 */


#ifndef PARSER_H
#define PARSER_H
#define HEADLEN 6

/**
 * Binary Packet attributes
 */

typedef struct E4E_BinaryPacket_Header_ {
    uint16_t start; /**< start bytes of message 0-1 */
    uint16_t length; /**< length of message 2-3 */
    uint16_t cmd_id; /**< command id 4-5 */
} E4E_BinaryPacket_Header_t;

/**
 * Binary Packet Message State for parse()
 */

typedef enum E4E_BinaryPacket_State_
{
    E4E_BinaryPacket_State_SEARCH, /**< SEARCH */
    E4E_BinaryPacket_State_HEADER, /**< HEADER */
    E4E_BinaryPacket_State_MSG, /**< MESSAGE */
    E4E_BinaryPacket_State_ERROR, /**< ERROR */
    E4E_BinaryPacket_State_NELEMS, /**< Number of states */
} E4E_BinaryPacket_State_t;


/**
 * Binary Packet Function Commands
 */

typedef enum E4E_BinaryPacket_CMD_
{
    E4E_BinaryPacket_CMD_SET_TIME, /**< setTime */
    E4E_BinaryPacket_CMD_SET_ALARM, /**< setAlarm */
    E4E_BinaryPacket_CMD_CLEAR_ALARM, /**< clearAlarm */
    E4E_BinaryPacket_CMD_GET_TIME, /**< getTime */
    E4E_BinaryPacket_CMD_SET_STATE, /**< setState */
    E4E_BinaryPacket_CMD_NELEMS, /**< Number of commands */
} E4E_BinaryPacket_CMD__t;

/**
 * Function type for glue functions
 */
typedef int(*ParserCallback_t)(void*, size_t);

/**
 * @brief Parses message received and stores in buffer
 * @param char is stored in buffer[]
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int parse(char);

void E4E_BinaryPacket_registerCallback(E4E_BinaryPacket_CMD__t cmd_id, ParserCallback_t cb);

int E4E_BinaryPacket_reset(void);
#endif
