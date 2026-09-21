#include "Arduino.h"
#include "FreeRTOS.h"
#include "task.h"

extern "C" {

	void yield(void) __attribute__((weak));

	void yield(void) {
    		taskYIELD();
	}

}
