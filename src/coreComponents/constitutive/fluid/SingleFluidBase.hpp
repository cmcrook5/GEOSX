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
 * @file SingleFluidBase.hpp
 */

#ifndef GEOSX_CONSTITUTIVE_FLUID_SINGLEFLUIDBASE_HPP
#define GEOSX_CONSTITUTIVE_FLUID_SINGLEFLUIDBASE_HPP

#include "constitutive/ConstitutiveBase.hpp"

namespace geosx
{

namespace constitutive
{
//START_SPHINX_INCLUDE_01
/**
 * @brief Base class for single-phase fluid model kernel wrappers.
 */
class SingleFluidBaseUpdate
{
public:

  /**
   * @brief Get number of elements in this wrapper.
   * @return number of elements
   */
  GEOSX_HOST_DEVICE
  localIndex numElems() const { return m_density.size( 0 ); }

  /**
   * @brief Get number of gauss points per element.
   * @return number of gauss points per element
   */
  GEOSX_HOST_DEVICE
  localIndex numGauss() const { return m_density.size( 1 ); };

protected:

  /**
   * @brief Constructor.
   * @param density     fluid density
   * @param dDens_dPres derivative of density w.r.t. pressure
   * @param viscosity   fluid viscosity
   * @param dVisc_dPres derivative of viscosity w.r.t. pressure
   */
  SingleFluidBaseUpdate( arrayView2d< real64 > const & density,
                         arrayView2d< real64 > const & dDens_dPres,
                         arrayView2d< real64 > const & viscosity,
                         arrayView2d< real64 > const & dVisc_dPres )
    : m_density( density ),
    m_dDens_dPres( dDens_dPres ),
    m_viscosity( viscosity ),
    m_dVisc_dPres( dVisc_dPres )
  {}

  /**
   * @brief Copy constructor.
   */
  SingleFluidBaseUpdate( SingleFluidBaseUpdate const & ) = default;

  /**
   * @brief Move constructor.
   */
  SingleFluidBaseUpdate( SingleFluidBaseUpdate && ) = default;

  /**
   * @brief Deleted copy assignment operator
   * @return reference to this object
   */
  SingleFluidBaseUpdate & operator=( SingleFluidBaseUpdate const & ) = delete;

  /**
   * @brief Deleted move assignment operator
   * @return reference to this object
   */
  SingleFluidBaseUpdate & operator=( SingleFluidBaseUpdate && ) = delete;


  /// Fluid density
  arrayView2d< real64 > m_density;

  /// Derivative of density w.r.t. pressure
  arrayView2d< real64 > m_dDens_dPres;

  /// Fluid viscosity
  arrayView2d< real64 > m_viscosity;

  /// Derivative of viscosity w.r.t. pressure
  arrayView2d< real64 > m_dVisc_dPres;
//END_SPHINX_INCLUDE_01
//START_SPHINX_INCLUDE_02
private:

  /**
   * @brief Compute fluid properties at a single point.
   * @param[in]  pressure the target pressure value
   * @param[out] density fluid density
   * @param[out] viscosity fluid viscosity
   */
  GEOSX_HOST_DEVICE
  virtual void compute( real64 const pressure,
                        real64 & density,
                        real64 & viscosity ) const = 0;

  /**
   * @brief Compute fluid properties and derivatives at a single point.
   * @param[in]  pressure the target pressure value
   * @param[out] density fluid density
   * @param[out] dDensity_dPressure fluid density derivative w.r.t. pressure
   * @param[out] viscosity fluid viscosity
   * @param[out] dViscosity_dPressure fluid viscosity derivative w.r.t. pressure
   */
  GEOSX_HOST_DEVICE
  virtual void compute( real64 const pressure,
                        real64 & density,
                        real64 & dDensity_dPressure,
                        real64 & viscosity,
                        real64 & dViscosity_dPressure ) const = 0;

  /**
   * @brief Update fluid state at a single point.
   * @param[in] k        element index
   * @param[in] q        gauss point index
   * @param[in] pressure the target pressure value
   */
  GEOSX_HOST_DEVICE
  virtual void update( localIndex const k,
                       localIndex const q,
                       real64 const pressure ) const = 0;

};
//END_SPHINX_INCLUDE_02

/**
 * @brief Base class for single-phase fluid models.
 */
class SingleFluidBase : public ConstitutiveBase
{
public:

  /**
   * @brief Constructor.
   * @param name name of the group
   * @param parent pointer to parent group
   */
  SingleFluidBase( string const & name, Group * const parent );

  /**
   * @brief Save the current density into the initial density (needed for single-phase poromechanics)
   */
  void initializeState() const;

  // *** ConstitutiveBase interface

  virtual void allocateConstitutiveData( dataRepository::Group & parent,
                                         localIndex const numConstitutivePointsPerParentIndex ) override;

  // *** SingleFluid-specific interface

  arrayView2d< real64 > density() { return m_density; }
  arrayView2d< real64 const > density() const { return m_density; }

  arrayView2d< real64 > dDensity_dPressure() { return m_dDensity_dPressure; }
  arrayView2d< real64 const > dDensity_dPressure() const { return m_dDensity_dPressure; }

  arrayView2d< real64 const > initialDensity() const { return m_initialDensity; }

  arrayView2d< real64 > viscosity() { return m_viscosity; }
  arrayView2d< real64 const > viscosity() const { return m_viscosity; }

  arrayView2d< real64 > dViscosity_dPressure() { return m_dViscosity_dPressure; }
  arrayView2d< real64 const > dViscosity_dPressure() const { return m_dViscosity_dPressure; }

  virtual real64 defaultDensity() const = 0; // { return 1.0; }
  virtual real64 defaultViscosity() const = 0; // { return 1.0; }

protected:

  virtual void postProcessInput() override;

  //START_SPHINX_INCLUDE_00
  array2d< real64 > m_density;
  array2d< real64 > m_dDensity_dPressure;

  array2d< real64 > m_initialDensity;

  array2d< real64 > m_viscosity;
  array2d< real64 > m_dViscosity_dPressure;
  //END_SPHINX_INCLUDE_00
};

} //namespace constitutive

} //namespace geosx

#endif //GEOSX_CONSTITUTIVE_FLUID_SINGLEFLUIDBASE_HPP
