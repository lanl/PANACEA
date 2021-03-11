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
  }
}

#endif // PANACEA_PRIVATE_SETTINGS_H
