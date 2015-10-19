#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/abstraction/http-comm/http_api.o \
	${OBJECTDIR}/abstraction/permanent-storage/storage_api.o \
	${OBJECTDIR}/abstraction/timing/lhings_time.o \
	${OBJECTDIR}/abstraction/udp-comm/udp_api.o \
	${OBJECTDIR}/core/crypto/base64.o \
	${OBJECTDIR}/core/crypto/hmac-sha1.o \
	${OBJECTDIR}/core/crypto/memxor.o \
	${OBJECTDIR}/core/crypto/sha1.o \
	${OBJECTDIR}/core/http-comm/lhings_api.o \
	${OBJECTDIR}/core/lhings.o \
	${OBJECTDIR}/core/logging/log.o \
	${OBJECTDIR}/core/stun-messaging/stun_message.o \
	${OBJECTDIR}/core/utils/data_structures.o \
	${OBJECTDIR}/core/utils/lhings_json_api.o \
	${OBJECTDIR}/core/utils/utils.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/tests/data_structures_tests.o \
	${OBJECTDIR}/tests/hmac_sha1_test.o \
	${OBJECTDIR}/tests/logging_system_tests.o \
	${OBJECTDIR}/tests/stun_message_tests.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs libcurl` -lm   

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lhings-c

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lhings-c: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lhings-c ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/abstraction/http-comm/http_api.o: abstraction/http-comm/http_api.c 
	${MKDIR} -p ${OBJECTDIR}/abstraction/http-comm
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/abstraction/http-comm/http_api.o abstraction/http-comm/http_api.c

${OBJECTDIR}/abstraction/permanent-storage/storage_api.o: abstraction/permanent-storage/storage_api.c 
	${MKDIR} -p ${OBJECTDIR}/abstraction/permanent-storage
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/abstraction/permanent-storage/storage_api.o abstraction/permanent-storage/storage_api.c

${OBJECTDIR}/abstraction/timing/lhings_time.o: abstraction/timing/lhings_time.c 
	${MKDIR} -p ${OBJECTDIR}/abstraction/timing
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/abstraction/timing/lhings_time.o abstraction/timing/lhings_time.c

${OBJECTDIR}/abstraction/udp-comm/udp_api.o: abstraction/udp-comm/udp_api.c 
	${MKDIR} -p ${OBJECTDIR}/abstraction/udp-comm
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/abstraction/udp-comm/udp_api.o abstraction/udp-comm/udp_api.c

${OBJECTDIR}/core/crypto/base64.o: core/crypto/base64.c 
	${MKDIR} -p ${OBJECTDIR}/core/crypto
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/crypto/base64.o core/crypto/base64.c

${OBJECTDIR}/core/crypto/hmac-sha1.o: core/crypto/hmac-sha1.c 
	${MKDIR} -p ${OBJECTDIR}/core/crypto
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/crypto/hmac-sha1.o core/crypto/hmac-sha1.c

${OBJECTDIR}/core/crypto/memxor.o: core/crypto/memxor.c 
	${MKDIR} -p ${OBJECTDIR}/core/crypto
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/crypto/memxor.o core/crypto/memxor.c

${OBJECTDIR}/core/crypto/sha1.o: core/crypto/sha1.c 
	${MKDIR} -p ${OBJECTDIR}/core/crypto
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/crypto/sha1.o core/crypto/sha1.c

${OBJECTDIR}/core/http-comm/lhings_api.o: core/http-comm/lhings_api.c 
	${MKDIR} -p ${OBJECTDIR}/core/http-comm
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/http-comm/lhings_api.o core/http-comm/lhings_api.c

${OBJECTDIR}/core/lhings.o: core/lhings.c 
	${MKDIR} -p ${OBJECTDIR}/core
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/lhings.o core/lhings.c

${OBJECTDIR}/core/logging/log.o: core/logging/log.c 
	${MKDIR} -p ${OBJECTDIR}/core/logging
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/logging/log.o core/logging/log.c

${OBJECTDIR}/core/stun-messaging/stun_message.o: core/stun-messaging/stun_message.c 
	${MKDIR} -p ${OBJECTDIR}/core/stun-messaging
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/stun-messaging/stun_message.o core/stun-messaging/stun_message.c

${OBJECTDIR}/core/utils/data_structures.o: core/utils/data_structures.c 
	${MKDIR} -p ${OBJECTDIR}/core/utils
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/utils/data_structures.o core/utils/data_structures.c

${OBJECTDIR}/core/utils/lhings_json_api.o: core/utils/lhings_json_api.c 
	${MKDIR} -p ${OBJECTDIR}/core/utils
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/utils/lhings_json_api.o core/utils/lhings_json_api.c

${OBJECTDIR}/core/utils/utils.o: core/utils/utils.c 
	${MKDIR} -p ${OBJECTDIR}/core/utils
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core/utils/utils.o core/utils/utils.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/tests/data_structures_tests.o: tests/data_structures_tests.c 
	${MKDIR} -p ${OBJECTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tests/data_structures_tests.o tests/data_structures_tests.c

${OBJECTDIR}/tests/hmac_sha1_test.o: tests/hmac_sha1_test.c 
	${MKDIR} -p ${OBJECTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tests/hmac_sha1_test.o tests/hmac_sha1_test.c

${OBJECTDIR}/tests/logging_system_tests.o: tests/logging_system_tests.c 
	${MKDIR} -p ${OBJECTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tests/logging_system_tests.o tests/logging_system_tests.c

${OBJECTDIR}/tests/stun_message_tests.o: tests/stun_message_tests.c 
	${MKDIR} -p ${OBJECTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall `pkg-config --cflags libcurl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tests/stun_message_tests.o tests/stun_message_tests.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lhings-c

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
