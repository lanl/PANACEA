
// Standard includes
#include <any>
#include <tuple>
#include <unordered_map>

namespace panacea {
/*
  class BaseTypeNode {
    BaseTypeNode * next = nullptr;
    
  };

  template<class T> 
  class TypeNode : BaseTypeNode {
    using type = T;
  }*/

/*
  template<typename ...Ts>
  class TypeRegister {

    private:
      std::tuple<Ts...> types_;
      template<class T, unsigned int N>
        T returnType(const std::any val){
          if( val.type() == typeid(std::tuple_element<N, std::tuple<Args...>>::type)){
            return std::any_cast<std::tuple_element<N, std::tuple<Args...>>::type>(val);
          }
          return returnType<T,N-1>(val);
        }

      template<class T, 1>
        T returnType(const std::any val){
          return std::any_cast<
            std::tuple_element<N, std::tuple<Args...>>::type 
            >(val); 
        }


    public:
 //     template<class T>
  //    TypeRegister() {};

      // This is problematic because the function signature is the same
      template<class T>
        T convert(const std::any val){
          return returnType<T,
                 std::tuple_size<decltype(types_)>::value
                 >(val);
        }
      

  };
*/
}
