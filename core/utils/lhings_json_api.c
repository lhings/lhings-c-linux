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
#include "json.h"
#include "lhings_json_api.h"
#include "../logging/log.h"

char* lh_json_get_dev_uuid(char *json_text){
    json_value *parsed_json = json_parse(json_text, strlen(json_text));
    LH_Dict* name_value_pair = lh_dict_new();
    if (parsed_json->type != json_object || parsed_json->u.object.length != 2) {
        log_error("Given json could not be parsed.");
        return NULL;
    }

    if(parsed_json->u.object.values[0].value->type != json_string || parsed_json->u.object.values[1].value->type != json_string){
        log_error("Only strings supported for values in name value pairs.");
        return NULL;
    }
    
    int j;
    for (j=0; j<2; j++){
        char* key = parsed_json->u.object.values[j].name;
        char* value = parsed_json->u.object.values[j].value->u.string.ptr;
        lh_dict_put(name_value_pair, key, value);
    }
    
    char *uuid = (char*)lh_dict_get(name_value_pair, "uuid");
    char *returned_uuid = malloc(strlen(uuid) * sizeof(returned_uuid) + 1);
    strcpy(returned_uuid, uuid);
    json_value_free(parsed_json);
    lh_dict_free(name_value_pair);
    return returned_uuid;
}

char* lh_json_get_api_key(char *json_text) {
    json_value *parsed_json = json_parse(json_text, strlen(json_text));
    LH_Dict* name_value_pair = lh_dict_new();
    if (parsed_json->type != json_object || parsed_json->u.object.length != 2) {
        log_error("Given json could not be parsed as name value pair.");
        return NULL;
    }

    if(parsed_json->u.object.values[0].value->type != json_string || parsed_json->u.object.values[1].value->type != json_string){
        log_error("Only strings supported for values in name value pairs.");
        return NULL;
    }
    
    int j;
    for (j=0; j<2; j++){
        char* key = parsed_json->u.object.values[j].name;
        char* value = parsed_json->u.object.values[j].value->u.string.ptr;
        lh_dict_put(name_value_pair, key, value);
    }
    
    char *api_key = (char*)lh_dict_get(name_value_pair, "value");
    char *returned_api_key = malloc(strlen(api_key) * sizeof(returned_api_key) + 1);
    strcpy(returned_api_key, api_key);
    json_value_free(parsed_json);
    lh_dict_free(name_value_pair);
    return returned_api_key;
}
