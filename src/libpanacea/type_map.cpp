// Local private PANACEA includes
#include "type_map.hpp"

#include "attributes/covariance.hpp"
#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "kernels/kernel_specifications.hpp"
#include "matrix/matrix.hpp"
#include "primitives/primitive.hpp"
#include "primitives/primitive_group.hpp"
#include "vector/vector.hpp"

// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/entropy_term.hpp"

// Standard includes
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

namespace panacea {

  const std::unordered_map<std::type_index,std::string> type_map {
    {std::type_index(typeid(std::vector<double>)),"std::vector<double>"},
    {std::type_index(typeid(std::vector<double>*)),"std::vector<double>*"},
    {std::type_index(typeid(std::vector<std::vector<double>>)),"std::vector<std::vector<double>>"},
    {std::type_index(typeid(std::vector<std::vector<double>>*)),"std::vector<std::vector<double>>*"},
    {std::type_index(typeid(double ***)),"double ***"},
    {std::type_index(typeid(std::vector<std::deque<double>>)),"std::vector<std::deque<double>>"},
    {std::type_index(typeid(BaseKernelWrapper *)),"BaseKernelWrapper *"},
    {std::type_index(typeid(BaseDescriptorWrapper *)),"BaseDescriptorWrapper *"},
    {std::type_index(typeid(Matrix *)),"Matrix *"},
    {std::type_index(typeid(Vector *)),"Vector *"},
    {std::type_index(typeid(Covariance *)),"Covariance *"},
    {std::type_index(typeid(Covariance)),"Covariance"},
    {std::type_index(typeid(ReducedCovariance *)),"ReducedCovariance *"},
    {std::type_index(typeid(ReducedCovariance)),"ReducedCovariance"},
    {std::type_index(typeid(ReducedInvCovariance *)),"ReducedInvCovariance *"},
    {std::type_index(typeid(ReducedInvCovariance)),"ReducedInvCovariance"},
    {std::type_index(typeid(KernelSpecification)),"KernelSpecification"},
    {std::type_index(typeid(KernelSpecification *)),"KernelSpecification *"},
    {std::type_index(typeid(Primitive *)),"Primitive *"},
    {std::type_index(typeid(EntropyTerm *)),"EntropyTerm *"},
    {std::type_index(typeid(PrimitiveGroup *)),"PrimitiveGroup *"},

    {std::type_index(typeid(const std::vector<double>)),"const std::vector<double>"},
    {std::type_index(typeid(const std::vector<double>*)),"const std::vector<double>*"},
    {std::type_index(typeid(const std::vector<std::vector<double>>)),"const std::vector<std::vector<double>>"},
    {std::type_index(typeid(const std::vector<std::vector<double>>*)),"const std::vector<std::vector<double>>*"},
    {std::type_index(typeid(const double ***)),"const double ***"},
    {std::type_index(typeid(const std::vector<std::deque<double>>)),"const std::vector<std::deque<double>>"},
    {std::type_index(typeid(const BaseKernelWrapper *)),"const BaseKernelWrapper *"},
    {std::type_index(typeid(const BaseDescriptorWrapper *)),"const BaseDescriptorWrapper *"},
    {std::type_index(typeid(const Matrix *)),"const Matrix *"},
    {std::type_index(typeid(const Vector *)),"const Vector *"},
    {std::type_index(typeid(const Covariance *)),"const Covariance *"},
    {std::type_index(typeid(const Covariance)),"const Covariance"},
    {std::type_index(typeid(const ReducedCovariance *)),"const ReducedCovariance *"},
    {std::type_index(typeid(const ReducedCovariance)),"const ReducedCovariance"},
    {std::type_index(typeid(const ReducedInvCovariance *)),"const ReducedInvCovariance *"},
    {std::type_index(typeid(const ReducedInvCovariance)),"const ReducedInvCovariance"},
    {std::type_index(typeid(const KernelSpecification)),"const KernelSpecification"},
    {std::type_index(typeid(const KernelSpecification *)),"const KernelSpecification *"},
    {std::type_index(typeid(const Primitive *)),"const Primitive *"},
    {std::type_index(typeid(const EntropyTerm *)),"const EntropyTerm *"},
    {std::type_index(typeid(const PrimitiveGroup *)),"const PrimitiveGroup *"},
  };

}

