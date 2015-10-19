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

#include <string.h>
#include "lhings_json_api.h"
#include "../logging/log.h"

char* lh_json_get_dev_uuid(char *json_text) {
    char *token = strtok(json_text, "\"");
    char error_message[] = "Given json could not be parsed.";
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    
    if (strlen(token) != UUID_STRING_LEN){
        log_error("UUID with wrong length received");
        return NULL;
    }
    char *returned_uuid = malloc((UUID_STRING_LEN + 1) * sizeof *returned_uuid);
    strncpy(returned_uuid, token, UUID_STRING_LEN + 1);
    return returned_uuid;
}

char* lh_json_get_api_key(char *json_text) {
    char *token = strtok(json_text, "\"");
    char error_message[] = "Given json could not be parsed.";
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    token = strtok(NULL, "\"");
    if(token == NULL){
        log_error(error_message);
        return NULL;
    }
    if (strlen(token) != UUID_STRING_LEN){
        log_error("API-key with wrong length received");
        return NULL;
    }
    
    char *returned_uuid = malloc((UUID_STRING_LEN + 1) * sizeof *returned_uuid);
    strncpy(returned_uuid, token, UUID_STRING_LEN + 1);
    return returned_uuid;
}
