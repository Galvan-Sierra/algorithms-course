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

## ⚙️ Generar Wrapper

Ejecuta el siguiente comando para generar los archivos de interfaz entre C++ y Python:

```bash
swig -c++ -python HMMmethodsDynamic.i
```

Esto generará:

- `HMMmethodsDynamic_wrap.cxx`
- `HMMmethodsDynamic.py`

---

## 🛠️ Compilar Wrapper

Compila el archivo generado por SWIG:

```bash
g++ -O2 -fPIC -c HMMmethodsDynamic_wrap.cxx -I/usr/include/python3.10
```

---

## 📚 Crear Biblioteca Compartida

Crea la librería compartida que Python podrá importar como módulo:

```bash
g++ -shared HMMmethodsDynamic_wrap.o -o _HMMmethodsDynamic.so
```

---

## 🧪 Probar en Python

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
- Si planeas distribuir la librería, considera automatizar la compilación con un `Makefile` o `setup.py`.
