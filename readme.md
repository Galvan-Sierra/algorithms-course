# Generar wrapper

swig -c++ -python HMMmethodsDinamic.i

# Compilar wrapper

g++ -O2 -fPIC -c HMMmethodsDinamic_wrap.cxx -I/usr/include/python3.10

# Crear biblioteca compartida

g++ -shared HMMmethodsDinamic_wrap.o -o \_HMMmethodsDinamic.so

# test python3

test_hmm.py
