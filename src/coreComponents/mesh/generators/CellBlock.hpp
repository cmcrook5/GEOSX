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

#ifndef GEOSX_MESH_CELLBLOCK_HPP_
#define GEOSX_MESH_CELLBLOCK_HPP_

#include "dataRepository/Group.hpp"
#include "mesh/utilities/ComputationalGeometry.hpp"
#include "common/GEOS_RAJA_Interface.hpp"
#include "mesh/generators/CellBlockABC.hpp"
#include "mesh/ElementType.hpp"

namespace geosx
{

/**
 * This implementation of CellBlockABC mainly use the cell patterns/shapes
 * to build all the element to nodes, faces and edges mappings.
 */
class CellBlock : public CellBlockABC
{
public:

  /**
   * @name Constructor / Destructor
   */
  ///@{

  CellBlock() = delete;

  /**
   * @brief Constructor for this class.
   * @param[in] name the name of this object manager
   * @param[in] parent the parent Group
   */
  CellBlock( string const & name, Group * const parent );

  /**
   * @brief Copy constructor.
   * @param[in] init the source to copy
   */
  CellBlock( const CellBlock & init ) = delete;

  /**
   * @brief Default destructor.
   */
  ~CellBlock() override = default;

  ///@}

  /**
   * @name Geometry computation / Connectivity
   */
  ///@{

  ///@}
  /**
   * @name Getters / Setters
   */
  ///@{

  /**
   * @brief Defines the underlying element type (hex, tet...)
   * @param[in] elementType the element type
   *
   * @note Allocates the values of the element to nodes, edges, faces accordingly.
   */
  void setElementType( ElementType elementType );

  ElementType getElementType() const override
  { return m_elementType; }

  localIndex numNodesPerElement() const override
  { return m_numNodesPerElement; }

  localIndex numEdgesPerElement() const override
  { return m_numEdgesPerElement; }

  localIndex numFacesPerElement() const override
  { return m_numFacesPerElement; }

  localIndex numElements() const override
  { return size(); }

  /**
   * @brief Puts the nodes of face @p iFace of element @p iElement inside vector @p nodesInFaces.
   * @param[in] iElement The element index.
   * @param[in] iFace The local face index (not the global index). E.g. an hexahedron have face 6 indices from 0 to 5.
   * @param[out] nodesInFaces The result vector gets resized to the appropriate dimensions before getting filled.
   *
   * @p nodesInFaces is sorted from lower to larger node indices values.
   * @p nodesInFaces is exactly the size of the number of nodes.
   */
  void getFaceNodes( localIndex iElement,
                     localIndex iFace,
                     array1d< localIndex > & nodesInFaces ) const;

  /**
   * @brief Get the element to nodes mapping, non-const version.
   * @return The mapping relationship as a array.
   *
   * @deprecated This accessor is meant to be used like a setter even though it's a bit like having public attribute...
   * Use a real setter instead.
   */
  array2d< localIndex, cells::NODE_MAP_PERMUTATION > & getElemToNode()
  { return m_elementsToNodes; }

  array2d< localIndex, cells::NODE_MAP_PERMUTATION > getElemToNodes() const override
  { return m_elementsToNodes; }

  array2d< localIndex > getElemToEdges() const override
  { return m_elementsToEdges; }

  array2d< localIndex > getElemToFaces() const override
  { return m_elementsToFaces; }

  /**
   * @brief Get the element-to-faces map.
   * @return A const view of the mapping.
   */
  arrayView2d< localIndex const > getElemToFacesConstView() const
  { return m_elementsToFaces.toViewConst(); }

  /**
   * @brief Sets an entry in the element to faces mapping.
   * @param[in] iElement Index of the element
   * @param[in] iFaceLoc Local index of the face of the element @p iElement (typically from 0 to 5 for an hexahedron).
   * @param[in] iFace The face index.
   */
  void setElementToFaces( localIndex iElement,
                          localIndex iFaceLoc,
                          localIndex iFace )
  {
    m_elementsToFaces( iElement, iFaceLoc ) = iFace;
  }

  /**
   * @brief Sets an entry in the element to edges mapping.
   * @param[in] iElement Index of the element.
   * @param[in] iEdgeLoc Local index of the edge of the element @p iElement (typically from 0 to 11 for an hexahedron).
   * @param[in] iEdge Index of the edge.
   *
   * In the element to edges mapping, element @p iElement has a given number of edges (typically 12 for a hexahedron).
   * Then edge @p iEdgeLoc of this local indexing (typically 0 to 11) is meant to have global indexing of @p iEdge.
   */
  void setElementToEdges( localIndex iElement,
                          localIndex iEdgeLoc,
                          localIndex iEdge )
  {
    m_elementsToEdges( iElement, iEdgeLoc ) = iEdge;
  }

  /**
   * @brief Checks if edge @p iEdge of element @p iElement has been defined.
   * @param[in] iElement Index of the element
   * @param[in] iEdgeLoc Index of the edge of the element @p iElement (typically from 0 to 11 for an hexahedron).
   * @param[in] iEdge The edge index.
   * @return True if the entry is already there in the mapping. False otherwise.
   */
  bool hasElementToEdges( localIndex iElement,
                          localIndex iEdgeLoc,
                          localIndex iEdge ) const
  {
    return m_elementsToEdges( iElement, iEdgeLoc ) == iEdge;
  }

  /**
   * @brief Get local to global map, non-const version.
   * @return The mapping relationship as a array.
   *
   * @deprecated This accessor is meant to be used like a setter even though it's a bit like having public attribute...
   * Use a real setter instead.
   */
  arrayView1d< globalIndex > localToGlobalMap()
  { return m_localToGlobalMap; }

  array1d< globalIndex > localToGlobalMap() const override
  { return m_localToGlobalMap; }

  /**
   * @brief Resize the cell block to hold @p numElements
   * @param numElements The new number of elements.
   */
  void resize( dataRepository::indexType const numElements ) override final;

  ///@}

  /**
   * @name Properties
   */
  ///@{

  ///@}

private:

  /// Number of nodes per element in this subregion.
  localIndex m_numNodesPerElement;

  /// Number of edges per element in this subregion.
  localIndex m_numEdgesPerElement;

  /// Number of faces per element in this subregion.
  localIndex m_numFacesPerElement;

  /// Element-to-node relation
  array2d< localIndex, cells::NODE_MAP_PERMUTATION > m_elementsToNodes;

  /// Element-to-edges relation
  array2d< localIndex > m_elementsToEdges;

  /// Element-to-node relation
  array2d< localIndex > m_elementsToFaces;

  /// Contains the global index of each object.
  array1d< globalIndex > m_localToGlobalMap;

  /// Name of the properties registered from an external mesh
  string_array m_externalPropertyNames;

  /// Type of element in this subregion.
  ElementType m_elementType;

  std::list< dataRepository::WrapperBase * > getExternalProperties() override
  {
    std::list< dataRepository::WrapperBase * > result;
    for( string const & externalPropertyName : m_externalPropertyNames )
    {
      result.push_back( &this->getWrapperBase( externalPropertyName ) );
    }
    return result;
  }
};

}

#endif /* GEOSX_MESH_CELLBLOCK_HPP_ */
