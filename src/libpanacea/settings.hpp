#ifndef PANACEA_PRIVATE_SETTINGS_H
#define PANACEA_PRIVATE_SETTINGS_H
#pragma once

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

    // Whether we are taking the gradient with respect to the descriptor or the kernel
    enum class GradSetting {
      WRTKernel,
      WRTDescriptor
    };

    enum class KernelPrimitive {
      Gaussian,
      Exponential    
    };

    /**
     * How the descriptors are represented by the kernel
     * Single - a single kernel is used to represent all the descriptors
     * OneToOne - each descriptor has it's own kernel
     * Fixed - there are a fixed number of kernels
     **/
    enum class KernelCount {
      Single,
      OneToOne,
      Fixed
    };

    enum class KernelCorrelation {
      Correlated,
      Uncorrelated
    };

    enum class KernelNormalization {
      None,
      Variance
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

    enum class KernelCenterCalculation {
      None,
      Mean,
      Median
    };
  }
}

#endif // PANACEA_PRIVATE_SETTINGS_H
