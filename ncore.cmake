cmake_minimum_required(VERSION 2.8.9)

set(NFAST_PATH /opt/nfast)
set(NFAST_GCC_PATH ${NFAST_PATH}/c/csd/gcc/include)
set(NFAST_EXAMPLE_PATH ${NFAST_PATH}/c/csd/examples)

include_directories(${NFAST_GCC_PATH}/sworld)
include_directories(${NFAST_GCC_PATH}/hilibs) 
include_directories(${NFAST_GCC_PATH}/nflog) 
include_directories(${NFAST_GCC_PATH}/cutils) 
include_directories(${NFAST_EXAMPLE_PATH}/sworld) 
include_directories(${NFAST_EXAMPLE_PATH}/hilibs) 
include_directories(${NFAST_EXAMPLE_PATH}/nflog) 
include_directories(${NFAST_EXAMPLE_PATH}/cutils)


set(CMAKE_C_LINK_EXECUTABLE "gcc -o <TARGET> <OBJECTS> <LINK_LIBRARIES> ${NFAST_PATH}/c/csd/gcc/lib/librqcard.a ${NFAST_PATH}/c/csd/gcc/lib/libnfkm.a ${NFAST_PATH}/c/csd/gcc/lib/libnfstub.a ${NFAST_PATH}/c/csd/gcc/lib/libnflog.a ${NFAST_PATH}/c/csd/gcc/lib/libcutils.a -lpthread")

