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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../core/logging/log.h"
#include "lhings_api.h"
#include "../../abstraction/http-comm/http_api.h"
#include "../utils/lhings_json_api.h"
#include "../utils/lhings_json_api.h"
#include "../utils/data_structures.h"
#include "../stun-messaging/stun_message.h"
#include "../../abstraction/udp-comm/udp_api.h"



int lh_api_start_session(LH_Device* device){
    char* request_body = "{\"name\": \"online\", \"value\": true}";
    char url[94];
    url[0] = 0;
    strcat(url, LHINGS_V1_API_PREFIX);
    strcat(url, "devices/");
    strncat(url, device->uuid, 36);
    strcat(url, "/states/online");
    
    LH_Dict* headers = lh_dict_new();
    lh_dict_put(headers, "X-Api-Key", device->api_key);
    lh_dict_put(headers, "Content-Type", "application/json");
    
    LH_HttpResponse* response = lh_http_execute_put(url, headers, request_body);
    if (*response->http_code != HTTP_OK){
        int server_response_len = strlen(response->response_body);
        char* message_template = "Could not start session. Reason:\n %s\n";
        char* message = malloc((strlen(message_template) + server_response_len) * sizeof message);
        sprintf(message, message_template, response->response_body);
        log_error(message);
        free(message);
        lh_http_free(response);
        lh_dict_free(headers);
        return 0;
    }
    lh_dict_free(headers);
    lh_http_free(response);
    return 1;
}


int lh_api_end_session(LH_Device* device){
    char* request_body = "{\"name\": \"online\", \"value\": false}";
    char url[94];
    url[0] = 0;
    strcat(url, LHINGS_V1_API_PREFIX);
    strcat(url, "devices/");
    strncat(url, device->uuid, 36);
    strcat(url, "/states/online");
    
    LH_Dict* headers = lh_dict_new();
    lh_dict_put(headers, "X-Api-Key", device->api_key);
    lh_dict_put(headers, "Content-Type", "application/json");
    
    LH_HttpResponse* response = lh_http_execute_put(url, headers, request_body);
    if (*response->http_code != HTTP_OK){
        int server_response_len = strlen(response->response_body);
        char* message_template = "Could not start session. Reason:\n %s\n";
        char* message = malloc((strlen(message_template) + server_response_len) * sizeof message);
        sprintf(message, message_template, response->response_body);
        log_error(message);
        free(message);
        lh_http_free(response);
        lh_dict_free(headers);
        return 0;
    }
    lh_dict_free(headers);
    lh_http_free(response);
    return 1;
}


char* lh_api_get_api_key(const char* username, const char* password){
    char* url = malloc((62 + strlen(username) + strlen(password)) * sizeof url);
    url[0] = 0;
    strcat(url, LHINGS_V1_API_PREFIX);
    strcat(url, "accounts/");
    strcat(url, username);
    strcat(url, "/apikey?password=");
    strcat(url, password);
    
    LH_Dict* headers = lh_dict_new();
    
    LH_HttpResponse* response = lh_http_execute_get(url, headers);
    if (*response->http_code != HTTP_OK){
        int server_response_len = strlen(response->response_body);
        char* message_template = "Could not start session. Reason:\n %s\n";
        char* message = malloc((strlen(message_template) + server_response_len) * sizeof message);
        sprintf(message, message_template, response->response_body);
        log_error(message);
        free(message);
        lh_http_free(response);
        lh_dict_free(headers);
        free(url);
        return NULL;
    }
    char* api_key = lh_json_get_api_key(response->response_body);
    
    lh_http_free(response);
    lh_dict_free(headers);
    free(url);
    return api_key;

}
int lh_api_register_device(LH_Device* device){
    char url[94];
    url[0] = 0;
    strcat(url, LHINGS_V1_API_PREFIX);
    strcat(url, "devices/");
    char *request_body_template = "{\"name\": \"deviceName\", \"value\": \"%s\"}";
    uint16_t template_len = strlen(request_body_template);
    uint16_t dev_name_len = strlen(device->name);
    char *request_body = malloc((template_len + dev_name_len + 1) * sizeof(request_body));
    sprintf(request_body, request_body_template, device->name);
    LH_Dict* headers = lh_dict_new();
    lh_dict_put(headers, "X-Api-Key", device->api_key);
    lh_dict_put(headers, "Content-Type", "application/json");
    LH_HttpResponse* response = lh_http_execute_post(url, headers, request_body);
    if (*response->http_code != HTTP_OK){
        int server_response_len = strlen(response->response_body);
        char* message_template = "Could not register device. Reason:\n %s\n";
        char* message = malloc((strlen(message_template) + server_response_len) * sizeof message);
        sprintf(message, message_template, response->response_body);
        log_error(message);
        free(message);
        lh_http_free(response);
        lh_dict_free(headers);
        return 0;
    }
    
    device->uuid = lh_json_get_dev_uuid(response->response_body);
    
    lh_http_free(response);
    free(request_body);
    lh_dict_free(headers);
    return 1;
}

int lh_api_send_event(LH_Device* device, char* event_name, char* payload){
    StunMessage *msg_event = stun_get_event_message(device, event_name, payload);
    int sent_success = lh_send_to_server(msg_event);
    stun_free(msg_event);
    if (sent_success){
        log_info("Keepalive sent");
        return 1;
    } else {
        log_warn("Event message could not be sent")
        return 0;
    }
    
//    char* url = malloc((LHINGS_V1_API_PREFIX_LEN + strlen("devices/") + UUID_STRING_LEN + strlen("/events/") + strlen(event_name) + 1) * sizeof url);
//    url[0] = 0;
//    strcat(url, LHINGS_V1_API_PREFIX);
//    strcat(url, "devices/");
//    strcat(url, device->uuid);
//    strcat(url, "/events/");
//    strcat(url, event_name);
//    
//    LH_Dict* headers = lh_dict_new();
//    lh_dict_put(headers, "X-Api-Key", device->api_key);
//    lh_dict_put(headers, "Content-Type", "application/json");
//    LH_HttpResponse* response = lh_http_execute_post(url, headers, payload);
//    if (*response->http_code != HTTP_OK){
//        char* log_msg = lh_get_message_str("Could not send event. Reason: %s", response->response_body);
//        log_warn(log_msg);
//        free(log_msg);
//        lh_http_free(response);
//        lh_dict_free(headers);
//        free(url);
//        return 0;
//    }
//    char *message = lh_get_message_str("Sent event %s", event_name);
//    log_info(message);
//    free(message);
//    lh_http_free(response);
//    lh_dict_free(headers);
//    free(url);
//    return 1;
}

int lh_api_send_descriptor(LH_Device* device, char* descriptor){
    char* url = malloc((LHINGS_V1_API_PREFIX_LEN + strlen("devices/") + UUID_STRING_LEN + strlen("/") + 1) * sizeof url);
    url[0] = 0;
    strcat(url, LHINGS_V1_API_PREFIX);
    strcat(url, "devices/");
    strcat(url, device->uuid);
    strcat(url, "/");
    
    LH_Dict* headers = lh_dict_new();
    lh_dict_put(headers, "X-Api-Key", device->api_key);
    lh_dict_put(headers, "Content-Type", "application/json");
    LH_HttpResponse* response = lh_http_execute_put(url, headers, descriptor);
    if (*response->http_code != HTTP_CREATED){
        int server_response_len = strlen(response->response_body);
        char* message_template = "Could not send descriptor. Reason:\n %s\n";
        char* message = malloc((strlen(message_template) + server_response_len) * sizeof message);
        sprintf(message, message_template, response->response_body);
        log_error(message);
        free(message);
        lh_http_free(response);
        lh_dict_free(headers);
        free(url);
        return 0;
    }
    
    log_info("Sent descriptor");
    lh_http_free(response);
    lh_dict_free(headers);
    free(url);
    return 1;
}

char* generate_store_status_json(LH_List *components){
    char str_buffer[16] = "";
    int true_len = 4;
    int false_len = 5;
    char str_true[5] = "true";
    char str_false[6] = "false";
    int json_len = 0;
    int j;
    for (j = 0; j < components->size; j++) {
        LH_Component *component = lh_list_get(components, j);
        json_len += strlen(component->name) + 2;
        switch (component->type) {
            case LH_TYPE_BOOLEAN:
                if (*(uint32_t *) component->value)
                    json_len += true_len;
                else
                    json_len += false_len;
                break;
            case LH_TYPE_INTEGER:
            case LH_TYPE_TIMESTAMP:
                snprintf(str_buffer, 16, "%d", (int) *(uint32_t*) component->value);
                json_len += strlen(str_buffer);
                break;
            case LH_TYPE_FLOAT:
                snprintf(str_buffer, 16, "%f", *(float *) component->value);
                json_len += strlen(str_buffer);
                break;
            case LH_TYPE_STRING:
                json_len += strlen((char*) component->value) + 2;
                break;
            case LH_TYPE_NO_TYPE:
            default:
                break;
        }
    }

    json_len += components->size * 2 + 3;
    char *json_str = malloc(json_len * sizeof json_str);
    strcpy(json_str, "{");
    for (j = 0; j < components->size; j++) {
        LH_Component *component = lh_list_get(components, j);
        strcat(json_str, "\"");
        strcat(json_str, component->name);
        strcat(json_str, "\":");
        switch (component->type) {
            case LH_TYPE_BOOLEAN:
                if (*(uint32_t *) component->value)
                    strcat(json_str, str_true);
                else
                    strcat(json_str, str_false);
                break;
            case LH_TYPE_INTEGER:
            case LH_TYPE_TIMESTAMP:
                snprintf(str_buffer, 16, "%d", (int) *(uint32_t*) component->value);
                strcat(json_str, str_buffer);
                break;
            case LH_TYPE_FLOAT:
                snprintf(str_buffer, 16, "%f", *(float *) component->value);
                strcat(json_str, str_buffer);
                break;
            case LH_TYPE_STRING:
                strcat(json_str, "\"");
                strcat(json_str, (char*) component->value);
                strcat(json_str, "\"");
                break;
            case LH_TYPE_NO_TYPE:
            default:
                break;
        }
        if (j != components->size -1)
            strcat(json_str, ",");
    }
    strcat(json_str, "}");
    return json_str;
}

int lh_api_store_status(LH_Device* device){
    LH_List* components = device->status_components;
    if (components == NULL)
        return 1;
    char* request_body = generate_store_status_json(components);
    char* url = malloc((LHINGS_V1_API_PREFIX_LEN + strlen("devices/") + UUID_STRING_LEN + strlen("/states") + 1) * sizeof url);
    url[0] = 0;
    strcat(url, LHINGS_V1_API_PREFIX);
    strcat(url, "devices/");
    strcat(url, device->uuid);
    strcat(url, "/states");
    
    LH_Dict* headers = lh_dict_new();
    lh_dict_put(headers, "X-Api-Key", device->api_key);
    lh_dict_put(headers, "Content-Type", "application/json");
    LH_HttpResponse* response = lh_http_execute_post(url, headers, request_body);
    if (*response->http_code != HTTP_OK){
        int server_response_len = strlen(response->response_body);
        char* message_template = "Could not send store status. Reason:\n %s\n";
        char* message = malloc((strlen(message_template) + server_response_len) * sizeof message);
        sprintf(message, message_template, response->response_body);
        log_error(message);
        free(message);
        lh_http_free(response);
        lh_dict_free(headers);
        return 0;
    }
    
    printf("%s\n", request_body);
    lh_http_free(response);
    lh_dict_free(headers);
    free(url);
    free(request_body);
    return 1;
}
