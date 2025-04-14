#include "dash.h"

volatile DashInfo globalStatus = {0};

int32_t prevRefresh = BAD_TIME_Negative1;
int32_t tempsRefreshRateMillis = 50;

void recievedNewInformationPleaseRefresh(void)
{
    // TODO: Implement functionality to call for a refresh the screen based off of the just-updated DashInfo from CANFD
}
