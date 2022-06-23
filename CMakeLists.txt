project(motor_driver)
cmake_minimum_required(VERSION 3.3)

set(CMAKE_C_COMPILER "/usr/bin/avr-gcc")
set(CMAKE_C_STANDARD 17)
set(F_CPU 20000000UL)
set(MCU "atmega328p")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3 -mmcu=${MCU} -DF_CPU=${F_CPU} -flto")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(LFUSE 0xe2)
set(HFUSE 0xd9)
set(EFUSE 0xff)

# 0xdf
# 0xd1
# 0xff

# 0xe2 8MHz
# 0xd9
# 0xff

add_executable(motor_driver
	"${PROJECT_SOURCE_DIR}/src/motor_driver.c"
	"${PROJECT_SOURCE_DIR}/src/main.c"
	"${PROJECT_SOURCE_DIR}/src/uart_com.c"
	"${PROJECT_SOURCE_DIR}/src/adc_control.c"
)

target_link_libraries(motor_driver)
target_include_directories(motor_driver PRIVATE "${PROJECT_SOURCE_DIR}/src")

add_custom_target(prog avrdude ${CMAKE_BUILD_DIR}/${EXECUTABLE_OUTPUT_PATH} 
	-c usbasp -p ${MCU} -U flash:w:${PROJECT_NAME})
add_custom_target(prog_fuse avrdude ${CMAKE_BUILD_DIR}/${EXECUTABLE_OUTPUT_PATH} 
	-c usbasp -p ${MCU} -U lfuse:w:${LFUSE}:m -U hfuse:w:${HFUSE}:m -U efuse:w:${EFUSE}:m)
