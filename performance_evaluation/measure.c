#include <sys/time.h>
#include "measure.h"

long long int to_usec(struct timeval t) {
    return t.tv_sec*1000000LL + t.tv_usec;
}
