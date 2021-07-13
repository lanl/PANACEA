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

    /**
     * How the memory of the entropy terms is handled, this is different
     * from the KernelMemory but informs how KernelMemory should be handled.
     **/
    enum class Memory {
      SelfOwnIfRestartCrossOwn,
      Own,
      Share,
      OwnIfRestart
    };

    // Should attempt to make settings classes specefic to the correct
    // categories so as to avoid allowing values to be passed in that shouldn't
    // be
    enum class CalculationType {
      Numerical,
      Analytical
    };

    enum class EquationSetting {
      None,
      IgnoreExp,
      IgnoreExpAndPrefactor
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
     *
     * OwnIfRestart option is used in the case that an instance of
     * an object is created without all the data. What this means
     * is that if the rest of the data is loaded in from a restart
     * file then the object of course must have ownership because
     * it does not exist in memory anywhere else. In the case that
     * the data is simply passed in after the object is created
     * then the memory will be treated as shared.
     **/
    enum class KernelMemory {
      Default,
      Own,
      OwnIfRestart,
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
            return "KernelMemory=Default";
          } else if( setting == KernelMemory::Own ) {
            return "KernelMemory=Own";
          } else if( setting == KernelMemory::Share ) {
            return "KernelMemory=Share";
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
          } else if(setting == KernelPrimitive::GaussianLog) {
            return "Primitive=GaussianLog";
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
        } else if constexpr( std::is_same<RandomizeDimensions,T>::value ) {
          if( setting == RandomizeDimensions::Yes ) {
            return "RandomizeDimensions=Yes";
          }else if( setting == RandomizeDimensions::No ) {
            return "RandomizeDimensions=No";
          }
        } else if constexpr( std::is_same<RandomizeNumberDimensions,T>::value ) {
          if( setting == RandomizeNumberDimensions::Yes ) {
            return "RandomizeNumberDimensions=Yes";
          }else if( setting == RandomizeNumberDimensions::No ) {
            return "RandomizeNumberDimensions=No";
          }
        } else if constexpr( std::is_same<EntropyOption,T>::value ) {
          if( setting == EntropyOption::Weight ) {
            return "EntropyOption=Weight";
          }else if( setting == EntropyOption::IncrementRatio ) {
            return "EntropyOption=IncrementRatio";
          }else if( setting == EntropyOption::NumericalGrad ) {
            return "EntropyOption=NumericalGrad";
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

  std::istream& operator>>(std::istream& is, settings::CalculationType & );
  std::istream& operator>>(std::istream& is, settings::EquationSetting & );
  std::istream& operator>>(std::istream& is, settings::None         & );
  std::istream& operator>>(std::istream& is, settings::GradSetting  &);
  std::istream& operator>>(std::istream& is, settings::KernelAlgorithm & );
  std::istream& operator>>(std::istream& is, settings::KernelMemory &);


}
#endif // PANACEA_PRIVATE_SETTINGS_H
