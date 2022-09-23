# <font color=#0099ff> **androidTools** </font>

> `@think3r` 2022-09-23 10:46:04
> 1. [另一个Android性能剖析工具——simpleperf](https://zhuanlan.zhihu.com/p/25277481)
> 2. [西瓜视频稳定性治理体系建设二：Raphael 原理及实践](https://www.infoq.cn/article/yvpsastyybycbua3po1q)
> 3. [Android Native Hook技术路线概述](https://gtoad.github.io/2018/07/05/Android-Native-Hook/)
> 4. [调试本地内存使用](https://source.android.com/docs/core/tests/debug/native-memory)
> 5. [Malloc Debug](https://android.googlesource.com/platform/bionic/+/master/libc/malloc_debug/README.md)

## <font color=#009A000> 0x00 简介 </font>

本文章来进行一些 Android 工具的使用研究, 包含

1. perf 性能探测工具
2. 内存泄漏工具

## <font color=#009A000> 0x01 mallocDebug </font>

```shell
adb shell stop
adb shell setprop libc.debug.malloc.program com.think3rTest.phone
adb shell setprop libc.debug.malloc.options backtrace
adb shell start

# 国内的 Android 手机不能用(小米需 root)
adb shell setprop wrap.com.think3rTest.phone '"LIBC_DEBUG_MALLOC_OPTIONS=backtrace logwrapper"'
```

## <font color=#009A000> 0x02 perf </font>

> 1. <https://android.googlesource.com/platform/prebuilts/simpleperf>
> 2. [安卓性能分析工具Simpleperf详解与应用](http://www.luzexi.com/2020/11/13/%E5%AE%89%E5%8D%93%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96%E5%B7%A5%E5%85%B7Simpleperf%E8%AF%A6%E8%A7%A3)
> 3. [perf Examples](https://www.brendangregg.com/perf.html)
> 4. [linux性能分析工具perf：十八般武器之cache](https://zhuanlan.zhihu.com/p/445267642)
> 5. [CPU最高性能预估之“理论最大IPC”](https://zhuanlan.zhihu.com/p/364661188)
> 6. [计算机组成原理：超标量，让CPU的吞吐率超过1](https://blog.csdn.net/zhizhengguan/article/details/121271156)

### <font color=#FF4500> CPU 基础知识 </font>

1. 流水线 : 是一种将每条指令分解为多步，并让各步操作重叠，从而实现几条指令并行处理的技术。
   1. 程序中的指令仍是一条条顺序执行，但可以预先取若干条指令，并在当前指令尚未执行完时，提前启动后续指令的另一些操作步骤。这样显然可加速一段程序的运行过程。
2. 超级流水线 : 以增加流水线级数的方法来缩短机器周期，相同的时间内超级流水线执行了更多的机器指令。
   1. 采用简单指令以加快执行速度是所有流水线的共同特点，但超级流水线配置了多个功能部件和指令译码电路，采用多条流水线并行处理，还有多个寄存器端口和总线，可以同时执行多个操作，因此比普通流水线执行的更快，在一个机器周期内可以流出多条指令。
3. 超标量（superscalar）: 是指在 CPU 中有一条以上的流水线，并且每时钟周期内可以完成一条以上的指令，这种设计就叫超标量技术。
   1. 其实质是以空间换取时间。
   2. 而超流水线是通过细化流水、提高主频，使得在一个机器周期内完成一个甚至多个操作，其实质是以时间换取空间。

### <font color=#FF4500> 名词解释 </font>

- `perf stat` 用于运行指令，并分析其统计结果。
- `cpu-clock` : 任务真正占用的处理器时间，单位为 `ms`
  - CPUs utilized = task-clock / time elapsed
  > CPU的占用率 = 任务真正的处理时间 / 消耗的时间
- `cpu-cycles` : 消耗的处理器周期数
  - 如果把被 ls 使用的 cpu-cycles 看成是一个处理器的，那么它的主频为 2.486GHz。可以用 cycles / task-clock 算出。
- `Cycles` ：处理器时钟，一条机器指令可能需要多个 cycles
- `instructions` : 执行了多少条指令
- `IPC` (Instructions Per Clock/Cycle) ：是 `Instructions/Cycles` 的比值，该值越大越好，说明程序充分利用了处理器的特性
  - **IPC 为平均每个 cpu-cycle 执行了多少条指令**
  - 在频率一样情况下，越大表示处理能力越强，指令吞吐量就越大。
  - 例如通过多发射和多 fu 并行，可以让现代高性能 CPU 的 IPC 超过 1
- `context-switches` : 程序在运行过程中上下文的切换次数
- `CPU-migrations` : 程序在运行过程中发生的处理器迁移次数。
  - Linux 为了维持多个处理器的负载均衡，在特定条件下会将某个任务从一个 CPU 迁移到另一个 CPU
  - CPU 迁移和上下文切换：发生上下文切换不一定会发生 CPU 迁移，而发生 CPU 迁移时肯定会发生上下文切换。发生上下文切换有可能只是把上下文从当前 CPU 中换出，下一次调度器还是将进程安排在这个 CPU 上执行。
- `page-faults` : 缺页异常的次数
  - 当应用程序请求的页面尚未建立、请求的页面不在内存中，或者请求的页面虽然在内存中，但物理地址和虚拟地址的映射关系尚未建立时，都会触发一次缺页异常
  - 另外 TLB 不命中，页面访问权限不匹配等情况也会触发缺页异常
- `stalled-cycles-frontend` : 指令读取或解码的质量步骤，未能按理想状态发挥并行左右，发生停滞的时钟周期
- `stalled-cycles-backend` : 指令执行步骤，发生停滞的时钟周期
- `branches` : 遇到的分支指令数
  - `branch-misses` : 是预测错误的分支指令数
- `Cache-references` : cache 命中的次数
- `Cache-misses: cache 失效的次数`

### <font color=#FF4500> 使用 </font>

1. `./simpleperf stat -h`
2. `./simpleperf stat ./neonOsdDemo`
3. ``
