#include <manco/manco.h>
using namespace manco;

#include <iostream>


#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>

void receiveDestroyGroup(zeroeq::gmrv::ConstDestroyGroupPtr)
{
  std::cout << "RECEIVED DESTROY GROUP" << std::endl;
}
void receiveChangeNameGroup(zeroeq::gmrv::ConstChangeNameGroupPtr)
{
  std::cout << "RECEIVED CHANGE NAME GROUP" << std::endl;
}

int main( )
{
  std::cout << "HELLO CONSUMER" << std::endl;
  manco::ZeqManager::init( "hbp://" );

  manco::ZeqManager::_receivedDestroyGroupCallback = receiveDestroyGroup;//std::bind(&example);
  manco::ZeqManager::_receivedChangeNameGroupUpdateCallback = receiveChangeNameGroup;//std::bind(&example);

  while(true) 
  {
  }
  return 0;
}
