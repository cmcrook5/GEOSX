/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2019, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-746361
 *
 * All rights reserved. See COPYRIGHT for details.
 *
 * This file is part of the GEOSX Simulation Framework.
 *
 * GEOSX is a free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (as published by the
 * Free Software Foundation) version 2.1 dated February 1999.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @file SurfaceGenerator.hpp
 */
#ifndef SRC_COMPONENTS_SURFACEGENERATION_SURFACEGENERATOR_HPP_
#define SRC_COMPONENTS_SURFACEGENERATION_SURFACEGENERATOR_HPP_

#include "MPI_Communications/NeighborCommunicator.hpp"
#include "physicsSolvers/SolverBase.hpp"
#include "managers/DomainPartition.hpp"

namespace geosx
{

struct ModifiedObjectLists
{
  std::set<localIndex> newNodes;
  std::set<localIndex> newEdges;
  std::set<localIndex> newFaces;
  std::set<localIndex> modifiedNodes;
  std::set<localIndex> modifiedEdges;
  std::set<localIndex> modifiedFaces;
  std::map< std::pair<localIndex,localIndex>, std::set<localIndex> > newElements;
  std::map< std::pair<localIndex,localIndex>, std::set<localIndex> > modifiedElements;

  void clearNewFromModified();
};


class SpatialPartition;

class NodeManager;
class EdgeManager;
class FaceManager;
class ExternalFaceManager;
class ElementRegionManager;
class ElementRegion;

/**
 * @class SurfaceGenerator
 *
 * This solver manages the mesh topology splitting methods.
 *
 */
class SurfaceGenerator : public SolverBase
{
public:
  SurfaceGenerator( const std::string& name,
                    ManagedGroup * const parent );
  ~SurfaceGenerator() override;


  static string CatalogName() { return "SurfaceGenerator"; }

  virtual void RegisterDataOnMesh( ManagedGroup * const MeshBody ) override final;

  /**
   * @defgroup Solver Interface Functions
   *
   * These functions provide the primary interface that is required for derived classes
   */
  /**@{*/

  virtual void Execute( real64 const time_n,
                        real64 const dt,
                        integer const cycleNumber,
                        integer const eventCounter,
                        real64 const eventProgress,
                        dataRepository::ManagedGroup * domain ) override
  {
    SolverStep( time_n, dt, cycleNumber, domain->group_cast<DomainPartition*>());
  }

  virtual real64 SolverStep( real64 const& time_n,
                             real64 const& dt,
                             integer const cycleNumber,
                             DomainPartition * domain) override;
//
//  virtual void ImplicitStepSetup( real64 const& time_n,
//                              real64 const& dt,
//                              DomainPartition * const domain,
//                              systemSolverInterface::EpetraBlockSystem * const blockSystem ) override;
//
//
//  virtual void AssembleSystem( DomainPartition * const domain,
//                               systemSolverInterface::EpetraBlockSystem * const blockSystem,
//                               real64 const time,
//                               real64 const dt ) override;
//
//  virtual void ApplyBoundaryConditions( DomainPartition * const domain,
//                                        systemSolverInterface::EpetraBlockSystem * const blockSystem,
//                                        real64 const time,
//                                        real64 const dt ) override;
//
//  virtual real64
//  CalculateResidualNorm( systemSolverInterface::EpetraBlockSystem const * const blockSystem ) override;

//  virtual void SolveSystem( systemSolverInterface::EpetraBlockSystem * const blockSystem,
//                            SystemSolverParameters const * const params ) override;

//  virtual void
//  ApplySystemSolution( systemSolverInterface::EpetraBlockSystem const * const blockSystem,
//                       real64 const scalingFactor,
//                       DomainPartition * const domain ) override;

//  virtual void ResetStateToBeginningOfStep( DomainPartition * const domain ) override;
//
//  virtual  void ImplicitStepComplete( real64 const & time,
//                                      real64 const & dt,
//                                      DomainPartition * const domain ) override;
/**@}*/


  int SeparationDriver( DomainPartition * domain,
                        MeshLevel * const mesh,
                        array1d<NeighborCommunicator> & neighbors,
                        int const tileColor,
                        int const numTileColors,
                        const bool prefrac,
                        const realT time);

  /**
   * @brief Function to generate new global indices of a simple object (node, edge, face)
   * @param[in/out] object A reference to the object that needs new global indices
   * @param[in] indexList the list of local indices that need new global indices
   */
  void AssignNewGlobalIndicesSerial( ObjectManagerBase & object,
                                     std::set<localIndex> const & indexList );


  int Test ( int test);

  //wu40: Since the field nodalForceFromElement is used in another solver (SolidMechanicsLagrangianFEM), I have to make the viewKeyStruct public here.
  struct viewKeyStruct : SolverBase::viewKeyStruct
  {
    constexpr static auto flowFacePressureString = "flowFacePressure";
    constexpr static auto flowFaceTypeString = "flowFaceType";
    constexpr static auto ruptureStateString = "ruptureState";
    constexpr static auto SIFonFaceString = "SIFonFace";
    constexpr static auto K_ICString = "K_IC";
    constexpr static auto primaryCandidateFaceString = "primaryCandidateFace";
    constexpr static auto isFaceSeparableString = "isFaceSeparable";
    constexpr static auto stressNOnFaceString = "stressNOnFace";
    constexpr static auto failCriterionString = "failCriterion";
    constexpr static auto maxTurnAngleString = "maxTurnAngle";
    constexpr static auto faceToEdgeProjectionTolString = "faceToEdgeProjectionTol";
    constexpr static auto faceToFaceCoplaneTolString = "faceToFaceCoplaneTol";
    constexpr static auto faceToEdgeCoplaneTolString = "faceToEdgeCoplaneTol";
    constexpr static auto markExtendedLayerString = "markExtendedLayer";
    constexpr static auto degreeFromCrackString = "degreeFromCrack";
    constexpr static auto nodalForceFromElementString = "nodalForceFromElement";
    constexpr static auto solidMaterialNameString = "solidMaterialName";
    constexpr static auto displacementBasedSIFString = "displacementBasedSIF";
    constexpr static auto allowVacuumFracString = "allowVacuumFrac";
    constexpr static auto rockToughnessString = "rockToughness";
    constexpr static auto saturationPressureCuttoffString = "saturationPressureCuttoff";
    constexpr static auto SIF_IString = "SIF_I";
    constexpr static auto SIF_IIString = "SIF_II";
    constexpr static auto SIF_IIIString = "SIF_III";

    constexpr static auto fractureRegionNameString = "fractureRegion";

  }; //SurfaceGenViewKeys;

  /**
   * @brief Function to generate new global indices for elements
   * @param[in/out] elementManager A reference to the ElementRegionManager that needs new global indices
   * @param[in] indexList the list of local indices that need new global indices
   */
  void
  AssignNewGlobalIndicesSerial( ElementRegionManager & elementManager,
                                std::map< std::pair<localIndex,localIndex>, std::set<localIndex> > const & indexList );

protected:
  virtual void InitializePostInitialConditions_PreSubGroups( ManagedGroup * const problemManager ) override final;

private:


  /**
   * @brief Function to identify which faces are ready for rupture
   * @param nodeManager
   * @param edgeManager
   * @param faceManager
   * @param elementManager
   * @param partition
   * @param prefrac
   */
  void IdentifyRupturedFaces( DomainPartition * domain,
                              NodeManager & nodeManager,
                              EdgeManager & edgeManager,
                              FaceManager & faceManager,
                              ElementRegionManager & elementManager,
                              const bool prefrac);

  /**
   * @brief
   * @param edgeID
   * @param trailFaceID
   * @param nodeManager
   * @param edgeManager
   * @param faceManager
   * @param elementManager
   * @param vecTipNorm
   * @param vecTip
   * @return
   */
  realT CalculateEdgeSIF ( DomainPartition * domain,
                           const localIndex edgeID,
                           localIndex& trailFaceID,
                           NodeManager & nodeManager,
                           EdgeManager & edgeManager,
                           FaceManager & faceManager,
                           ElementRegionManager & elementManager,
                           R1Tensor& vecTipNorm,
                           R1Tensor& vecTip );

  /**
   * @brief Function to calculate f_disconnect and f_u.
   * @param edgeID
   * @param edgeLength
   * @param nodeIndices
   * @param nodeManager
   * @param edgeManager
   * @param elementManager
   * @param vecTipNorm
   * @param fNode
   * @param GdivBeta
   * @param threeNodesPinched
   * @param calculatef_u. True: calculate f_u; False: calculate f_disconnect.
   */
  int CalculateElementForcesOnEdge ( DomainPartition * domain,
                           const localIndex edgeID,
                           realT edgeLength,
                           localIndex_array & nodeIndices,
                           NodeManager & nodeManager,
                           EdgeManager & edgeManager,
                           ElementRegionManager & elementManager,
                           R1Tensor& vecTipNorm,
                           R1Tensor& fNode,
                           realT& GdivBeta,
                           bool threeNodesPinched,
                           bool calculatef_u);

  /**
   * @brief
   * @param edgeID
   * @param trailFaceID
   * @param nodeManager
   * @param edgeManager
   * @param faceManager
   * @param vecTipNorm
   * @param vecTip
   * @param modifiedObjects
   * @param edgeMode
   */
  void MarkRuptureFaceFromEdge ( const localIndex edgeID,
                                 localIndex& trailFaceID,
                                 NodeManager & nodeManager,
                                 EdgeManager & edgeManager,
                                 FaceManager & faceManager,
                                 ElementRegionManager & elementManager,
                                 R1Tensor& vecTipNorm,
                                 R1Tensor& vecTip,
                                 ModifiedObjectLists& modifiedObjects,
                                 const int edgeMode );

  /**
   *
   * @param nodeManager
   * @param edgeManager
   * @param faceManager
   * @param elementManager
   * @param nodesToRupturedFaces
   * @param edgesToRupturedFaces
   */
  void PostUpdateRuptureStates( NodeManager & nodeManager,
                                EdgeManager & edgeManager,
                                FaceManager & faceManager,
                                ElementRegionManager & elementManager,
                                array1d<std::set<localIndex> >& nodesToRupturedFaces,
                                array1d<std::set<localIndex> >& edgesToRupturedFaces );

  /**
   *
   * @param elementManager
   * @param faceManager
   * @param iFace
   */
  int CheckOrphanElement( ElementRegionManager & elementManager,
                           FaceManager & faceManager,
                           localIndex iFace);

  /**
   *
   * @param edgeID
   * @param nodeManager
   * @param faceManager
   * @param edgeManager
   * @param prefrac
   * @return
   */
  int CheckEdgeSplitability( const localIndex edgeID,
                             NodeManager & nodeManager,
                             FaceManager & faceManager,
                             EdgeManager & edgeManager,
                             const bool prefrac );

  /**
   *
   * @param nodeID
   * @param nodeManager
   * @param faceManager
   * @param edgeManager
   * @param prefrac
   * @return
   */
  int CheckNodeSplitability( const localIndex nodeID,
                             NodeManager & nodeManager,
                             FaceManager & faceManager,
                             EdgeManager & edgeManager,
                             const bool prefrac );

//  void UpdatePathCheckingArrays();

  /**
   * @brief check and split node in mesh
   * @param nodeID
   * @param nodeManager
   * @param edgeManager
   * @param faceManager
   * @param elemManager
   * @param nodesToRupturedFaces
   * @param edgesToRupturedFaces
   * @param elementManager
   * @param modifiedObjects
   * @param prefrac
   * @return
   */
  bool ProcessNode( const localIndex nodeID,
                    NodeManager & nodeManager,
                    EdgeManager & edgeManager,
                    FaceManager & faceManager,
                    ElementRegionManager & elemManager,
                    arrayView1d<std::set<localIndex> >& nodesToRupturedFaces,
                    arrayView1d<std::set<localIndex> >& edgesToRupturedFaces,
                    ElementRegionManager & elementManager,
                    ModifiedObjectLists& modifiedObjects,
                    const bool prefrac );

  /**
   * @brief Find a fracture path for surface generation
   * @param nodeID
   * @param nodeManager
   * @param edgeManager
   * @param faceManager
   * @param elemManager
   * @param nodesToRupturedFaces
   * @param edgesToRupturedFaces
   * @param separationPathFaces
   * @param edgeLocations
   * @param faceLocations
   * @param elemLocations
   * @return
   */
  bool FindFracturePlanes( const localIndex nodeID,
                           const NodeManager & nodeManager,
                           const EdgeManager & edgeManager,
                           const FaceManager & faceManager,
                           ElementRegionManager & elemManager,
                           const arrayView1d<std::set<localIndex> >& nodesToRupturedFaces,
                           const arrayView1d<std::set<localIndex> >& edgesToRupturedFaces,
                           std::set<localIndex>& separationPathFaces,
                           map<localIndex, int>& edgeLocations,
                           map<localIndex, int>& faceLocations,
                           map< std::pair<CellElementSubRegion*, localIndex >, int>& elemLocations );


  /**
   * @brief given a fracture path, split the mesh according to the path.
   * @param nodeID
   * @param nodeManager
   * @param edgeManager
   * @param faceManager
   * @param elementManager
   * @param modifiedObjects
   * @param nodesToRupturedFaces
   * @param edgesToRupturedFaces
   * @param separationPathFaces
   * @param edgeLocations
   * @param faceLocations
   * @param elemLocations
   */
  void PerformFracture( const localIndex nodeID,
                        NodeManager & nodeManager,
                        EdgeManager & edgeManager,
                        FaceManager & faceManager,
                        ElementRegionManager & elementManager,
                        ModifiedObjectLists& modifiedObjects,
                        arrayView1d<std::set<localIndex> >& nodesToRupturedFaces,
                        arrayView1d<std::set<localIndex> >& edgesToRupturedFaces,
                        const std::set<localIndex>& separationPathFaces,
                        const map<localIndex, int>& edgeLocations,
                        const map<localIndex, int>& faceLocations,
                        const map< std::pair<CellElementSubRegion*, localIndex >, int>& elemLocations );

  void MapConsistencyCheck( const localIndex nodeID,
                            NodeManager const & nodeManager,
                            EdgeManager const & edgeManager,
                            FaceManager const & faceManager,
                            ElementRegionManager const & elementManager,
                            const map< std::pair<CellElementSubRegion*, localIndex >, int>& elemLocations );

  /**
   * @brief function to set which side of the fracture plane all objects are on
   * @param separationPathFaces
   * @param elemManager
   * @param faceManager
   * @param nodesToElements
   * @param localFacesToEdges
   * @param edgeLocations
   * @param faceLocations
   * @param elemLocations
   * @return
   */
  bool SetLocations( const std::set<localIndex>& separationPathFaces,
                     ElementRegionManager & elemManager,
                     const FaceManager & faceManager,
                     const std::set< std::pair<CellElementSubRegion*, localIndex> >& nodesToElements,
                     const map< localIndex, std::pair<localIndex, localIndex> >& localFacesToEdges,
                     map<localIndex, int>& edgeLocations,
                     map<localIndex, int>& faceLocations,
                     map< std::pair<CellElementSubRegion*, localIndex >, int>& elemLocations );

  /**
   * @brief function to set which side of the fracture plane all objects are on
   * @param side
   * @param elem
   * @param separationPathFaces
   * @param elemManager
   * @param faceManager
   * @param nodesToElements
   * @param localFacesToEdges
   * @param edgeLocations
   * @param faceLocations
   * @param elemLocations
   * @return
   */
  bool SetElemLocations( const int side,
                         const std::pair<CellElementSubRegion*, localIndex >& elem,
                         const std::set<localIndex>& separationPathFaces,
                         ElementRegionManager & elemManager,
                         const FaceManager & faceManager,
                         const std::set< std::pair<CellElementSubRegion*, localIndex> >& nodesToElements,
                         const map< localIndex, std::pair<localIndex, localIndex> >& localFacesToEdges,
                         map<localIndex, int>& edgeLocations,
                         map<localIndex, int>& faceLocations,
                         map< std::pair<CellElementSubRegion*, localIndex >, int>& elemLocations );

  /**
   *
   * @param nodeID
   * @param nodeManager
   * @param edgeManager
   * @param faceManager
   * @return
   */
  realT CalculateKinkAngle ( const localIndex nodeID,
                             const NodeManager & nodeManager,
                             EdgeManager & edgeManager,
                             FaceManager & faceManager );

  /**
   *
   * @param faceManager
   * @param edgeManager
   * @param nodeManager
   * @param modifiedObjects
   * @param prefrac
   */
  void CalculateKinkAngles ( FaceManager & faceManager,
                             EdgeManager & edgeManager,
                             NodeManager & nodeManager,
                             ModifiedObjectLists& modifiedObjects,
                             const bool prefrac );

  /**
   *
   * @param edgeID
   * @param nodeManager
   * @param edgeManager
   * @param faceManager
   * @return
   */
  realT MinimumToughnessOnEdge( const localIndex edgeID,
                                const NodeManager & nodeManager,
                                EdgeManager & edgeManager,
                                FaceManager & faceManager );


private:
  /// choice of failure criterion
  integer m_failCriterion=1;

  //Maximum angle that the fracture can change from its previous propagation direction.
  realT m_maxTurnAngle;

  int m_markExtendedLayer;

  realT m_faceToEdgeProjectionTol;

  realT m_faceToFaceCoplaneTol;

  realT m_faceToEdgeCoplaneTol;

  // solid solver name
  string m_solidMaterialName;

  localIndex m_solidMaterialFullIndex;

  int m_displacementBasedSIF;

  int m_allowVacuumFrac;

  realT m_rockToughness;

  realT m_saturationPressureCuttoff;

  /// set of separable faces
  localIndex_set m_separableFaceSet;

  /// copy of the original node->face mapping prior to any separation
  array1d< set<localIndex> > m_originalNodetoFaces;

  /// copy of the original node->edge mapping prior to any separation
  array1d< set<localIndex> > m_originalNodetoEdges;

  /// copy of the original face->edge mapping prior to any separation
  array1d< array1d<localIndex> > m_originalFaceToEdges;

  /// collection of faces that have been used for separation of each node
  array1d< set<localIndex> > m_usedFacesForNode;

  /// copy of the original face->elemRegion mapping prior to any separation
  array2d< localIndex > m_originalFacesToElemRegion;

  /// copy of the original face->elemSubRegion mapping prior to any separation
  array2d< localIndex > m_originalFacesToElemSubRegion;

  /// copy of the original face->elemIndex mapping prior to any separation
  array2d< localIndex > m_originalFacesToElemIndex;

  /// name of the element region to place all new fractures
  string m_fractureRegionName;

};

} /* namespace geosx */

#endif /* SRC_COMPONENTS_SURFACEGENERATION_SURFACEGENERATOR_HPP_ */
