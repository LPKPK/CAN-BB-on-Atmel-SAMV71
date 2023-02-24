


#include "blackboard.h"

void CanMessageControl_Task(void){

    #ifndef TEST
  static uint16_t currentCount = (uint16_t)0;
  #endif

  if ((uint16_t)0 == currentCount)
  { // 20Hz (req >= 10Hz)
    bb_set_SCM_motor1MaxSpeed(4095.5);
  }
  if ((currentCount & (uint16_t)1) == (uint16_t)1)
  { // 100Hz (req >= 50Hz)
    bb_set_SCM_motor1MinSpeed(5072);
  }
  // 200Hz (req >= 100Hz)

	bb_set_SCM_Motor1Control(42);
	bb_set_SCM_Motor3Control(90);

  ++currentCount;
  if ((uint16_t)10 <= currentCount)
  {
    currentCount = (uint16_t)0;
  }

}