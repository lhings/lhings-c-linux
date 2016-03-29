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
 * @file platform-logging.h
 * @brief This file defines all the functions related to logging and printing
 * to console that are platform dependent.
 * 
 * All the functions in this header file belong to the abstraction API of the 
 * library and need to be reimplemented when changing platform. The documentation
 * of each function contains all the information about its expected behaviour. This
 * information must be carefully followed when porting the library to other platforms.
 */

#ifndef PLATFORM_LOGGING_H
#define	PLATFORM_LOGGING_H

#ifdef	__cplusplus
extern "C" {
#endif

    void lh_printf(const char* log_level, const char* filename, int linenumber, const char* message);


#ifdef	__cplusplus
}
#endif

#endif	/* PLATFORM_LOGGING_H */

