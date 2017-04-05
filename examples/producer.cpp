#include <manco/manco.h>
using namespace manco;

#include <iostream>

#include <chrono>
#include <thread>

int randomMessage( )
{
  static int min = 0;
  static int max = 2;
  return min + (rand() % (int)(max - min + 1));
}

int main( )
{
  std::cout << "HELLO PRODUCER" << std::endl;
  manco::ZeqManager::init( "hbp://" );
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
    }
  }
  return 0;
}
