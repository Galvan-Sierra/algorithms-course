# HMMmethodsDynamic - Python Wrapper con SWIG

Este proyecto genera un **wrapper en Python** para la librería `HMMmethodsDynamic` escrita en C++, utilizando [SWIG](http://www.swig.org/).

---

## 📦 Requisitos

- Python 3.10+
- g++
- SWIG

En Ubuntu/Debian puedes instalarlos con:

```bash
sudo apt update
sudo apt install swig g++ python3-dev
```

---

## ⚙️ 1. Generar Wrapper con SWIG

Ejecuta el siguiente comando para generar los archivos de interfaz entre C++ y Python:

```bash
swig -c++ -python HMMmethodsDynamic.i
```

Esto generará:

- `HMMmethodsDynamic_wrap.cxx`
- `HMMmethodsDynamic.py`

---

## 🛠️ 2. Compilar tu código fuente en C++

Compila la implementación principal de tu librería:

```bash
g++ -O2 -fPIC -c HMMmethods.cpp -std=c++11
```

---

## 🛠️ 3. Compilar el Wrapper generado por SWIG

Compila el archivo de _wrapper_ generado:

```bash
g++ -O2 -fPIC -c HMMmethodsDynamic_wrap.cxx -I/usr/include/python3.10 -std=c++11
```

---

## 📚 4. Enlazar ambos objetos en la Biblioteca Compartida

Crea la librería compartida que Python podrá importar como módulo:

```bash
g++ -shared HMMmethods.o HMMmethodsDynamic_wrap.o -o _HMMmethodsDynamic.so
```

---

## 🧪 5. Probar en Python

Ejecuta el script de prueba:

```bash
python3 test_hmm.py
```

Si todo está correcto, podrás importar y usar el módulo:

```python
import HMMmethodsDynamic
```

---

## 🚀 Notas

- Asegúrate de que la versión de Python usada en la compilación coincide con la de ejecución (`python3-config --includes` puede ayudarte a obtener el path correcto).
- Usa `-std=c++11` (o superior) si tu código requiere características modernas de C++.
- Si planeas distribuir la librería, considera automatizar la compilación con un `Makefile` o `setup.py`.

```

👉 ¿Quieres que te arme también el **Makefile** para que solo corras `make` y se construya todo automáticamente?
```

```

```
