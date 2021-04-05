
/*
 * Class is the main interface with the panacea functionality
 */

// Local private headers
#include "type_register.hpp"
#include "kernels/kernel_wrapper_factory.hpp"

namespace panacea {

  class PANACEA {


    public:
      // Method for registering all the static types,
      // to initialize the type registry all of the types need to be provided
      // together
      template<class... Ts>
        bool Register() {

//          TypeRegister<Ts> type_register_();
//          kern_factory<T>::registerType();
          return true;
        }

    private:

//      TypeRegister type_register_;
      KernelWrapperFactory kern_factory_;
  };

}
