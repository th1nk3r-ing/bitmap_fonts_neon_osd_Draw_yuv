prt_Green="\e[1;32m"
prt_Yellow="\e[1;33m"
prt_Red="\e[1;31m"
prt_End="\e[0m"

# ndk 根目录(使用绝对路径)
Ndk_Path=/home/think3r/bin/android-ndk-r22

# 编译器
ToolChains_PreFix=$(Ndk_Path)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android30-
CC=$(ToolChains_PreFix)clang

# SDK根目录(sysroot)
SysRoot_Path=$(Ndk_Path)/toolchains/llvm/prebuilt/linux-x86_64/sysroot

# 启动多核编译
MAKEFLAGS = -j

Target = osd_demo

Include_Dir :=  -I./inc/ \
				-I../osd_Draw/

# 编译选项
C_Flags += -g -Wall -Wno-unused -Wconversion -Wno-uninitialized -Wno-sign-conversion
C_Flags += $(Include_Dir)

# 源文件
Src_Files = ../osd_Draw/osd_base.c \
			../osd_Draw/neon_osd.c \
			../osd_Draw/cpu_osd.c \
			../osd_Draw/frontRes.c \
			./src/build_time.c \
			./src/neon_intrinsics_test.c \
			./src/osd_test.c \
			./src/test.c
#Src_Files += $(patsubst %.c,%.o,$(wildcard ./*.c))

#利用模式变量/目标变量, 指定复杂运算代码的优化等级;
../osd_Draw%.o : C_Flags += -O2
#../osd_Draw/neon_osd.o ../osd_Draw/cpu_osd.o : C_Flags += -O2

#Object File
Objs := $(Src_Files:.c=.o)

# 链接输出
$(Target) : $(Objs)
	$(CC) $(C_Flags) -o $(Target) $(Objs)
	@echo -e $(prt_Green) "\n****** make OK! *****\n" $(prt_End)

# 编译源文件
$(Objs) : %.o : %.c
	$(CC) $(C_Flags) -c $< -o $@

.PHONY : clean test run

run :
	adb push $(Target) /data/local/tmp/
	adb shell chmod +x /data/local/tmp/$(Target)
	adb shell "cd /data/local/tmp/ && ./${Target}"
	adb pull /data/local/tmp/osdOut.bin .

clean :
	-rm -rf $(Target) $(Objs)
