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
 * @file BrineInternalEnergy.cpp
 */

#include "constitutive/fluid/PVTFunctions/BrineInternalEnergy.hpp"

namespace geosx
{

using namespace stringutilities;

namespace constitutive
{

namespace PVTProps
{

BrineInternalEnergy::KernelWrapper
BrineInternalEnergy::createKernelWrapper() const
{
  return KernelWrapper( m_componentMolarWeight );
}

REGISTER_CATALOG_ENTRY( PVTFunctionBase, BrineInternalEnergy, string const &, string_array const &, string_array const &, array1d< real64 > const & )

} // namespace PVTProps

} // namespace constitutive

} // namespace geosx
