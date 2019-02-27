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

#ifndef MANCO_ZEQ_MANAGER_H
#define MANCO_ZEQ_MANAGER_H

#include <zeroeq/zeroeq.h>
#include <vishnulex/vishnulex.h>
#include <thread>
#include <memory>

#include <manco/api.h>

#include "Definitions.hpp"

namespace manco
{
  class ZeqManager
  {
  public:

    MANCO_API 
      static ZeqManager& instance( );

    MANCO_API 
      void init( const std::string& session );

    MANCO_API 
      zeroeq::Subscriber* subscriber( void );

    MANCO_API 
      void publishChangeColor( const std::string& key,
      const unsigned int& red, const unsigned int& green,
      const unsigned int& blue );

    MANCO_API 
      void publishChangeName( const std::string& key,
      const std::string& name );

    MANCO_API 
      void publishDestroyGroup( const std::string& key );

    MANCO_API 
      void publishSyncGroup( const std::string& key,
      const std::string& name, const std::string& owner,
      const std::vector<std::string>& ids, const unsigned int& red,
      const unsigned int& green, const unsigned int& blue );

    MANCO_API 
      void publishSyncNeeded( void );

    MANCO_API 
      void publishSyncXml( const std::string& filename );

    MANCO_API 
      void publishSyncTransferFunc( const std::map<std::string,
      float>& scores, const std::vector<vishnulex::Color>& colors );

    MANCO_API 
      void setReceivedSyncGroupCallback( 
      const std::function<void( vishnulex::ConstSyncGroupPtr )>& cb );
    
    MANCO_API 
      void setReceivedChangeColorUpdateCallback( 
      const std::function<void( vishnulex::ConstChangeColorGroupPtr )>& cb );
    
    MANCO_API 
      void setReceivedDestroyGroupCallback( 
      const std::function<void( vishnulex::ConstDestroyGroupPtr )>& cb );

    MANCO_API 
      void setReceivedChangeNameGroupUpdateCallback( 
      const std::function<void( vishnulex::ConstChangeNameGroupPtr )>& cb );

    MANCO_API 
      void setReceivedSyncNeededCallback( 
      const std::function<void( void )>& cb );

    MANCO_API 
      void setReceivedSyncXmlCallback( 
      const std::function<void( vishnulex::ConstSyncXmlPtr )>& cb );

    MANCO_API 
      void setReceivedSyncTransferFuncCallback(
      const std::function<void( vishnulex::ConstSyncTransferFuncPtr )>& cb );

    MANCO_API 
      static std::string getKeyOwner( const std::string& name,
      const std::string& owner );

    MANCO_API 
      bool isListen( void );

    MANCO_API 
      void enableListen( void );

    MANCO_API 
      void disableListen( void );
    
    MANCO_API 
      static void removeEmptyStrings(
      std::vector<std::string>& strings );

    MANCO_API 
      static std::vector<std::string> split( const std::string& str,
      const std::string& delimiter );

  protected:
    bool _isInit;
    bool _listen;
    bool _runThread;
    zeroeq::Subscriber* _subscriber;
    zeroeq::Publisher* _publisher;
    std::thread th;

  private:
    ZeqManager( void );
    ~ZeqManager( void );
    ZeqManager( ZeqManager const& ) = delete;
    void operator=( ZeqManager const& ) = delete;

    std::function<void( vishnulex::ConstSyncGroupPtr )>
      _receivedSyncGroupCallback;
    std::function<void( vishnulex::ConstChangeColorGroupPtr )>
      _receivedChangeColorUpdateCallback;
    std::function<void( vishnulex::ConstDestroyGroupPtr )>
      _receivedDestroyGroupCallback;
    std::function<void( vishnulex::ConstChangeNameGroupPtr )>
      _receivedChangeNameGroupUpdateCallback;
    std::function<void( void )> _receivedSyncNeededCallback;
    std::function<void( vishnulex::ConstSyncXmlPtr )>
      _receivedSyncXmlCallback;
    std::function<void( vishnulex::ConstSyncTransferFuncPtr )>
      _receivedSyncTransferFuncCallback;

  };
}

#endif /* MANCO_ZEQ_MANAGER */
