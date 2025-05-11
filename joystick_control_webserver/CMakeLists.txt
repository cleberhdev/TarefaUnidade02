# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# ==============================================
# Configurações padrão do SDK - não alterar
# ==============================================
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)

if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# ==============================================
# Configurações principais do projeto
# ==============================================
cmake_minimum_required(VERSION 3.13)

# Configuração da placa (Pico W com Wi-Fi)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa o SDK
include(pico_sdk_import.cmake)

# Nome do projeto (DEVE bater com o nome do arquivo .c)
project(joystick_control_webserver C CXX ASM)

# Inicializa o SDK
pico_sdk_init()

# ==============================================
# Configurações do executável
# ==============================================
add_executable(joystick_control_webserver joystick_control_webserver.c)

# Metadados do programa
pico_set_program_name(joystick_control_webserver "joystick_control_webserver")
pico_set_program_version(joystick_control_webserver "0.1")

# Configuração de saída (USB ativado, UART desativado)
pico_enable_stdio_uart(joystick_control_webserver 0)
pico_enable_stdio_usb(joystick_control_webserver 1)

# ==============================================
# Bibliotecas necessárias
# ==============================================
target_link_libraries(joystick_control_webserver
    pico_stdlib               # Biblioteca padrão
    hardware_adc              # Para leitura analógica do joystick
    hardware_gpio             # Para o botão do joystick
    pico_cyw43_arch_lwip_threadsafe_background  # Para Wi-Fi
    m                         # Biblioteca matemática (atan2f)
)

# ==============================================
# Configurações de include
# ==============================================
target_include_directories(joystick_control_webserver PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}                   # Pasta atual
    ${PICO_SDK_PATH}/lib/lwip/src/include       # LWIP (Wi-Fi)
    ${PICO_SDK_PATH}/lib/lwip/src/include/arch
    ${PICO_SDK_PATH}/lib/lwip/src/include/lwip
)

# ==============================================
# Arquivos adicionais do lwIP (necessários para Wi-Fi)
# ==============================================
target_sources(joystick_control_webserver PRIVATE
    ${PICO_SDK_PATH}/lib/lwip/src/apps/http/httpd.c
    ${PICO_SDK_PATH}/lib/lwip/src/apps/http/fs.c
)

# ==============================================
# Geração de arquivos adicionais (.uf2, .hex, etc)
# ==============================================
pico_add_extra_outputs(joystick_control_webserver)