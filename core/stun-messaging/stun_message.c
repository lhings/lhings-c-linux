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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../utils/utils.h"
#include "../logging/log.h"
#include "../crypto/hmac.h"
#include "stun_message.h"
#include "../lhings.h"
#include "../../abstraction/timing/lhings_time.h"

int stun_process_stun_message(uint8_t *bytes, uint16_t length, StunMessage *message) {
    if (!stun_is_well_formed(bytes, length)) return 0;

    // initialize StunMessage
    message->bytes = bytes;
    message->length = length;
    uint8_t message_type[2];
    message_type[0] = bytes[0] >> 2;
    message_type[1] = (bytes[1] & 0x0F) + ((bytes[1] & 0xE0) >> 1) + ((bytes[0] & 0x02) << 6);
    message->method = byte_array_to_uint16_t(message_type);
    message->api_key = NULL;
    message_type[0] = bytes[0] & 0x01;
    message_type[1] = bytes[1] & 0x10;
    message->class = byte_array_to_uint16_t(message_type);
    return 1;
}

int stun_get_attribute(const StunMessage *message, uint16_t attribute_code, StunAttribute *attribute) {
    int position = STUN_MIN_MESS_LEN; // start reading bytes just after message header
    while (position < message->length) {
        uint16_t attr_type = byte_array_to_uint16_t(message->bytes + position);
        uint16_t attr_length = byte_array_to_uint16_t(message->bytes + position + 2);
        if (attr_type == attribute_code) {
            attribute->attr_type = attr_type;
            attribute->length = attr_length;
            attribute->bytes = (message->bytes + position + 4);
            return 1;
        }
        // jump to the next attribute
        int shift = attr_length % 4;
        if (shift != 0)
            shift = 4 - shift;
        // to take into account the padding to 32 bits boundary and the 4
        // bytes of type length
        position += attr_length + shift + 4;
    }
    // not found, return false
    return 0;
}

int stun_is_well_formed(const uint8_t *bytes, uint16_t length) {

    // header of a STUN message is at least 20 bytes long
    if (length < 20) {
        log_warn("STUN message shorter than 20 bytes");
        return STUN_FALSE;
    }
    // check two first bits are 00
    if ((bytes[0] & 0xC0) != 0) {
        log_warn("Two first bits in header are not zero");
        return STUN_FALSE;
    }

    uint32_t magic = MAGIC_COOKIE;

    uint8_t magicCookie[4];
    uint32_to_byte_array(magic, magicCookie);
    // check magic cookie

    if (!(bytes[4] == magicCookie[0] && bytes[5] == magicCookie[1]
            && bytes[6] == magicCookie[2] && bytes[7] == magicCookie[3])) {
        log_warn("Magic cookie is not correct");
        return STUN_FALSE;
    }
    // check message length
    uint16_t declared_length = byte_array_to_uint16_t(&bytes[2]);
    if (declared_length != length - 20) {
        log_warn("Incorrect message length");
        return STUN_FALSE;
    }

    return STUN_TRUE;
}

int stun_is_integrity_correct(const StunMessage *message, const char *api_key) {
    const uint8_t *in = message->bytes;
    uint16_t inlen = message->length - 24; // discard last 24 bytes (the message integrity)
    uint16_t keylen = 36; // default because api keys are always uuids
    uint8_t hmac_signature[20];
    int fail = hmac_sha1(api_key, keylen, in, inlen, hmac_signature);
    
    // check for success of hmac calculation
    if (fail) {
        char error_message[70];
        char trId[15];
        encode_hex((message->bytes + 8), 12, trId);
        snprintf(error_message, 70, "hmac_sha1 couldn't be determined for message with trId %s", trId);
        log_error(error_message);
        return STUN_FALSE;
    }

    // check message integrity is correct
    const uint8_t *declared_hmac_signature = (message->bytes + message->length - 20);
    int hmac_incorrect = memcmp(declared_hmac_signature, hmac_signature, 20);
    if (hmac_incorrect) {
        char error_message[150];
        char good_hmac[43], bad_hmac[43];
        encode_hex(hmac_signature, 20, good_hmac);
        encode_hex(declared_hmac_signature, 20, bad_hmac);
        snprintf(error_message, 150, "hmac verification failed: should be %s but received %s", good_hmac, bad_hmac);
        log_error(error_message);
        return STUN_FALSE;
    }
    return STUN_TRUE;
}

uint16_t stun_get_error_code(const StunMessage *message, char *error_message_buffer, int buffer_len) {
    if (message->class != CL_ERROR) {
        log_warn("class of message was not CL_ERROR");
        return 0;
    }

    StunAttribute error_attribute;
    if (!stun_get_attribute(message, ATTR_ERROR_CODE, &error_attribute)) {
        log_warn("message did not contain ATTR_ERROR_CODE");
        return 0;
    }

    uint8_t hundreds = error_attribute.bytes[2];
    uint8_t remainder = error_attribute.bytes[3];
    uint16_t error_code = 100 * hundreds + remainder;

    uint16_t error_message_length = error_attribute.length - 4;
    if (error_message_length > buffer_len - 1)
        // allow room to add a null char at the end of the buffer
        error_message_length = buffer_len - 1;
    memcpy(error_message_buffer, error_attribute.bytes + 4, error_message_length);
    error_message_buffer[error_message_length] = 0x00; //null terminate string
    return error_code;
}

void stun_set_method_and_class(StunMessage *message, uint16_t method, uint16_t class) {
    uint8_t type_bytes[2];
    uint8_t method_bytes[2];
    uint16_to_byte_array(method, method_bytes);
    // encode method
    type_bytes[1] = (0x0F & method_bytes[1]) + ((0x70 & method_bytes[1]) << 1);
    type_bytes[0] = ((0x80 & method_bytes[1]) >> 6) + ((0x0F & method_bytes[0]) << 2);
    // encode class
    switch (class) {
        case CL_REQUEST:
            type_bytes[0] = type_bytes[0] & 0xFE; // set bit to 0
            type_bytes[1] = type_bytes[1] & 0xEF; // set bit to 0
            break;
        case CL_SUCCESS:
            type_bytes[0] = type_bytes[0] | 0x01; // set bit to 1
            type_bytes[1] = type_bytes[1] & 0xEF; // set bit to 0
            break;
        case CL_ERROR:
            type_bytes[0] = type_bytes[0] | 0x01; // set bit to 1
            type_bytes[1] = type_bytes[1] | 0x10; // set bit to 1
            break;
        case CL_INDICATION:
            type_bytes[0] = type_bytes[0] & 0xFE; // set bit to 0
            type_bytes[1] = type_bytes[1] | 0x10; // set bit to 1
            break;
    }

    // set type bytes in STUN message
    message->bytes[0] = type_bytes[0];
    message->bytes[1] = type_bytes[1];

}

void stun_get_method_and_class(StunMessage *message, uint16_t *method, uint16_t *class){
    uint16_t message_type = byte_array_to_uint16_t(message->bytes);
    *class = message_type & 0x0110;
    *method = (message_type & 0x000f) + ((message_type & 0x00e0) >> 1) + ((message_type & 0x0e00) >> 2) + ((message_type & 0x3000) >> 2);
}

StunMessage* stun_new_empty_stun_message(){
    return stun_new_stun_message(20, STUN_TRUE);
}

StunMessage* stun_new_stun_message(uint16_t length, uint8_t set_tr_id) {
    static uint8_t random_generator_initialized = STUN_FALSE;
    if (!random_generator_initialized) {
        srand(lh_get_UTC_unix_time());
        random_generator_initialized = STUN_TRUE;
    }

    StunMessage *stun_message = malloc(sizeof *stun_message);
    stun_message->bytes = malloc(length * sizeof stun_message->bytes);
    stun_message->length = length;
    // set STUN message length to default minimum 
    uint16_to_byte_array(STUN_MIN_MESS_DECL_LEN, (stun_message->bytes + 2));
    // set magic cookie
    uint32_to_byte_array(MAGIC_COOKIE, (stun_message->bytes + 4));
    // set random transaction id (12 random bytes)
    if (set_tr_id) {
        int j;
        for (j = 0; j < 12; j = j + 2) {
            uint8_t rand_bytes[2];
            uint16_to_byte_array(rand() % 65536, rand_bytes);
            stun_message->bytes[j + 8] = rand_bytes[0];
            stun_message->bytes[j + 8 + 1] = rand_bytes[1];
        }
    }
    return stun_message;
}

char* stun_to_string(StunMessage *message){
    static char hex_message[300];
    strcpy(hex_message, ""); //initialize string
    encode_hex(message->bytes, message->length, hex_message);
    return hex_message;
}

void stun_free(StunMessage* stun_message) {
    free(stun_message->bytes);
    free(stun_message);
}

StunMessage* stun_set_message_integrity(StunMessage *message, char *api_key) {
    uint16_t declared_length = byte_array_to_uint16_t(message->bytes + 2);
    uint16_t length_after_mi_added = declared_length + 24;
    uint16_to_byte_array(length_after_mi_added, message->bytes + 2);
    // lower case api key
    char *cursor = api_key;
    for (;*cursor; ++cursor){ 
        char a = *cursor;
        a = tolower(a);
        *cursor = a;
    }
    uint8_t message_integrity[STUN_ATTR_MESS_INTEGR_VALUE_LEN];
    int fail = hmac_sha1(api_key, STUN_API_KEY_LEN, message->bytes, declared_length + STUN_MIN_MESS_LEN, message_integrity);
    if (fail)
        return NULL;
    
    // restore declared length to original value so that stun_add_attribute does not break the message
    uint16_to_byte_array(declared_length, message->bytes + 2);

    
    return stun_add_attribute(message, ATTR_MESSAGE_INTEGRITY, STUN_ATTR_MESS_INTEGR_VALUE_LEN, message_integrity);
}

StunMessage* stun_add_attribute(StunMessage *message, uint16_t attr_type, uint16_t value_length, uint8_t *value){
    uint16_t declared_length = byte_array_to_uint16_t(message->bytes + 2);
    uint8_t num_padding_bytes = value_length % 4;
    if (num_padding_bytes != 0)
        num_padding_bytes = 4 - num_padding_bytes;
    // check if call to realloc is needed
    uint16_t needed_length = declared_length + STUN_MIN_MESS_LEN + value_length + num_padding_bytes + 4;
    if(needed_length > message->length){
        // increase size of message
        log_debug("Increasing size of message.");
        message->bytes = realloc(message->bytes, needed_length * sizeof *message->bytes);
        memset(message->bytes + message->length, needed_length - message->length, 0);
        message->length = needed_length;
    }
    // set attribute length and type
    uint8_t ptr_attr_start = declared_length + STUN_MIN_MESS_LEN;
    uint8_t byte_array[2];
    uint16_to_byte_array(attr_type, byte_array);
    message->bytes[ptr_attr_start] = byte_array[0];
    message->bytes[ptr_attr_start+1] = byte_array[1];
    uint16_to_byte_array(value_length, byte_array);
    message->bytes[ptr_attr_start+2] = byte_array[0];
    message->bytes[ptr_attr_start+3] = byte_array[1];
    // copy bytes 
    memcpy(message->bytes + ptr_attr_start + 4, value, value_length);
    // set padding bytes
    if (num_padding_bytes != 0){
        int j;
        for (j = 0; j < num_padding_bytes; j++){
            message->bytes[ptr_attr_start+value_length + j + 4] = 0x00;
        }
    }
    //update declared message length and byte array length
    uint16_to_byte_array(needed_length - STUN_MIN_MESS_LEN, byte_array);
    memcpy(message->bytes+2, byte_array, 2);
    return message;
}

StunMessage* stun_add_common_attrs(StunMessage* message, LH_Device *device){
    message = stun_add_attribute(message, ATTR_USERNAME, strlen(device->username), (uint8_t*)device->username);
    uint8_t time_bytes[4];
    uint32_to_byte_array(lh_get_UTC_unix_time(), time_bytes);
    message = stun_add_attribute(message, ATTR_TIMESTAMP, 4, time_bytes);
    uint8_t uuid_bytes[16];
    uuid_string_to_byte_array(device->uuid, uuid_bytes);
    message = stun_add_attribute(message, ATTR_LYNCPORT_ID, 16, uuid_bytes);
    return message;       
}


StunMessage* stun_get_keepalive_message(LH_Device *device){
    StunMessage *message = stun_new_empty_stun_message();
    message = stun_add_common_attrs(message, device);
    stun_set_method_and_class(message, M_KEEP_ALIVE, CL_REQUEST);
    message = stun_set_message_integrity(message, device->api_key);
    return message;
}

StunMessage* stun_get_success_response(LH_Device *device, StunMessage *received_message, StunAttribute *additional_attr){
    StunMessage *response_message = stun_new_empty_stun_message();
    response_message = stun_add_common_attrs(response_message, device);
    if (additional_attr != NULL){
        response_message = stun_add_attribute(response_message, additional_attr->attr_type, additional_attr->length, additional_attr->bytes);
    }
    uint16_t method, class;
    stun_get_method_and_class(received_message, &method, &class);
    stun_set_method_and_class(response_message, method, CL_SUCCESS);
    memcpy(response_message->bytes + 8, received_message->bytes + 8, 12);
    response_message = stun_set_message_integrity(response_message, device->api_key);
    return response_message;
}
