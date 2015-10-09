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
#include <stdlib.h>
#include "../core/stun-messaging/stun_message.h"
#include "../core/utils/utils.h"

int stun_message_tests() {
    puts("**********************************************************");
    puts("****             Running STUN message tests           ****");
    puts("**********************************************************");
    puts("");

    uint8_t plain_binding_req_with_message_integrity[] = {0x00, 0x01, 0x00, 0x28, 0x21, 0x12, 0xA4, 0x42, 0x11, 0x11,
        0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x33, 0x33, 0x33,
        0x00, 0x06, 0x00, 0x0B, 0x6a, 0x6f, 0x73, 0x65, 0x61, 0x6E,
        0x74, 0x6F, 0x6E, 0x69, 0x6F, 0x00, 0x00, 0x08, 0x00, 0x14,
        0x6f, 0xa1, 0x45, 0x29, 0xa6, 0x7c, 0xf0, 0xec, 0x8c, 0xb2,
        0xce, 0x81, 0xa7, 0x12, 0x91, 0x6f, 0x29, 0x71, 0x9b, 0x23};

    char *api_key = "45ced63d-a080-434a-a53b-df3d404814b2";

    uint8_t binding_req_success_response[] = {0x01, 0x01, 0x00, 0x34, 0x21, 0x12, 0xA4, 0x42,
        0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x33,
        0x33, 0x33, 0x00, 0x20, 0x00, 0x08, 0x00, 0x01, 0xA7,
        0x61, 0x5E, 0x12, 0xA4, 0x43, 0x80, 0x22, 0x00,
        0x22, 0x72, 0x65, 0x54, 0x55, 0x52, 0x4E, 0x53, 0x65, 0x72,
        0x76, 0x65, 0x72, 0x20, 0x30, 0x2E, 0x34, 0x20, 0x2D, 0x20,
        0x52, 0x46, 0x43, 0x35, 0x33, 0x38, 0x39, 0x2F, 0x74, 0x75,
        0x72, 0x6E, 0x2D, 0x31, 0x32, 0x20, 0x20};

    uint8_t error4xxToBindingRequest[] = {0x01, 0x11, 0x00, 0x24, 0x21, 0x12,
        0xA4, 0x42, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
        0x33, 0x33, 0x33, 0x33, 0x00, 0x09, 0x00, 0x10, 0x00, 0x00, 0x04,
        0x01, 0x55, 0x6e, 0x61, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x7a,
        0x65, 0x64, 0x0C, 0x13, 0x00, 0x04, 0x13, 0x8E, 0xD3,
        0xA8, 0x0C, 0x14, 0x00, 0x04, 0x00, 0x00, 0x00, 0x3C};

    uint8_t error500ToBindingRequest[] = {0x01, 0x11, 0x00, 0x58, 0x21, 
        0x12, 0xA4, 0x42, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
        0x33, 0x33, 0x33, 0x33, 0x00, 0x06, 0x00, 0x0B, 0x6a, 0x6f, 0x73,
        0x65, 0x61, 0x6E, 0x74, 0x6F, 0x6E, 0x69, 0x6F, 0x00, 0x00, 0x09,
        0x00, 0x1C, 0x00, 0x00, 0x05, 0x00, 0x49, 0x6e, 0x74, 0x65, 0x72,
        0x6e, 0x61, 0x6c, 0x20, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20,
        0x65, 0x72, 0x72, 0x6f, 0x72, 0x00, 0x00, 0x00, 0x0C, 0x13, 0x00,
        0x04, 0x13, 0x8E, 0xD3, 0xA8, 0x0C, 0x14, 0x00, 0x04, 0x00, 0x00, 
        0x00, 0x3C, 0x00, 0x08, 0x00, 0x14, 0x79, 0xee, 0x24, 0xe6, 0x7e, 
        0xc8, 0x0d, 0x46, 0x05, 0x9f, 0x52, 0xb0, 0x74, 0x5e, 0x67, 0x7b,
        0xf2, 0x4a, 0xce, 0x7b};


    printf("TEST CASE 1: ");
    if (stun_is_well_formed(plain_binding_req_with_message_integrity, 60))
        printf("OK\n");
    else {
        printf("FAILED: stun_is_well_formed reported a well formed message as a malformed one\n");
        return EXIT_FAILURE;
    }

    printf("TEST CASE 2: ");

    StunMessage message;

    stun_process_stun_message(binding_req_success_response, 72, &message);
    if (message.method == M_BINDING && message.class == CL_SUCCESS)
        printf("OK\n");
    else {
        printf("FAILED: message reported method 0x%04x and class 0x%04x but 0x%04x and 0x%04x were expected.\n", message.method, message.class, M_BINDING, CL_SUCCESS);
        return (EXIT_FAILURE);
    }


    stun_process_stun_message(plain_binding_req_with_message_integrity, 60, &message);
    if (message.method == M_BINDING && message.class == CL_REQUEST)
        printf("OK\n");
    else {
        printf("FAILED: message reported method 0x%04x and class 0x%04x but 0x%04x and 0x%04x were expected.\n", message.method, message.class, M_BINDING, CL_REQUEST);
        return EXIT_FAILURE;
    }


    printf("TEST CASE 3: ");
    if (!stun_is_integrity_correct(&message, api_key)) {
        printf("FAILED: stun_is_integrity_correct reported bad integrity but it is correct.\n");
        return EXIT_FAILURE;
    }
    // change 1 byte to change integrity 
    plain_binding_req_with_message_integrity[6] = 0xEE;
    if (stun_is_integrity_correct(&message, api_key)) {
        printf("FAILED: stun_is_integrity_correct reported good integrity but message was corrupted.\n");
        return EXIT_FAILURE;
    }
    printf("OK (failed as expected)\n");

    printf("TEST CASE 4: ");
    char buffer[100];
    stun_process_stun_message(error500ToBindingRequest, 108, &message);
    if (stun_get_error_code(&message, buffer, 100) == 500){
        printf ("OK\n");
    } else {
        printf ("FAILED: bad error code returned\n");
        return EXIT_FAILURE;
    }
    
    printf("TEST CASE 5: ");
    if (message.method == M_BINDING && message.class == CL_ERROR)
        printf("OK\n");
    else {
        printf("FAILED: message reported method 0x%04x and class 0x%04x but 0x%04x and 0x%04x were expected.\n", message.method, message.class, M_BINDING, CL_ERROR);
        return (EXIT_FAILURE);
    }
    
    printf("TEST CASE 6: ");
    // modify error500ToBindingRequest so that it is M_ACTION and CL_SUCCESS
    message.bytes[0] = 0x3B;
    message.bytes[1] = 0xC5;
    stun_process_stun_message(error500ToBindingRequest, 108, &message);
    if (message.method == M_ACTION && message.class == CL_SUCCESS)
        printf("OK\n");
    else {
        printf("FAILED: message reported method 0x%04x and class 0x%04x but 0x%04x and 0x%04x were expected.\n", message.method, message.class, M_ACTION, CL_SUCCESS);
        return (EXIT_FAILURE);
    }
    
    
    printf("TEST CASE 7: ");
    // modify error500ToBindingRequest so that it is M_STATUS_REQUEST and CL_ERROR
    stun_set_method_and_class(&message, M_STATUS_REQUEST, CL_ERROR);
    if (message.bytes[0] == 0x2B && message.bytes[1] == 0xDE)
        printf("OK\n");
    else {
        printf("FAILED: message reported message type 0x%04x but 0x%04x was expected.\n", byte_array_to_uint16_t(message.bytes), 0x2BDE);
        return (EXIT_FAILURE);
    }    
    
    printf("TEST CASE 8: ");
    // modify method and class to M_KEEP_ALIVE and CL_ERROR
    stun_set_method_and_class(&message, M_KEEP_ALIVE, CL_REQUEST);
    if (message.bytes[0] == 0x3A && message.bytes[1] == 0xCA)
        printf("OK\n");
    else {
        printf("FAILED: message reported message type 0x%04x but 0x%04x was expected.\n", byte_array_to_uint16_t(message.bytes), 0x3ACA);
        return (EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
