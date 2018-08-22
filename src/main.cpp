#include <stdbool.h>
#include "mbed.h"
#include "fsl_flexcan.h"

#define RXBUFID 9

DigitalOut green(LED_GREEN);
Serial pc(USBTX, USBRX); // tx, rx
flexcan_config_t flexcanConfig;
flexcan_rx_mb_config_t mbconfig;
flexcan_frame_t rxFrame;
uint32_t counter = 0;

void can_pins_init(void) {

    /* Enable the clock to the FLEXCAN module */
    SIM->SCGC6 |= SIM_SCGC6_FLEXCAN0_MASK;

    /* Enable the clock to PORT B */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    /* Select the CAN function (Alternative 2) for pins 18 and 19 of PORT B */
    PORTB->PCR[18] |= PORT_PCR_MUX(2) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
    PORTB->PCR[19] |= PORT_PCR_MUX(2) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
}

int main() {
    can_pins_init();
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    FLEXCAN_Init(CAN0, &flexcanConfig, CLOCK_GetOsc0ErClkFreq());
    mbconfig.type = kFLEXCAN_FrameTypeData;
    mbconfig.format = kFLEXCAN_FrameFormatStandard;
    mbconfig.id = FLEXCAN_ID_STD(0x123);
    FLEXCAN_SetRxMbConfig(CAN0, RXBUFID, &mbconfig, true);
    pc.printf("\n\rCAN Receiver Test\n\r");
    while(true) {
        if (kStatus_Success == FLEXCAN_TransferReceiveBlocking(CAN0, RXBUFID, &rxFrame)) {
            printf("RxOk: %09ld\n\r", rxFrame.dataWord0);
            green = 1 - green;
        } else {
            printf("No Rx\n\r");
        }
    }
}
