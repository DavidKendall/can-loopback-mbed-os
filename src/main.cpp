#include <stdbool.h>
#include "mbed.h"
#include "MK64F12.h"
#include "fsl_flexcan.h"

#define TX_BUF_ID 8

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
    pc.printf("CAN Error Counter: %ld\n\r", CAN0->ECR);
    pc.printf("CAN Status Flags: 0x%08X\n\r", FLEXCAN_GetStatusFlags(CAN0));
    pc.printf("Sending %09ld... ", counter);
    //if (kStatus_Success == FLEXCAN_TransferSendBlocking(CAN0, TX_BUF_ID, &txFrame)) {
    if (kStatus_Success == FLEXCAN_WriteTxMb(CAN0, TX_BUF_ID, &txFrame)) {
        red = 1; // off
        green = 1 - green;
        pc.printf("Ok\n\r");
    } else {
        green = 1; // off
        red = 1 - red;
        pc.printf("Fail\n\r");
    }
}

void can_pins_init(void) {

    /* Enable the clock to the FLEXCAN module */
    SIM->SCGC6 |= SIM_SCGC6_FLEXCAN0_MASK;

    /* Enable the clock to PORT B */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    /* Select the CAN function (Alternative 2) for pin 18 of PORT B */
    PORTB->PCR[18] |= PORT_PCR_MUX(2);
}

int main() {
    red = 0;    // on
    green = 1;  // off
    can_pins_init();
    pc.printf("OSC_CR[ERCLKEN]: %d\n\r", (OSC->CR & OSC_CR_ERCLKEN_MASK) ? 1 : 0);
    pc.printf("Osc0ErClkFreq: %d\n\r", CLOCK_GetOsc0ErClkFreq());
    //pc.printf("PeriphClkFreq: %d\n\r", CLOCK_GetFreq(kCLOCK_McgPeriphClk));
    pc.printf("PeriphClkFreq: %d\n\r", CLOCK_GetPll0Freq());
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    //flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
    FLEXCAN_Init(CAN0, &flexcanConfig, CLOCK_GetOsc0ErClkFreq());
    //FLEXCAN_Init(CAN0, &flexcanConfig, CLOCK_GetFreq(kCLOCK_McgPeriphClk));
    //FLEXCAN_Init(CAN0, &flexcanConfig, 50000000UL);
    FLEXCAN_SetTxMbConfig(CAN0, TX_BUF_ID, true);
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
