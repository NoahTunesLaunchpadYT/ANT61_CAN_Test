/*
 * can.c
 *
 *  Created on: Jul 4, 2024
 *      Author: noahs
 */

#include "can.h"

FDCAN_TxHeaderTypeDef create_TxHeader();
void Error_Handler();
void send_CAN(FDCAN_HandleTypeDef hfdcan1, uint16_t *TxData);

FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader = {0};
uint16_t RxData[4] = {0};

void initialise_CAN(FDCAN_HandleTypeDef *hfdcan1)
{
	TxHeader = create_TxHeader();

	if (HAL_FDCAN_Start(hfdcan1) != HAL_OK)
	{
	  Error_Handler();
	}
	if (HAL_FDCAN_ActivateNotification(hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	{
	   // Notification Error
	   Error_Handler();
	}
}

FDCAN_TxHeaderTypeDef create_TxHeader()
{
	FDCAN_TxHeaderTypeDef Header;
	Header.Identifier = 0x00;
	Header.IdType = FDCAN_STANDARD_ID;
	Header.TxFrameType = FDCAN_DATA_FRAME;
	Header.DataLength = FDCAN_DLC_BYTES_8;
	Header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	Header.BitRateSwitch = FDCAN_BRS_OFF;
	Header.FDFormat = FDCAN_CLASSIC_CAN;
	Header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	Header.MessageMarker = 0;

	return Header;
}

void send_CAN(FDCAN_HandleTypeDef hfdcan1, uint16_t *TxData16) {

	uint8_t *TxData = (uint8_t*)TxData16;

	int result = HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
	if (result != HAL_OK)
	{
	Error_Handler ();
	}

	while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) != 2)  {
		HAL_Delay(100);
	}
}

void send_angles(FDCAN_HandleTypeDef hfdcan1, uint16_t shoulder, uint16_t elbow1, uint16_t elbow2, uint16_t wrist) {
	uint16_t TxData[4] = {shoulder, elbow1, elbow2, wrist};

	send_CAN(hfdcan1, TxData);

	HAL_Delay(10);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	uint8_t *RxData_8 = (uint8_t*)RxData;

    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData_8) != HAL_OK)
    {
        // Receive Error
        Error_Handler();
    }
}

void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{
    HAL_Delay(10);
}

void HAL_FDCAN_ErrorCallback(FDCAN_HandleTypeDef *hfdcan)
{
    hfdcan->ErrorCode &= ~(FDCAN_IR_ELO | FDCAN_IR_WDI | FDCAN_IR_PEA | FDCAN_IR_PED | FDCAN_IR_ARA);
}

uint16_t get_shoulder() {
	return RxData[0];
}

uint16_t get_elbow1() {
	return RxData[1];
}

uint16_t get_elbow2() {
	return RxData[2];
}

uint16_t get_wrist() {
	return RxData[3];
}

uint16_t encode(float degrees) {
	uint16_t steps = (uint16_t)(((degrees - 90) / 360) * 65536);

	while (steps < 0) {
		steps += 65536;
	}

	return steps;
}

float decode(uint16_t steps) {
	float degrees = ((float)(steps)/65536*360 + 90);

	while (degrees >= 360) {
		degrees -= 360;
	}

	return degrees;
}
