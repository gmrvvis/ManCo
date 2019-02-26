/* 
 * Copyright (c) 2017-2019 GMRV/URJC.
 * 
 * Authors: Cristian Rodriguez Bernal <ccrisrober@gmail.com>
 *          Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 * 
 * This file is part of ManCo <https://gitlab.gmrv.es/retrieval/manco>
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

#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>

#include <chrono>
#include <thread>

void receivedDestroyGroup( vishnulex::ConstDestroyGroupPtr o )
{
  std::cout << "Received DestroyGroup (" << o->getKeyString( ) << ")" <<
    std::endl;
}

void receivedChangeNameGroup( vishnulex::ConstChangeNameGroupPtr )
{
  std::cout << "Received ChangeNameGroup" << std::endl;
}

void receivedSyncGroup( vishnulex::ConstSyncGroupPtr o )
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

void receivedChangeColor( vishnulex::ConstChangeColorGroupPtr o )
{
  std::vector<unsigned int> color = o->getColorVector( );
  std::cout << "Received ChangeColor (" << o->getKeyString( ) << ": " <<
    color[0] << ", " << color[1] << ", "  << color[2] <<
    ")" << std::endl;
}

void receivedSyncXml( vishnulex::ConstSyncXmlPtr o )
{
  std::cout << "Received SyncXml (" << o->getFilenameString( ) << ")" <<
    std::endl;
}

void receivedSyncTransferFunc( vishnulex::ConstSyncTransferFuncPtr o )
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
