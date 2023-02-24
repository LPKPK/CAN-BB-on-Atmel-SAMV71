

#include "sample.h"
#include "blackboard.h"

void CanMessageControl_Task(void){

    #ifndef TEST
  static uint16_t currentCount = (uint16_t)0;
  #endif

  if ((uint16_t)0 == currentCount)
  { // 20Hz (req >= 10Hz)
    bb_set_SCM_motor1MaxSpeed(4782.5);
  }
  if ((currentCount & (uint16_t)1) == (uint16_t)1)
  { // 100Hz (req >= 50Hz)
    bb_set_SCM_motor1MinSpeed(2302);
  }
  // 200Hz (req >= 100Hz)

	bb_set_SCM_Motor1Control(add_1());
	bb_set_SCM_Motor3Control(add_1()+add_1());

  ++currentCount;
  if ((uint16_t)10 <= currentCount)
  {
    currentCount = (uint16_t)0;
  }

}