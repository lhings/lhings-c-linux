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

#ifndef STUN_MESSAGE_H
#define	STUN_MESSAGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "../lhings.h"

#define STUN_TRUE  1
#define STUN_FALSE 0

    /**
     * STUN message definitions
     */
#define  MAGIC_COOKIE   0x2112A442
#define  CL_REQUEST     0x0000
#define  CL_INDICATION  0x0010
#define  CL_SUCCESS     0x0100
#define  CL_ERROR       0x0110

#define STUN_ATTR_MESS_INTEGR_LEN 24
#define STUN_MIN_MESS_LEN 20
#define STUN_MIN_MESS_DECL_LEN 0
#define STUN_API_KEY_LEN 36
#define STUN_ATTR_MESS_INTEGR_VALUE_LEN 20
    
    
    // method code definitions (RFC 5389)
#define  M_BINDING  0x0001

    // STUN message attribute type definitions
    // comprehension required (RFC 5389)
#define  ATTR_MAPPED_ADDRESS        0x0001
#define  ATTR_USERNAME              0x0006
#define  ATTR_MESSAGE_INTEGRITY     0x0008
#define  ATTR_ERROR_CODE            0x0009
#define  ATTR_UNKNOWN_ATTRIBUTES    0x000A
#define  ATTR_REALM                 0x0014
#define  ATTR_NONCE                 0x0015
#define  ATTR_XOR_MAPPED_ADDRESS    0x0020

    // comprehension optional (RFC 5389)
#define  ATTR_SOFTWARE              0x8022
#define  ATTR_ALTERNATE_SERVER      0x8023
#define  ATTR_FINGERPRINT           0x8028

    // error codes (RFC 5389)
#define  ERR_TRY_ALTERNATE      300
#define  ERR_BAD_REQUEST        400
#define  ERR_UNAUTHORIZED       401
#define  ERR_UNKNOWN_ATTRIBUTE  420
#define  ERR_STALE_NONCE        438
#define  ERR_SERVER_ERROR       500


    // Lhings protocol defined methods
#define  M_KEEP_ALIVE         0x0EEA
#define  M_CHANGE_NAME        0x0CAE
#define  M_STATUS_REQUEST     0x0AEE
#define  M_FILEDOWN_REQUEST   0x0FED
#define  M_FIRMINST_REQUEST   0x0FEE
#define  M_P2P_LOCATE         0x02CA
#define  M_P2P_CONN_NOTIFY    0x02CF
#define  M_P2P_LINK           0x02D0
#define  M_P2P_CONN_CLOSE     0x0CCE
#define  M_P2P_NO_NAT         0x0AAA
#define  M_PING               0x0EE0
#define  M_LOG                0x000A
#define  M_RESET              0x0EE1
#define  M_DELETE             0x0EEE
#define  M_SUBSCRIBE          0x0EE2
#define  M_UNSUBSCRIBE        0x0EE3
#define  M_EVENT              0x0EE4
#define  M_NOTIFICATION       0x0EE5
#define  M_ACTION             0x0EE5
#define  M_CUSTOM_COMMAND     0x0EE6

    // LyncNat protocol defined attributes
#define  ATTR_TIMESTAMP                   0x0C01
#define  ATTR_BEGIN_SESSION               0x0C02
#define  ATTR_LYNCPORT_ID                 0x0C03
#define  ATTR_LYNCPORT_ID_CALLEE          0x0C04
#define  ATTR_LYNCPORT_ID_CALLER          0x0C05
#define  ATTR_NEW_NAME                    0x0C06
#define  ATTR_LYNCPORT_CONNECT_TO_PORT    0x0C07
#define  ATTR_FILENAME                    0x0C08
#define  ATTR_URL                         0x0C09
#define  ATTR_FILE_INTEGRITY              0x0C10
#define  ATTR_FILE_LENGTH                 0x0C11
#define  ATTR_NAME                        0x0C12
#define  ATTR_SERVER_TIME                 0x0C13
#define  ATTR_EXPIRATION_POLICY           0x0C14
#define  ATTR_LOG_MESSAGE                 0x0C15
#define  ATTR_ERROR_LEVEL                 0x0C16
#define  ATTR_EVENT_NAME                  0x0C17
#define  ATTR_PAYLOAD                     0x0C18
#define  ATTR_ARGUMENTS                   0x0C19
#define  ATTR_GCM_REGID                   0x0C20

    //LyncNat protocol defined error codes
#define  ERR_INVALID_NAME        600
#define  ERR_OUT_OF_MEMORY       601
#define  ERR_BAD_FILE            602
#define  ERR_INSTALL_FAILED      603
#define  ERR_NOT_AVAILABLE       604
#define  ERR_BAD_TIMESTAMP       605
#define  ERR_DEV_QUOTA_EXCEEDED  606
#define  ERR_DUPLICATE_UUID      607

    /**
     * STUN message data structure
     */
    typedef struct StunMessage {
        // array of bytes which stores the actual STUN message
        uint8_t *bytes;
        // lenght of the array of bytes, does not need to be equal to the length
        // declared in the actual STUN message.
        uint16_t length;
        uint16_t class;
        uint16_t method;
        char *api_key;
    } StunMessage;

    typedef struct StunAttribute {
        // array of bytes which stores the attribute (including the bytes that
        // declare its length and attribute type)
        const uint8_t *bytes;
        // length of the array of bytes
        uint16_t length;
        // type of the attribute
        uint16_t attr_type;
    } StunAttribute;


    /**
     * Creates a STUN message out of an array of bytes. This function checks
     * the validity of the message returned.  
     * @param bytes
     * @param length
     * @param message A pointer to a preallocated memory region where the processed
     * StunMessage will be stored.
     * @return true is the message has been successfully processed, false otherwise. 
     */
    int stun_process_stun_message(uint8_t *bytes, uint16_t length, StunMessage *message);

    /**
     * Frees the memory allocated by a StunMessage created 
     * using stun_new_stun_message.
     * 
     * @param stun_message
     */
    void stun_free(StunMessage* stun_message);

    /**
     * Creates a STUN message and allocates memory (using malloc) for a message of the given
     * length. This method sets the following fields of the STUN message: 
     * magic cookie, transaction ID (which is randomly generated), and STUN message
     * length (initialized to 0). Also, StunMessage.length is set to the lenth of
     * the number of bytes allocated for StunMessage.bytes. All other fields must
     * be set manually to their desired values before using the message.
     * 
     * @param length
     *            Length in bytes of the byte array in which the message will be
     *            stored.
     * @param set_tr_id 
     *            If false, transaction id of the message will not be
     *            generated.
     * @return A pointer to the allocated StunMessage. 
     */
    StunMessage* stun_new_stun_message(uint16_t length, uint8_t set_tr_id);

    /**
     * Creates an empty STUN message (only the header, no attributes) and
     * sets the length (0) and the transaction Id of the message. It is equivalent
     * to the call
     * 
     * stun_new_stun_message(0, 1);
     * @return 
     */
    StunMessage* stun_new_empty_stun_message();
    
    /**
     * Sets the class and method of the given message.
     * @param message
     * @param method
     * @param class
     */
    void stun_set_method_and_class(StunMessage *message, uint16_t method, uint16_t class);

    /**
     * Retrieves the method and class of the given StunMessage.
     * @param message
     * @param method If not null, the method of the message is stored in the variable which method points to.
     * @param class If not null, the class of the message is stored in the variable which class points to.
     */
    void stun_get_method_and_class(StunMessage *message, uint16_t *method, uint16_t *class);
    
    /**
     * Adds the given attribute to the given STUN message updating all the required
     * fields, except message integrity. This method will automatically allocate
     * memory using realloc to allow room in memory for the attribute. The actual
     * length of the STUN message must be set in the corresponding place of the byte
     * array bytes (see RFC 5389) before calling this function. This length will
     * be automatically updated after calling this function. This method does 
     * not check for duplicate attributes, so care must be taken to avoid adding
     * an attribute twice.
     * @param message A pointer to the StunMessage struct to be modified. This 
     * pointer should have been created through stun_new_stun_message.
     * @param attr_type The type of the attribute.
     * @param length The length in bytes of the value of the attribute.
     * @param value The value of the attribute (without padding).
     * @return A pointer to the newly allocated StunMessage
     */
    StunMessage* stun_add_attribute(StunMessage *message, uint16_t attr_type, uint16_t value_length, uint8_t *value);

    /**
     * Sets the message integrity of the message, by working out its HMAC-SHA1
     * signature using the given api key as key. This function will automatically 
     * allocate memory using realloc to allow room in memory for the attribute,
     * if needed. Before adding the message integrity, the actual length of 
     * the STUN message must be set in the corresponding place of the byte
     * array bytes (see RFC 5389). This length will be automatically updated
     * after calling this function.
     * @param message A pointer to the StunMessage struct to be modified. This 
     * pointer should have been created through stun_new_stun_message.
     * @param api_key The API key which will be used to sign the message.
     * @return 
     */
    StunMessage* stun_set_message_integrity(StunMessage *message, char *api_key);

    /**
     * Returns the attribute of the given STUN message with the given attribute
     * code.
     * @param message The message from which the attribute should be returned.
     * @param attribute_code The attribute code.
     * @param attribute 
     * @return false if the given attribute is
     * not contained in the given STUN message. true otherwise.
     */
    int stun_get_attribute(const StunMessage *message, uint16_t attribute_code, StunAttribute *attribute);

    /**
     * If the class of the given message is CL_ERROR, returns an integer
     * describing the error. If an error message is available it will be 
     * returned in error_message_buffer. If the class of the message is not CL_ERROR,
     * this method will return 0.
     * @param message
     * @param error_message_buffer
     * @param buffer_len maximum capacity of error_message_buffer
     * @return 
     */
    uint16_t stun_get_error_code(const StunMessage *message, char *error_message_buffer, int buffer_len);

    /**
     * Validates the HMAC-SHA1 signature of the message using the given api key.
     * @param message
     * @param api_key
     * @return STUN_TRUE if message integrity is correct, false otherwise.
     */
    int stun_is_integrity_correct(const StunMessage *message, const char *api_key);

    /**
     * Checks whether the given bytes constitute a well formed STUN message.
     * @param bytes
     * @return If message is well formed returns STUN_TRUE, otherwise STUN_FALSE is returned.
     */
    int stun_is_well_formed(const uint8_t *bytes, uint16_t length);

    /**
     * Prints the hex representation of the STUN message to stdout.
     * @param message
     */
    char* stun_to_string(StunMessage *message);
    
    /**
     * Generates a ready to send keepalive message for the given device.
     * @param device
     * @return A pointer to a struct StunMessage. After using the instance, memory
     * must be freed using stun_free.
     */
    StunMessage* stun_get_keepalive_message(LH_Device *device);
    
    
    /**
     * Generates a ready to send success message that acknowledges the reception
     * of the given message.
     * @param device
     * @param message
     * @param additional_attr If not NULL, adds this attribute to the response message.
     * @return 
     */
    StunMessage* stun_get_success_response(LH_Device *device, StunMessage *received_message, StunAttribute *additional_attr);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* STUN_MESSAGE_H */

