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



#include <stdio.h>
#include <stdlib.h>
#include "../core/logging/log.h"





/**
 * Runs the test cases for the logging system.
 * @return 
 */
int logging_system_tests() {
    
    puts("**********************************************************");
    puts("****           Running logging system tests           ****");
    puts("**********************************************************");
    puts("");

    puts("TEST CASE 1: ");
    int level = log_log(LOG_DEBUG, "\tdebug");
    if (level == 0)
        printf("OK\n");
    else {
        printf("FAILED! Expected no output with logging system uninitialized but log_write_log returned %d\n", level);
        return EXIT_FAILURE;
    }
        
    log_set_level(LOG_DEBUG);
    puts("TEST CASE 2: ");
    level = log_log(LOG_DEBUG, "test message debug");
    if (level == LOG_DEBUG)
        printf("OK\n");
    else {
        printf("FAILED! Expected DEBUG but log_write_log returned %d\n", level);
        return EXIT_FAILURE;
    }
    
    log_set_level(LOG_INFO);
    
    level = log_log(LOG_INFO, "test message info");
    puts("TEST CASE 3: ");
    if (level == LOG_INFO)
        printf("OK\n");
    else {
        printf("FAILED! Expected INFO but log_write_log returned %d\n", level);
        return EXIT_FAILURE;
    }
    
    puts("TEST CASE 4: ");
    level = log_log(LOG_DEBUG, "test message debug");
    if (level == 0)
        printf("OK\n");
    else {
        printf("FAILED! Expected no output but log_write_log returned %d\n", level);
        return EXIT_FAILURE;
    }
    
    level = log_log(LOG_ERROR, "test message error");
    puts("TEST CASE 5: ");
    if (level == LOG_ERROR)
        printf("OK\n");
    else {
        printf("FAILED! Expected ERROR but log_write_log returned %d\n", level);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
    
}
