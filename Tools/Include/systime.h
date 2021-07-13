//
// Created by xinyang on 19-7-31.
//
// 提供一个多平台统一的精确到毫秒的系统时间接口
// 该时间仅用于表示相对时间

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <sys/time.h>

typedef double systime;

void getsystime(systime &t);
double getTimeIntervalms(const systime &now, const systime &last);


#endif /* _PLATFORM_H_ */
