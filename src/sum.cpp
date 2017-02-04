#include <lvec_interface.h>

// This file demonstrates three different methods of calculating the sum of an
// lvec object in C++. 
// 1. Use the get_of_type method of the ldat::vec class to get elements of the
//    lvec object as doubles. Sum these. 
// 2. Use the visitor ldat::lvec_visitor class to construct a visitor that is
//    passes to the ldat::vec object. This allows one to write separate methods
//    for each type of lvec.
// 3. Assume that the input is a numeric lvec. Cast the vec to lvec<double> and
//    calculate the sum of that. In case one is sure that the lvec is of a
//    certain type, this is probably the simplest method.


// ============================ SUM1 ===========================================
// Use the get_of_type method of the vec class. Each lvec object implements
// three different versions of this method:
// 1. double get_of_type(vecsize i, double type)
// 2. int get_of_type(vecsize i, int type)
// 3. std::string get_of_type(vecsize i, const std::string& type)
// These methods cast element i of the vector to the given type and return the
// result. In this case all values are converted to double. These values are
// summed up and returned.

extern "C" {
  SEXP sum1(SEXP rv) {
    CPPRTRY
    ldat::vec* v = sexp_to_vec(rv);
    double sum = 0.0;
    for (ldat::vec::vecsize i = 0; i < v->size(); ++i) {
      double val = v->get_of_type(i, sum);
      if (cppr::is_na(val)) {
        sum = cppr::na<double>();
        break;
      }
      sum += val;
    }
    cppr::rvec<cppr::numeric> res{1};
    res[0] = sum;
    return res.sexp();
    CPPRCATCH
  }
}


// ============================ SUM2 ===========================================
// Use an lvec_visitor object to calculate the sum of an lvec. 
// First define a subclass of lvec_visitor. This subclass should implement a
// visit method for each of the possible types of lvec (double, int, std::string
// and cppr::logical). This allows one to write separate methods for each lvec
// type. In this case the implementation for each type is the same, except for
// the type of the elements of the lvec. Therefore, a template (visit_template)
// is used. This template is called from each of the visit methods. 
//
// The visitor solution results in more code, but most of that can be
// copy-pasted and it allows for more flexibility than the other two methods of
// calculating the sum.

class sum_visitor : public ldat::lvec_visitor {
  public:
    sum_visitor() : sum_(0.0) {}

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i) {
        double val = cppr::cast_value<double>(vec.get(i));
        if (cppr::is_na(val)) {
          sum_ = cppr::na<double>();
          break;
        }
        sum_ += val;
      }
    }

    void visit(ldat::lvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<int>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<std::string>& vec) {
      throw std::runtime_error("Sum is not defined for character lvecs");
    }

    SEXP result() const {
      cppr::rvec<cppr::numeric> res{1};
      res[0] = sum_;
      return res.sexp();
    }

  private:
    double sum_;
};

extern "C" {
  SEXP sum2(SEXP rv) {
    CPPRTRY
    ldat::vec* v = sexp_to_vec(rv);
    sum_visitor visitor;
    v->visit(&visitor);
    return visitor.result();
    CPPRCATCH
  }
}


// ============================ SUM3 ===========================================
// Cast the vec to an lvec<double> using a dynamic_cast. Check if the cast is
// succesful and if so calculate the result. 

extern "C" {
  SEXP sum3(SEXP rv) {
    CPPRTRY
    ldat::vec* tmp = sexp_to_vec(rv);
    ldat::lvec<double>* v = dynamic_cast<ldat::lvec<double>*>(tmp);
    if (v == 0) 
      throw std::runtime_error("Could lvec is not of type numeric.");

    double sum = 0.0;
    for (ldat::vec::vecsize i = 0; i < v->size(); ++i) {
      double val = v->get(i);
      if (cppr::is_na(val)) {
        sum = cppr::na<double>();
        break;
      }
      sum += val;
    }
    cppr::rvec<cppr::numeric> res{1};
    res[0] = sum;
    return res.sexp();
    CPPRCATCH
  }
}

