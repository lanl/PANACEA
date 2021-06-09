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

* Eigen - matrix opertions
* Catch2 - for unit and integration testing

# Compiling & Installing

PANACEA can be built and installed with CMake relatively easy.

```bash
cd PANACEA
cmake -S. -B build
cmake --build build --parallel
cmake --build build --target test
cmake --build build --target install
```

Unit and integration tests can be run with:

```bash
cd build
ctest
```
