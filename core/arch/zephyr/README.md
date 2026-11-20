# Hints for building 4diac FORTE for Zephyr OS

In general, the same instructions as for the other target platforms apply. You can build via cmake-gui or use the setup_zephyr.sh script.

The easiest integration into a Zephyr application can be achieved by adding 4diac FORTE to the app's West / CMake build process. This has the
important benefit that you don't have to discover and set all the compiler paths, includes, flags, by yourself.

4diac FORTE requires Zephyr OS version 3.5 or later.

## Integrating 4diac FORTE into a Zephyr app

In your app's CMakeLists.txt, add the following snippet:

```
cmake_minimum_required(VERSION 3.20.0)

# Standard Zephyr package initialization
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(forte_zephyr_app LANGUAGES C CXX)

target_sources(app PRIVATE src/main.cpp)

include(ExternalProject)

set(FORTE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/4diac-forte")
set(FORTE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/forte-build")
set(FORTE_STATIC_LIB "${FORTE_BINARY_DIR}/src/libforte-static.a")

# Extract Zephyr's include directories and compile definitions for FORTE
get_target_property(ZEPHYR_INCLUDES zephyr_interface INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(ZEPHYR_DEFINES zephyr_interface INTERFACE_COMPILE_DEFINITIONS)

# Format lists with the '|' separator so CMake passes them cleanly to ExternalProject_Add
string(REPLACE ";" "|" FORTE_ZEPHYR_INC "${ZEPHYR_INCLUDES}")
string(REPLACE ";" "|" FORTE_ZEPHYR_DEF "${ZEPHYR_DEFINES}")

# Grab active C/CXX flags (including architecture, FPU, and C++ standard flags)
set(COMBINED_C_FLAGS "${CMAKE_C_FLAGS} $<TARGET_PROPERTY:zephyr_interface,INTERFACE_COMPILE_OPTIONS>")
set(COMBINED_CXX_FLAGS "${CMAKE_CXX_FLAGS} $<TARGET_PROPERTY:zephyr_interface,INTERFACE_COMPILE_OPTIONS>")

ExternalProject_Add(forte_external
    SOURCE_DIR "${FORTE_SOURCE_DIR}"
    BINARY_DIR "${FORTE_BINARY_DIR}"
    LIST_SEPARATOR |
    CMAKE_ARGS
        -G "Ninja"
        -DCMAKE_SYSTEM_NAME=Generic
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_AR=${CMAKE_AR}
        -DCMAKE_RANLIB=${CMAKE_RANLIB}
        -DCMAKE_C_COMPILER_WORKS=ON
        -DCMAKE_CXX_COMPILER_WORKS=ON
        -DCMAKE_C_FLAGS=${COMBINED_C_FLAGS}
        -DCMAKE_CXX_FLAGS=${COMBINED_CXX_FLAGS}
        -DNULLPTR_SUPPORTED=ON
        # FORTE Zephyr Architecture & Library Settings
        -DFORTE_ARCHITECTURE=Zephyr
        -DFORTE_BUILD_EXECUTABLE=OFF
        -DFORTE_BUILD_STATIC_LIBRARY=ON
        -DFORTE_C_INTERFACE=ON
        -DFORTE_TESTS=OFF
        -DFORTE_SUPPORT_BOOT_FILE=ON
        -DFORTE_SUPPORT_MONITORING=ON
        # Function Block Modules
        -DFORTE_MODULE_CONVERT=ON
        -DFORTE_MODULE_IEC61131=ON
        -DFORTE_IO=ON
        # Forward Zephyr includes & definitions
        "-DCMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES=${FORTE_ZEPHYR_INC}"
        "-DCMAKE_C_STANDARD_INCLUDE_DIRECTORIES=${FORTE_ZEPHYR_INC}"
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR>
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS "${FORTE_STATIC_LIB}"
)

# Register the compiled static library as an imported target
add_library(forte_lib STATIC IMPORTED GLOBAL)
set_target_properties(forte_lib PROPERTIES IMPORTED_LOCATION "${FORTE_STATIC_LIB}")
add_dependencies(forte_lib forte_external)

# Include FORTE headers for main.cpp (specifically forte_c.h)
target_include_directories(app PRIVATE
    "${FORTE_SOURCE_DIR}/src"
    "${FORTE_SOURCE_DIR}/src/arch"
    "${FORTE_SOURCE_DIR}/src/arch/zephyr"
    "${FORTE_BINARY_DIR}/src_gen"
)

# Link with --whole-archive so static FB factories are not discarded by the linker
target_link_libraries(app PRIVATE
    -Wl,--whole-archive
    forte_lib
    -Wl,--no-whole-archive
)
```

In your app's `prj.conf`, turn on the required C++ support:

```
# C++ & Full Standard Library Support
CONFIG_CPP=y
CONFIG_STD_CPP17=y
CONFIG_REQUIRES_FULL_LIBCPP=y
CONFIG_GLIBCXX_LIBCPP=y
CONFIG_CPP_EXCEPTIONS=n
CONFIG_CPP_RTTI=n

# POSIX Compatibility Layer (used by FORTE threading, mutexes, and sockets)
CONFIG_POSIX_API=y
CONFIG_PTHREAD_IPC=y
CONFIG_POSIX_CLOCK=y
CONFIG_EVENTFD=y
CONFIG_MAX_PTHREAD_COUNT=16

# Memory & Stack Sizes (FORTE uses dynamic allocation for FBs and events)
CONFIG_HEAP_MEM_POOL_SIZE=131072
CONFIG_MAIN_STACK_SIZE=8192
CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE=4096

# Networking (Required for 4diac IDE deployment and monitoring over TCP port 61499)
CONFIG_NETWORKING=y
CONFIG_NET_IPV4=y
CONFIG_NET_TCP=y
CONFIG_NET_UDP=y
CONFIG_NET_SOCKETS=y
CONFIG_NET_SOCKETS_POSIX_NAMES=y
CONFIG_NET_DHCPV4=y

# Console & Logging
CONFIG_PRINTK=y
CONFIG_LOG=y
```

## Implementing and extending a Modular IO Controller Configuration FB

Use the ZephyrIO function block template for modular IO included with the 4diac IDE TypeLibrary to start a controller configuration FB. The FBT is located in `.../TypeLibrary/io/zephyr/ZephyrIO.fbt` of 4diac projects. The exported FB for compiling and linking with FORTE is provided in `src/modules/zephyr/types/ZephyrIO_fbt.*` of the FORTE sources. We recommend that you perform a 4diac IDE Type Export once to familiarize yourself with the non-generated implementation portions in the provided `ZephyrIO_fbt.*` sources. In order to sucessfully do this, run the 4diac IDE Type Export Wizard, and leave the "Overwrite with warning" checkbox unselected, plus when asked whether to perform an overwrite or merge, always choose Merge.

In detail, the header file for Zephyr modular IO controller configuration FBs has to be merged with the following non-automatic sections:

- The includes
- The base class specification
- Removing the executeEvent() override
- The section containing number of IOs, setConfig(), and onStartup()


The compilation unit file in turn to be merged with the following non-automatic sections:

- The includes
- The base class specification
- Removing the executeEvent() override
- The section containing setConfig(), and onStartup()

Next, copy the `.FBT` and `_fbt.*` to match your own board, and seek and replace all uses of `ZephyrIO` in the files to match your new name. Verify that export and merge still gives the expected result.

In order to add the actual IOs of your board configuration, you can look at the differences between ZephyrIO and Esp32EthernetKitIO. The latter has a sample set of GPIO input/output, ADC, and PWM, and it should all be sufficiently self explaining if you know your way around developing applications with the Zephyr OS.

