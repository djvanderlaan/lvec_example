#include <lvec_interface.h>

// This file demonstrates how a new lvec can be calculated from an other lvec.
// In this case each of the elements in the input are squared. The resulting
// lvec has the same type as the input lvec. 

// First define a subclass of lvec_visitor. This subclass should implement a
// visit method for each of the possible types of lvec (double, int, std::string
// and cppr::logical). This allows one to write separate methods for each lvec
// type. In this case the implementations for double and int are the same,
// except for the type of the elements of the lvec. Therefore, a template
// (visit_template) is used. This template is called from the visit methods. For
// the lvec<std::string> it is not possible to define the square. This method
// will therefore generate an error. For the lvec<cppr:logical> the square of
// the input is the same as the input. Therefore, a clone of the input is
// returned. 
//

class square_visitor : public ldat::lvec_visitor {
  public:
    square_visitor() : result_(0) {}

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      // create result vector: same size and type of vec; we use a unique_ptr
      // should there be an error somewhere in the code, this ensures that
      // the result is properly deleted. 
      std::unique_ptr<ldat::lvec<T> > result(new ldat::lvec<T>(vec.size()));
      // loop over vec and calculate the square
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i) {
        T val = vec.get(i);
        if (cppr::is_na(val)) {
          result->set(i, cppr::na<T>());
        } else {
          result->set(i, val * val);
        }
      }
      // assign the result to the ouput variable; using release ensures that the 
      // unique_ptr does not delete the result when this function exits
      result_ = result.release();
    }

    void visit(ldat::lvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<int>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      // if we assume that TRUE=1 and FALSE=0, the square should just return
      // vec
      result_ = vec.clone();
    }

    void visit(ldat::lvec<std::string>& vec) {
      throw std::runtime_error("Square is not defined for character lvecs");
    }

    SEXP result() const {
      return vec_to_sexp(result_);
    }

  private:
    ldat::vec* result_;
};

extern "C" {
  // square is the function that will be called from R. It has a SEXP as input 
  // which should be the lvec that needs to be squared and it returns a SEXP 
  // with the resulting lvec.
  //
  // The pattern followed in this function can be used in most of the functions
  // that use the visitor pattern to perform calculations on an lvec:
  // - Convert the SEXP to a pointer to a vec
  // - Create an instance of the visitor; passing it any information it will
  //   need. 
  // - Call the visit function of the vec passing it the visitor.
  // - Obtain the result from the visitor and returning that to R
  //
  SEXP square(SEXP rv) {
    CPPRTRY
    ldat::vec* v = sexp_to_vec(rv);
    square_visitor visitor;
    v->visit(&visitor);
    return visitor.result();
    CPPRCATCH
  }
}

