#include <manco/manco.h>
using namespace manco;

#include <iostream>

#include <chrono>
#include <thread>

int main( )
{
  std::cout << "HELLO PRODUCER" << std::endl;
  manco::ZeqManager::init( "hbp://" );
  while(true)
  {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "Sending destroy event" << std::endl;
	manco::ZeqManager::publishDestroyGroup( std::string( "group" ) );
  }
  return 0;
}
