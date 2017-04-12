/**
 *	Keil project
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "stm32f10x.h"
#include "stm32f10x.h"
#include "iNEMO_Led.h"
#include "stm32f10x_usart.h"
#include "tm_stm32f4_nrf24l01.h"

// My address
uint8_t MyAddress[] = {
	0xE7,
	0xE7,
	0xE7,
	0xE7,
	0xE7
};
// Receiver address
uint8_t TxAddress[] = {
	0x7E,
	0x7E,
	0x7E,
	0x7E,
	0x7E
};

uint8_t dataOut[32], dataIn[32];
void DEBUG_Usart_Init(void) {

		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;

		/* enable peripheral clock for USART2 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		/* Enable UART clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

		/* Configure USART Tx as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* Configure USART Rx as input floating
		 GPIO_InitStructure.GPIO_Pin = COMM_USART_RxPin;
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		 GPIO_Init(COMM_USART_GPIO, &GPIO_InitStructure);
		 */
		GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl =
		USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure);

		USART_Cmd(USART1, ENABLE); // enable USART2

}

int main(void) {
	TM_NRF24L01_Transmit_Status_t transmissionStatus;
	char str[40];

	//Initialize system
	SystemInit();
	DEBUG_Usart_Init();
	iNEMO_Led_Init(LED1);
	printf("System Init!\r\n");
	//Initialize NRF24L01+ on channel 15 and 32bytes of payload
	//By default 2Mbps data rate and 0dBm output power
	//NRF24L01 goes to RX mode by default
	TM_NRF24L01_Init(15, 32);

	//Set 2MBps data rate and -18dBm output power
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_0dBm);

	//Set my address, 5 bytes
	TM_NRF24L01_SetMyAddress(MyAddress);
	//Set TX address, 5 bytes
	TM_NRF24L01_SetTxAddress(TxAddress);
	printf("Nrf Init!\r\n");
	uint16_t i=0,j=0;
	while (1) {
		if(i==0){
		//Fill data with something
		sprintf((char *)dataOut, "abcdefghijklmnoszxABCDEFCBDA");
		//Display on USART
		printf("pinging: \r\n");
		j=0;
		//Transmit data, goes automatically to TX mode
		TM_NRF24L01_Transmit(dataOut);

		iNEMO_Led_On(LED1);
		//Wait for data to be sent
		do {
			transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
		} while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
		iNEMO_Led_Off(LED1);

		//Go back to RX mode
		TM_NRF24L01_PowerUpRx();

		//Wait received data, wait max 100ms, if time is larger, than data was probably lost
		while (!TM_NRF24L01_DataReady() && j++ < 1000);

		//Show ping time
		printf("\t%d j",j);

		//Get data from NRF2L01+
		TM_NRF24L01_GetData(dataIn);

		//Check transmit status
		if (transmissionStatus == TM_NRF24L01_Transmit_Status_Ok) {
			//Transmit went OK
			printf("\tOK\n\r");
		} else if (transmissionStatus == TM_NRF24L01_Transmit_Status_Lost) {
			//Message was LOST
			printf("\tLOST\n\r");
		} else {
			//This should never happen
			printf("\tSENDING\n\r");
		}
	}
	else{
		i++;
		if(i==2000) i=0;

	}

}
}
