#include "dash.h"

volatile DashInfo globalStatus = {0};

int32_t prevRefresh = BAD_TIME_Negative1;
int32_t tempsRefreshRateMillis = 50;
