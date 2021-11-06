//
// Created by WSJ on 2021/11/2.
//

#include "System_Config.h"
#include "chassis_task.h"
#include "task.h"
#include "referee_task.h"
#include "UI_task.h"


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
TaskHandle_t UI_Task_Handle;


/**
* @brief Load and start User Tasks.
* @note  Edit this function to add tasks into the activated tasks list.
*/
void Task_start(void) {
    /* Syetem Service init --------------*/
    /* Applications Init ----------------*/
    xTaskCreate(chassis_task, "chassis_task", Normal_Stack_Size, nullptr, PriorityHigh, &Chassis_Task_Handle);
    xTaskCreate(UI_task, "UI_task", Normal_Stack_Size, nullptr, PriorityHigh, &UI_Task_Handle);
}