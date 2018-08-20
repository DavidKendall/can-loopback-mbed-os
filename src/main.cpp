#include <stdbool.h>
#include "mbed.h"
#include "fsl_flexcan.h"

Ticker ticker;
DigitalOut green(LED_GREEN);
Serial pc(USBTX, USBRX); // tx, rx
flexcan_config_t flexcanConfig;
flexcan_rx_mb_config_t mbconfig;
flexcan_frame_t txFrame;
flexcan_frame_t rxFrame;
uint32_t counter = 0;

void send(void) {
    counter += 1;
    txFrame.dataWord0 = counter;
    if (kStatus_Success == FLEXCAN_TransferSendBlocking(CAN0, 0, &txFrame)) {
        pc.printf("TxOk: %09ld\n\r", counter);
        green = 1;
    } else {
        pc.printf("Tx error\n\r");
    }
}

int main() {
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    flexcanConfig.enableLoopBack = true;
    FLEXCAN_Init(CAN0, &flexcanConfig, 50000000UL);
    mbconfig.type = kFLEXCAN_FrameTypeData;
    mbconfig.format = kFLEXCAN_FrameFormatStandard;
    mbconfig.id = FLEXCAN_ID_STD(0x123);
    FLEXCAN_SetRxMbConfig(CAN0, 1, &mbconfig, true);
    txFrame.length = 4;
    txFrame.type = kFLEXCAN_FrameTypeData; 
    txFrame.format = kFLEXCAN_FrameFormatStandard;
    txFrame.id = FLEXCAN_ID_STD(0x123);
    pc.printf("\n\rCAN Loopback Test\n\r");
    ticker.attach(send, 1);
    while(true) {
        if (kStatus_Success == FLEXCAN_TransferReceiveBlocking(CAN0, 1, &rxFrame)) {
            pc.printf("RxOk: %09ld\n\r", rxFrame.dataWord0);
            green = 0;
        } else {
            pc.printf("No Rx\n\r");
        }
    }
}
