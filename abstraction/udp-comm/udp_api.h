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
 * @file udp_api.h
 * @brief This header file contains all the functions that abstract the UDP
 * communications of the library.
 * 
 * All the function in this header file belong to the abstraction API of the 
 * library and need to be reimplemented when changing platform. The documentation
 * of each function contains all the information about its expected behaviour. This
 * information must be carefully followed when porting the library to other platforms.
 */

#ifndef UDP_API_H
#define	UDP_API_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LHINGS_SERVER_HOST "www.lhings.com"
#define LHINGS_SERVER_UDP_PORT "3478"
#define MAXBUFLEN 2048
#include <stdint.h>
#include "../../core/stun-messaging/stun_message.h"
    
    /**
     * Sends to Lhings server (www.lhings.com) UDP port 3478 the bytes that make up the given
     * STUN message. Implementations must bind to a port and use it for the 
     * entire session, so that the server knows where to send replies and
     * requests to the device.
     * @param message 
     * @return 1 on success, 0 otherwise.
     */
    int lh_send_to_server(StunMessage *message);
    
    /**
     * Listens asynchronously to UDP packets sent from Lhings server UDP port 3478. The call must not block
     * indefinitely until a packet is received, since this would stall the main
     * loop of execution (this C library is implemented using a single thread). 
     * Listening must be done in the same port as the one used
     * by lh_send_to_server.
     * @param bytes_recv If not null, the number of bytes received will be stored
     * in the address it points to.
     * @return null if no data is received, otherwise an array of bytes containing the
     * data received.
     */
    uint8_t* lh_receive_from_server(uint16_t *bytes_recv);
    


#ifdef	__cplusplus
}
#endif

#endif	/* UDP_API_H */

