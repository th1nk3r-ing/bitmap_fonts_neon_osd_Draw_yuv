# @think3r
#====================================================================================================================
# cmake 内置命令支持大写、小写或者混合使用; 而内置变量是区分大小写的，或者干脆就说，cmake的所有变量都是区分大小写的
# cmake最低版本需求，不加入此行会受到警告信息
cmake_minimum_required(VERSION 3.0.0)

# 配置交叉编译工具链
set(CMAKE_SYSTEM_NAME Linux)
set(ndk_path "/home/think3r/bin/android-ndk-r14b")
set(ndk_toolchains_prefix "${ndk_path}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-")
set(CMAKE_C_COMPILER "${ndk_toolchains_prefix}gcc")
set(CMAKE_CXX_COMPILER "${ndk_toolchains_prefix}g++")
set(CMAKE_SYSROOT "${ndk_path}/platforms/android-14/arch-arm") # --sysroot 选项

#====================================================================================================================

project(Osd_Neon_Draw_Project C CXX ASM)  #项目名称
set(Library_OutPutName "neonOsd")
set(My_Target "neonOsdDemo")	#可执行文件名称

# 是否导出编译过程到 compile_commands.json 文件中
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS  "----------------------------------------->")
message(STATUS  "project name: " ${PROJECT_NAME})
message(STATUS  "PROJECT_BINARY_DIR:" ${PROJECT_BINARY_DIR})
message(STATUS  "PROJECT_SOURCE_DIR:" ${PROJECT_SOURCE_DIR})
message(STATUS  "CMAKE_C_COMPILER:" ${CMAKE_C_COMPILER})
message(STATUS  "CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})
message(STATUS  "CMAKE_SYSROOT:" ${CMAKE_SYSROOT})
message(STATUS  "<-----------------------------------------")
#message(SEND_ERROR "test apoidjacposidjcapsoidj")
#message(FATAL_ERROR "test apoidjacposidjcapsoidj")


# 配置编译的宏
set(gnu_flags " -Wall -Wno-unused -Wconversion -Wno-uninitialized -Wno-sign-conversion ")
set(neno_flags "-mfloat-abi=softfp -mfpu=neon-vfpv4 ")
set(android_5_flags "-fPIE -pie ")
set(CMAKE_CXX_FLAGS " ${gnu_flags} ${neno_flags} ${android_5_flags} ")
set(CMAKE_C_FLAGS " ${gnu_flags} ${neno_flags} ${android_5_flags} ")

#======================================== 库的生成 =================================================================
add_subdirectory(./osd_Draw lib)

#========================================== 测试 demo 的生成 =======================================================
add_subdirectory(./demo bin)
