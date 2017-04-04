#include <manco/manco.h>
using namespace manco;

#include <iostream>


#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>

void example(zeroeq::gmrv::ConstDestroyGroupPtr)
{
	std::cout << "RECEIVED DESTROY GROUP" << std::endl;
}

void print_num(int i)
{
    std::cout << i << '\n';
}

int main( )
{
  std::cout << "HELLO CONSUMER" << std::endl;
  manco::ZeqManager::init( "hbp://" );

  manco::ZeqManager::_receivedDestroyGroupCallback = example;//std::bind(&example);

  while(true) 
  {
  }
  return 0;
}
