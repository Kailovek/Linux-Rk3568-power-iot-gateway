cmake_minimum_required(VERSION 3.16)
project(power_iot_gateway_showcase C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(power_gateway
    src/crc16.c
    src/mqtt_session.c
    src/mqtt_topic.c
    src/telemetry.c
    src/telemetry_queue.c
    src/store_forward.c
)
target_include_directories(power_gateway PUBLIC include)

add_library(reference_adapter platform/reference_adapter.c)
target_include_directories(reference_adapter PUBLIC include platform)
target_link_libraries(reference_adapter PUBLIC power_gateway)

add_executable(gateway_flow examples/gateway_flow.c)
target_link_libraries(gateway_flow PRIVATE power_gateway reference_adapter)
