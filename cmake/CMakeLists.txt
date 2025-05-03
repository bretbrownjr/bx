# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(GNUInstallDirs)

install(
  FILES
    bx.cmake
    use-fetch-content.cmake
  COMPONENT bx
  DESTINATION "${CMAKE_INSTALL_SHAREDIR}/cmake"
)
