#ifndef PANACEA_PRIVATE_BASEDISTRIBUTION_H
#define PANACEA_PRIVATE_BASEDISTRIBUTION_H
#pragma once

namespace panacea {

  class BaseDescriptorWrapper;

  class BaseDistribution {
    virtual double compute(const BaseDescriptorWrapper * descriptor_wrapper, const int desc_ind) = 0;
    virtual std::vector<double> compute_grad(
        const BaseDescriptorWrapper * descriptor_wrapper,
        const int desc_ind,
        const std::vector<settings::EquationSetting> & prim_settings, 
        const settings::GradSetting & grad_setting) = 0;

    virtual ~BaseDistribution() = 0;
  };
}

#endif // PANACEA_PRIVATE_BASEDISTRIBUTION_H

