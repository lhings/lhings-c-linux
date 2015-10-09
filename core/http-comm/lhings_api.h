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

#ifndef LHINGS_API_H
#define	LHINGS_API_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../lhings.h"
#include "../utils/data_structures.h"
    
#define LHINGS_V1_API_PREFIX  "https://www.lhings.com/laas/api/v1/"
#define LHINGS_ERROR_HTTP_STATUS                457
#define LHINGS_V1_API_BAD_REQUEST_ERROR_CODE    400
#define LHINGS_V1_API_UNAUTHORIZED_ERROR_CODE   401
#define LHINGS_V1_API_NOT_FOUND_ERROR_CODE      404
    
#define UUID_STRING_LEN 36
#define LHINGS_V1_API_PREFIX_LEN 35


    LH_List* lh_api_device_list(LH_Device* device);
    int lh_api_start_session(LH_Device* device);
    int lh_api_end_session(LH_Device* device);
    char* lh_api_get_api_key(const char* username, const char* password);
    LH_Dict* lh_api_get_status(LH_Device* device, const char* uuid);
    int lh_api_register_device(LH_Device* device);
    int lh_api_request_action(LH_Device* device_requester, char* uuid_performer, char* action_name, LH_Dict* arguments);
    int lh_api_send_descriptor(LH_Device* device, char* descriptor);
    int lh_api_send_event(LH_Device* device, char* event_name, char* payload);
    int lh_api_store_status(LH_Device* device, LH_Dict* status_component_values);
    
#ifdef	__cplusplus
}
#endif

#endif	/* LHINGS_API_H */

