// SPDX-License-Identifier: MIT
// Copyright (c) 2026 SuperJupiter
#ifndef SUPERJUPITER_SENSOR_AGREEMENT_H
#define SUPERJUPITER_SENSOR_AGREEMENT_H
#if defined(__FAST_MATH__) || (defined(__FINITE_MATH_ONLY__) && __FINITE_MATH_ONLY__ > 0)
#error "SuperJupiter requires IEEE non-finite checks; disable fast-math"
#endif
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#define SUPERJUPITER_SENSOR_AGREEMENT_VERSION "0.1.0"
namespace superjupiter {
namespace sensor_agreement {

namespace detail {
inline bool finite(double value) { return value == value && value >= -DBL_MAX && value <= DBL_MAX; }
inline double oneMinusExpNegative(double ratio) {
  // Stable at small elapsed/tau without requiring AVR's unavailable expm1.
  if (ratio < 0.01) {
    double term = ratio, total = ratio;
    for (unsigned n = 2; n <= 12; ++n) {
      term *= -ratio / n;
      total += term;
    }
    return total;
  }
  return 1 - ::exp(-ratio);
}
} // namespace detail

enum class Status { Agree, Disagree, Insufficient, InvalidConfig, NumericError };
struct Sample {
  double value;
  uint32_t ageMs;
};
struct Config {
  double absoluteTolerance, relativeTolerance;
  uint32_t maxAgeMs;
  size_t minSensors;
  Config(double a = 1, double r = 0, uint32_t age = 1000, size_t n = 2)
      : absoluteTolerance(a), relativeTolerance(r), maxAgeMs(age), minSensors(n) {}
};
struct Result {
  Status status;
  size_t eligible, excluded;
  double mean, spread, tolerance;
};
inline Result compare(const Sample *samples, size_t count, Config c = Config()) {
  Result r = {Status::InvalidConfig, 0, 0, 0, 0, 0};
  if ((!samples && count) || !c.minSensors || !detail::finite(c.absoluteTolerance) ||
      !detail::finite(c.relativeTolerance) || c.absoluteTolerance < 0 || c.relativeTolerance < 0)
    return r;
  double low = 0, high = 0, scale = 0;
  for (size_t i = 0; i < count; ++i) {
    const Sample &s = samples[i];
    if (!detail::finite(s.value) || s.ageMs > c.maxAgeMs) {
      ++r.excluded;
      continue;
    }
    if (!r.eligible)
      low = high = s.value;
    else {
      if (s.value < low)
        low = s.value;
      if (s.value > high)
        high = s.value;
    }
    ++r.eligible;
    const double n = static_cast<double>(r.eligible);
    r.mean = r.mean * (1 - 1 / n) + s.value / n;
    if (::fabs(s.value) > scale)
      scale = ::fabs(s.value);
  }
  if (r.eligible < c.minSensors) {
    r.status = Status::Insufficient;
    return r;
  }
  r.spread = high - low;
  r.tolerance = c.absoluteTolerance + c.relativeTolerance * scale;
  if (!detail::finite(r.mean) || !detail::finite(r.spread) || !detail::finite(r.tolerance)) {
    r.status = Status::NumericError;
    return r;
  }
  r.status = r.spread <= r.tolerance ? Status::Agree : Status::Disagree;
  return r;
}
} // namespace sensor_agreement
} // namespace superjupiter
#endif
