#include <stdbool.h>

#include "cmsis_os2.h"
#include "dash.h"
#include "msgIDs.h"
#include "grIDs.h"
#include "fdcan.h"

// TODO Confirm all scaling is appropriate
volatile DashInfo globalStatus = {0};

int32_t prevRefresh = BAD_TIME_Negative1;
int32_t tempsRefreshRateMillis = 50;

void recievedNewInformationPleaseRefresh(void)
{
    writeMessage(MSG_DASH_STATUS, GR_ECU, (uint8_t*)&globalStatus.dashStatusMsg, 3);

    // TODO: Implement functionality to call for a refresh the screen based off of the just-updated DashInfo from CANFD
}
