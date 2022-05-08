//
// Created by o.o on 2022/4/16.
//

#ifndef ZJY_TEAM_ZJY_H
#define ZJY_TEAM_ZJY_H
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"

typedef uint8_t Is_OK;
typedef uint8_t State;


#define PWM_UP    HAL_GPIO_WritePin(pwm_GPIO_Port, pwm_Pin, GPIO_PIN_SET)
#define PWM_DOWN  HAL_GPIO_WritePin(pwm_GPIO_Port, pwm_Pin, GPIO_PIN_RESET)

#define BEEP_ON  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET)
#define BEEP_OFF HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET)

#define LED_LX_ON  HAL_GPIO_WritePin(LED_LX_GPIO_Port,LED_LX_Pin,GPIO_PIN_SET)
#define LED_DX_ON  HAL_GPIO_WritePin(LED_DX_GPIO_Port,LED_DX_Pin,GPIO_PIN_SET)
#define LED_SM_ON  HAL_GPIO_WritePin(LED_SM_GPIO_Port,LED_SM_Pin,GPIO_PIN_SET)

#define LED_LX_OFF  HAL_GPIO_WritePin(LED_LX_GPIO_Port,LED_LX_Pin,GPIO_PIN_RESET)
#define LED_DX_OFF  HAL_GPIO_WritePin(LED_DX_GPIO_Port,LED_DX_Pin,GPIO_PIN_RESET)
#define LED_SM_OFF  HAL_GPIO_WritePin(LED_SM_GPIO_Port,LED_SM_Pin,GPIO_PIN_RESET)

#define Just_LX LED_LX_ON;LED_DX_OFF;LED_SM_OFF
#define Just_DX LED_LX_OFF;LED_DX_ON;LED_SM_OFF
#define Just_SM LED_LX_OFF;LED_DX_OFF;LED_SM_ON

#define Ok 1
#define Error 0

#define pulse_width 200


typedef struct
{

    State PA2_MC_TIM;///Check_mechanical  1-work 0-not-work
    State PA1_Fre_start;///check Fre_State
    State PA6_ADC_gear;///frequency(20-100)
    State PB3_Mode;///three waveform
    uint32_t Cur_LED;///1-5
    State Is_Out;///is out pwm _ point to hight
    uint8_t period;///count the times of one cycle
    uint8_t pause_us;
    uint8_t us;
    uint32_t time_of_off;
}ZJY_state;

///the stream of all
Is_OK First_Check(uint8_t mode, uint8_t cur_led,uint8_t mc,uint8_t adc_gear,uint8_t fre_start,uint8_t period, uint8_t is_out,uint8_t pause_us,uint32_t time_of_off);
Is_OK Cur_Check();///check the state and update
Is_OK PWM_Start();

///IO control
void Beep_Times(uint8_t ms,uint8_t times);
void LED_Work();
uint8_t Get_Fre();
uint8_t Get_TIM();
uint8_t Key_scan(uint8_t mode);

///callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif
#endif //ZJY_TEAM_ZJY_H
