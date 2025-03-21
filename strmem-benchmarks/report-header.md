# RISE QEMU function benchmarks
These are hand-written versions of common memory and string library functions
provided by SiFive Inc.  For each benchmarked function, we are comparing a
"baseline" QEMU against a "latest" QEMU.  By default, we look at three
versions of the code.

1. the standard library "scalar" implementation of the function
2. the hand-written vector code for VLEN=128 and LMUL=1 ("small vector")
3. the hand-written vector code for VLEN=1024 and LMUL=8 ("large vector")

We thus have a total of 6 datasets.

There are four graphs.  The first looks at the average number of instructions
executed per iteration, and is a sanity check.  Although we plot all 6
datasets, only 3 should show on the graph - the "latest" versions of "scalar",
"small vector" and "large vector", since the version of QEMU should have no
impact on the number of instructions being executed.  This graph is useful to
developers of the vector implementations to see how their code behaves for
different sizes of data, but this is outside the scope of this project.

The remaining three graphs capture QEMU performance for each of the three
versions of the code.  They use the metric of nanoseconds per instruction, to
measure the efficiency of QEMU.  Each graph shows this metric, plotted against
problem size, one line for the "baseline" version of QEMU, the other the
"latest" QEMU.

This report is generated entirely automatically using the command

```
./run-all-benchmarks.py
```

This takes less than 20 minutes to run on a 40 thread AMD Threadripper 1950X
at 3.4GHz.  The code is in the `strmem-benchmarks` directory of the
[rise-rvv-tcg-qemu-tooling](https://github.com/embecosm/rise-rvv-tcg-qemu-tooling)
repository.  It is intended to be portable, but to date has only been tested
on Ubuntu 22.04 LTS.

## Document details
