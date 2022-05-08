//
// Created by o.o on 2022/4/16.
//

#include "ZJY/ZJY.h"
#include "ZJY/retarget.h"
///when it was set 0,the counter will be enable
static ZJY_state zjy_state;
static uint32_t counter = 1;
ADC_ChannelConfTypeDef sConfig = {0};

static uint8_t key_lock_tim = 0;
static uint8_t key_lock_fre = 0;
static uint8_t key_lock_mode = 0;

static uint8_t  stop = 0;


/// 对应  20/ 40/  60/ 80/100
/// 对应 250/120/85/60/50
uint32_t times_1[6]    = {0,250,120,85,60,50};
uint32_t times_2[11] = {0,0,2489,0,625,0,416,0,312,0,250};
/// 对应  4/ 8/ 12  /16  /20
/// 对应 430/250/400/120 /100

uint8_t pre_mode = 0;
State pwm_state = 0;///用于主循环的地方是否开启pwm的

///we should check PA1 PB3
Is_OK First_Check(uint8_t mode, uint8_t cur_led,uint8_t mc,uint8_t adc_gear,uint8_t fre_start,uint8_t period, uint8_t is_out,uint8_t pause_us ,uint32_t time_of_off) {
    ///All LED_mode start
    LED_LX_ON;
    LED_DX_ON;
    LED_SM_ON;
    HAL_Delay(1000);///wait
    if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))
    {
        LED_LX_OFF;
        LED_DX_OFF;
        LED_SM_OFF;
        Beep_Times(100,1);
        ///顺便在此初始化
        zjy_state.PB3_Mode = mode;
        zjy_state.Cur_LED = cur_led;
        zjy_state.PA2_MC_TIM = mc;///mc_timer not work
        zjy_state.PA6_ADC_gear = adc_gear;///1-5
        zjy_state.PA1_Fre_start = fre_start;///fre not work
        zjy_state.period = period;
        zjy_state.Is_Out = is_out;
        zjy_state.pause_us = 200;
        zjy_state.time_of_off = 0;
        printf("oepn- timer");
        ///并开启计时器
        HAL_TIM_Base_Start_IT(&htim1);
        return Ok;
    }
    else{
        Beep_Times(200,100);
        return Error;
    }
}
///mode_key check
void check_key_mode()
{
    uint8_t key;
    if (HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) && !key_lock_mode)
    {
        if(key = Key_scan(0))
        {
            //mode_key
            if(key == 1)///mode_select
            {
                Beep_Times(100,1);
                zjy_state.PB3_Mode += 1;
                if(zjy_state.PB3_Mode > 3)
                    zjy_state.PB3_Mode  = 1;
            }
        }
    }

}
///mc_timer_check
void mc_timer_check()
{
    if(HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) == 1)
    {
        HAL_Delay(20);
        if(HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) == 1)///Mc_timer  not work
        {
            zjy_state.PA2_MC_TIM = 0;
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) == 0)
    {
        HAL_Delay(20);
        if(HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) == 0)///Mc_timer  work
        {
            zjy_state.PA2_MC_TIM = 1;
        }
    }
}
///check F_key work
void check_F_key_work()
{
    if (HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) == 0)
    {
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) == 0)
        {
            zjy_state.PA1_Fre_start = 1;
            key_lock_tim = 1;
            key_lock_fre = 1;
            key_lock_mode = 1;
        }
    }
    else
    {
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) == 1)
        {
            zjy_state.PA1_Fre_start = 0;
            key_lock_tim = 0;
            key_lock_fre = 0;
            key_lock_mode = 0;
        }
    }
}
Is_OK Cur_Check() {
    check_key_mode();
    mc_timer_check();
    check_F_key_work();
    Get_Fre();
    Get_TIM();
    if (zjy_state.time_of_off <= 200)
    {
        zjy_state.time_of_off = 0;
        stop = 1;
    }
    else
        stop = 0;
    return Ok;
}
uint8_t Get_Fre() {
    ///ten
    uint16_t fre = 0;
    if (HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) && !key_lock_fre)///如果强度开了，才去检查时间
    {
        key_lock_fre = 0;
        sConfig.Channel = ADC_CHANNEL_6;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        HAL_ADCEx_Calibration_Start(&hadc1);					 //校准ADC
        HAL_ADC_Start(&hadc1);                               //开启ADC
        HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
        fre = (uint16_t)HAL_ADC_GetValue(&hadc1)+1;	        	//返回最近一次ADC1规则组的转换结果
        if      (0    < fre && fre <  820)
            zjy_state.PA6_ADC_gear  = 1;
        else if (820  < fre && fre <=1639)
            zjy_state.PA6_ADC_gear  = 2;
        else if (1639 < fre && fre <=2458)
            zjy_state.PA6_ADC_gear  = 3;
        else if (2458 < fre && fre <=3280)
            zjy_state.PA6_ADC_gear  = 4;
        else if (3280 < fre && fre <=4096)
            zjy_state.PA6_ADC_gear  = 5;
    }    else{
    }
}

uint8_t Get_TIM() {
    ///tim
    uint16_t tim = 0;
    ///
    if (!key_lock_tim && HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin))
    {
        sConfig.Channel = ADC_CHANNEL_5;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        HAL_ADCEx_Calibration_Start(&hadc1);					 //校准ADC
        HAL_ADC_Start(&hadc1);                               //开启ADC
        HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
        tim = (uint16_t)HAL_ADC_GetValue(&hadc1)+1;	        	//返回最近一次ADC1规则组的转换结果
        uint32_t min_10 = 600000000;
        if      (0    < tim && tim <  820)
            ///test
            zjy_state.time_of_off  = ((1*min_10)/60)*2; ///10min
        else if (820  < tim && tim <=1639)
            zjy_state.time_of_off  = 2*min_10;
        else if (1639 < tim && tim <=2458)
            zjy_state.time_of_off  = 3*min_10;
        else if (2458 < tim && tim <=3280)
            zjy_state.time_of_off  = 4*min_10;
        else if (3280 < tim && tim <=4096)
            zjy_state.time_of_off  = 5*min_10;///50min
    }    else{///timer = 0
    }
}
///beep loop
void Beep_Times(uint8_t ms, uint8_t times) {
    for (int i = 0; i < times; ++i) {
        BEEP_ON;
        HAL_Delay(ms);
        BEEP_OFF;
        HAL_Delay(ms);
    }
}
///key_mode
uint8_t Key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;     //按键松开标志
    ///如果是1，就进去，继续汇报按下
    if(mode==1)key_up=1;    //支持连按
    ///此处是如果up是0，则第二次进入的时候，就不再进去，就直接返回0
    ///如果是0，判断按键是否全部复位，如果复位后才使能up，如此开启下一次
    if(key_up && (HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port,KEY_MODE_Pin)==0))
    {
        HAL_Delay(10);
        key_up=0;
        if(HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port,KEY_MODE_Pin)==0)
            return 1;
    }else if(HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port,KEY_MODE_Pin)==1)
        key_up=1;
    return 0;   //无按键按下
}
///show_LED
void LED_Work() {
    switch (zjy_state.PB3_Mode) {
        case 1:
            Just_LX;
            break;
        case 2:
            Just_DX;
            break;
        case 3:
            Just_SM;
            break;
    }
}
Is_OK PWM_Start() {
    ///此处做了修改，用作调试
    if (!HAL_GPIO_ReadPin(KEY_F_START_GPIO_Port,KEY_F_START_Pin) )
    {        ///开启输出
        if (zjy_state.time_of_off > 200)
        {
            pwm_state = 1;
        }
        else if (pwm_state == 1)
        {
            pwm_state = 0;
            Beep_Times(100,5);
            HAL_GPIO_WritePin(pwm_GPIO_Port,pwm_Pin,GPIO_PIN_RESET);
        }
    }
    else if (pwm_state)
    {
        pwm_state = 0;
        Beep_Times(100,5);
        HAL_GPIO_WritePin(pwm_GPIO_Port,pwm_Pin,GPIO_PIN_RESET);
    }
}
///对于down，每次轮询都经过，对于up仅在需要的时候调用这个
void pwm_up_and_down()
{
    PWM_UP;
    zjy_state.Is_Out = 1;
}

void down_the_pwm()
{
    if (zjy_state.Is_Out == 1)
    {
        PWM_DOWN;
        zjy_state.Is_Out = 0;
    }
}
void LED_FLow()
{
    ///gear 1-5
    ///对应200-1000
    if (counter % times_2[zjy_state.PA6_ADC_gear * 2]== 0)
    {
        HAL_GPIO_TogglePin(LED_F_GPIO_Port,LED_F_Pin);
    }
    ///如果快到一分钟时候，疯狂闪动
    if (zjy_state.time_of_off < 1*60*1000*1000 && counter % times_2[10]== 0)
    {
        HAL_GPIO_TogglePin(LED_MIT_GPIO_Port,LED_MIT_Pin);
    }

}
void pwm_set_of_flu()
{
    if (counter % times_1[zjy_state.PA6_ADC_gear] == 0)
    {
        PWM_UP;
        zjy_state.Is_Out = 1;
    }
}
void pwm_set_of_flu2()
{
    if (counter % times_2[(zjy_state.PA6_ADC_gear)*2] == 0)
    {
        PWM_UP;
        zjy_state.Is_Out = 1;
    }
}
void pwm_set_up()
{
    ///100 = 100hz 200 = 50hz 150= 65hz
    if (pwm_state == 1)
    {
        if (pre_mode != zjy_state.PB3_Mode)
        {
            counter = 1;
            pre_mode = zjy_state.PB3_Mode;
        }
        if (zjy_state.PB3_Mode == 1)
        {
            pwm_set_of_flu();
        }
        else if(zjy_state.PB3_Mode == 2 && counter < (100001/4)*3)
        {
            pwm_set_of_flu();
        }
        else if (zjy_state.PB3_Mode == 3)
        {
            if (counter < (100001/4))
            {
                pwm_set_of_flu2();
            }
            if (counter > (100001/4))
            {
                pwm_set_of_flu();
            }

        }
    }
    else{
        PWM_DOWN;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    ///因为时钟不准确，所以是90ms
    if (htim->Instance == TIM1)
    {
        ///逻辑，每一轮都判断是否需要关闭
        ///每轮的时常是200us
        ///mode1，是连续波，以100，80，60，40，20五个挡位
        /// 对应，10000us，12500us，16666us，25000us，50000us
        /// 对应，50,62,83,250
        ///因为是最大的是250，则当最大的时候归为0
        counter++;
        ///换算成时间：.
        zjy_state.us = (counter - 1) * 200;
        if (zjy_state.time_of_off>=200)
        zjy_state.time_of_off-=200;
        down_the_pwm();///因为定时器等于脉宽因此每此进入都可以判断是否关闭pwm
        LED_FLow();
        pwm_set_up();

        ///因为现在最大的定时为20秒的周期循环，因此在20秒就counter复位
        if (counter>100001)///(20s*1000*1000)
            counter = 1;
    }
}





