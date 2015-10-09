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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "udp_api.h"
#include "../../core/stun-messaging/stun_message.h"
#include "../../core/logging/log.h"


static struct sockaddr_in *destination_addr = NULL;
static char str_client_port[6];
static int listener_sock_fd = -1;

int lh_send_to_server(StunMessage *message) {
    uint8_t *bytes = message->bytes;
    uint16_t length = message->length;
    int status;
    char *log_msg;


    //initialize socket and bind
    if (destination_addr == NULL) {
        struct addrinfo hints;
        struct addrinfo *servinfo, *p;
        srand(time(NULL));
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;

        if ((status = getaddrinfo(LHINGS_SERVER_HOST, LHINGS_SERVER_UDP_PORT, &hints, &servinfo)) != 0) {
            log_msg = lh_get_message_str("Unable to send UDP. Reason: %s", gai_strerror(status));
            log_error(log_msg);
            free(log_msg);
            return 0;
        }


        for (p = servinfo; p != NULL; p = p->ai_next) {
            if (p->ai_addr != NULL) {
                destination_addr = malloc(sizeof *destination_addr);
                memcpy(destination_addr, (struct sockaddr_in *) p->ai_addr, sizeof *p->ai_addr);
            }
        }
        int client_port = 1027 + rand() % 50000;
        snprintf(str_client_port, 6, "%d", client_port);
        freeaddrinfo(servinfo);
    }

    int send_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (send_socket_fd == -1) {
        log_msg = lh_get_message_str("Unable to create UDP socket. Reason: %s", strerror(errno));
        log_error(log_msg);
        free(log_msg);
        return 0;
    }

    int yes = 1;
    if (setsockopt(send_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) {
        perror("setsockopt");
        return 0;
    }
    // bind to local address
    struct addrinfo *source_addr_info;
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    if ((status = getaddrinfo(NULL, str_client_port, &hints, &source_addr_info)) != 0) {
        log_msg = lh_get_message_str("Unable to send UDP. Reason: %s", gai_strerror(status));
        log_error(log_msg);
        free(log_msg);
        return 0;
    }

    status = bind(send_socket_fd, source_addr_info->ai_addr, source_addr_info->ai_addrlen);
    if (status == -1) {
        log_msg = lh_get_message_str("Bind failed. Reason: %s", strerror(errno));
        log_error(log_msg);
        free(log_msg);
        freeaddrinfo(source_addr_info);
        return 0;
    }

    // send
    struct sockaddr destination_sock_addr = *((struct sockaddr *) destination_addr);
    int bytes_sent = sendto(send_socket_fd, bytes, length, 0, &destination_sock_addr, sizeof (destination_sock_addr));
    if (bytes_sent != length) {
        if (bytes_sent == -1) {
            log_msg = lh_get_message_str("UDP send failed. Reason: %s", strerror(errno));
            log_error(log_msg);
            free(log_msg);
        } else {
            log_warn("UDP: not all bytes could be sent.");
        }
    }

    freeaddrinfo(source_addr_info);
    close(send_socket_fd);
    return 1;
}

uint8_t* lh_receive_from_server(uint16_t *bytes_recv) {
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char *log_msg;

    if (listener_sock_fd == -1) {
        // initialize listener socket
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET; // set to AF_INET to force IPv4
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE; // use my IP

        if ((rv = getaddrinfo(NULL, str_client_port, &hints, &servinfo)) != 0) {
            log_msg = lh_get_message_str("getaddrinfo: %s\n", gai_strerror(rv));
            log_error(log_msg);
            free(log_msg);
            return NULL;
        }

        // loop through all the results and bind to the first we can
        for (p = servinfo; p != NULL; p = p->ai_next) {
            if ((listener_sock_fd = socket(p->ai_family, p->ai_socktype,
                    p->ai_protocol)) == -1) {
                perror("listener: socket");
                continue;
            }
            // set socket non-blocking
            fcntl(listener_sock_fd, F_SETFL, O_NONBLOCK);
            int yes = 1;
            if (setsockopt(listener_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) {
                perror("setsockopt");
                return 0;
            }

            if (bind(listener_sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
                close(listener_sock_fd);
                perror("listener: bind");
                continue;
            }

            break;
        }

        if (p == NULL) {
            log_error("listener: failed to bind socket\n");
            return NULL;
        }

        freeaddrinfo(servinfo);
    }


    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(listener_sock_fd, buf, MAXBUFLEN - 1, 0,
            (struct sockaddr *) &their_addr, &addr_len)) == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK){
            perror("recvfrom");
            printf("Error: %d %d %s\n", numbytes, errno, strerror(errno));
        }
        return NULL;
    }

    uint8_t *datagram = malloc(numbytes * sizeof *datagram);
    memcpy(datagram, buf, numbytes);
    if (bytes_recv != NULL){
        *bytes_recv = (uint16_t) numbytes;
    }
    log_msg = lh_get_message_int("listener: packet is %d bytes long", numbytes);
    log_debug(log_msg);
    free(log_msg);

    return datagram;
}
