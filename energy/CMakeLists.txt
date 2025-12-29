# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/FreeRTOS/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(energy C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(
    energy
    energy.c

    app/app_init.c
    app/app_tasks.c

    core/system_state.c
    core/time_manager.c

    tasks/task_env_sensors.c
    tasks/task_energy.c
    tasks/task_ui.c

    services/wifi_manager.c
    services/mqtt_manager.c
    services/mqtt_payload_service.c
    services/lwip_poll_service.c
    services/webserver/webserver_task.c
    services/auth/auth_storage.c
    services/factory_reset/factory_reset.c

    drivers/aht10/aht10.c
    drivers/bh1750/bh1750.c
    drivers/pzem004t/pzem004t.c
    drivers/rtc_ds3231/rtc_ds3231.c
    drivers/st7735/st7735.c
    drivers/st7735/fonts.c
    drivers/eeprom_at24c32/eeprom_at24c32.c
    
    ui/ui_energy.c
    ui/ui_environment.c
    ui/ui_events.c

    utils/crypto/sha256.c

    )

pico_set_program_name(energy "energy")
pico_set_program_version(energy "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(energy 0)
pico_enable_stdio_usb(energy 1)

# Add the standard library to the build
target_link_libraries(energy
        pico_stdlib
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
)

# Add the standard include files to the build
target_include_directories(energy PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/app
        ${CMAKE_CURRENT_LIST_DIR}/core
        ${CMAKE_CURRENT_LIST_DIR}/tasks
        ${CMAKE_CURRENT_LIST_DIR}/services
        ${CMAKE_CURRENT_LIST_DIR}/services/webserver
        ${CMAKE_CURRENT_LIST_DIR}/services/auth
        ${CMAKE_CURRENT_LIST_DIR}/services/factory_reset
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/drivers
        ${CMAKE_CURRENT_LIST_DIR}/drivers/aht10
        ${CMAKE_CURRENT_LIST_DIR}/drivers/bh1750
        ${CMAKE_CURRENT_LIST_DIR}/drivers/pzem004t
        ${CMAKE_CURRENT_LIST_DIR}/drivers/rtc_ds3231
        ${CMAKE_CURRENT_LIST_DIR}/drivers/st7735
        ${CMAKE_CURRENT_LIST_DIR}/drivers/eeprom_at24c32
        ${CMAKE_CURRENT_LIST_DIR}/ui
        ${CMAKE_CURRENT_LIST_DIR}/utils
        ${CMAKE_CURRENT_LIST_DIR}/utils/crypto
)

# Add any user requested libraries
target_link_libraries(energy 
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_mqtt
        hardware_i2c
        hardware_spi
        )

pico_add_extra_outputs(energy)

