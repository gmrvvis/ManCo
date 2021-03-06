/* 
 * Copyright (c) 2017-2019 GMRV/URJC.
 * 
 * Authors: Cristian Rodriguez Bernal <cristian.rodriguez@urjc.es>
 *          Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of his software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <manco/manco.h>
using namespace manco;

#include <iostream>

#include <chrono>
#include <thread>
#include <ctime>

int randomMessage( )
{
  static int min = 0;
  static int max = 3;
  return min + ( rand( ) % static_cast<int>( max - min + 1 ) );
}

int randomColorChannel( )
{
  static int min = 0;
  static int max = 255;
  return min + ( rand( ) % static_cast<int>( max - min + 1 ) );
}

std::vector<std::string> randomNumbers( const int& n )
{
  std::vector<std::string> numbers;
  static int min = 0;
  static int max = 1000;

  for ( int i = 0; i < n; ++i )
  {
    std::string number = std::to_string( min + ( rand( ) %
      static_cast<int>( max - min + 1 ) ) );
    numbers.push_back( number );
  }
  return numbers;
}

std::vector<std::string> customRandom( )
{
  std::vector<std::string> numbers;
  for ( int i = 0; i < randomColorChannel( ); ++i )
  {
    int r = randomMessage( ) + 1;

    std::string name = std::string("B") + std::to_string( r );
    name += "-700000";

    static int min = 0;
    int max = 200;
    if ( r == 1 ) 
    {
      max = 500;
    }
    else if ( r == 2 ) 
    {
      max = 500;
    }
    else if ( r == 3 ) 
    {
      max = 980;
    }
    else if ( r == 4 ) 
    {
      max = 990;
    }
    r = min + ( rand() % static_cast<int>( max - min + 1 ) );
    std::string number = std::to_string( r );
    if ( r > 10 && r < 100 )
    {
      number = std::string( "0" ) + number;
    }
    else if ( r < 10 )
    {
      number = std::string( "00" ) + number;
    }
    numbers.push_back( name + number );
  }
  return numbers; // 100-200
}

int main( int argc, char** argv )
{

  std::cout << "HELLO PRODUCER" << std::endl;

  if( argc < 2 )
  {
    std::cout << "mancoProducer <session>" << std::endl;
    return -1;
  }

  manco::ZeqManager::instance( ).init( argv[ 1 ] );

  std::this_thread::sleep_for( std::chrono::milliseconds( 2500 ) );

  //Example of Sync xml event
  std::cout << "Sending sync xml event" << std::endl;
  manco::ZeqManager::instance( ).publishSyncXml( std::string( "xml" ) );

  //Example of Sync transfer function event
  std::cout << "Sending sync transfer func event" << std::endl;
  //std::vector<vishnulex::Color> colors;

  std::map<std::string, float> scores;
  scores["B-123213"] = 0.343f;
  scores["B-456546"] = 0.6667f;
  scores["B-345564"] = 0.966f;
  //colors.push_back( new vishnulex::Color( 2, 3, 4 ) );
  manco::ZeqManager::instance( ).publishSyncTransferFunc( scores, {
    {1, 2, 3}, {254, 4, 0}, {124, 50, 27}, {231, 0, 66}   
  } );

  std::cout << "Sending sync group event" << std::endl;
  manco::ZeqManager::instance().publishSyncGroup( std::string( "group" ),
    std::string( "randomName" ), "CLINT", customRandom( ),
    randomColorChannel( ), randomColorChannel( ), randomColorChannel( ) );

  while( true )
  {
    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    switch( randomMessage( ) )
    {
      case 0:
        std::cout << "Sending destroy event" << std::endl;
        manco::ZeqManager::instance().publishDestroyGroup(
          std::string( "group" ) );
        break;
      case 1:
        std::cout << "Sending change name event" << std::endl;
        manco::ZeqManager::instance().publishChangeName( std::string( "group" ),
          std::string( "randomName" ) );
        break;
      case 2:
        std::string gName = std::to_string( static_cast<int>( time( NULL ) ) );
        std::cout << "Sending sync group event" << std::endl;
        manco::ZeqManager::instance().publishSyncGroup(
          manco::ZeqManager::getKeyOwner( gName, "CLINT" ), gName,
          "PYRAMIDAL", customRandom( ),
          randomColorChannel( ), randomColorChannel( ), randomColorChannel( ) );
        break;
      /*case 3:
        std::cout << "Sending sync transfer func event" << std::endl;
        std::vector<vishnulex::Color> colors;
        colors.push_back( ( float )randomColorChannel( ),
          ( float )randomColorChannel( ), ( float )randomColorChannel( ) );
        manco::ZeqManager::instance().publishSyncTransferFunc( { 
          {1, 2, 3}, {254, 4, 0}, {124, 50, 27}, {231, 0, 66}   
        } );      
        break;
*/
    }
  }
  return 0;
}
