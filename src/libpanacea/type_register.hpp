
// Standard includes
#include <unordered_map>

namespace panacea {


  /**
   * @brief Class is meant to provide a means to convert a std any back to the
   * appropriate supported classes
   *
   * Users, can register the types they want to be able to store data in so
   * they can be handled correctly internally.
   *
   * They can do this by providing functions 
   */
  struct BaseFunctionNode {
    typedef type;
  }

  class TypeRegister {
    private:
      std::unordered_map<type, std::function<>> converters_; 

    public:

  };
}
