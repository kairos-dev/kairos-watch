# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/artur/esp/esp-idf/components/bootloader/subproject"
  "/home/artur/projects/personal/kairos-watch/build/bootloader"
  "/home/artur/projects/personal/kairos-watch/build/bootloader-prefix"
  "/home/artur/projects/personal/kairos-watch/build/bootloader-prefix/tmp"
  "/home/artur/projects/personal/kairos-watch/build/bootloader-prefix/src/bootloader-stamp"
  "/home/artur/projects/personal/kairos-watch/build/bootloader-prefix/src"
  "/home/artur/projects/personal/kairos-watch/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/artur/projects/personal/kairos-watch/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
