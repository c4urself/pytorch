#pragma once

#include <pybind11/pybind11.h>

#include <torch/csrc/profiler/collection.h>
#include <torch/csrc/utils/python_stub.h>

namespace pybind11 {
namespace detail {
namespace {

// Strong typedefs don't make much sense in Python since everything is duck
// typed. So instead we simply cast them to ints, return them, and let the
// caller handle correctness.
template <typename T>
struct strong_pointer_type_caster {
  template <typename T_>
  static handle cast(T_&& src, return_value_policy policy, handle parent) {
    const auto* ptr = reinterpret_cast<const void*>(src.value_of());
    return ptr ? handle(PyLong_FromLong(reinterpret_cast<intptr_t>(ptr)))
               : none();
  }

  bool load(handle src, bool convert) {
    return false;
  }

  PYBIND11_TYPE_CASTER(T, _("strong_pointer"));
};
} // namespace

using torch::profiler::impl::StorageImplData;
using torch::profiler::impl::TensorImplAddress;

template <>
struct type_caster<StorageImplData>
    : public strong_pointer_type_caster<StorageImplData> {};

template <>
struct type_caster<TensorImplAddress>
    : public strong_pointer_type_caster<TensorImplAddress> {};
} // namespace detail
} // namespace pybind11

namespace torch {
namespace profiler {

void initPythonBindings(PyObject* module);

} // namespace profiler
} // namespace torch
