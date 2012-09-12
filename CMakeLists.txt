CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_MODULE_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/../openlpc-deps/cmake)
SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../openlpc-deps/cmake/Toolchains/Toolchain-lpc1759.cmake)
SET(LINKER_SCRIPT_FILE   ${CMAKE_CURRENT_SOURCE_DIR}/../openlpc-deps/basefiles/lpc1759BL.ld)

PROJECT(FreeRTOS C)

INCLUDE(lpc21isp)
INCLUDE(usb_msc)
#
#
#

SET(CMSIS_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/../openlpc-deps/CMSIS)
SET(BASEFILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../openlpc-deps/basefiles)
SET(DSPLIB_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/../openlpc-deps/CMSIS/DSP_Lib)
SET(FreeRTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../openlpc-deps/basefiles_FreeRTOS)
SET(LPCUSB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/usbstack)

INCLUDE_DIRECTORIES(
	.
	./include
	${FreeRTOS_DIR}
	${FreeRTOS_DIR}/include
	${FreeRTOS_DIR}/portable/ARM_CM3
	${CMSIS_DIR}/include
	${DSPLIB_DIR}/include
	${DSPLIB_DIR}/src/Common/include
	${LPCUSB_DIR}/include

)

ADD_DEFINITIONS(-Wall -Wextra -O2 -ffunction-sections -fdata-sections)

ADD_EXECUTABLE(main
	./src/main.c
	${FreeRTOS_DIR}/startup_lpc17xx.c
	${FreeRTOS_DIR}/syscalls.c
	${FreeRTOS_DIR}/tasks.c
	${FreeRTOS_DIR}/queue.c
	${FreeRTOS_DIR}/timers.c
	${FreeRTOS_DIR}/croutine.c
	${FreeRTOS_DIR}/list.c
	${FreeRTOS_DIR}/portable/MemMang/heap_2.c
	${FreeRTOS_DIR}/portable/ARM_CM3/port.c
	${CMSIS_DIR}/src/core_cm3.c
	${CMSIS_DIR}/src/system_lpc17xx.c
	${DSPLIB_DIR}/src/Common/source/math_helper.c
	${LPCUSB_DIR}/src/usb_cdc.c
	${LPCUSB_DIR}/src/usbcontrol.c
	${LPCUSB_DIR}/src/usbhw_lpc.c
	${LPCUSB_DIR}/src/usbinit.c
	${LPCUSB_DIR}/src/usbstdreq.c
)

TARGET_LINK_LIBRARIES(main -lm ${DSPLIB_DIR}/lib/arm_cortexM3_math.lib)

GENERATE_HEX(main)

LPC21ISP(main /dev/ttyUSB0 115200 12000 PROGRAM_TARGET)

USB_MSC(main /dev/sdb)


