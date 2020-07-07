/**
 * @file arma_config.hpp
 *
 * This file is specific to R Bindings.
 *
 * Don't touch this file.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_CORE_UTIL_ARMA_CONFIG_HPP
#define MLPACK_CORE_UTIL_ARMA_CONFIG_HPP

#ifdef ARMA_64BIT_WORD
  #define MLPACK_ARMA_64BIT_WORD
#else
  #define MLPACK_ARMA_NO_64BIT_WORD
#endif

#ifdef ARMA_USE_OPENMP
  #define MLPACK_ARMA_USE_OPENMP
  #define ENS_USE_OPENMP 1
#else
  #define MLPACK_ARMA_DONT_USE_OPENMP
  #define ENS_DONT_USE_OPENMP 1
#endif

#endif
