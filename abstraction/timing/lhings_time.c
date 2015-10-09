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


#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "lhings_time.h"
#include "../../core/stun-messaging/stun_message.h"
#include "../../core/logging/log.h"

uint32_t get_UTC_unix_time_worker(int update_offset, uint32_t server_time){
    static uint32_t offset = 0;
    uint32_t time_now = time(NULL);
    if (update_offset){
        offset = time_now - server_time;
        char *log_msg = lh_get_message_int("Adjusting clock by %d seconds", (int)offset);
        log_info(log_msg);
        free(log_msg);
    } 
    return (uint32_t) (time_now - offset);
}

uint32_t lh_get_UTC_unix_time(){
    return get_UTC_unix_time_worker(STUN_FALSE, 0);
}

void lh_update_time_offset(uint32_t server_time){
    get_UTC_unix_time_worker(STUN_TRUE, server_time);
}

void lh_sleep(uint32_t millis){
    struct timespec sleep_time;
    sleep_time.tv_sec = millis / 1000;
    sleep_time.tv_nsec = (millis % 1000) * 1000000;
    nanosleep(&sleep_time, NULL);
}

uint32_t lh_get_absolute_time_millis(){
    static struct timeval init_time;
    static int initialized = 0;
    if (!initialized){
        gettimeofday(&init_time, NULL);
        initialized = 1;
    }
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    
    long secs = ((long)time_now.tv_sec - (long)init_time.tv_sec);
    long msecs = ((long)time_now.tv_usec - (long)init_time.tv_usec);
    return (uint32_t)(secs*1000L + msecs/1000L);
}
