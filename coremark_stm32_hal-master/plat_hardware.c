/** 
 ****************************************************************************** 
 * @file     
 * @author  wind 
 * @version 
 * @date 
 * @brief 
 ****************************************************************************** 
 */  
  
#include "plat_hardware.h"  
     
  
//__attribute__(clock_t) clock(void)     
//__ATTRIBUTES clock_t clock(void)  
clock_t clock(void)  
{  
  return (clock_t)HAL_GetTick();  
}  