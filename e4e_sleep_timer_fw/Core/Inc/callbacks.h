#ifndef GLUE_H
#define GLUE_H

/**
 * @brief Callback to setAlarm function for parser
 * @param void* is setAlarm parameters from binary packet
 * @param size_t is length of parameter message
 * @return E4E_OK if successful, E4E_ERROR otherwise
 */
int setAlarmCallback(void*, size_t);

/**
 * @brief Callback to getTime function for parser
 * @param void* is getTime parameters from binary packet
 * @param size_t is length of parameter message
 * @return E4E_OK if successful, E4E_ERROR otherwise
 */
int getTimeCallback(void*, size_t);

/**
 * @brief Callback to clearAlarm function for parser
 * @param void* is clearAlarm parameters from binary packet
 * @param size_t is length of parameter message
 * @return E4E_OK if successful, E4E_ERROR otherwise
 */
int clearAlarmCallback(void*, size_t);

#endif
