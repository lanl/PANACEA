#ifndef PANACEA_PRIVATE_SETTINGS_H
#define PANACEA_PRIVATE_SETTINGS_H
#pragma once

// Public PANACEA settings
#include "panacea/settings.hpp"

// Standard includes
#include <fstream>
#include <type_traits>

/*
 * Simple class in charge of the settings used with panacea
 * this class should only store enums and or bool types
 */
namespace panacea {

  namespace settings {

    // Should attempt to make settings classes specefic to the correct 
    // categories so as to avoid allowing values to be passed in that shouldn't
    // be 
    enum class CalculationType {
      Numerical,
      Analytical
    };

    enum class EquationSetting {
      None,
      IgnoreExp
    };

    enum class None {
      None
    };

    // Whether we are taking the gradient with respect to the descriptor or the kernel
    enum class GradSetting {
      WRTBoth,
      WRTKernel,
      WRTDescriptor
    };

    enum class KernelAlgorithm {
      Strict,   
      Flexible  
    };

    /**
     * Specifies if the kernel needs to actually create the memory
     * or not. This is option is not always going to be viable. 
     *
     * For instance if the KernelCount is set to 0neToOne then 
     * either option is viable we could recreate all the descriptor
     * points in memory or use the passed in descriptor object as
     * the centers of the kernel.
     *
     * However, in the case that you have specified a single kernel
     * but have passed in multiple descriptors, you cannot share
     * the descriptors raw pointer because you need different 
     * data.
     **/
    enum class KernelMemory {
      Default,
      Own,
      Share
    };


    template<class T>
      constexpr const char * toString(T setting) {
        if constexpr( std::is_same<KernelCenterCalculation,T>::value ) {
          if( setting == KernelCenterCalculation::None ) {
            return "Center-Calculation=None";
          }else if(setting == KernelCenterCalculation::Mean ) {
            return "Center-Calculation=Mean";
          }else if(setting == KernelCenterCalculation::Median ) {
            return "Center-Calculation=Median";
          }
        } else if constexpr( std::is_same<KernelMemory,T>::value ) {
          if( setting == KernelMemory::Default ) {
            return "Memory=Default";
          } else if( setting == KernelMemory::Own ) {
            return "Memory=Own";
          } else if( setting == KernelMemory::Share ) {
            return "Memory=Share";
          } 
        } else if constexpr( std::is_same<KernelNormalization,T>::value ) {
          if( setting == KernelNormalization::None ) {
            return "Normalization=None";
          } else if (setting == KernelNormalization::Variance ) {
            return "Normalization=Variance";
          }
        } else if constexpr( std::is_same<KernelCorrelation,T>::value ) {
          if( setting == KernelCorrelation::Uncorrelated ) {
            return "Correlation=Uncorrelated";
          } else if (setting == KernelCorrelation::Correlated ) {
            return "Correlation=Correlated";
          }
        } else if constexpr( std::is_same<KernelCount,T>::value ) {
          if( setting == KernelCount::OneToOne ) {
            return "Count=OneToOne";
          } else if(setting == KernelCount::Single) {
            return "Count=Single";
          } else if(setting == KernelCount::Fixed) {
            return "Count=Fixed";
          }
        } else if constexpr( std::is_same<KernelPrimitive,T>::value ) {
          if( setting == KernelPrimitive::Gaussian ) {
            return "Primitive=Gaussian";
          } else if(setting == KernelPrimitive::Exponential) {
            return "Primitive=Exponential";
          }
        } else if constexpr( std::is_same<KernelAlgorithm,T>::value ) {
          if( setting == KernelAlgorithm::Strict ) {
            return "Algorithm=Strict";
          } else if(setting == KernelAlgorithm::Flexible) {
            return "Primitive=Flexible";
          }
        } else if constexpr( std::is_same<CalculationType,T>::value ) {
          if( setting == CalculationType::Numerical ) {
            return "CalculationType=Numerical";
          }else if( setting == CalculationType::Analytical ) {
            return "CalculationType=Analytical";
          }
        }
        return "";
      }
  } // namespace settings

  std::ostream& operator<<(std::ostream& os, const settings::CalculationType & );
  std::ostream& operator<<(std::ostream& os, const settings::EquationSetting & );
  std::ostream& operator<<(std::ostream& os, const settings::None         & );
  std::ostream& operator<<(std::ostream& os, const settings::GradSetting  &); 
  std::ostream& operator<<(std::ostream& os, const settings::KernelAlgorithm & );
  std::ostream& operator<<(std::ostream& os, const settings::KernelMemory &); 

}
#endif // PANACEA_PRIVATE_SETTINGS_H
