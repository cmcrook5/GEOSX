

================================ =========================================================================================================== ==================================================================== 
Name                             Type                                                                                                        Description                                                          
================================ =========================================================================================================== ==================================================================== 
domainBoundaryIndicator          integer_array                                                                                               (no description available)                                           
edgesToFractureConnectors        geosx_mapBase< int, int, std_integral_constant< bool, true > >                                              A map of edge local indices to the fracture connector local indices. 
faceList                         geosx_InterObjectRelation< LvArray_ArrayOfSets< int, int, LvArray_ChaiBuffer > >                            (no description available)                                           
fractureConnectorsToEdges        integer_array                                                                                               A map of fracture connector local indices to edge local indices.     
fractureConnectorsToElementIndex LvArray_ArrayOfArrays< int, int, LvArray_ChaiBuffer >                                                       A map of fracture connector local indices face element local indices 
ghostRank                        integer_array                                                                                               (no description available)                                           
globalToLocalMap                 geosx_mapBase< long long, int, std_integral_constant< bool, false > >                                       (no description available)                                           
isExternal                       integer_array                                                                                               (no description available)                                           
localToGlobalMap                 globalIndex_array                                                                                           Array that contains a map from localIndex to globalIndex.            
nodeList                         geosx_InterObjectRelation< LvArray_Array< int, 2, camp_int_seq< long, 0l, 1l >, int, LvArray_ChaiBuffer > > (no description available)                                           
neighborData                     node                                                                                                        :ref:`DATASTRUCTURE_neighborData`                                    
sets                             node                                                                                                        :ref:`DATASTRUCTURE_sets`                                            
================================ =========================================================================================================== ==================================================================== 


