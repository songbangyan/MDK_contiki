#ifndef __PRODUCT_VER_H__
#define __PRODUCT_VER_H__
#include "bsp.h"

extern const char pd_sft_ver[];

void product_info(unsigned char com);
void GetBuildTime(Time_Def *rtc);
void GetBuildVer(u8 *Version);

#endif
  

/********************************end of file****************************/
