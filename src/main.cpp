#include <stdbool.h>
#include "mbed.h"
#include "fsl_flexcan.h"

DigitalOut green(LED_GREEN);
Serial pc(USBTX, USBRX); // tx, rx
flexcan_config_t flexcanConfig;
flexcan_rx_mb_config_t mbconfig;
flexcan_frame_t rxFrame;
uint32_t counter = 0;

int main() {
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
    FLEXCAN_Init(CAN0, &flexcanConfig, CLOCK_GetFreq(kCLOCK_McgPeriphClk));
    mbconfig.type = kFLEXCAN_FrameTypeData;
    mbconfig.format = kFLEXCAN_FrameFormatStandard;
    mbconfig.id = FLEXCAN_ID_STD(0x123);
    FLEXCAN_SetRxMbConfig(CAN0, 1, &mbconfig, true);
    pc.printf("\n\rCAN Receiver Test\n\r");
    while(true) {
        if (kStatus_Success == FLEXCAN_TransferReceiveBlocking(CAN0, 1, &rxFrame)) {
            printf("RxOk: %09ld\n\r", rxFrame.dataWord0);
            green = 1 - green;
        } else {
            printf("No Rx\n\r");
        }
    }
}
