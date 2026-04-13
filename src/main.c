#include <stdio.h>

#include "logs_demo.h"
#include "blink_demo.h"
#include "i2c_demo.h"
#include "wifi_demo.h"
#include "tcp_demo.h"


void app_main(void) 
{
    wifi_demo();
    tcp_demo();
}