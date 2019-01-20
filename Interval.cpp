/**The MIT License (MIT)

Copyright (c) 2019 by Radim Keseg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
