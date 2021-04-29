#ifndef PANACEA_PRIVATE_TYPEMAP_H
#define PANACEA_PRIVATE_TYPEMAP_H
#pragma once

// Standard includes
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

namespace panacea {

  static const std::unordered_map<std::type_index,std::string> type_map {
    {std::type_index(typeid(std::vector<double>)),"std::vector<double>"},
    {std::type_index(typeid(std::vector<double>*)),"std::vector<double>*"},
    {std::type_index(typeid(std::vector<std::vector<double>>)),"std::vector<std::vector<double>>"},
    {std::type_index(typeid(std::vector<std::vector<double>>*)),"std::vector<std::vector<double>>*"},
    {std::type_index(typeid(double ***)),"double ***"},
    {std::type_index(typeid(std::vector<std::deque<double>>)),"std::vector<std::deque<double>>"}};

}

#endif // PANACEA_PRIVATE_TYPEMAP_H

