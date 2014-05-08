#pragma once
#include "inc/binary.h"
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "GPIO/gpio.h"

#define ON  	0
#define OFF 	1
#define TOGGLE 	2

void debug_pin(INT8U state);
