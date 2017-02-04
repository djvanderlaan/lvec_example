
#' Demonstration functions for calculating the square of an lvec
#'
#' @param x an lvec.
#'
#' @details
#' This file demonstrates how a new lvec can be calculated from an other lvec.
#' In this case each of the elements in the input are squared. The resulting
#' lvec has the same type as the input lvec. 
#'
#' First define a subclass of lvec_visitor. This subclass should implement a
#' visit method for each of the possible types of lvec (double, int, std::string
#' and cppr::logical). This allows one to write separate methods for each lvec
#' type. In this case the implementations for double and int are the same,
#' except for the type of the elements of the lvec. Therefore, a template
#' (visit_template) is used. This template is called from the visit methods. For
#' the lvec<std::string> it is not possible to define the square. This method
#' will therefore generate an error. For the lvec<cppr:logical> the square of
#' the input is the same as the input. Therefore, a clone of the input is
#' returned. 
#' 
#' There is also an implementation of the same function in pure R
#' (\code{square_r}). Many operations on lvecs can also be implemented by 
#' processing an lvec in chunks. This will often give a performance comparable 
#' to working directly in c++ (in this case the performance is better in C++).
#'
#' @useDynLib lvec.example
#' @import lvec
#' @rdname square
#' @export
square <- function(x) {
  if (!is_lvec(x)) stop("x is not an lvec")
  requireNamespace("lvec")
  res <- .Call("square", PACKAGE = "lvec.example", x)
  structure(res, class = "lvec")
}



#' @rdname square
#' @export
square_r <- function(x) {
  if (!is_lvec(x)) stop("x is not an lvec")
  # create result vector
  res <- lvec(length(x), type = lvec_type(x))
  chunks <- chunk(x)
  for (c in chunks) {
    d <- as_rvec(lget(x, range = c))
    # there is unfortunately not yet an range option to lset, so convert range
    # to indices
    i <- seq.int(c[1], c[2])
    # calculate result and assign to res
    lset(res, i, d^2)
  }
  res
}

