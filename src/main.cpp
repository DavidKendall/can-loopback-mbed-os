#include <stdbool.h>
#include "mbed.h"
#include "fsl_flexcan.h"

Ticker ticker;
DigitalOut red(LED_RED);
DigitalOut green(LED_GREEN);
Serial pc(USBTX, USBRX);
flexcan_config_t flexcanConfig;
flexcan_frame_t txFrame;
uint32_t counter = 0;

void send(void) {
    counter += 1;
    txFrame.dataWord0 = counter;
    pc.printf("Sending %09ld... ", counter);
    if (kStatus_Success == FLEXCAN_TransferSendBlocking(CAN0, 0, &txFrame)) {
        red = 1; // off
        green = 1 - green;
        pc.printf("Ok\n\r");
    } else {
        green = 1; // off
        red = 1 - red;
        pc.printf("Fail\n\r");
    }
}

int main() {
    red = 0;    // on
    green = 1;  // off
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    FLEXCAN_Init(CAN0, &flexcanConfig, 8000000UL);
    FLEXCAN_SetTxMbConfig(CAN0, 0, true);
    txFrame.length = 4;
    txFrame.type = kFLEXCAN_FrameTypeData; 
    txFrame.format = kFLEXCAN_FrameFormatStandard;
    txFrame.id = FLEXCAN_ID_STD(0x123);
    red = 1;    // off
    pc.printf("CAN Send Test\n\r");
    ticker.attach(send, 1);
    while(true) {
    }
}
