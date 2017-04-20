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

std::vector<std::string> randomNumbers( const int& n )
{
  std::vector<std::string> numbers;
  static int min = 0;
  static int max = 1000;

  for (int i = 0; i < n; ++i)
  {
     std::string number = std::to_string(min + ( rand() % ( int )( max - min + 1 ) ));
     numbers.push_back(number);
  }
  return numbers;
}

int main( int argc, char** argv )
{
  std::cout << "HELLO PRODUCER" << std::endl;

  if( argc < 2 )
  {
    std::cout << "mancoProducer <session>" << std::endl;
    return -1;
  }

  manco::ZeqManager::instance().init( argv[ 1 ] );
  //manco::ZeqManager::instance().init( argv[ 1 ] );

  std::this_thread::sleep_for( std::chrono::milliseconds(2500) );

  std::cout << "Sending sync xml event" << std::endl;
  manco::ZeqManager::instance().publishSyncXml( std::string( "xml" ) );

  std::cout << "Sending sync group event" << std::endl;
  manco::ZeqManager::instance().publishSyncGroup( std::string( "group" ),
    std::string( "randomName" ), std::string( "OWNER" ),
    randomNumbers(5) , 255, 255, 255 );

  while(true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    switch(randomMessage( ))
    {
      case 0:
        std::cout << "Sending destroy event" << std::endl;
        manco::ZeqManager::instance().publishDestroyGroup( std::string( "group" ) );
        break;
      case 1:
        std::cout << "Sending change name event" << std::endl;
        manco::ZeqManager::instance().publishChangeName( std::string( "group" ),
          std::string( "randomName" ) );
        break;
      case 2:
        std::cout << "Sending sync group event" << std::endl;
        manco::ZeqManager::instance().publishSyncGroup( std::string( "group" ),
          std::string( "randomName" ), std::string( "OWNER" ),
          randomNumbers(5), 255, 255, 255 );
        break;
    }
  }
  return 0;
}
