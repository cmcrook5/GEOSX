/*
 * DataObjectManager.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: rrsettgast
 */

#include "WrapperCollection.hpp"

namespace geosx
{
namespace dataRepository
{

WrapperCollection::WrapperCollection( std::string const & name ):
m_size(0),
m_name(name),
m_path(),
m_keyLookup(),
m_dataObjects(),
m_parent(nullptr),
m_subObjectManagers()
{}

WrapperCollection::~WrapperCollection()
{
  // TODO Auto-generated destructor stub
}

//DataObjectManager::DataObjectManager( DataObjectManager const & source ):
//    m_size( source.m_size ),
//    m_name( source.m_name ),
//    m_path( source.m_path ),
//    m_keyLookup( source.m_keyLookup ),
//    m_dataObjects( source.m_dataObjects ),
//    m_parent( source.m_parent )
//{}

WrapperCollection::WrapperCollection( WrapperCollection&& source ):
    m_size( std::move(source.m_size) ),
    m_name( std::move(source.m_name) ),
    m_path( std::move(source.m_path) ),
    m_keyLookup( std::move(source.m_keyLookup) ),
    m_dataObjects( std::move(source.m_dataObjects) ),
    m_parent( std::move(source.m_parent) )
{}

WrapperBase& WrapperCollection::RegisterWrapper( std::string const & name, rtTypes::TypeIDs const & type )
{
  return *( rtTypes::ApplyTypeLambda( type,
                                   [this, &name]( auto a ) -> WrapperBase*
                                   {
                                     return &( this->RegisterWrapper<decltype(a)>(name) );
                                   } ) );
}

void WrapperCollection::resize( std::size_t const newsize )
{
  for( auto&& i : this->m_dataObjects )
  {
    i->resize(newsize);
  }
  m_size = newsize;
}


}
} /* namespace ODS */
