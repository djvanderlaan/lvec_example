
#' Demonstration functions for calculating the sum of an lvec
#'
#' @param x an lvec.
#'
#' @details
#' The c++ code behind these functions demonstrate three different methods of 
#' calculating the sum of an lvec object in C++ and one in pure R.
#' 1. Use the get_of_type method of the ldat::vec class to get elements of the
#'    lvec object as doubles. Sum these. 
#' 2. Use the visitor ldat::lvec_visitor class to construct a visitor that is
#'    passes to the ldat::vec object. This allows one to write separate methods
#'    for each type of lvec.
#' 3. Assume that the input is a numeric lvec. Cast the vec to lvec<double> and
#'    calculate the sum of that. In case one is sure that the lvec is of a
#'    certain type, this is probably the simplest method.
#' 4. This function is completed completely in R showing how one can process
#'    data in chunks. If this is possible, this will often give a similar 
#'    performance as working in C++. 
#'
#' @useDynLib lvec.example
#' @rdname sum
#' @import lvec
#' @export
sum1 <- function(x) {
  if (!is_lvec(x)) stop("x is not an lvec")
  requireNamespace("lvec")
  .Call("sum1", PACKAGE = "lvec.example", x)
}

#' @useDynLib lvec.example
#' @rdname sum
#' @export
sum2 <- function(x) {
  if (!is_lvec(x)) stop("x is not an lvec")
  requireNamespace("lvec")
  .Call("sum2", PACKAGE = "lvec.example", x)
}

#' @useDynLib lvec.example
#' @rdname sum
#' @export
sum3 <- function(x) {
  if (!is_lvec(x)) stop("x is not an lvec")
  if (!lvec_type(x) == "numeric") stop("x is not a numeric lvec")
  requireNamespace("lvec")
  .Call("sum3", PACKAGE = "lvec.example", x)
}


#' @rdname sum
#' @export
sum4 <- function(x) {
  if (!is_lvec(x)) stop("x is not an lvec")
  chunks <- chunk(x)
  s <- 0
  for (c in chunks) {
    # read in a chunk of data and convert to R object
    d <- as_rvec(lget(x, range = c))
    # process chunk
    s <- s + sum(d)
  }
  s
}

