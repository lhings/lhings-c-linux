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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lhings.h"
#include "logging/log.h"
#include "http-comm/lhings_api.h"
#include "utils/data_structures.h"
#include "../abstraction/timing/lhings_time.h"
#include "../abstraction/permanent-storage/storage_api.h"
#include "../abstraction/udp-comm/udp_api.h"
#include "stun-messaging/stun_message.h"
#include "utils/utils.h"

int component_json_len(LH_Component *component) {
    return MIN_COMP_JSON_LEN + strlen(component->name) + MAX_STR_TYPE_LEN;
}

int action_json_len(LH_Action *action) {
    int json_len = MIN_ACTION_JSON_LEN;
    json_len += strlen(action->name);
    if (action->description != NULL)
        json_len += strlen(action->description);
    if (action->arguments != NULL) {
        int num_args = action->arguments->size;
        int j;
        for (j = 0; j < num_args; j++) {
            json_len += component_json_len((LH_Component *) lh_list_get(action->arguments, j));
        }
    }
    return json_len;
}

int event_json_len(LH_Event *event) {
    int json_len = MIN_EVENT_JSON_LEN;
    json_len += strlen(event->name);
    if (event->components != NULL) {
        int num_comps = event->components->size;
        int j;
        for (j = 0; j < num_comps; j++) {
            json_len += component_json_len((LH_Component *) lh_list_get(event->components, j));
        }
    }
    return json_len;
}

int get_descriptor_len(LH_Device *device) {
    int json_len = MIN_DESCRIPTOR_JSON_LEN;
    if (device->info.model_name != NULL)
        json_len += strlen(device->info.model_name);
    if (device->info.model_name != NULL)
        json_len += strlen(device->info.manufacturer);
    if (device->info.model_name != NULL)
        json_len += strlen(device->info.device_type);
    if (device->info.model_name != NULL)
        json_len += strlen(device->info.serial_number);
    int j;
    if (device->status_components != NULL) {
        for (j = 0; j < device->status_components->size; j++) {
            json_len += component_json_len((LH_Component*) lh_list_get(device->status_components, j));
        }
    }
    if (device->actions != NULL) {
        for (j = 0; j < device->actions->size; j++) {
            json_len += action_json_len((LH_Action*) lh_list_get(device->actions, j));
        }
    }
    if (device->events != NULL) {
        for (j = 0; j < device->events->size; j++) {
            json_len += event_json_len((LH_Event*) lh_list_get(device->events, j));
        }
    }
    return json_len;
}

void add_components(char* str_json, LH_List *components) {
    if (components == NULL)
        return;
    int num_comps = components->size;
    int j;
    for (j = 0; j < num_comps; j++) {
        LH_Component *component = (LH_Component*) lh_list_get(components, j);
        strcat(str_json, "{\"name\":\"");
        strcat(str_json, component->name);
        strcat(str_json, "\",\"type\":\"");
        switch (component->type) {
            case LH_TYPE_BOOLEAN:
                strcat(str_json, "boolean");
                break;
            case LH_TYPE_INTEGER:
                strcat(str_json, "integer");
                break;
            case LH_TYPE_TIMESTAMP:
                strcat(str_json, "timestamp");
                break;
            case LH_TYPE_FLOAT:
                strcat(str_json, "float");
                break;
            case LH_TYPE_STRING:
                strcat(str_json, "string");
                break;
            case LH_TYPE_NO_TYPE:
            default:
                break;
        }
        strcat(str_json, "\"}");
        if (j != num_comps - 1)
            strcat(str_json, ",");
    }
}

void add_actions(char *str_json, LH_List *actions) {
    int num_actions = actions->size;
    int j;
    for (j = 0; j < num_actions; j++) {
        LH_Action *action = (LH_Action*) lh_list_get(actions, j);
        strcat(str_json, "{\"name\":\"");
        strcat(str_json, action->name);
        strcat(str_json, "\",\"description\":\"");
        if (action->description != NULL)
            strcat(str_json, action->description);
        strcat(str_json, "\",\"inputs\":[");
        add_components(str_json, action->arguments);
        strcat(str_json, "]}");
        if (j != num_actions - 1)
            strcat(str_json, ",");
    }
}

void add_events(char *str_json, LH_List *events) {
    int num_events = events->size;
    int j;
    for (j = 0; j < num_events; j++) {
        LH_Event *event = (LH_Event*) lh_list_get(events, j);
        strcat(str_json, "{\"name\":\"");
        strcat(str_json, event->name);
        strcat(str_json, "\",\"components\":[");
        add_components(str_json, event->components);
        strcat(str_json, "]}");
        if (j != num_events - 1)
            strcat(str_json, ",");
    }
}

char* generate_descriptor(LH_Device *device) {
    int json_len = get_descriptor_len(device);
    char* str_json = malloc(json_len * sizeof str_json);
    strcpy(str_json, "{\"modelName\":\"");
    if (device->info.model_name != NULL)
        strcat(str_json, device->info.model_name);
    strcat(str_json, "\",\"manufacturer\":\"");
    if (device->info.manufacturer != NULL)
        strcat(str_json, device->info.manufacturer);
    strcat(str_json, "\",\"deviceType\":\"");
    if (device->info.device_type != NULL)
        strcat(str_json, device->info.device_type);
    strcat(str_json, "\",\"serialNumber\":\"");
    if (device->info.serial_number != NULL)
        strcat(str_json, device->info.serial_number);
    strcat(str_json, "\",\"version\":1,\"stateVariableList\":[");
    add_components(str_json, device->status_components);
    strcat(str_json, "],\"actionList\":[");
    add_actions(str_json, device->actions);
    strcat(str_json, "],\"eventList\":[");
    add_events(str_json, device->events);
    strcat(str_json, "]}");

//    printf("***********\n%s\n*************\n", str_json);
    return str_json;
}

int retry_send_device_descriptor(LH_Device *device, char *descriptor) {
    static int delay = 1;
    if (delay != 1)
        lh_sleep(1000 * delay);
    int success = lh_api_send_descriptor(device, descriptor);
    delay = 2 * delay;
    if (delay > MAX_DELAY_BETWEEN_RETRIES_SECS)
        delay = MAX_DELAY_BETWEEN_RETRIES_SECS;
    if (!success) {
        char *message = lh_get_message_int("Descriptor could not be sent, retrying again in %d seconds.", delay);
        log_warn(message);
        free(message);
    }
    return success;
}

int retry_start_session(LH_Device *device) {
    static int delay = 1;
    if (delay != 1)
        lh_sleep(1000 * delay);
    int success = lh_api_start_session(device);
    delay = 2 * delay;
    if (delay > MAX_DELAY_BETWEEN_RETRIES_SECS)
        delay = MAX_DELAY_BETWEEN_RETRIES_SECS;
    if (!success) {
        char *message_template = "Session could not be started, retrying again in %d seconds.";
        char *message = malloc((strlen(message_template) + 4) * sizeof message);
        sprintf(message, message_template, delay);
        log_warn(message);
        free(message);
    }
    return success;
}

void send_keepalive(LH_Device *device) {
    StunMessage *msg_keepalive = stun_get_keepalive_message(device);
    int sent_success = lh_send_to_server(msg_keepalive);
    stun_free(msg_keepalive);
    if (sent_success)
        log_info("Keepalive sent");
}

void send_keepalive_if_needed(LH_Device *device) {
    static uint32_t last_keepalive = 0;
    uint32_t now = lh_get_absolute_time_millis();
    //    if (last_keepalive != now && now - last_keepalive <= DELAY_BETWEEN_KEEPALIVES_SECS * 1000)
    if (now - last_keepalive <= DELAY_BETWEEN_KEEPALIVES_SECS * 1000)
        return;

    last_keepalive = now;
    send_keepalive(device);
}

void loop_driver() {
    static uint32_t last_loop_execution = 0;
    uint32_t now = lh_get_absolute_time_millis();
    if (now - last_loop_execution < config.loop_frequency_millis)
        return;

    last_loop_execution = now;
    loop();
}

int is_string_arg(uint8_t mask, uint8_t position) {
    return ((mask >> position) & 0x01) == 1;
}

LH_ComponentType action_get_component_type(const char *search_name, LH_Action *action) {
    int num_components = action->arguments->size;
    int j;
    for (j = 0; j < num_components; j++) {
        LH_Component *component = (LH_Component *) lh_list_get(action->arguments, j);
        char *component_name = component->name;
        if (strcmp(component_name, search_name) == 0)
            return component->type;
    }
    return LH_TYPE_NO_TYPE;
}

void free_args_dictionary(LH_Dict *dict) {
    LH_List *keys = lh_dict_get_keys(dict);
    int size = dict->size;
    int j;
    for (j = 0; j < size; j++) {
        char *key = lh_list_get(keys, j);
        void *value = lh_dict_get(dict, key);
        lh_dict_remove(dict, key);
        free(value);
    }
    lh_list_free(keys);
    lh_dict_free(dict);
}

LH_Dict* process_arguments_attribute(StunAttribute *attr, LH_Action *action) {
    int num_args = attr->bytes[0];
    if (num_args > 8 || num_args < 0) {
        log_error("Maximum 8 arguments are allowed.");
        return NULL;
    }

    uint8_t string_mask = attr->bytes[num_args + 1];
    LH_Dict *processed_args = lh_dict_new();
    int j = 0;
    int position = num_args + 2;
    for (j = 0; j < num_args; j++) {
        int arg_len = attr->bytes[j + 1];
        char *arg_name;
        void *arg_value;
        if (is_string_arg(string_mask, j)) {
            int arg_value_len = byte_array_to_uint16_t(attr->bytes + position);
            int arg_name_len = byte_array_to_uint16_t(attr->bytes + position + 2);
            arg_name = malloc((arg_name_len + 1) * sizeof *arg_name);
            arg_value = malloc((arg_value_len + 1) * sizeof (char));
            memcpy(arg_value, attr->bytes + position + 4, arg_value_len);
            memcpy(arg_name, attr->bytes + position + arg_value_len + 4, arg_name_len);
            ((char*) arg_value)[arg_value_len] = 0;
            arg_name[arg_name_len] = 0;
        } else {
            arg_name = malloc((arg_len + 1) * sizeof *arg_name);
            memcpy(arg_name, attr->bytes + position + 4, arg_len);
            arg_name[arg_len] = 0;
            LH_ComponentType type = action_get_component_type(arg_name, action);
            arg_value = malloc(sizeof (uint32_t));
            switch (type) {
                case LH_TYPE_INTEGER:
                case LH_TYPE_BOOLEAN:
                case LH_TYPE_TIMESTAMP:
                    *((uint32_t *) arg_value) = byte_array_to_uint32(attr->bytes + position);
                    break;
                case LH_TYPE_FLOAT:
                    *((float *) arg_value) = byte_array_to_float(attr->bytes + position);
                    break;
                case LH_TYPE_STRING:
                case LH_TYPE_NO_TYPE:
                    log_error("Arguments attribute: expected four byte argument type but received string or no type.");
                    free_args_dictionary(processed_args);
                    return NULL;
            }
        }
        lh_dict_put(processed_args, arg_name, arg_value);
        free(arg_name);
        position = position + arg_len + 4;
    }
    return processed_args;
}

void perform_action(StunMessage *message) {
    StunAttribute attr_name;
    int attr_present = stun_get_attribute(message, ATTR_NAME, &attr_name);
    if (!attr_present)
        return;
    char* action_name = malloc((attr_name.length + 1) * sizeof *action_name);
    memcpy(action_name, attr_name.bytes, attr_name.length);
    action_name[attr_name.length] = 0;
    int num_actions_of_device = this_device.actions->size;
    int j;
    LH_Action *action_to_execute = NULL;
    for (j = 0; j < num_actions_of_device; j++) {
        LH_Action *action = (LH_Action *) lh_list_get(this_device.actions, j);
        if (strcmp(action_name, action->name) == 0) {
            action_to_execute = action;
            break;
        }
    }
    if (action_to_execute == NULL) {
        char *log_msg = lh_get_message_str("Device has no action with name %s", action_name);
        log_warn(log_msg);
        free(log_msg);
        free(action_name);
        return;
    }

    StunAttribute attr_arguments;
    attr_present = stun_get_attribute(message, ATTR_ARGUMENTS, &attr_arguments);
    if (!attr_present) {
        free(action_name);
        return;
    }

    LH_Dict *arguments = process_arguments_attribute(&attr_arguments, action_to_execute);
    if (arguments == NULL) {
        log_error("Could not process arguments attribute. Action not performed.");
        free(action_name);
        return;
    }

    action_to_execute->action_function(arguments);
    free_args_dictionary(arguments);
    free(action_name);
}

uint8_t* build_arguments_attribute(LH_Device *device, int *len) {
    int num_status = device->status_components->size;
    if (num_status > 8) {
        log_error("More than 8 components are not allowed.");
        return NULL;
    }
    int size_to_alloc = 2 + num_status;
    int j;
    for (j = 0; j < num_status; j++) {
        LH_Component *status = lh_list_get(device->status_components, j);
        int name_len = strlen(status->name);
        int value_len = 0;
        if (status->type == LH_TYPE_STRING) {
            char *value = (char *) status->value;
            value_len = strlen(value);
        }
        size_to_alloc += name_len + value_len + 4;
    }
    uint8_t *attr_arguments_bytes = malloc(size_to_alloc * sizeof *attr_arguments_bytes);
    memset(attr_arguments_bytes, size_to_alloc, 0);
    *len = size_to_alloc;
    attr_arguments_bytes[0] = num_status;
    int position = 2 + num_status;
    for (j = 0; j < num_status; j++) {
        LH_Component *status = lh_list_get(device->status_components, j);
        int str_len = 0;
        int name_len = 0;
        switch (status->type) {
            case LH_TYPE_INTEGER:
            case LH_TYPE_TIMESTAMP:
                uint32_to_byte_array(*(uint32_t *) status->value, attr_arguments_bytes + position);
                position += 4;
                str_len = strlen((char*) status->name);
                memcpy(attr_arguments_bytes + position, status->name, str_len);
                // update status length
                *(attr_arguments_bytes + j + 1) = (uint8_t) str_len;
                position += str_len;
                break;
            case LH_TYPE_BOOLEAN:
                if (*(int *) status->value)
                    uint32_to_byte_array((uint32_t) 1, attr_arguments_bytes + position);
                else
                    uint32_to_byte_array((uint32_t) 0, attr_arguments_bytes + position);
                position += 4;
                str_len = strlen((char*) status->name);
                memcpy(attr_arguments_bytes + position, status->name, str_len);
                position += str_len;
                *(attr_arguments_bytes + j + 1) = (uint8_t) str_len;
                break;
            case LH_TYPE_FLOAT:
                float_to_byte_array(*(float *) status->value, attr_arguments_bytes + position);
                position += 4;
                str_len = strlen((char*) status->name);
                memcpy(attr_arguments_bytes + position, status->name, str_len);
                position += str_len;
                *(attr_arguments_bytes + j + 1) = (uint8_t) str_len;
                break;
            case LH_TYPE_STRING:
                // set string mask bit
                *(attr_arguments_bytes + num_status + 1) = *(attr_arguments_bytes + num_status + 1) | (1 << j);
                str_len = strlen((char*) status->value);
                uint16_to_byte_array((uint16_t) str_len, attr_arguments_bytes + position);
                name_len = strlen(status->name);
                uint16_to_byte_array((uint16_t) name_len, attr_arguments_bytes + position + 2);
                position += 4;
                memcpy(attr_arguments_bytes + position, status->value, str_len);
                position += str_len;
                memcpy(attr_arguments_bytes + position, status->name, name_len);
                position += str_len;
                *(attr_arguments_bytes + j + 1) = (uint8_t) str_len + name_len;
                break;
            case LH_TYPE_NO_TYPE:
            default:
                break;
        }

    }
    return attr_arguments_bytes;
}

void send_status(StunMessage *message) {
    StunAttribute attr_status;
    int length;
    uint8_t *attr_status_bytes = build_arguments_attribute(&this_device, &length);
    attr_status.attr_type = ATTR_ARGUMENTS;
    attr_status.bytes = attr_status_bytes;
    attr_status.length = length;
    StunMessage *response = stun_get_success_response(&this_device, message, &attr_status);
    int sent_success = lh_send_to_server(response);
    if (!sent_success)
        log_error("Status response could not be sent.");
    //    char hex_response[500];
    //    encode_hex(attr_status_bytes, length, hex_response);
    //    printf("Sent to server status response: %s\n", hex_response);
    stun_free(response);
    free(attr_status_bytes);
}

void send_success_response(StunMessage *message) {
    StunMessage *response = stun_get_success_response(&this_device, message, NULL);
    int sent_success = lh_send_to_server(response);
    if (!sent_success)
        log_error("Success response could not be sent.");
    stun_free(response);
}

void process_messages() {
    uint16_t length;
    uint8_t *bytes = lh_receive_from_server(&length);
    if (bytes == NULL)
        return;

    StunMessage message;
    if (!stun_process_stun_message(bytes, length, &message)) {
        log_warn("Discarding malformed message received.");
        free(bytes);
        return;
    }

    // TODO add here check of trId to avoid processing duplicated messages

    if (!stun_is_integrity_correct(&message, this_device.api_key)) {
        log_warn("Discarding message received with bad integrity.");
        free(bytes);
        return;
    }

    uint16_t method, class;
    stun_get_method_and_class(&message, &method, &class);
    char str[100];
    encode_hex(message.bytes + 8, 12, str);
    //printf("STUN message received: class %0x, method %0x, trId %s\n", class, method, str);
    if (class == CL_ERROR) {
        // check for bad timestamp message
        StunAttribute attribute;
        if (stun_get_attribute(&message, ATTR_ERROR_CODE, &attribute)) {
            int hundreds = (int) attribute.bytes[2];
            int remainder = (int) attribute.bytes[3];
            int error_code = 100 * hundreds + remainder;
            if (error_code == ERR_BAD_TIMESTAMP)
                log_warn("Bad timestamp message received.");
        }
        if (stun_get_attribute(&message, ATTR_SERVER_TIME, &attribute)) {
            uint32_t server_time = byte_array_to_uint32(attribute.bytes);
            lh_update_time_offset(server_time);
        }
        free(bytes);
        return;
    }

    if (class == CL_REQUEST) {
        switch (method) {
            case M_ACTION:
                perform_action(&message);
                send_success_response(&message);
                break;
            case M_STATUS_REQUEST:
                send_status(&message);
                break;
            default:
                break;
        }
    }
    free(bytes);
    return;
}

int lh_start_device(LH_Device *device, char *device_name, char *username, char *password) {
    device->name = device_name;
    device->username = username;
    char *apikey = lh_api_get_api_key(username, password);
    if (apikey == NULL)
        return 0;

    // initialize device
    device->actions = NULL;
    device->events = NULL;
    device->status_components = NULL;
    device->api_key = apikey;

    char* uuid = lh_storage_get_uuid(device->name);
    if (uuid == NULL) {
        // device has not been registered before, do it now
        if (!lh_api_register_device(device)) {
            log_error("Device registration failed.");
            free(apikey);
            return 0;
        } else {
            lh_storage_save_uuid(device->name, device->uuid);
        }
    } else {
        device->uuid = uuid;
    }
    // call user defined setup function
    log_info("Configuring device");
    setup();

    // send descriptor file
    char *device_descriptor = generate_descriptor(device);
    if (LOG_DESCRIPTOR)
        log_info(device_descriptor);
    int success;
    do {
        success = retry_send_device_descriptor(device, device_descriptor);
    } while (!success);
    free(device_descriptor);

    // start session in Lhings
    do {
        success = retry_start_session(device);
    } while (!success);
    log_info("Session started!");
    // send first keepalive
    send_keepalive(device);
    // main loop
    while (1) {
        send_keepalive_if_needed(device);
        process_messages();
        loop_driver();
    }
}

void log_frequency_change() {
    char *template = "loop frequency: interval set to %d millis";
    int buffer_size = (strlen(template) + 12);
    char *message = malloc(buffer_size * sizeof message);
    snprintf(message, buffer_size, template, (int) config.loop_frequency_millis);
    log_info(message);
    free(message);
}

void lh_set_loop_frequency_hz(double freq) {
    config.loop_frequency_millis = (uint32_t) (1 / freq * 1000);
    log_frequency_change();
}

void lh_set_loop_frequency_secs(uint32_t secs) {
    config.loop_frequency_millis = secs * 1000;
    log_frequency_change();
}

void lh_model_add_event(LH_Device *device, char *name, LH_List *components) {
    if (device->events == NULL)
        device->events = lh_list_new();

    LH_Event *event = malloc(sizeof *event);
    lh_list_add(device->events, event);
    event->name = name;

    event->components = components;
}

void lh_model_add_status_component(LH_Device *device, char *name, LH_ComponentType type, void *value) {
    if (device->status_components == NULL)
        device->status_components = lh_list_new();

    LH_Component *component = malloc(sizeof *component);
    component->name = name;
    component->type = type;
    component->value = value;
    lh_list_add(device->status_components, component);

}

void lh_model_add_action(LH_Device *device, char *name, char *description, LH_List *arguments, void (*action_function)(LH_Dict *argument_values)) {
    if (device->actions == NULL)
        device->actions = lh_list_new();

    LH_Action *action = malloc(sizeof *action);
    lh_list_add(device->actions, action);
    action->name = name;
    action->description = description;
    action->action_function = action_function;
    action->arguments = arguments;
}

LH_Component* lh_model_create_component(char *name, LH_ComponentType type, void *value) {
    LH_Component *component = malloc(sizeof *component);
    component->name = name;
    component->type = type;
    component->value = value;
    return component;
}

void lh_model_free_component(LH_Component *component) {
    free(component);
}

char *build_structured_payload(LH_List *components) {
    char name_key[9] = "\"name\":\"";
    char value_key[11] = "\",\"value\":";
    char str_true[5] = "true";
    char str_false[6] = "false";
    char str_buffer[16] = "";
    int true_len = 4;
    int false_len = 5;
    int name_len = 8;
    int value_len = 10;
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

    json_len += j * (name_len + value_len + 4) + 3;
    char *json_str = malloc(json_len * sizeof json_str);
    strcpy(json_str, "[");
    for (j = 0; j < components->size; j++) {
        LH_Component *component = lh_list_get(components, j);
        strcat(json_str, "{");
        strcat(json_str, name_key);
        strcat(json_str, component->name);
        strcat(json_str, value_key);
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
            strcat(json_str, "},");
        else
            strcat(json_str, "}");
    }
    strcat(json_str, "]");
    return json_str;
}

int lh_send_event(LH_Device *device, char *event_name, char *payload, LH_List * components) {
    int j;
    int event_found = 0;
    LH_Event *event;
    for (j = 0; j < device->events->size; j++) {
        event = (LH_Event *) lh_list_get(device->events, (uint16_t) j);
        if (strcmp(event_name, event->name) == 0) {
            event_found = 1;
            break;
        }
    }
    if (!event_found) {
        char *log_msg = lh_get_message_str("This device has no event named %s", event_name);
        log_warn(log_msg);
        free(log_msg);
        return 0;
    }

    if (payload != NULL) {
        lh_api_send_event(device, event_name, payload);
        return 1;
    }

    if (components == NULL) {
        lh_api_send_event(device, event_name, NULL);
        return 1;
    }

    char *json_payload = build_structured_payload(components);
    lh_api_send_event(device, event_name, json_payload);
//    printf("json_payload: %s\n", json_payload);
    free(json_payload);
    return 1;
}


int lh_store_status(LH_Device *device){
    return lh_api_store_status(device);
}