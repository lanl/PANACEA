# Mean Vs Median

In this example, we show how using different kernel center calucaltions are able to
track a distribution and which settings to use.

One problem you might encounter is a situation where descriptors tend to clump
but are not necessarily centered around the clump e.g. they might have a long
non symmetric tail. In such a case, using the mean of a single kernel will be
ineffective at representing the distribution because it will likely place the
kernel off center from the clump of points.

# Building the Example

To building the example requires that panacea has been installed.

```bash
$ cd examples/examlple1
$ cmake -S. -B build
$ cmake --build build
$ cd build
$ ./example1
```

How to plot, this is assuming that you have installed the included
python scripts.

```bash
$ plot_descriptors_correlation.py -rb cross_median_log -re ".txt" -rp . -de ".txt" -dp . -db descriptors -s
```
