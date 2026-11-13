cmake_minimum_required(VERSION 3.15)
project(starhex VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(starhex src/main.cpp)

target_compile_options(starhex PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-O3 -Wall -Wextra -pedantic>
)

include(GNUInstallDirs)
install(TARGETS starhex DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES completions/starhex.bash DESTINATION ${CMAKE_INSTALL_DATADIR}/bash-completion/completions RENAME starhex OPTIONAL)
install(FILES completions/_starhex DESTINATION ${CMAKE_INSTALL_DATADIR}/zsh/site-functions OPTIONAL)
install(FILES completions/starhex.fish DESTINATION ${CMAKE_INSTALL_DATADIR}/fish/vendor_completions.d OPTIONAL)
install(FILES doc/starhex.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1 OPTIONAL)
