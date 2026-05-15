set(USER_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../han_shu/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../BSP/LED/
)

set(USER_Application_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../han_shu/han_shu.c
     ${CMAKE_CURRENT_SOURCE_DIR}/../../BSP/LED/BSP_LED.c
)

target_include_directories(stm32cubemx INTERFACE ${USER_Include_Dirs})

target_sources(stm32cubemx PRIVATE ${USER_Application_Src})