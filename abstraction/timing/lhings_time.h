/* Copyright 2015 Lyncos Technologies S. L.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

/**
 * @file lhings_time.h
 * @brief This header file defines the functions that provide access to the
 * hardware clock of the device.
 * 
 * All the functions in this header file belong to the abstraction API of the 
 * library and need to be reimplemented when changing platform. The documentation
 * of each function contains all the information about its expected behaviour. This
 * information must be carefully followed when porting the library to other platforms.
 * 
 */

#ifndef LHINGS_TIME_H
#define	LHINGS_TIME_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdint.h>
    
    /**
     * Returns a 32 bit integer with the UNIX time in seconds. 
     * 
     * This method must take into account the calls to lh_update_time_offset so
     * that the time it returns is synchronized with the server time. The recommended
     * way of doing this is by storing the time offset between local time and
     * server time, and substracting that difference from local time when this
     * function is called. Look at the implementation of the Linux version of 
     * this C Library to see an example of how to do it.
     * 
     * If UNIX time is not
     * available, the seconds elapsed since a constant reference time in the past
     * must be returned. This time reference must not change during the execution
     * of the program, although it may change between device reboots.
     * @return 
     */
    uint32_t lh_get_UTC_unix_time();
    /**
     * Updates an internal time reference so that the values returned by
     * lh_get_UTC_unix_time are synchronized with the time of the server.
     * This function is used internally by the library to synchronize system 
     * clock with the server clock.
     * @param server_time The server time in seconds.
     */
    void lh_update_time_offset(uint32_t server_time);
    
    /**
     * Stops the execution of the program during a given period of time.
     * @param millis The number of milliseconds execution will be stopped.
     */
    void lh_sleep(uint32_t millis);
    
    /**
     * Function used for precision timing. Returns the time elapsed since an
     * arbitrary reference in time, in milliseconds. The time reference can be
     * any (the start of execution of the program, the UNIX epoch, whatwever) as
     * long as it does not change during program execution.
     * 
     * @return A 32 bit integer containing representing the number of milliseconds
     * elapsed since the time reference.
     */
    uint32_t lh_get_absolute_time_millis();



#ifdef	__cplusplus
}
#endif

#endif	/* LHINGS_TIME_H */

