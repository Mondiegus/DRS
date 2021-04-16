/*
 * servo.c
 *
 *		The MIT License.
 *  Created on: 01.08.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *		mateusz@msalamon.pl
 *
 *      Website: https://msalamon.pl/jak-pomachac-swoim-orczykiem-stm32-spotyka-sie-z-serwem/
 *      GitHub:  https://github.com/lamik/Servos_PWM_STM32_HAL
 */
#include "main.h"
#include "tim.h"
#include "servo.h"

void Servo_Init(servo_t *servo, TIM_HandleTypeDef *_htim, uint32_t _channel)
{
	servo->htim = _htim;
	servo->channel = _channel;

	HAL_TIM_PWM_Start(servo->htim, servo->channel);
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Servo_SetAngle(servo_t *servo, uint16_t angle)
{
	if(angle < 0) angle = 0;
	if(angle > 180) angle = 180;

	  uint16_t tmp = map(angle, ANGLE_MIN, ANGLE_MAX, SERVO_MIN, SERVO_MAX);
	  __HAL_TIM_SET_COMPARE(servo->htim, servo->channel, tmp);
}

void ProceedServo(servo_t *servo1, servo_t *servo2, uint16_t *DrsFrame)
{
	static uint8_t CurrentAngle = 0;
	static uint8_t angleDirection = 0;

	if(*DrsFrame)
	  {
		  if (CurrentAngle == 0)
		  {
			  angleDirection = 1;
		  }
		  else if (CurrentAngle == 180)
		  {
			  angleDirection = 0;
		  }

		  if (angleDirection)
		  {
			  CurrentAngle++;
		  }
		  else
		  {
			  CurrentAngle--;
		  }
		  Servo_SetAngle(servo2, CurrentAngle);
		  Servo_SetAngle(servo1, 180-CurrentAngle);
	  }
}
