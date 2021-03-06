LA-UR-21-24386

# PANACEA (Portable Approach for Novel Arrangements using a Cross Entropy Algorithm)

This is the repository for PANACEA. PANACEA provides a means for generating
unique configurations. The primary motivation for it's development was for
generating unique configurations for the purpose of training machine learned
interatomic potentials.

Under the hood PANACEA makes use of information entropy to drive uniqueness or
similarity.

# The Directory structure of this repository is layed out as

* include - public interface, where the header files are stored
* external - external and third party libraries used to build panacea
* src - where the source code is located
* tst - where the unit and integration tests are located

# Dependencies

Third party dependencies auto downloaded and used by PANACEA when built

* [Eigen - matrix opertions](https://eigen.tuxfamily.org/index.php?title=Main_Page)
* [Catch2 - for unit and integration testing](https://github.com/catchorg/Catch2)

# Compiling & Installing

PANACEA can be built and installed with CMake relatively easy.

```bash
cd PANACEA
cmake -S. -B build
cmake --build build --parallel
cmake --build build --target test
cmake --build build --target install
```

NOTE: By default python scripts are added to the test infrastructure, if you do
not want to build with python tests the ENABLE_PYTHON_TESTS can be set to OFF
when running the CMake configuration step. E.g. 

```bash
cmake -S. -B build -DENABLE_PYTHON_TESTS=OFF
```

Unit and integration tests can be run with:

```bash
cd build
ctest
```
