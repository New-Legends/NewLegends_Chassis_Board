//
// Created by WSJ on 2021/11/4.
//

#include "referee_data.h"
#include "main.h"
#include "cmsis_os.h"

#include "bsp_usart.h"
//#include "detect_task.h"

#include "CRC8_CRC16.h"
#include "fifo.h"
#include "protocol.h"

extern UART_HandleTypeDef huart6;

uint8_t usart6_buf[2][USART_RX_BUF_LENGHT];

fifo_s_t referee_fifo;
uint8_t referee_fifo_buf[REFEREE_FIFO_BUF_LENGTH];
unpack_data_t referee_unpack_obj;

frame_header_struct_t referee_receive_header;
frame_header_struct_t referee_send_header;

ext_game_state_t game_state;
ext_game_result_t game_result;
ext_game_robot_HP_t game_robot_HP_t;

ext_event_data_t field_event;
ext_supply_projectile_action_t supply_projectile_action_t;
ext_supply_projectile_booking_t supply_projectile_booking_t;
ext_referee_warning_t referee_warning_t;
ext_dart_remaining_time_t dart_remaining_time_t;

ext_game_robot_state_t robot_state;
ext_power_heat_data_t power_heat_data_t;
ext_game_robot_pos_t game_robot_pos_t;
ext_buff_musk_t buff_musk_t;
aerial_robot_energy_t aerial_robot_energy;
ext_robot_hurt_t robot_hurt_t;
ext_shoot_data_t shoot_data_t;
ext_bullet_remaining_t bullet_remaining_t;
ext_rfid_status_t rfid_status_t;
ext_dart_client_cmd_t dart_client_cmd;
ext_student_interactive_data_t student_interactive_data_t;

void init_referee_struct_data(void) {
    memset(&referee_receive_header, 0, sizeof(frame_header_struct_t));
    memset(&referee_send_header, 0, sizeof(frame_header_struct_t));

    memset(&game_state, 0, sizeof(ext_game_state_t));
    memset(&game_result, 0, sizeof(ext_game_result_t));
    memset(&game_robot_HP_t, 0, sizeof(ext_game_robot_HP_t));

    memset(&field_event, 0, sizeof(ext_event_data_t));
    memset(&supply_projectile_action_t, 0, sizeof(ext_supply_projectile_action_t));
    memset(&supply_projectile_booking_t, 0, sizeof(ext_supply_projectile_booking_t));
    memset(&referee_warning_t, 0, sizeof(ext_referee_warning_t));
    memset(&dart_remaining_time_t, 0, sizeof(ext_dart_remaining_time_t));

    memset(&robot_state, 0, sizeof(ext_game_robot_state_t));
    memset(&power_heat_data_t, 0, sizeof(ext_power_heat_data_t));
    memset(&game_robot_pos_t, 0, sizeof(ext_game_robot_pos_t));
    memset(&buff_musk_t, 0, sizeof(ext_buff_musk_t));
    memset(&aerial_robot_energy, 0, sizeof(aerial_robot_energy_t));
    memset(&robot_hurt_t, 0, sizeof(ext_robot_hurt_t));
    memset(&shoot_data_t, 0, sizeof(ext_shoot_data_t));
    memset(&bullet_remaining_t, 0, sizeof(ext_bullet_remaining_t));


    memcpy(&rfid_status_t, 0, sizeof(rfid_status_t));
    memcpy(&dart_client_cmd, 0, sizeof(dart_client_cmd));
    memset(&student_interactive_data_t, 0, sizeof(ext_student_interactive_data_t));

}

void referee_data_solve(uint8_t *frame) {
    uint16_t cmd_id = 0;

    uint8_t index = 0;

    memcpy(&referee_receive_header, frame, sizeof(frame_header_struct_t));

    index += sizeof(frame_header_struct_t);

    memcpy(&cmd_id, frame + index, sizeof(uint16_t));
    index += sizeof(uint16_t);

    switch (cmd_id) {
        case GAME_STATE_CMD_ID: {
            memcpy(&game_state, frame + index, sizeof(ext_game_state_t));
        }
            break;
        case GAME_RESULT_CMD_ID: {
            memcpy(&game_result, frame + index, sizeof(game_result));
        }
            break;
        case GAME_ROBOT_HP_CMD_ID: {
            memcpy(&game_robot_HP_t, frame + index, sizeof(ext_game_robot_HP_t));
        }
            break;

        case FIELD_EVENTS_CMD_ID: {
            memcpy(&field_event, frame + index, sizeof(field_event));
        }
            break;
        case SUPPLY_PROJECTILE_ACTION_CMD_ID: {
            memcpy(&supply_projectile_action_t, frame + index, sizeof(supply_projectile_action_t));
        }
            break;
        case SUPPLY_PROJECTILE_BOOKING_CMD_ID: {
            memcpy(&supply_projectile_booking_t, frame + index, sizeof(supply_projectile_booking_t));
        }
            break;
        case REFEREE_WARNING_CMD_ID: {
            memcpy(&referee_warning_t, frame + index, sizeof(ext_referee_warning_t));
        }
            break;
        case DART_REMAINING_TIME_CMD_ID: {
            memcpy(&dart_remaining_time_t, frame + index, sizeof(ext_dart_remaining_time_t));
        }
            break;

        case ROBOT_STATE_CMD_ID: {
            memcpy(&robot_state, frame + index, sizeof(robot_state));
        }
            break;
        case POWER_HEAT_DATA_CMD_ID: {
            memcpy(&power_heat_data_t, frame + index, sizeof(power_heat_data_t));
        }
            break;
        case ROBOT_POS_CMD_ID: {
            memcpy(&game_robot_pos_t, frame + index, sizeof(game_robot_pos_t));
        }
            break;
        case BUFF_MUSK_CMD_ID: {
            memcpy(&buff_musk_t, frame + index, sizeof(buff_musk_t));
        }
            break;
        case AERIAL_ROBOT_ENERGY_CMD_ID: {
            memcpy(&aerial_robot_energy, frame + index, sizeof(aerial_robot_energy));
        }
            break;
        case ROBOT_HURT_CMD_ID: {
            memcpy(&robot_hurt_t, frame + index, sizeof(robot_hurt_t));
        }
            break;
        case SHOOT_DATA_CMD_ID: {
            memcpy(&shoot_data_t, frame + index, sizeof(shoot_data_t));
        }
            break;
        case BULLET_REMAINING_CMD_ID: {
            memcpy(&bullet_remaining_t, frame + index, sizeof(ext_bullet_remaining_t));
        }
            break;
        case RFID_STATUS_CMD_ID: {
            memcpy(&rfid_status_t, frame + index, sizeof(rfid_status_t));
        }
            break;
        case DART_CLIENT_CMD_ID: {
            memcpy(&dart_client_cmd, frame + index, sizeof(dart_client_cmd));
        }
            break;
        case STUDENT_INTERACTIVE_DATA_CMD_ID: {
            memcpy(&student_interactive_data_t, frame + index, sizeof(student_interactive_data_t));
        }
            break;
        default: {
            break;
        }
    }
}

/**
  * @brief          单字节解包
  * @param[in]      void
  * @retval         none
  */
void referee_unpack_fifo_data(void) {
    uint8_t byte = 0;
    uint8_t sof = HEADER_SOF;
    unpack_data_t *p_obj = &referee_unpack_obj;

    while (fifo_s_used(&referee_fifo)) {
        byte = fifo_s_get(&referee_fifo);
        switch (p_obj->unpack_step) {
            case STEP_HEADER_SOF: {
                if (byte == sof) {
                    p_obj->unpack_step = STEP_LENGTH_LOW;
                    p_obj->protocol_packet[p_obj->index++] = byte;
                } else {
                    p_obj->index = 0;
                }
            }
                break;

            case STEP_LENGTH_LOW: {
                p_obj->data_len = byte;
                p_obj->protocol_packet[p_obj->index++] = byte;
                p_obj->unpack_step = STEP_LENGTH_HIGH;
            }
                break;

            case STEP_LENGTH_HIGH: {
                p_obj->data_len |= (byte << 8);
                p_obj->protocol_packet[p_obj->index++] = byte;

                if (p_obj->data_len < (REF_PROTOCOL_FRAME_MAX_SIZE - REF_HEADER_CRC_CMDID_LEN)) {
                    p_obj->unpack_step = STEP_FRAME_SEQ;
                } else {
                    p_obj->unpack_step = STEP_HEADER_SOF;
                    p_obj->index = 0;
                }
            }
                break;
            case STEP_FRAME_SEQ: {
                p_obj->protocol_packet[p_obj->index++] = byte;
                p_obj->unpack_step = STEP_HEADER_CRC8;
            }
                break;

            case STEP_HEADER_CRC8: {
                p_obj->protocol_packet[p_obj->index++] = byte;

                if (p_obj->index == REF_PROTOCOL_HEADER_SIZE) {
                    if (verify_CRC8_check_sum(p_obj->protocol_packet, REF_PROTOCOL_HEADER_SIZE)) {
                        p_obj->unpack_step = STEP_DATA_CRC16;
                    } else {
                        p_obj->unpack_step = STEP_HEADER_SOF;
                        p_obj->index = 0;
                    }
                }
            }
                break;

            case STEP_DATA_CRC16: {
                if (p_obj->index < (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len)) {
                    p_obj->protocol_packet[p_obj->index++] = byte;
                }
                if (p_obj->index >= (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len)) {
                    p_obj->unpack_step = STEP_HEADER_SOF;
                    p_obj->index = 0;

                    if (verify_CRC16_check_sum(p_obj->protocol_packet, REF_HEADER_CRC_CMDID_LEN + p_obj->data_len)) {
                        referee_data_solve(p_obj->protocol_packet);
                    }
                }
            }
                break;

            default: {
                p_obj->unpack_step = STEP_HEADER_SOF;
                p_obj->index = 0;
            }
                break;
        }
    }
}

void USART6_IRQHandler(void) {
    static volatile uint8_t res;
    if (USART6->SR & UART_FLAG_IDLE) {
        __HAL_UART_CLEAR_PEFLAG(&huart6);

        static uint16_t this_time_rx_len = 0;

        if ((huart6.hdmarx->Instance->CR & DMA_SxCR_CT) == RESET) {
            __HAL_DMA_DISABLE(huart6.hdmarx);
            this_time_rx_len = USART_RX_BUF_LENGHT - __HAL_DMA_GET_COUNTER(huart6.hdmarx);
            __HAL_DMA_SET_COUNTER(huart6.hdmarx, USART_RX_BUF_LENGHT);
            huart6.hdmarx->Instance->CR |= DMA_SxCR_CT;
            __HAL_DMA_ENABLE(huart6.hdmarx);
            fifo_s_puts(&referee_fifo, (char *) usart6_buf[0], this_time_rx_len);
            //detect_hook(REFEREE_TOE);
        } else {
            __HAL_DMA_DISABLE(huart6.hdmarx);
            this_time_rx_len = USART_RX_BUF_LENGHT - __HAL_DMA_GET_COUNTER(huart6.hdmarx);
            __HAL_DMA_SET_COUNTER(huart6.hdmarx, USART_RX_BUF_LENGHT);
            huart6.hdmarx->Instance->CR &= ~(DMA_SxCR_CT);
            __HAL_DMA_ENABLE(huart6.hdmarx);
            fifo_s_puts(&referee_fifo, (char *) usart6_buf[1], this_time_rx_len);
            //detect_hook(REFEREE_TOE);
        }
    }
}


