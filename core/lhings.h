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
 * @file lhings.h
 * @brief Main header file for the Lhings C library.
 * 
 * This header file contains all the main functions used to implement a device
 * that connects to Lhings. 
 * 
 * The structure of a program that uses the library is very simple, it has to 
 * implement the functions setup() and loop() and, of course, a main() function. 
 * 
 * setup() is used to configure the 
 * device and is where all the initialization code should go. Configuring the device
 * means telling the Lhings library which are its actions, events and status components
 * (see http://support.lhings.com/Getting-started.html to get a quick introduction into
 * the main Lhings concepts).
 * 
 * Actions, events and status components are added to the Lhings device using the 
 * functions lh_model_add_action(), lh_model_add_event() and lh_model_add_status_component()
 * respectively. This functions populate the struct LH_Device this_device adding to it
 * all the information needed so that the Lhings library is able to generate the 
 * descriptor of the device (see http://support.lhings.com/The-Device-Descriptor.html for
 * more details about the device descriptor).
 * 
 * The loop() function will be periodically executed at a rate which can be
 * changed calling lh_set_loop_frequency_hz() or lh_set_loop_frequency_secs(). You
 * can use this method to implement anything that your device needs to perform or
 * check periodically.
 * 
 * In addition to this, you can define action functions, which will be executed 
 * any time the corresponding action is requested from the Lhings mobile or web
 * apps, or using the Lhings REST API (http://support.lhings.com/Lhings-API-Documentation.html).
 * These action functions should have the following signature:
 * @code
 * void action_function_example(LH_Dict *function_arguments)
 * @endcode
 * A pointer to this functions must be passed to lh_model_add_action() so that
 * the Lhings library knows which function to call when the named action is
 * requested. 
 * 
 * Likewise, you can use variables in your code to define status components of 
 * your device, passing a pointer to them in the call to the function 
 * lh_model_add_status_component(). For instance, if your device is a sensor
 * that measures temperature and you store its value in a @c float variable
 * called @c temp, you can expose its value as a status component passing a 
 * pointer to temp to the aforementioned function. Then its real time value
 * could be checked from the mobile and web apps of Lhings and also using the 
 * Lhings REST API.
 * 
 * Whenever you need to send an event you can use the function lh_send_event().
 * 
 * In order to start your device, you have to call the function
 * lh_start_device() from your main() function.
 * 
 * Finally a note on conventions used by the library:
 * <ul>
 * <li>All library API functions start with @c lh_* . In the same way, all
 * library defined data types start with @c LH_* . </li>
 * <li>The library makes extensive use of dynamic memory allocation. Some functions
 * return pointers. By convention, this functions will always return a pointer to a
 * dynamically allocated memory block. In the case of primitive data types (@c int ,
 * @c float, @c char, etc) the memory must be freed with free() when it is no longer
 * needed. In the case of library defined data types (@c LH_Device, @c LH_Action, @c LH_Dict, @c LH_List, etc.)
 * always exists a function @c lh_free_* that must be used to properly release the 
 * memory allocated by those methods. The library itself is checked against memory leaks
 * using <a href="http://valgrind.org/">Valgrind</a> before each release. </li>
 * </ul>
 * 
 * @see http://support.lhings.com
 */

#ifndef MODEL_H
#define	MODEL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "utils/data_structures.h"
    
#define MAX_DELAY_BETWEEN_RETRIES_SECS 120
#define DELAY_BETWEEN_KEEPALIVES_SECS 30
#define LOG_DESCRIPTOR 0
    
    
// json serialization 
#define MAX_STR_TYPE_LEN 9
#define MIN_COMP_JSON_LEN 22
#define MIN_ACTION_JSON_LEN 40
#define MIN_EVENT_JSON_LEN 27
#define MIN_DESCRIPTOR_JSON_LEN 153

    
    
    /**
     * enum reflecting the <a href = "http://support.lhings.com/Typed-Parameters.html">Lhings data types</a>.
     */
    typedef enum {LH_TYPE_INTEGER, LH_TYPE_FLOAT, LH_TYPE_STRING, LH_TYPE_TIMESTAMP, LH_TYPE_BOOLEAN, LH_TYPE_NO_TYPE} LH_ComponentType;
    
    typedef struct _lh_config{
        float loop_frequency_millis;
    } LH_Config;
    
    /**
     * LH_Info contains generic information about the device. This information
     * will be added to the device descriptor.
     */
    typedef struct _device_info {
        char *model_name, *manufacturer, *device_type, *serial_number;
    } LH_Info;
    
    /**
     * LH_Component is used to define Lhings <a href="http://support.lhings.com/Typed-Parameters.html">typed parameters</a>. Typed parameters are used in 3 places
     * in the Lhings library:
     * <ul>
     * <li>To define status components of the device in the call to lh_model_add_status_component().</li>
     * <li>To define event components of the device in the call to lh_model_add_event().</li>
     * <li>To define arguments of the actions in the call to lh_model_add_action().</li>
     * </ul>
     * 
     */
    typedef struct _status_component{
        /**
         * The name of the component.
         */
        char *name;
        /**
         * The component type.
         */
        LH_ComponentType type;
        /**
         * A pointer to the variable that contains the value of this component.
         */
        void *value;
    } LH_Component;
    
    /**
     * LH_Device is the main data type used to store the data of a device and
     * its capabilities. It also stores the information about the Lhings 
     * account to which the device belongs.
     */
    typedef struct _device {
        char *name, *type, *uuid, *api_key, *username;
        LH_Info info;
        /**
         * A list of LH_Component that contains the information of the status
         * components of the device.
         */
        LH_List *status_components;
        /**
         * A list of LH_Event that contains all the events that can be notified
         * by the device.
         */
        LH_List *events;
        /**
         * A list of LH_Action that contains all the information regarding the
         * actions that can be performed by the device.
         */
        LH_List *actions;
    } LH_Device;

    /**
     * LH_Event stores the details of an event, like its name and its 
     * components, if any.
     */
    typedef struct _event {
        char *name;
        LH_List *components;
    } LH_Event;
    
    /**
     * LH_Action stores the details of an action, like its name and arguments.
     */
    typedef struct _action {
        char *name, *description;
        LH_List *arguments;
        /**
         * A pointer to the function that actually performs the action.
         */
        void (*action_function) (LH_Dict* argument_values);
    }LH_Action;
    
    /**
     * This instance of LH_Device is used internally by the library to track
     * the state and configuration of the device while it is being executed.
     * Its reference is the one that must be passed in the call to
     * lh_start_device().
     */
    LH_Device this_device;
    LH_Config config;
    
    /**
     * This function must be used to define the actions, events and status components
     * of the device. 
     * 
     * In addition all initialization required by code external to
     * the library can be put here.
     */
    void setup();
    
    /**
     * Main loop of execution. 
     * 
     * This function will be called periodically by the library.
     * The period between two successive calls can be defined using the functions
     * lh_set_loop_frequency_hz and lh_set_loop_frequency_secs
     */
    void loop();
    
    /**
     * This function starts the execution of the device. 
     * 
     * It will connect to Lhings, register the
     * device in Lhings if needed, call setup(), send the descriptor and periodically
     * execute the function loop(). If there are no errors, the call to this function never returns.
     * 
     * @param device A pointer to a structure LH_Device. In practice, a pointer to the variable this_device (defined in lhings.h) must always be passed.
     * @param device_name A string with the name given to the device.
     * @param username A string with the username of the account of Lhings in which the device will be registered.
     * @param password A string with the password of the account of Lhings in which the device will be registered.
     * @return 0 if there is no error. On success this function never returns.
     */
    int lh_start_device(LH_Device *device, char *device_name, char *username, char *password);
    /**
     * Set the frequency at which the function loop will be called. 
     * 
     * The library will do this on a best
     * effort basis, there is no guarantee on the precision of timing between calls.
     * @param freq The frequency in hertz at which loop function will be called.
     */
    void lh_set_loop_frequency_hz(double freq);
    /**
     * Set the frequency at which the function loop will be called. 
     * 
     * The library will do this on a best
     * effort basis, there is no guarantee on the precision of timing between calls.
     * @param secs The time in seconds between two consecutive calls to loop function.
     */
    void lh_set_loop_frequency_secs(uint32_t secs);
    
    /**
     * Used to create components, either to define device capabilities (in the function setup)
     * or to define the <a href="http://support.lhings.com/Event-Payload.html">payload</a> to be sent with an event. 
     * 
     * @param name The name of the component.
     * @param type The type of the component (see http://support.lhings.com/Typed-Parameters.html for more details).
     * @param value A pointer to the variable that holds the value of the component, if any. If this component has no associated value, pass null.
     * @return A pointer to the LH_Component created. The returned pointer must be freed when it is no longer needed using lh_model_free_component.
     */
    LH_Component* lh_model_create_component(char *name, LH_ComponentType type, void *value);
    
    /**
     * Frees the memory allocated by lh_model_create_component.
     * @param component
     */
    void lh_model_free_component(LH_Component *component);
    
    /**
     * Tells the library that the device is able to send the named event. 
     * 
     * This information
     * will be used by the library to automatically build the device descriptor and send it to 
     * Lhings. A device can send as many events as needed. See http://support.lhings.com/The-Device-Descriptor.html
     * for more details on the device descriptor.
     * @param device The device that is able to send the event (usually this will be the variable this_device).
     * @param name A string with the name of the event.
     * @param components A list with the components of the event. If null is passed no components are added. For more details, see http://support.lhings.com/Event-Payload.html#How-to-use-structured-payloads
     */
    
    void lh_model_add_event(LH_Device *device, char *name, LH_List *components);
    /**
     * Tells the library that the device has the named status component. 
     * 
     * This information
     * will be used by the library to automatically build the device descriptor and send it to 
     * Lhings. A device can have up to 8 status components. See http://support.lhings.com/The-Device-Descriptor.html
     * for more details on the device descriptor.
     * @param device The device to which the status component belongs (usually this will be the variable this_device).
     * @param name A string with the name of the status component.
     * @param type The type of the status component (see http://support.lhings.com/Typed-Parameters.html for more details).
     * @param value A pointer to the variable that stores the value of the status component.
     */
    void lh_model_add_status_component(LH_Device *device, char *name, LH_ComponentType type, void *value);
    /**
     * Tells the library that the device can perform the named action. 
     * 
     * This information
     * will be used by the library to automatically build the device descriptor and send it to 
     * Lhings. A device can have as many actions as needed. See http://support.lhings.com/The-Device-Descriptor.html
     * for more details on the device descriptor.
     * @param device The device that is capable of performing this action (usually this will be the variable this_device).
     * @param name A string with the name of the action.
     * @param description A string with the description of the action (optional, can be null).
     * @param arguments The list of arguments the action needs to know to be executed. If null is passed no arguments are added.
     * @param action_function A function pointer to the function that actually performs the requested action.
     */
    void lh_model_add_action(LH_Device *device, char *name, char *description, LH_List *arguments, void (*action_function)(LH_Dict *argument_values));
    /**
     * Notifies Lhings about the occurence of the named event, optionally with a <a href="http://support.lhings.com/Event-Payload.html">payload</a>.
     * @param device The device that notifies about the occurence of the event (usually this will be the variable this_device).
     * @param event_name A string containing the name of the event.
     * @param payload A string containing the payload of the event (optional, can be null). 
     * If both payload and components are not null, then components is discarded and has no effect.
     * @param components A list of LH_Component structs that contains the components that want to be sent with the event (optional, can be null).
     * @return 1 if event is sent successfully, 0 otherwise.
     */
    int lh_send_event(LH_Device *device, char *event_name, char *payload, LH_List *components);
    

    
#ifdef	__cplusplus
}
#endif

#endif	/* MODEL_H */

