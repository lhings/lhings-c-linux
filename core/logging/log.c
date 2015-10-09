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
#include <string.h>
#include "log.h"

int log_write_log(int level, const char *message, const char *filename, int line) {
    static int log_level = LOG_OFF;
    static const char *log_level_tag[] = {"", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL", "OFF"};

    if (line == LOG_SET_LEVEL) {
        // signals we want to change the log level 
        log_level = level;
        return 0;
    } else if (line == LOG_GET_LEVEL) {
        // signals we want to retrieve the current log level
        return log_level;
    } 
    
    if (!(level < log_level || level > LOG_FATAL)) {
        // print log line
        printf("%s (in %s:%d): %s\n", log_level_tag[level], filename, line, message);
        return level;
    }
    return 0;
}

void log_set_level(int level) {
    log_write_log(level, NULL, NULL, LOG_SET_LEVEL);
}

int log_get_level() {
    return log_write_log(LOG_GET_LEVEL, NULL, NULL, LOG_GET_LEVEL);
}

char* log_get_str_level() {
    static char *log_level_tag[] = {"", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL", "OFF"};
    int level = log_write_log(LOG_GET_LEVEL, NULL, NULL, LOG_GET_LEVEL);
    return log_level_tag[level];
}


char* lh_get_message_str(const char *template, const char *str){
    int buffer_len = strlen(template) + strlen(str) +1;
    char *message = malloc(buffer_len * sizeof message);
    snprintf(message, buffer_len, template, str);
    return message;
}
    
 
char* lh_get_message_int(const char *template, const int number){
    int buffer_len = strlen(template) + 15 +1;
    char *message = malloc(buffer_len * sizeof message);
    snprintf(message, buffer_len, template, number);
    return message;
}
