# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/ramulator2-be93be78055d922aa1d4d33e15bcc8f2b0c61a9d/ext/spdlog"
  "/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/build/_deps/spdlog-build"
  "/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/build/_deps/spdlog-subbuild/spdlog-populate-prefix"
  "/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/build/_deps/spdlog-subbuild/spdlog-populate-prefix/tmp"
  "/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/build/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp"
  "/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/build/_deps/spdlog-subbuild/spdlog-populate-prefix/src"
  "/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/build/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/build/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/build/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
