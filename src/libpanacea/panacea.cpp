
/*
 * Class is the main interface with the panacea functionality
 */

// Local private headers
#include "type_register.hpp"
#include "kernels/kernel_wrapper_factor.hpp"

namespace panacea {

  class PANACEA {


    public:
      // Method for registering all the static types
      template<class T>
        bool Register<T>() {

          TypeRegister::registerType();
          kern_factory<T>::registerType();
        }

    private:

      TypeRegister type_register_;
      KernelWrapperFactory kern_factory_;
  };

}
