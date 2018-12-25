extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}
#include <arduino.h>
#include "interval.h"

// Public Methods //////////////////////////////////////////////////////////////
uint32_t Interval::remains(void)
{

	return timeout - (millis() - timefrom);
}

uint32_t Interval::elapsed(void)
{

	return millis() - timefrom;
}

bool Interval::expired(void)
{

	if ((millis() - timefrom) >= timeout)
		return true;
	else
		return false;
}

void Interval::set(uint32_t tmout)
{

	timefrom = millis();
	timeout = tmout;
}

uint32_t Interval::getTimeout(){
    return timeout;	
}
