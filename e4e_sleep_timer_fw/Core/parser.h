#define HEADLEN 6

typedef struct header_ {
    uint16_t start;
    uint16_t length;
    uint16_t cmd_id;
} header_t;

/* SET ALARM & SET TIME */
typedef struct extractTime_ {
    header_t head;
    uint32_t sec;
} extime_t;

typedef enum state_
{
    SEARCH,
    HEADER,
    MSG,
    ERROR,
} state_t;

typedef enum command_
{
    SET_TIME,
    SET_ALARM,
    CLEAR_ALARM,
    GET_TIME,
    SET_STATE,
} cmd_t;

int parse(char);

//Time and Alarm Functions
int E4E_HAL_RTC_setTime(E4E_HAL_RTCDesc_t *pDesc, int64_t datetime);
int E4E_HAL_RTC_setAlarm(E4E_HAL_RTCDesc_t *pDesc, int64_t alarm);
int E4E_HAL_RTC_clearAlarm(E4E_HAL_RTCDesc_t *pDesc);
int E4E_HAL_RTC_getTime(E4E_HAL_RTCDesc_t *pDesc, int64_t *pDatetime);

//Power Control Functions
int E4E_HAL_PwrCtrl_setState(E4E_HAL_PWRCTRLDesc_t *pDesc,E4E_HAL_PWRCTRL_State_e state);

//General call function
int callfunc();
