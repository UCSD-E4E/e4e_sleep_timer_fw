typedef struct header_ {
    uint16_t start;
    uint16_t length;
    uint16_t cmd_id;
} header_t;
uint8_t buffer[128];
header_t* pHeader = (header_t*) buffer;
size_t idx = 0;


typedef enum state_
{
    SEARCH,
    HEADER,
    MSG,
    ERROR,
} state_t;

state_t state = SEARCH;

...
int parse(char c)
{
	buffer[idx++] = c;
    switch(state)
    {
        case SEARCH:
            if (buffer[0] != 0xE4)
            {
				idx = 0;
			}
			else
			{
				state++;
			}
			break;
		case HEADER:
			if (pHeader->start != 0xE4EB)
			{
				idx = 0;
				state = SEARCH;
			}
			if (idx == 6)
			{
				state++;
			};
			break;
		case MSG:
			if (idx == pHeader->length)
			{
			    // process
		    }
			state = SEARCH;
			idx = 0;
		default:
			idx = 0;
			state = SEARCH;
	}
}
