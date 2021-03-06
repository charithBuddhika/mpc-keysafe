cmake_minimum_required(VERSION 2.8.9)

set(NFAST_PATH /opt/nfast)
if (DEFINED ENV{NCIPHER_TOOLS_LOCATION})
  set(NFAST_PATH $ENV{NCIPHER_TOOLS_LOCATION})
endif ()

set(NFAST_DEV ${NFAST_PATH}/c/csd)

find_program(
    CODESAFE_GCC
    powerpc-codesafe-linux-gnu-gcc
    PATHS ${NFAST_PATH}/gcc/bin
)

find_program(
    CODESAFE_LD
    powerpc-codesafe-linux-gnu-ld
    PATHS ${NFAST_PATH}/gcc/bin
)

include_directories(${NFAST_PATH}/c/csd/include-see/module ${NFAST_PATH}/c/csd/include-see/module/glibsee ${NFAST_PATH}/c/csd/include-see/hilibs ${NFAST_PATH}/c/csd/include-see/cutils/)

set(LIBPATH_GENERAL "${NFAST_DEV}/lib-ppc-linux-gcc/")
set(CMAKE_C_COMPILER ${CODESAFE_GCC})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mpowerpc -mcpu=e5500 -mno-toc -mbig-endian -mhard-float -mno-multiple -mno-string -meabi -mprototype -mstrict-align -memb -fno-builtin -DNF_CROSSCC=1 -DNFAST_CONF_H='\"nfast-conf-auto.h\"' -DNF_CROSSCC_PPC_GCC=1 -D_THREAD_SAFE -DBYTE_ORDER=4321")

set(CMAKE_C_LINK_EXECUTABLE
    "${CODESAFE_GCC} -Wl,-wrap=read -Wl,-wrap=write -Wl,-wrap=socket -Wl,-wrap=send  -Wl,-wrap=sendto -Wl,-wrap=recv -Wl,-wrap=recvfrom -Wl,-wrap=listen -Wl,-wrap=connect -Wl,-wrap=bind -Wl,-wrap=setsockopt -Wl,-wrap=select -Wl,-wrap=accept -Wl,-wrap=poll -Bstatic -o <TARGET> <OBJECTS> <LINK_LIBRARIES> ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/hoststdioeinetsocks.o ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libfaksys.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libcutils.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libvfsextras.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libseewrpr.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libipccore.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libsolotrace.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libnfstub.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libfaksys.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libseewrpr.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/seelib.a ${NFAST_PATH}/c/csd/lib-ppc-linux-gcc/libipccore.a -lpthread -lrt")