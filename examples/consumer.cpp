#include <manco/manco.h>
using namespace manco;

#include <iostream>


#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>

std::vector<std::string> split( const std::string& str, const std::string& delimiter )
{
  std::string s = str;
  std::vector< std::string > v;
  size_t pos = 0;
  std::string token;
  while ( ( pos = s.find( delimiter ) ) != std::string::npos )
  {
    token = s.substr( 0, pos );
    v.push_back( token );
    s.erase( 0, pos + delimiter.length( ) );
  }
  return v;
}

void receivedDestroyGroup(zeroeq::gmrv::ConstDestroyGroupPtr o )
{
  std::cout << "Received DestroyGroup (" << o->getKeyString( ) << ")" << std::endl;
}
void receivedChangeNameGroup(zeroeq::gmrv::ConstChangeNameGroupPtr)
{
  std::cout << "RECEIVED ChangeNameGroup" << std::endl;
}
void receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o )
{
  std::vector<unsigned int> color = o->getColorVector( );
  std::cout << "Received SyncGroup (" <<
    "\n\tkey: " << o->getKeyString( ) << 
    "\n\tname: " << o->getNameString( ) << 
    "\n\towner: " << o->getOwnerString( ) << 
    "\n\tcolor: (" << color[0] << ", " << color[1] << ", "  << color[2] << ")" <<
    "\n\tids: "; // << o->getIdsString( ) <<
  std::vector< std::string > v = split( o->getIdsString( ), DELIMITER );
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

int main( )
{
  //split("aa|&|bb|&|cc|&|dd", "|&|");

  std::cout << "HELLO CONSUMER" << std::endl;
  manco::ZeqManager::init( "hbp://" );

  manco::ZeqManager::_receivedDestroyGroupCallback = receivedDestroyGroup;
  manco::ZeqManager::_receivedChangeNameGroupUpdateCallback = receivedChangeNameGroup;
  manco::ZeqManager::_receivedSyncGroupCallback = receivedSyncGroup;
  manco::ZeqManager::_receivedChangeColorUpdateCallback = receivedChangeColor;

  while(true) 
  {
  }
  return 0;
}
