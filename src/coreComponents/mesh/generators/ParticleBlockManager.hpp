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
 * @file ParticleBlockManager.hpp
 */

#ifndef GEOSX_MESH_PARTICLEBLOCKMANAGER_H_
#define GEOSX_MESH_PARTICLEBLOCKMANAGER_H_

#include "ParticleBlock.hpp"
#include "mesh/ParticleType.hpp"
#include "common/DataTypes.hpp"

namespace geosx
{

namespace dataRepository
{
namespace keys
{
/// String for particleBlocks
string const particleBlocks = "particleBlocks";
}
}

/**
 * @class ParticleBlockManager
 * @brief The ParticleBlockManager class provides an interface to ObjectManagerBase in order to manage ParticleBlock data.
 */
class ParticleBlockManager : public ObjectManagerBase
{
public:

  /**
   * @brief The function is to return the name of the ParticleBlockManager in the object catalog
   * @return string that contains the catalog name used to register/lookup this class in the object catalog
   */
  static string catalogName()
  {
    return "ParticleBlockManager";
  }

  virtual const string getCatalogName() const override final
  { return ParticleBlockManager::catalogName(); }


  /**
   * @brief Constructor for ParticleBlockManager object.
   * @param name name of this instantiation of ParticleBlockManager
   * @param parent pointer to the parent Group of this instantiation of ParticleBlockManager
   */
  ParticleBlockManager( string const & name, Group * const parent );

  /**
   * @brief Destructor
   */
  virtual ~ParticleBlockManager() override;

  virtual Group * createChild( string const & childKey, string const & childName ) override;

  using Group::resize;

  /**
   * @brief Set the number of particles for a set of particle regions.
   * @param numParticles list of the new particle numbers
   * @param regionNames list of the particle region names
   */
  void resize( integer_array const & numParticles,
               string_array const & regionNames );

  /**
   * @brief Get particle sub-region.
   * @param regionName name of the particle sub-region
   * @return pointer to the particle sub-region
   */
  ParticleBlock & getRegion( string const & regionName )
  {
    return this->getGroup( dataRepository::keys::particleBlocks ).getGroup< ParticleBlock >( regionName );
  }


  /**
   * @brief Launch kernel function over all the sub-regions
   * @tparam LAMBDA type of the user-provided function
   * @param lambda kernel function
   */
  template< typename LAMBDA >
  void forParticleSubRegions( LAMBDA lambda )
  {
    this->getGroup( dataRepository::keys::particleBlocks ).forSubGroups< ParticleBlock >( lambda );
  }

  /**
   * @brief Registers and returns a particle block of name @p name.
   * @param name The name of the created particle block.
   * @return A reference to the new particle block. The ParticleBlockManager owns this new instance.
   */
  ParticleBlock & registerParticleBlock( string name );

  Group & getParticleBlocks();

private:

  /**
   * @brief Copy constructor.
   */
  ParticleBlockManager( const ParticleBlockManager & );

  /**
   * @brief Copy assignment operator.
   * @return reference to this object
   */
  ParticleBlockManager & operator=( const ParticleBlockManager & );

  struct viewKeyStruct
  {
    /// Cell blocks key
    static constexpr char const * particleBlocks() { return "particleBlocks"; }
  };

  /**
   * @brief Returns a group containing the particle blocks as ParticleBlockABC instances
   * @return Reference to the Group instance.
   */
  const Group & getParticleBlocks() const;

  /**
   * @brief Get particle block at index @p iParticleBlock.
   * @param[in] iParticleBlock The particle block index.
   * @return Const reference to the instance.
   *
   * @note Mainly useful for iteration purposes.
   */
  //const ParticleBlockABC & getParticleBlock( localIndex iParticleBlock ) const;

  /**
   * @brief Returns the number of particles blocks
   * @return Number of particle blocks
   */
  localIndex numParticleBlocks() const;


};
}
#endif /* GEOSX_MESH_PARTICLEBLOCKMANAGER_H_ */
