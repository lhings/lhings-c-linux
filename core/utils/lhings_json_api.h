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

#ifndef LHINGS_JSON_API_H
#define	LHINGS_JSON_API_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "json.h"
#include "data_structures.h"

char* lh_json_get_api_key(char *json_text);
char* lh_json_get_dev_uuid(char *json_text);


#ifdef	__cplusplus
}
#endif

#endif	/* LHINGS_JSON_API_H */

