cmake_minimum_required (VERSION 2.8.9)
project (walletSEE)

find_package(Threads REQUIRED)

#adding sub directories 
add_subdirectory(facil.io)

# include(CTest)
# enable_testing()

set(main_SOURCES
    main.c
    cli.c
    http_service.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wcast-qual -std=gnu11 -DPB_FIELD_16BIT")

# Main source files
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wno-unknown-pragmas -pthread")

add_executable(WalletCore ${main_SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
target_link_libraries(WalletCore PUBLIC facil.io)
target_link_libraries(WalletCore
  PRIVATE Threads::Threads
  PUBLIC pthread
  PUBLIC m
  )
# include(CPack)


# add_library(
#     crypto0
#     log.h
#     wallet.c
#     util.c
#     keystore.c
#     sigstore.c
#     files.h
#     aes_gcm.c
# )
# target_link_libraries(crypto0 MJson TrezorCrypto sss)
# target_link_libraries(cold0 crypto0 TrezorCrypto MJson sss)