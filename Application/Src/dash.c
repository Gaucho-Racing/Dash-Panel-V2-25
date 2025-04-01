#include "dash.h"

volatile DashInfo globalStatus = {0};

int32_t dischargeStartMillis = BAD_TIME_Negative1;