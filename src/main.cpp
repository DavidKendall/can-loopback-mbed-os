#include <stdbool.h>
#include "mbed.h"
#include "fsl_flexcan.h"

Ticker ticker;
DigitalOut red(LED_RED);
DigitalOut green(LED_GREEN);
flexcan_config_t flexcanConfig;
flexcan_frame_t txFrame;
uint32_t counter = 0;

void send(void) {
    counter += 1;
    txFrame.dataWord0 = counter;
    if (kStatus_Success == FLEXCAN_TransferSendBlocking(CAN0, 0, &txFrame)) {
        red = 1;
        green = 1 - green;
    } else {
        red = 0;
        green = 1;
    }
}

int main() {
    red = 0;
    green = 1;
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    FLEXCAN_Init(CAN0, &flexcanConfig, 8000000UL);
    FLEXCAN_SetTxMbConfig(CAN0, 0, true);
    txFrame.length = 4;
    txFrame.type = kFLEXCAN_FrameTypeData; 
    txFrame.format = kFLEXCAN_FrameFormatStandard;
    txFrame.id = FLEXCAN_ID_STD(0x123);
    ticker.attach(send, 1);
    red = 1;
    while(true) {
    }
}
