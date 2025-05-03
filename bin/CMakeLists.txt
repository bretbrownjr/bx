# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(GNUInstallDirs)

install(
  PROGRAMS bx
  COMPONENT bx
  DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
