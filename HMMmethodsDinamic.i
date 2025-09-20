
%module HMMmethodsDinamic

%{
#include "HMMmethods.h"
%}

%include "std_vector.i"
%include "std_string.i"
%include "std_map.i"
%include "exception.i"

// Manejo de excepciones
%exception {
    try {
        $action
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}

// Templates para los tipos que se usan
%template(StringVector) std::vector<std::string>;
%template(DoubleVector) std::vector<double>;
%template(RegionVector) std::vector<Region>;
%template(StringDoubleMap) std::map<std::string, double>;
%template(StringStringDoubleMap) std::map<std::string, std::map<std::string, double>>;

%include "HMMmethods.h"