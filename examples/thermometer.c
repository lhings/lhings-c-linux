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
 * @thermometer.c
 * @brief Simple example showing how a thermometer could be connected to Lhings.
 * 
 * Simple example showing how a thermometer could be connected to Lhings, and 
 * demonstrating some of the features provided by the C Lhings library.
 * 
 * The device has two status components:
 * <ul>
 * <li>@c temperature : the temperature measured by the thermometer.</li>
 * <li>@c heater_on : whether the heater is working or not.</li>
 * </ul>
 * 
 * It has also an action called @c toggle_heater that turns the heater on and off alternatively. 
 * And finally, it has an event called @c heater_turned_off used to notify that the 
 * heater has been automatically turned off, because the temperature reached a 
 * threshold value (set to 30 for demonstration purposes).
 * 
 * You can play with the 
 */
#include <stdio.h>
#include "../core/lhings.h"
#include "../core/logging/log.h"

// here we define the variable holding the value of the temperature of the thermometer
float temp = 0;
// here we define the variable that tells if heater is on or off
int heater_on = 0;

void toggle_heater(){
    if(heater_on){
        heater_on = 0;
        printf("Heater turned off\n");
    }
    else{
        heater_on = 1;
        printf("Heater turned on\n");
    }
}

void setup(){
    // makes loop being executed 5 times per second
    lh_set_loop_frequency_hz(5);
    // tell Lhings variable temp is a status component named "temperature"
    lh_model_add_status_component(&this_device, "temperature", LH_TYPE_FLOAT, &temp);
    // do the same with heater_on. These variables will appear in Lhings apps with those names
    lh_model_add_status_component(&this_device, "is_heater_on", LH_TYPE_BOOLEAN, &heater_on);
    // tell Lhings this device has also a event called heater_turned_off
    lh_model_add_event(&this_device, "heater_turned_off", NULL);
    // tell Lhings this device has an action named toggle_heater
    lh_model_add_action(&this_device, "toggle_heater", NULL, NULL, &toggle_heater);
    
}

void loop(){
    static int counter = 1;
    static int raising = 1;
    // turn off heater and send the corresponding event when it is on and temperature
    // reaches 30
    if (temp > 30 && heater_on){
        toggle_heater();
        lh_send_event(&this_device, "heater_turned_off", NULL, NULL);
    }
    // make temp oscillate between 0 and 30 so that the graph in the Lhings
    // web app is not so flat and boring ;-)
    if (temp > 30)
        raising = -1;
    if (temp < 0)
        raising = 1;
    // update temperature
    temp += 0.01 * raising + heater_on * 0.05;
    // periodically store the temperature reading in Lhings
    if (counter % 150 == 0){
        lh_store_status(&this_device);
    }
    counter++;
}

int main(){
    // set log level (see core/logging/log.h to see available log levels)
    // comment macro LOGGING_ON in the same file to completely turn off logging
    // at compile time
    log_set_level(LOG_DEBUG);
    // this call is used to start the device. enter your Lhings username and password here
    lh_start_device(&this_device, "Thermometer", "user@example.com", "xxxxxx");
    return 0;
}