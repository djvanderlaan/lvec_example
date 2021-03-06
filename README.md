
An example R-package demonstrating linking against lvec
=======================================================

[`lvec`](https://github.com/djvanderlaan/lvec) is an package that allows one to
work with vectors that are too large to fit into memory. It does that by storing
the data partially on disk. `lvec` contains some basic functionality for
reading, writing and modifying an `lvec` object from R. With this functionality
many of the operations one might like to do can be implemented directly in R.
However, some operations can be performed much more efficiently directly in C++
(the language in which the core of `lvec` is implmented). Thefore, `lvec`
exports the necessary headers and functions, so that dependent methods can work
with `lvec` objects in C++. 

This repository contains an example package showing how a package can work with
an `lvec` object in C++. 




Comparison of different methods to calculate the sum of an lvec
----------------------------------------------------------------

The package implements four different methods for calculating the sum of an
`lvec`. They are described and benchmarked below.

1. Use the `get_of_type` method, to convert each value in the `lvec` to double
   and sum those.
2. Use the visitor pattern to write a template function that handles each type
   of `lvec`. 
3. Assume the `lvec` is of type double; cast the `lvec` to an lvec of type 
   double and throw an error otherwise. 
4. Process the `lvec` in R using chunks.



```r
library(lvec.example)
```

```
## Loading required package: lvec
```

```
## 
## Attaching package: 'lvec'
```

```
## The following object is masked from 'package:base':
## 
##     order
```

```r
library(microbenchmark)

x <- as_lvec(rnorm(5E6))

microbenchmark(
  sum1(x),
  sum2(x),
  sum3(x),
  sum4(x), times = 25
)
```

```
## Unit: milliseconds
##     expr      min       lq     mean   median       uq       max neval
##  sum1(x) 79.21820 80.47602 81.24433 80.98998 81.31773  85.83961    25
##  sum2(x) 69.29169 69.75927 71.32489 70.02078 71.37561  83.98621    25
##  sum3(x) 40.99274 41.83947 42.18954 42.18168 42.29850  44.38764    25
##  sum4(x) 68.03154 80.48746 90.58930 83.48700 89.79656 137.53671    25
```

Comparison of different methode to calculate the square of an lvec
------------------------------------------------------------------

The package contains two methods to calculate the square of an lvec. This 
demonstrates how one can generate and return a new `lvec`. 

1. An implementation in c++ using the visitor pattern. 
2. An implementation in R using chunks. 



```r
microbenchmark(
  square(x),
  square_r(x), times = 25
)
```

```
## Unit: milliseconds
##         expr      min        lq     mean    median        uq       max
##    square(x)  80.6328  88.41677 109.6887  90.66791  96.99517  475.9629
##  square_r(x) 530.1446 633.28274 681.9261 647.75698 659.15259 1220.0663
##  neval
##     25
##     25
```
