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

#ifndef LOG_H
#define	LOG_H

#ifdef	__cplusplus
extern "C" {
#endif


#define LOG_DEBUG    1
#define LOG_INFO     2
#define LOG_WARNING  3
#define LOG_ERROR    4
#define LOG_FATAL    5
#define LOG_OFF      6
    
#define LOG_GET_LEVEL -100
#define LOG_SET_LEVEL -110

    
#define log_log(level, message) log_write_log(level, message, __FILE__, __LINE__)
#define log_debug(message) log_log(LOG_DEBUG, message);
#define log_info(message) log_log(LOG_INFO, message);
#define log_warn(message) log_log(LOG_WARNING, message);
#define log_warning(message) log_log(LOG_WARNING, message);
#define log_error(message) log_log(LOG_ERROR, message);
#define log_fatal(message) log_log(LOG_FATAL, message);
    
    /**
     * Sets the log level. Available levels are LOG_DEBUG, LOG_INFO, LOG_WARNING,
     * LOG_ERROR, and LOG_FATAL. To disable logging at all set the level to 
     * LOG_OFF. Logging is disabled by default, to enable it this function must
     * be called with a log level different from LOG_OFF.
     * @param level
     */
    void log_set_level(int level);
    
    /**
     * Returns the current value of the log level.
     */
    int log_get_level();
    
    /**
     * Writes a line to the log. This function is not meant to be used directly,
     * use log_log(level, message) instead.
     * @param level
     * @param message
     * @param filename
     * @param line
     * @return If nothing was logged it returns 0, otherwise it
     * returns the log level with which the message was logged.
     */
    int log_write_log(int level, const char *message, const char *filename, int line);
    
    /**
     * Function that generates a string containing a message, given a template
     * for printf and a string argument. Used to ease generation of log messages.
     * @param template
     * @param str
     * @return 
     */
    char* lh_get_message_str(const char *template, const char *str);
    
    /**
     * Function that generates a string containing a message, given a template
     * for printf and a integer argument. Used to ease generation of log messages.
     * @param template
     * @param str
     * @return 
     */
    char* lh_get_message_int(const char *template, const int number);
    

#ifdef	__cplusplus
}
#endif

#endif	/* LOG_H */

