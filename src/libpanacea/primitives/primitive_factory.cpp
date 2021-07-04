
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private includes
#include "primitive_factory.hpp"

#include "attributes/covariance.hpp"
#include "attributes/dimensions.hpp"
#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "attribute_manipulators/dimension_limiter.hpp"
#include "attribute_manipulators/inverter.hpp"
#include "attribute_manipulators/randomizer.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "error.hpp"
#include "gaussian_correlated.hpp"
#include "gaussian_log_correlated.hpp"
#include "gaussian_uncorrelated.hpp"
#include "kernels/kernel_wrapper_factory.hpp"
#include "kernels/kernel_specifications.hpp"
#include "kernels/mean_kernel_wrapper.hpp"
#include "kernels/median_kernel_wrapper.hpp"
#include "normalization_methods/normalization_method_factory.hpp"
#include "primitives/primitive.hpp"
#include "primitive_attributes.hpp"
#include "primitive_group.hpp"

// Standard includes
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace panacea {

  /*************************************************
   * Declaring Static Private Member Methods
   *************************************************/

  void PrimitiveFactory::OneToOne(const PassKey<PrimitiveFactory> &,
      PrimitiveGroup & prim_grp
      ){

    prim_grp.primitives.reserve(prim_grp.kernel_wrapper->getNumberPoints());

    const int initial_num_prim = prim_grp.primitives.size();
    const int diff = prim_grp.kernel_wrapper->getNumberPoints() - initial_num_prim;
    const int new_num_prim = initial_num_prim + diff;

    if( diff > 0 ) {
      // Add the difference
      for( int kernel_index = initial_num_prim;
          kernel_index < prim_grp.kernel_wrapper->getNumberPoints();
          ++kernel_index){

        prim_grp.primitives.push_back(
            create_methods_[prim_grp.getSpecification().get<settings::KernelPrimitive>()]\
            [prim_grp.getSpecification().get<settings::KernelCorrelation>()]\
            (PassKey<PrimitiveFactory>(),
             prim_grp.createPrimitiveAttributes(),
             kernel_index));
      }
    } else {
      // Shrink to fit
      prim_grp.primitives.resize(prim_grp.kernel_wrapper->getNumberPoints());
    }

    if( initial_num_prim != 0 ) {
      int num_prim_to_update = initial_num_prim;
      if( diff < 0 ) num_prim_to_update = prim_grp.kernel_wrapper->getNumberPoints();
      // make sure all the primitive attributes are up to date upto the initial_num_priming index
      for( int kernel_index = 0; kernel_index < num_prim_to_update; ++kernel_index){
        prim_grp.primitives.at(kernel_index)->update(prim_grp.createPrimitiveAttributes());
      }
    }
  }

  void PrimitiveFactory::Single(const PassKey<PrimitiveFactory> &,
      PrimitiveGroup & prim_grp
      ){

    const int kernel_index = 0;
    if( prim_grp.primitives.size() == 0 ) {
      prim_grp.primitives.emplace_back(
          create_methods_[prim_grp.getSpecification().get<settings::KernelPrimitive>()]\
          [prim_grp.getSpecification().get<settings::KernelCorrelation>()]\
          (PassKey<PrimitiveFactory>(),
           prim_grp.createPrimitiveAttributes(),
           kernel_index));
    } else {
      prim_grp.primitives.at(kernel_index)->update(prim_grp.createPrimitiveAttributes());
    }
  }

  /***************************************************************
   * Declaring private Member function maps
   **************************************************************/

  std::unordered_map<settings::KernelPrimitive,
    std::unordered_map<settings::KernelCorrelation,
    PrimitiveFactory::PrimitiveCreateMethod>>
      PrimitiveFactory::create_methods_;

  std::unordered_map<settings::KernelCount,
    PrimitiveFactory::PrimitiveCountMethod>
      PrimitiveFactory::count_methods_{
        {settings::KernelCount::OneToOne, PrimitiveFactory::OneToOne},
        {settings::KernelCount::Single, PrimitiveFactory::Single}
      };

  /***************************************************
   * File scope static functions
   ***************************************************/

  static std::unique_ptr<Normalizer> createNormalizer(
      const BaseDescriptorWrapper & dwrapper,
      const KernelSpecification & specification) {

    if( specification.is(settings::KernelAlgorithm::Strict) ) {
      return std::make_unique<Normalizer>(dwrapper,
          specification.get<settings::KernelNormalization>(),
          NormalizerOption::Strict);
    }
    // Flexible option will avoid errors if coefficients are 0.0,
    // e.g. if variance is 0.0, will set such coefficients to 1.0
    return std::make_unique<Normalizer>(dwrapper,
        specification.get<settings::KernelNormalization>(),
        NormalizerOption::Flexible);
  }

  static std::unique_ptr<Normalizer> createNormalizer(
      const KernelSpecification & specification) {

    if( specification.is(settings::KernelAlgorithm::Strict) ) {
      return std::make_unique<Normalizer>(
          specification.get<settings::KernelNormalization>(),
          NormalizerOption::Strict);
    }
    // Flexible option will avoid errors if coefficients are 0.0,
    // e.g. if variance is 0.0, will set such coefficients to 1.0
    return std::make_unique<Normalizer>(
        specification.get<settings::KernelNormalization>(),
        NormalizerOption::Flexible);
  }

  /***********************************************************
   * Declaring private methods
   ***********************************************************/

  void PrimitiveFactory::check_input_specifications_(
      const KernelSpecification & specification) const {
    if(create_methods_.count(specification.get<settings::KernelPrimitive>()) == 0){
      std::string error_msg = "Kernel Primitive is not supported: ";
      error_msg += settings::toString(specification.get<settings::KernelPrimitive>());
      PANACEA_FAIL(error_msg);
    }

    if( count_methods_.count(specification.get<settings::KernelCount>()) == 0){
      std::string error_msg = "Kernel count method is not supported: ";
      error_msg += settings::toString(specification.get<settings::KernelCount>());
      PANACEA_FAIL(error_msg);
    }
  }

  /***********************************************************
   * Declaring public methods
   ***********************************************************/

  PrimitiveFactory::PrimitiveFactory() {

    registerPrimitive<GaussUncorrelated,
      settings::KernelPrimitive::Gaussian,
      settings::KernelCorrelation::Uncorrelated>();

    registerPrimitive<GaussCorrelated,
      settings::KernelPrimitive::Gaussian,
      settings::KernelCorrelation::Correlated>();

    registerPrimitive<GaussLogCorrelated,
      settings::KernelPrimitive::GaussianLog,
      settings::KernelCorrelation::Correlated>();

  }

  PrimitiveGroup PrimitiveFactory::createGroup(
      const BaseDescriptorWrapper & dwrapper,
      const KernelSpecification & specification,
      const std::string & name) const {

    KernelWrapperFactory kfactory;
    PrimitiveGroup prim_grp(specification);
    prim_grp.name = name;
    prim_grp.kernel_wrapper = kfactory.create(dwrapper, specification);

    prim_grp.covariance = Covariance::create(
        dwrapper,
        specification.get<settings::KernelCorrelation>(),
        specification.get<settings::KernelAlgorithm>());

    // Create a normalizer with kwrapper
    prim_grp.normalizer = createNormalizer(dwrapper, specification);
    prim_grp.normalizer->normalize(*prim_grp.covariance);

    Dimensions dimensions(dwrapper.getNumberDimensions());

    Randomizer randomizer;
    randomizer.randomize(
        dimensions,
        specification.template get<settings::RandomizeDimensions>(),
        specification.template get<settings::RandomizeNumberDimensions>());

    int max_num = specification.getMaxNumberDimensions();
    if( dimensions.size() > max_num && max_num != -1 ) {
      DimensionLimiter dim_limiter;
      dim_limiter.limit(dimensions, specification.getMaxNumberDimensions());
    }

    Reducer reducer;
    prim_grp.reduced_covariance = std::make_unique<ReducedCovariance>(
        reducer.reduce(*prim_grp.covariance, dimensions));

    Inverter inverter;
    prim_grp.reduced_inv_covariance = std::make_unique<ReducedInvCovariance>(
        inverter.invert(*prim_grp.reduced_covariance));

    check_input_specifications_(specification);

    count_methods_[specification.get<settings::KernelCount>()](PassKey<PrimitiveFactory>(),prim_grp);

    return prim_grp;
  }

  PrimitiveGroup PrimitiveFactory::createGroup(
      const KernelSpecification & specification,
      const std::string & name) const {

    // The following objects need to be allocated when reading in from a restart file
    // kernel_wrapper
    // covariance
    // normalizer
    // specifications
    //
    // The remaining items in the primitive group are initialized and created while
    // reading in the values from a restart file
    KernelWrapperFactory kfactory;

    PrimitiveGroup prim_grp(specification);
    prim_grp.name = name;
    prim_grp.kernel_wrapper = kfactory.create(specification);

    prim_grp.covariance = Covariance::create(specification.get<settings::KernelCorrelation>());

    prim_grp.normalizer = createNormalizer(specification);

    return prim_grp;
  }


  void PrimitiveFactory::update(
      const PassKey<PrimitiveGroup> &,
      const BaseDescriptorWrapper & dwrapper,
      PrimitiveGroup & prim_grp) const {

    prim_grp.kernel_wrapper->update(dwrapper);
    // Unnormalize the covariance matrix before updating
    prim_grp.normalizer->unnormalize(*prim_grp.covariance);
    prim_grp.covariance->update(dwrapper);
    // Now we are free to update the normalization coefficients, note that the covariance
    // matrix must be uptodate before it can be passed into the normalizer, in the
    // case of the variance the diagonal is used to calculate the variance
    prim_grp.normalizer->update(dwrapper, prim_grp.covariance.get());
    prim_grp.normalizer->normalize(*prim_grp.covariance);

    Dimensions dimensions(dwrapper.getNumberDimensions());

    Randomizer randomizer;
    randomizer.randomize(
        dimensions,
        prim_grp.getSpecification().template get<settings::RandomizeDimensions>(),
        prim_grp.getSpecification().template get<settings::RandomizeNumberDimensions>());

    int max_num = prim_grp.getSpecification().getMaxNumberDimensions();
    if( dimensions.size() > max_num && max_num != -1 ) {
      DimensionLimiter dim_limiter;
      dim_limiter.limit(dimensions,prim_grp.getSpecification().getMaxNumberDimensions());
    }

    // Cannot update the reduced covariance matrix and reduced inv covariance matrices
    // these both need to be recalculated
    Reducer reducer;
    prim_grp.reduced_covariance = std::make_unique<ReducedCovariance>(
        reducer.reduce(*prim_grp.covariance, dimensions));

    Inverter inverter;
    prim_grp.reduced_inv_covariance = std::make_unique<ReducedInvCovariance>(
        inverter.invert(*prim_grp.reduced_covariance));

    check_input_specifications_(prim_grp.getSpecification());

    // Now we need to update all the primitives after resizing if appropriate
    count_methods_[prim_grp.getSpecification().get<settings::KernelCount>()](PassKey<PrimitiveFactory>(),prim_grp);
  }

  void PrimitiveFactory::initialize(
      const PassKey<PrimitiveGroup> &,
      const BaseDescriptorWrapper & dwrapper,
      PrimitiveGroup & prim_grp) const {


    // Before calling initialize we need to ensure that the descriptor
    // wrapper and the kernel wrapper are of compatible types
    //
    const auto & specification = prim_grp.getSpecification();

    // Here we are going to overwrite the kernels
    //
    // 1. We will overwrite instead of attempting to expand existing kernels
    // because we do not know if the kernel wrapper type is compatible with the
    // descriptor wrapper type.
    //
    // 2. If ownership is set to OwnIfRestart then we are going to temporarily switch
    // the ownership specification to shared so we can create the kernels, this is
    // allowed because if we were going to load from a restart file we would not be
    // calling initialize with an existing descriptor wrapper. By default we should
    // prefer to have shared ownership if the OwnIfRestart setting is specified.
    KernelWrapperFactory kfactory;
    if(specification.is(settings::KernelMemory::OwnIfRestart)) {
      auto local_spec = KernelSpecification(
          specification.get<settings::KernelCorrelation>(),
          specification.get<settings::KernelCount>(),
          specification.get<settings::KernelPrimitive>(),
          specification.get<settings::KernelNormalization>(),
          settings::KernelMemory::Share, // This is the only thing that is changed
          specification.get<settings::KernelCenterCalculation>(),
          specification.get<settings::KernelAlgorithm>(),
          specification.get<settings::RandomizeDimensions>(),
          specification.get<settings::RandomizeNumberDimensions>(),
          specification.getMaxNumberDimensions()
          );
      prim_grp.kernel_wrapper = kfactory.create(dwrapper, local_spec);
    } else {
      prim_grp.kernel_wrapper = kfactory.create(dwrapper, specification);
    }
    prim_grp.covariance = Covariance::create(
        dwrapper,
        specification.get<settings::KernelCorrelation>(),
        specification.get<settings::KernelAlgorithm>());

    prim_grp.normalizer = createNormalizer(dwrapper, specification);
    prim_grp.normalizer->normalize(*prim_grp.covariance);

    Dimensions dimensions(dwrapper.getNumberDimensions());

    Randomizer randomizer;
    randomizer.randomize(
        dimensions,
        specification.get<settings::RandomizeDimensions>(),
        specification.get<settings::RandomizeNumberDimensions>());

    int max_num = specification.getMaxNumberDimensions();
    if( dimensions.size() > max_num && max_num != -1 ) {
      DimensionLimiter dim_limiter;
      dim_limiter.limit(dimensions, specification.getMaxNumberDimensions());
    }

    Reducer reducer;
    prim_grp.reduced_covariance = std::make_unique<ReducedCovariance>(
        reducer.reduce(*prim_grp.covariance, dimensions));

    Inverter inverter;
    prim_grp.reduced_inv_covariance = std::make_unique<ReducedInvCovariance>(
        inverter.invert(*prim_grp.reduced_covariance));

    check_input_specifications_(specification);

    count_methods_[specification.get<settings::KernelCount>()](PassKey<PrimitiveFactory>(),prim_grp);
  }

  void PrimitiveFactory::reset(
      const PassKey<PrimitiveGroup> &,
      PrimitiveGroup & prim_grp,
      const ResetOption reset_opt) const {

    assert(prim_grp.covariance.get() != nullptr);
    assert(prim_grp.covariance->defined());
    assert(prim_grp.kernel_wrapper.get() != nullptr);
    // Cannot update the reduced covariance matrix and reduced inv covariance matrices
    if( prim_grp.covariance->is(NormalizationState::Unnormalized) ){
      prim_grp.normalizer->normalize(*prim_grp.covariance);
    }

    if(reset_opt == ResetOption::All || reset_opt == ResetOption::ReducedCovariance){
      // The number of rows in the covariance matrix should be equivalent to the number
      // of available dimensions.
      Dimensions dimensions(prim_grp.covariance->rows());

      Randomizer randomizer;
      randomizer.randomize(
          dimensions,
          prim_grp.getSpecification().template get<settings::RandomizeDimensions>(),
          prim_grp.getSpecification().template get<settings::RandomizeNumberDimensions>());

      int max_num = prim_grp.getSpecification().getMaxNumberDimensions();
      if( dimensions.size() > max_num && max_num != -1 ) {
        DimensionLimiter dim_limiter;
        dim_limiter.limit(dimensions, prim_grp.getSpecification().getMaxNumberDimensions());
      }

      Reducer reducer;
      prim_grp.reduced_covariance = std::make_unique<ReducedCovariance>(
          reducer.reduce(*prim_grp.covariance, dimensions));
    }

    if(reset_opt == ResetOption::All || reset_opt == ResetOption::ReducedInvCovariance){
    Inverter inverter;
    prim_grp.reduced_inv_covariance = std::make_unique<ReducedInvCovariance>(
        inverter.invert(*prim_grp.reduced_covariance));
    }

    if(reset_opt == ResetOption::All || reset_opt == ResetOption::Primitives){
      check_input_specifications_(prim_grp.getSpecification());
      count_methods_[prim_grp.getSpecification().get<settings::KernelCount>()](
          PassKey<PrimitiveFactory>(),prim_grp);
    }
  }


}
