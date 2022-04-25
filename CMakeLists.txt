cmake_minimum_required (VERSION 2.8.9)

if ($ENV{TARGET} MATCHES "hsm")
  message("HSM BUILD")
  include(codesafe.cmake)
endif ()

if ($ENV{TARGET} MATCHES "ncore")
  message("NCORE BUILD")
  include(ncore.cmake)
endif ()

project (walletSEE)

# Subdirectories
add_subdirectory(core)

