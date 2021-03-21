
#ifndef PANACEA_PRIVATE_MEMORY_H
#define PANACEA_PRIVATE_MEMORY_H
#pragma once

// Local private PANACEA includes
#include "error.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace panacea {

  class BaseMemoryNode {
    protected:
      const bool own_ = false;
      std::string name_;
    public:
      BaseMemoryNode(const bool own, const std::string & name = "") : own_(own), name_(name) {};
      virtual ~BaseMemoryNode() = default;
      const bool own() const noexcept { return own_; }
      virtual std::any getRawDataPointer() const noexcept = 0;
      const std::string & getName() const noexcept { return name_; };
  };

  template<class T >
  class MemoryNode : public BaseMemoryNode {
    private:
      T * data_ptr_;
    public:
      MemoryNode(T * data, const bool own = false, const std::string & name = "") : data_ptr_(data), BaseMemoryNode(own, name) {};
      std::any getRawDataPointer() const noexcept final {
        std::cout << "Returning pointer 1" << std::endl; 
        return data_ptr_; }
  };

  template<class T >
  class MemoryNodeUnique : public BaseMemoryNode {
    private:
      std::unique_ptr<T> data_ptr_;
    public:
      MemoryNodeUnique(std::unique_ptr<T> data, const bool own = false, const std::string & name = "") : data_ptr_(std::move(data)), BaseMemoryNode(own, name) {};
      std::any getRawDataPointer() const noexcept final {
        std::cout << "Returning pointer 1" << std::endl; 
        return data_ptr_.get(); }
  };

  /*
   * Class is responsible for managing the memory of objects used in panacea, particulalry
   * objects that might be passed around between several objects. 
   *
   * It also keeps track of whether the objects memory is actually mananaged by panacea or
   * externally
   */
  class MemoryManager {
    // Each of the below should be specefic to a distribution 
    // Covariance Matrix
    // Normalization Coefficients
    // Normalized Covariance Matrix
    // Reduced Covariance Matrix
    // Pointer to descriptors, with bool if memory is managed by panacea
    // Pointer to kernels, with bool if memory is managed by panacea
 
    private: 
      std::vector<std::unique_ptr<BaseMemoryNode>> memories_;
      std::unordered_map<std::string, int> name_to_index_;
    public:

      size_t size() const noexcept { return memories_.size(); }

      /// Here it is assumed the data in the node is not owned by the memory manager
      /// returns the id in the vector
      template<class T> 
        size_t managePointer(T & data, const std::string & name = "");

      template<class T> 
        size_t managePointer(T * data, const std::string & name = "");

      /// The data passed in will now be owned by the memory manager
      /// returns the id in the vector
      /// This is actually very unsame, a reference does not indicated ownership
      /// of the object, if we are are going to create a Node and assume ownership
      /// it has to aready be in a unique pointer
      /// E.g. it is non-trivial to convert a nested array of heap allocated pointers
      /// to a unique pointer, or what if an object on the stack is passed in
      template<class T> 
        size_t manageMemory(T & data, const std::string & name = "");

      template<class T> 
        size_t manageMemory(std::unique_ptr<T> && data, const std::string & name = "");

        BaseMemoryNode * getMemory(const int index) {
          assert(index < memories_.size() && index >= 0 );
          return memories_.at(index).get();
        }

         BaseMemoryNode * getMemory(const std::string & name) {
          if(name_to_index_.count(name) == 0) {
            PANACEA_FAIL("Cannot getMemory name of memory is not known.");
          }
          return memories_.at(name_to_index_[name]).get();
        }

      template<class T>
        T  getRawPointer(int index); 

      template<class T>
        T  getRawPointer(const std::string & name);

    };

  /// Here it is assumed the data in the node is not owned by the memory manager
      /// returns the id in the vector
      template<class T> 
      inline  size_t MemoryManager::managePointer(T & data, const std::string & name){

            if(name_to_index_.count(name)) {
              PANACEA_FAIL("Cannot addNode name of memory has already been assigned.");
            }

            T * data_ptr = &data;
            memories_.push_back(std::make_unique<MemoryNode<T>>(MemoryNode<T>(data_ptr,false,name)));
            name_to_index_[name] = memories_.size() - 1;
          return memories_.size() - 1;
        }

      template<class T> 
       inline size_t MemoryManager::managePointer(T * data, const std::string & name){

          if(name_to_index_.count(name)) {
            PANACEA_FAIL("Cannot addNode name of memory has already been assigned.");
          }
          memories_.push_back(std::make_unique<MemoryNode<T>>(data,false,name));
          name_to_index_[name] = memories_.size() - 1;
          return memories_.size() - 1;
        }

      /// The data passed in will now be owned by the memory manager
      /// returns the id in the vector
      /// This is actually very unsame, a reference does not indicated ownership
      /// of the object, if we are are going to create a Node and assume ownership
      /// it has to aready be in a unique pointer
      /// E.g. it is non-trivial to convert a nested array of heap allocated pointers
      /// to a unique pointer, or what if an object on the stack is passed in
//      template<class T> 
//    inline    size_t MemoryManager::(T & data, const std::string & name){
//          PANACEA_FAIL("Cannot use createNode with data that is already stored in a unique_ptr, consider using addNode instead.");
//          return -1;
//        }

      template<class T> 
     inline   size_t MemoryManager::manageMemory(std::unique_ptr<T> && data, const std::string & name){
if(name_to_index_.count(name)) {
            PANACEA_FAIL("Cannot createNode name of memory has already been assigned.");
          }
          auto mem_node = MemoryNodeUnique<T>(std::move(data),true,name);
          memories_.push_back(std::make_unique<MemoryNodeUnique<T>>(std::move(mem_node)));
          name_to_index_[name] = memories_.size() - 1;
          return memories_.size() - 1;
        }

      template<class T>
       inline T  MemoryManager::getRawPointer(int index) {
          assert(index < memories_.size() );
          return std::any_cast<T>(memories_.at(index)->getRawDataPointer()); 
        }

      template<class T>
      inline  T  MemoryManager::getRawPointer(const std::string & name) {
          if(name_to_index_.count(name) == 0) {
            PANACEA_FAIL("Cannot getRawPointer name of memory is not known.");
          }
          return std::any_cast<T>(memories_.at(name_to_index_[name])->getRawDataPointer()); 
        }

}
#endif // PANACEA_PRIVATE_MEMORY_H
