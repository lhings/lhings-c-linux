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

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "core/logging/log.h"
#include "core/utils/data_structures.h"
#include "core/lhings.h"

float temperature;
int quantity;
char surname[] = "Lorenzo";
uint32_t last_loop;

void action_example(LH_Dict *argument_values) {
    int *number = (int *) lh_dict_get(argument_values, "int_param");
    float *flt = (float *) lh_dict_get(argument_values, "float_param");
    char *message = (char *) lh_dict_get(argument_values, "str_param");
    printf("Action executed. Value: (str) %s, (int) %d, (float)%f\n", message, *number, *flt);

}

void setup() {
    lh_set_loop_frequency_hz(1);
    this_device.info.device_type = "cdevice";
    this_device.info.manufacturer = "Lhings";
    this_device.info.model_name = "Super Device";
    this_device.info.serial_number = "0123-AB";
    lh_model_add_status_component(&this_device, "temperature", LH_TYPE_FLOAT, &temperature);
    lh_model_add_status_component(&this_device, "quantity", LH_TYPE_INTEGER, &quantity);
    lh_model_add_status_component(&this_device, "surname", LH_TYPE_STRING, surname);

    LH_List *components = lh_list_new();
    lh_list_add(components, lh_model_create_component("quality", LH_TYPE_INTEGER, NULL));
    lh_list_add(components, lh_model_create_component("date", LH_TYPE_STRING, NULL));
    lh_model_add_event(&this_device, "finished", components);
    lh_model_add_event(&this_device, "not_finished", NULL);

    LH_List *arguments = lh_list_new();
    lh_list_add(arguments, lh_model_create_component("str_param", LH_TYPE_STRING, NULL));
    lh_list_add(arguments, lh_model_create_component("int_param", LH_TYPE_INTEGER, NULL));
    lh_list_add(arguments, lh_model_create_component("float_param", LH_TYPE_FLOAT, NULL));
    lh_model_add_action(&this_device, "example_action", "example action", arguments, action_example);

    temperature = 24.5;
    quantity = 8000;



}

void loop() {
    static int counter = 0;
    if ((counter + 1) % 15 == 0) {
        lh_send_event(&this_device, "not_finished", NULL, NULL);
        lh_store_status(&this_device);
    }

    if ((counter + 7) % 15 == 0) {
        LH_List *components = lh_list_new();
        char string[] = "OCT 21 2015";
        uint32_t number = 10;
        LH_Component *str_param = lh_model_create_component("date", LH_TYPE_STRING, string);
        LH_Component *int_param = lh_model_create_component("quality", LH_TYPE_INTEGER, &number);
        lh_list_add(components, str_param);
        lh_list_add(components, int_param);
        lh_send_event(&this_device, "finished", NULL, components);
        lh_model_free_component(str_param);
        lh_model_free_component(int_param);
        lh_list_free(components);
    }
    counter++;
}

int main() {
    log_set_level(LOG_DEBUG);
    lh_start_device(&this_device, "C_Super_Device2", "user@example.com", "xxxxxx");
    return 0;
}



