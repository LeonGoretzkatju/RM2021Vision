#include "../Include/systime.h"

static systime getsystime(){
    timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_usec / 1000.0 + tv.tv_sec * 1000.0;
}

void getsystime(systime &t) {
    static systime time_base = getsystime();
    timeval tv;
    gettimeofday(&tv, nullptr);
    t = tv.tv_usec / 1000.0 + tv.tv_sec * 1000.0 - time_base;
}

double getTimeIntervalms(const systime &now, const systime &last) {
    return now - last;
}
