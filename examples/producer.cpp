#include <manco/manco.h>
using namespace manco;

#include <iostream>

#include <chrono>
#include <thread>

int randomMessage( )
{
  static int min = 0;
  static int max = 3;
  return min + (rand() % (int)(max - min + 1));
}

int main( )
{
  std::cout << "HELLO PRODUCER" << std::endl;
  manco::ZeqManager::init( "hbp://" );

  std::this_thread::sleep_for(std::chrono::milliseconds(2500));
  std::cout << "Sending sync group event" << std::endl;
  manco::ZeqManager::publishSyncGroup( std::string( "group" ), std::string( "randomName" ), std::string( "OWNER" ),
    {}, 255, 255, 255 );

  while(true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    switch(randomMessage( ))
    {
      case 0:
        std::cout << "Sending destroy event" << std::endl;
        manco::ZeqManager::publishDestroyGroup( std::string( "group" ) );
        break;
      case 1:
        std::cout << "Sending change name event" << std::endl;
        manco::ZeqManager::publishChangeName( std::string( "group" ), std::string( "randomName" ) );
        break;
      case 2:
        std::cout << "Sending sync group event" << std::endl;
        manco::ZeqManager::publishSyncGroup( std::string( "group" ), std::string( "randomName" ), std::string( "OWNER" ),
          {}, 255, 255, 255 );
        break;
    }
  }
  return 0;
}
