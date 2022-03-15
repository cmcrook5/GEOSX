/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2019 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2019 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2019 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

/**
 * @file LevelBuilderBase.cpp
 */

#include "LevelBuilderBase.hpp"

#include "linearAlgebra/interfaces/InterfaceTypes.hpp"
#include "linearAlgebra/multiscale/MsrsbLevelBuilder.hpp"
#include "linearAlgebra/multiscale/MsrsbCoupledLevelBuilder.hpp"

namespace geosx
{
namespace multiscale
{

template< typename LAI >
std::unique_ptr< LevelBuilderBase< LAI > >
LevelBuilderBase< LAI >::create( string name,
                                 LinearSolverParameters::Multiscale params )
{
  switch( params.basisType )
  {
    case LinearSolverParameters::Multiscale::BasisType::msrsb:
    {
      if( params.subParams.empty() )
      {
        return std::make_unique< MsrsbLevelBuilder< LAI > >( std::move( name ), std::move( params ) );
      }
      else
      {
        return std::make_unique< MsrsbCoupledLevelBuilder< LAI > >( std::move( name ), std::move( params ) );
      }
    }
    default:
    {
      GEOSX_ERROR( "Unsupported interpolation type: " << params.basisType );
    }
  }
  return std::unique_ptr< LevelBuilderBase< LAI > >();
}

template< typename LAI >
LevelBuilderBase< LAI >::LevelBuilderBase( string name,
                                           LinearSolverParameters::Multiscale params )
  : m_name( std::move( name ) ),
  m_params( std::move( params ) )
{}

// -----------------------
// Explicit Instantiations
// -----------------------
#ifdef GEOSX_USE_TRILINOS
template class LevelBuilderBase< TrilinosInterface >;
#endif

#ifdef GEOSX_USE_HYPRE
template class LevelBuilderBase< HypreInterface >;
#endif

#ifdef GEOSX_USE_PETSC
template class LevelBuilderBase< PetscInterface >;
#endif

} // namespace multiscale
} // namespace geosx
