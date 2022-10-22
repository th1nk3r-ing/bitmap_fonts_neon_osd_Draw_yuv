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
> 7. [Linux内核性能剖析的方法学和主要工具](https://www.sohu.com/a/575894549_121124374)

### <font color=#FF4500> CPU 基础知识 </font>

1. 流水线 : 是一种将每条指令分解为多步，并让各步操作重叠，从而实现几条指令并行处理的技术。
   1. 程序中的指令仍是一条条顺序执行，但可以预先取若干条指令，并在当前指令尚未执行完时，提前启动后续指令的另一些操作步骤。这样显然可加速一段程序的运行过程。
2. 超级流水线 : 以增加流水线级数的方法来缩短机器周期，相同的时间内超级流水线执行了更多的机器指令。
   1. 采用简单指令以加快执行速度是所有流水线的共同特点，但超级流水线配置了多个功能部件和指令译码电路，采用多条流水线并行处理，还有多个寄存器端口和总线，可以同时执行多个操作，因此比普通流水线执行的更快，在一个机器周期内可以流出多条指令。
3. 超标量（superscalar）: 是指在 CPU 中有一条以上的流水线，并且每时钟周期内可以完成一条以上的指令，这种设计就叫超标量技术。
   1. 其实质是以空间换取时间。
   2. 而超流水线是通过细化流水、提高主频，使得在一个机器周期内完成一个甚至多个操作，其实质是以时间换取空间。
4. IPS: 每秒钟所能执行的指令条数，对于微型计算机可用 CPU 的主频和每条指令的执行所需的时钟周期来衡量
5. MIPS: 衡量 ARM 及其他的 cpu 的性能，表示每秒能运行多少个百万指令，MIPS 越高，性能越高
6. 频率: 表示一秒振荡多少个周期 (MHz 时钟速度)
7. 时钟频率又称主频 `f`，它是指 CPU 内部晶振的频率，常用单位为 MHz，它反映了 CPU 的基本工作节拍
8. 时钟周期 `t` : `t =1/f`, 主频的倒数
9. 机器周期 : `m*t`, 一个机器周期包含若干个（`m` 个）时钟周期
10. 指令周期：`mtn` 执行一条指令所需要的时间，一般包含若干个（`n` 个）机器周期
11. IPC（instruction per clock) : 表示每（时钟）周期运行多少个指令
12. CPI（clock per instruction）: 平均每条指令的平均时钟周期个数
13. 准确的 CPU 性能判断标准应该是： `MIPS = 频率 x IPC`
14. Ahmand Yasin 在它的 IEEE 论文《A top-down method for performance analysis and counter architercture》中，革命性地给出了一个从 CPU 指令执行的顺畅程度来评估和发现瓶颈的方法，允许我们从黑盒的角度以诸葛孔明“隆中对”式的格局来看问题。现在处理器，一般在 4 个方面占用流水线的时间，而 top-down 方法，可以黑盒地呈现软件在 CPU 上面运转的时候，CPU 的流水线究竟在干什么。
    1. `Front End Bound`（前端依赖）: 处理器的前端主要完成指令的译码，把获取的指令翻译为一系列的micro-ops（μops）。当 CPU stalled 在 Front End，通常意味着 CPU 在取指慢（比如 icache miss、解释执行等）,或者复杂指令的翻译过程由于 μops cache 不命中等原因而变地漫长。Front End stalled 多，意味着前端无法及时给后端“喂饱” μops。目前主流的 x86 处理器，每个 cycle 可以给 Back End 喂 4 个指令，如果 Back End 也及时执行的话，IPC 最高可达 4.0。
    2. `Back End Bound`（后端依赖）：处理器的后端主要完成前端“喂”过来的 μops 的执行，执行的过程可能涉及读写操作数（load/store）、对操作数进行加减乘除各种运算之类。Back End Bound又可再细分为 2 类，core bound 意味着软件更多依赖于微指令的处理能力；memory bound 意味着软件更加依赖 CPU L1～L3 缓存和 DRAM 内存性能。当 CPU stalled 在 Back End，通常意味着复杂运算指令延迟大，或操作数从 memory（包括 cache 和 DDR）获取的延迟大，导致部分 pipeline slots 为空（stall）。
    3. Retiring：μops 被执行完成，最终的 retire 动作，提交结果到寄存器或者内存。
    4. Bad Speculation：处理器虽然在干活，但是投机执行的指令可能没有用。比如分支本身应该进 `else` 的，预测的结果却进了 `if` 执行错误的分支，虽然没有 stall，但是这些错误分支里面的指令实际白白执行了不会 retire，所以也浪费了 pineline 的时间。
       - 这里有一个基本常识，比如 `if(a) do x; else do y;` ，处理器并不是等待 a 的判决结果后，再去做 x 或者 y，而是先根据历史情况投机执行 x 或者 y，当然这个投机有可能出错。
15. `wall-time` 字面意思是挂钟时间，实际上就是指的是现实的时间.
    > - perf provide two time based profilers cpu-clock and task-clock;
    > - cpu-clock is wall-clock based and samples are taken at regular intervals relative to walltime;
    > - task-clock is to sample the specific task run time

### <font color=#FF4500> 名词解释 </font>

- `perf stat` 用于运行指令，并分析其统计结果。
- `cpu-clock` : 任务真正占用的处理器时间，单位为 `ms`
  - CPUs utilized CPU的占用率 = task-clock 任务真正的处理时间 / time elapsed 消耗的时间
  - CPU 利用率，该值高，说明程序的多数时间花费在 CPU 计算上而非 IO
  -
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

1. 程序安装 :
   1. `adb push ./simpleperf /data/local/tmp`
   2. `adb shell chmod 777 /data/local/tmp/simpleperf`
2. 统计计算量 :
   1. `./simpleperf stat -h`
   2. `./simpleperf stat ./neonOsdDemo`
   3. `./simpleperf stat --app com.think3rTest.phone  --duration 10`
3. 统计热点函数 :
   1. `./simpleperf record ./neonOsdDemo -o ./pref.data`
   2. `./simpleperf record ./neonOsdDemo -o ./pref.data`
   3. `./simpleperf report -i ./pref.data`

```sh
# 直接用 adb shell 统计某个 app 的计算量
adb shell "cd /data/local/tmp/install/ && ./simpleperf stat --app com.think3rTest.phone  --duration 10"

# 统计 neonOsdDemo 的热点函数
adb shell "cd /data/local/tmp/install/ && ./simpleperf record ./neonOsdDemo -o perf.data && ./simpleperf report -i ./perf.data" | less
```

- TODO : `android-ndk-profiler` 的使用?
