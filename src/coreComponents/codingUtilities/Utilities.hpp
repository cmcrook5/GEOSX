/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 TotalEnergies
 * Copyright (c) 2019-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

/**
 * @file Utilities.hpp
 */

#ifndef GEOSX_CODINGUTILITIES_UTILITIES_H_
#define GEOSX_CODINGUTILITIES_UTILITIES_H_

#include "codingUtilities/StringUtilities.hpp"
#include "common/DataTypes.hpp"
#include "LvArray/src/limits.hpp"

namespace geosx
{

/**
 * @brief Compare two real values with a tolerance.
 * @tparam type of real value
 * @param val1 first value
 * @param val2 second value
 * @param relTol relative tolerance for comparison
 * @return @p true if @p val1 and @p val2 are within tolerance of each other
 */
template< typename T >
GEOSX_FORCE_INLINE GEOSX_HOST_DEVICE constexpr
bool isEqual( T const val1, T const val2, T const relTol = 0.0 )
{
  T const absTol = ( relTol > 10 * LvArray::NumericLimits< T >::epsilon ) ? relTol * ( fabs( val1 ) + fabs( val2 ) ) * 0.5 : 0.0;
  return ( val2 - absTol ) <= val1 && val1 <= ( val2 + absTol );
}

/**
 * @brief Test if a real value is (almost) zero.
 * @tparam T type of real value
 * @param val the value to test
 * @param tol absolute tolerance for comparison
 * @return @p true if @p val is within @p tol of zero
 */
template< typename T >
GEOSX_FORCE_INLINE GEOSX_HOST_DEVICE constexpr
bool isZero( T const val, T const tol = LvArray::NumericLimits< T >::epsilon )
{
  return -tol <= val && val <= tol;
}

template< typename T >
GEOSX_FORCE_INLINE GEOSX_HOST_DEVICE constexpr
bool isOdd( T x )
{
  static_assert( std::is_integral< T >::value, "Not meaningful for non-integral types" );
  return (x & 1);
}

template< typename T >
GEOSX_FORCE_INLINE GEOSX_HOST_DEVICE constexpr
bool isEven( T x )
{
  return !isOdd( x );
}

template< typename T1, typename T2, typename SORTED >
T2 & stlMapLookup( mapBase< T1, T2, SORTED > & Map, const T1 & key )
{
  typename mapBase< T1, T2, SORTED >::iterator MapIter = Map.find( key );
  GEOSX_ERROR_IF( MapIter==Map.end(), "Key not found: " << key );
  return MapIter->second;
}


template< typename T1, typename T2, typename SORTED >
const T2 & stlMapLookup( const mapBase< T1, T2, SORTED > & Map, const T1 & key )
{
  return (stlMapLookup( const_cast< mapBase< T1, T2, SORTED > & >(Map), key ));
}

template< typename T1, typename T2, typename SORTED, typename LAMBDA >
bool executeOnMapValue( mapBase< T1, T2, SORTED > const & Map, const T1 & key, LAMBDA && lambda )
{
  bool rval = false;
  typename mapBase< T1, T2, SORTED >::const_iterator MapIter = Map.find( key );
  if( MapIter!=Map.end() )
  {
    rval = true;
    lambda( MapIter->second );
  }

  return rval;
}

template< typename T_KEY, typename T_VALUE, typename SORTED >
T_VALUE softMapLookup( mapBase< T_KEY, T_VALUE, SORTED > const & theMap,
                       T_KEY const & key,
                       T_VALUE const failValue )
{
  T_VALUE rvalue;
  typename mapBase< T_KEY, T_VALUE, SORTED >::const_iterator iter = theMap.find( key );
  if( iter==theMap.end() )
  {
    rvalue = failValue;
  }
  else
  {
    rvalue = iter->second;
  }
  return rvalue;
}

/**
 * @brief Perform lookup in a map of options and throw a user-friendly exception if not found.
 * @tparam KEY map key type
 * @tparam VAL map value type
 * @tparam SORTED whether map is ordered or unordered
 * @param map the option map
 * @param option the lookep key
 * @param optionName name of the option to use in exception error message
 * @param contextName name of the lookup context (e.g. the data repository group)
 */
template< typename KEY, typename VAL, typename SORTED >
VAL findOption( mapBase< KEY, VAL, SORTED > const & map,
                KEY const & option,
                string const & optionName,
                string const & contextName )
{
  auto const iter = map.find( option );
  GEOSX_THROW_IF( iter == map.end(),
                  GEOSX_FMT( "{}: unsupported option '{}' for {}.\nSupported options are: {}",
                             contextName, option, optionName, stringutilities::join( mapKeys( map ), ", " ) ),
                  InputError );
  return iter->second;
}

/**
 * @brief Extract the keys from the given map.
 * @tparam MAP Type of the considered map.
 * @tparam C Type of the container holding the keys.
 * @param[in] map The map from which keys will be extracted.
 * @return The container with the keys.
 */
template< template< typename ... > class C = std::vector, typename MAP >
C< typename MAP::key_type > mapKeys( MAP const & map )
{
  C< typename MAP::key_type > keys;
  auto transformer = []( auto const & p ) { return p.first; };
  auto inserter = std::inserter( keys, keys.end() );
  std::transform( map.begin(), map.end(), inserter, transformer );
  return keys;
}

namespace internal
{
template< class F, class ... Ts, std::size_t ... Is >
void forEachArgInTuple( std::tuple< Ts ... > const & tuple, F && func, std::index_sequence< Is ... > )
{
  using expander = int[];
  (void) expander { 0, ( (void)func( std::get< Is >( tuple ), std::integral_constant< size_t, Is >{} ), 0 )... };
}
}

/**
 * @brief Visit every element in a tuple applying a function.
 * @tparam F type of function
 * @tparam Ts types of tuple elements
 * @param tuple the target tuple
 * @param func the function to apply
 *
 * The function will be called with a reference to the tuple element and
 * a compile-time (std::integral_constant) index of the tuple element.
 */
template< class F, class ... Ts >
void forEachArgInTuple( std::tuple< Ts ... > const & tuple, F && func )
{
  internal::forEachArgInTuple( tuple, std::forward< F >( func ), std::make_index_sequence< sizeof...( Ts ) >() );
}

// The code below should work with any subscriptable vector/matrix types

/**
 * @brief Utility function to copy a vector into another vector
 * @tparam VEC1 the type of the source vector
 * @tparam VEC2 the type of the destination vector
 * @param[in] N the number of values to copy
 * @param[in] v1 the source vector
 * @param[out] v2 the destination vector
 * @param[in] offset the first index of v2 at which we start copying values
 */
template< typename VEC1, typename VEC2 >
GEOSX_HOST_DEVICE
void copy( integer const N,
           VEC1 const & v1,
           VEC2 const & v2,
           integer const offset = 0 )
{
  for( integer i = 0; i < N; ++i )
  {
    v2[offset+i] = v1[i];
  }
}

/**
 * @brief Utility function to apply the chain rule to compute df_dx as a function of df_dy and dy_dx
 * @tparam MATRIX the type of the matrix of derivatives
 * @tparam VEC1 the type of the source vector
 * @tparam VEC2 the type of the destination vector
 * @param[in] N the number of derivative values
 * @param[in] dy_dx the matrix of derivatives of y(x) wrt x
 * @param[in] df_dy the derivatives of f wrt y
 * @param[out] df_dx the computed derivatives of f wrt x
 * @param[in] firstDerivativeOffset the first derivative offset in df_dy
 */
template< typename MATRIX, typename VEC1, typename VEC2 >
GEOSX_HOST_DEVICE
void applyChainRule( integer const N,
                     MATRIX const & dy_dx,
                     VEC1 const & df_dy,
                     VEC2 && df_dx,
                     integer const firstDerivativeOffset = 0 )
{
  // this could use some dense linear algebra
  for( integer i = 0; i < N; ++i )
  {
    df_dx[i] = 0.0;
    for( integer j = 0; j < N; ++j )
    {
      df_dx[i] += df_dy[firstDerivativeOffset+j] * dy_dx[j][i];
    }
  }
}

/**
 * @brief Utility function to apply the chain rule to compute df_dxy in place
 * @tparam MATRIX the type of the matrix of derivatives
 * @tparam VEC1 the type of the source vector
 * @tparam VEC2 the type of the utility vector
 * @param[in] N the number of derivative values
 * @param[in] dy_dx the matrix of derivatives of y(x) wrt x
 * @param[out] df_dxy the derivatives of f wrt y
 * @param[out] work utility array
 * @param[in] offset the first derivative offset in df_dy
 */
template< typename MATRIX, typename VEC1, typename VEC2 >
GEOSX_HOST_DEVICE
void applyChainRuleInPlace( integer const N,
                            MATRIX const & dy_dx,
                            VEC1 && df_dxy,
                            VEC2 && work,
                            integer const firstDeriv = 0 )
{
  applyChainRule( N, dy_dx, df_dxy, work, firstDeriv );
  copy( N, work, df_dxy, firstDeriv );
}

} // namespace geosx

#endif /* GEOSX_CODINGUTILITIES_UTILITIES_H_ */
