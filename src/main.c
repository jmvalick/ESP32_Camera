#include <stdio.h>

#include "logs_demo.h"
#include "blink_demo.h"
#include "wifi.h"
#include "tcp.h"
#include "sdspi.h"
#include "take_picture.h"


void app_main(void) 
{
    log_demo();
    //wifi_connect();
    //tcp_connect();
    take_picture();
}