//
// Created by WSJ on 2021/11/2.
//

#include "System_Config.h"
#include "chassis_task.h"
#include "task.h"
#include "remote_data.h"
#include "bsp_usart.h"


#define Tiny_Stack_Size       64
#define Small_Stack_Size      128
#define Normal_Stack_Size     256
#define Large_Stack_Size      512
#define Huge_Stack_Size       1024
#define PriorityVeryLow       1
#define PriorityLow           2
#define PriorityBelowNormal   3
#define PriorityNormal        4
#define PriorityAboveNormal   5
#define PriorityHigh          6
#define PrioritySuperHigh     7
#define PriorityRealtime      8

TaskHandle_t Chassis_Task_Handle;



void Task_init() {
    /* Syetem Service init --------------*/
    remote_control_init();
    usart1_tx_dma_init();
    /* Applications Init ----------------*/
}

/**
* @brief Load and start User Tasks.
* @note  Edit this function to add tasks into the activated tasks list.
*/
void Task_start(void) {
    /* Syetem Service init --------------*/
    /* Applications Init ----------------*/
    xTaskCreate(chassis_task, "chassis_task", Normal_Stack_Size, NULL, PriorityHigh, &Chassis_Task_Handle);
}