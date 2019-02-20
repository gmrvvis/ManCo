#include <manco/manco.h>
using namespace manco;

#include <iostream>

#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>

#include <chrono>
#include <thread>

void receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o )
{
  std::cout << "Received DestroyGroup (" << o->getKeyString( ) << ")" <<
    std::endl;
}

void receivedChangeNameGroup( zeroeq::gmrv::ConstChangeNameGroupPtr )
{
  std::cout << "Received ChangeNameGroup" << std::endl;
}

void receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o )
{
  std::vector<unsigned int> color = o->getColorVector( );
  std::cout << "Received SyncGroup (" <<
    "\n\tkey: " << o->getKeyString( ) << 
    "\n\tname: " << o->getNameString( ) << 
    "\n\towner: " << o->getOwnerString( ) << 
    "\n\tcolor: (" << color[0] << ", " << color[1] << ", "  <<
    color[2] << ")" <<
    "\n\tids: "; //<< o->getIdsString( ) << ")";
  std::vector< std::string > v = manco::ZeqManager::split( o->getIdsString( ),
    DELIMITER );
  for ( auto it = v.begin(); it != v.end(); ++it )
  {
    std::cout << *it << ' ';
  }
  std::cout << ")" << std::endl;
}

void receivedChangeColor( zeroeq::gmrv::ConstChangeColorGroupPtr o )
{
  std::vector<unsigned int> color = o->getColorVector( );
  std::cout << "Received ChangeColor (" << o->getKeyString( ) << ": " <<
    color[0] << ", " << color[1] << ", "  << color[2] <<
    ")" << std::endl;
}

void receivedSyncXml( zeroeq::gmrv::ConstSyncXmlPtr o )
{
  std::cout << "Received SyncXml (" << o->getFilenameString( ) << ")" <<
    std::endl;
}

void receivedSyncTransferFunc( zeroeq::gmrv::ConstSyncTransferFuncPtr o )
{
  std::cout << "Received SyncTransferFunc (" << std::endl;
  for ( const auto& color : o->getColors( ) )
  {
    std::cout << "\tcolor: (" << color.getRed( ) << ", " << color.getGreen( ) <<
      ", " << color.getBlue( ) << ")" << std::endl;
  }
  std::cout << ")" << std::endl;
  std::vector< std::string > v = manco::ZeqManager::split( o->getIdsString( ),
    DELIMITER );
  for ( auto it = v.begin(); it != v.end(); ++it )
  {
    std::cout << *it << ' ';
  }
  std::cout << ")" << std::endl;
}

int main( int argc, char** argv )
{
  std::cout << "HELLO CONSUMER" << std::endl;

  if( argc < 2 )
  {
    std::cout << "mancoProducer <session>" << std::endl;
    return -1;
  }

  manco::ZeqManager::instance( ).init( argv[ 1 ] );
  std::this_thread::sleep_for( std::chrono::milliseconds( 5000 ) );
  //manco::ZeqManager::instance().publishSyncNeeded( );

  manco::ZeqManager::instance( ).setReceivedSyncXmlCallback( receivedSyncXml );
  manco::ZeqManager::instance( ).setReceivedDestroyGroupCallback(
    receivedDestroyGroup );
  manco::ZeqManager::instance( ).setReceivedChangeNameGroupUpdateCallback(
    receivedChangeNameGroup );
  manco::ZeqManager::instance( ).setReceivedSyncGroupCallback(
    receivedSyncGroup );
  manco::ZeqManager::instance( ).setReceivedChangeColorUpdateCallback(
    receivedChangeColor );
  manco::ZeqManager::instance( ).setReceivedSyncTransferFuncCallback(
    receivedSyncTransferFunc );

  while( true )
  {
  }
  //std::this_thread::sleep_for( std::chrono::milliseconds( 5000 ) );
  return 0;
}
