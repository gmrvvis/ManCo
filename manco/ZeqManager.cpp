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

#include "ZeqManager.h"
#include <algorithm>
#include <iterator>

namespace manco
{
  ZeqManager& ZeqManager::instance( )
  {
    static ZeqManager _instance;
    return _instance;
  }
  
  ZeqManager::ZeqManager( void )
    : _isInit( false )
    , _listen( true )
    , _runThread( true )
  {

  }

  ZeqManager::~ZeqManager( void )
  {
    _runThread = false;
    th.join();
  }

  bool ZeqManager::isListen( )
  {
    return _listen;
  }
  void ZeqManager::enableListen( )
  {
    _listen = true;
  }
  void ZeqManager::disableListen( )
  {
    _listen = false;
  }

  zeroeq::Subscriber* ZeqManager::subscriber( void )
  {
    return _subscriber;
  }

  void ZeqManager::init( const std::string& session )
  {
    if (_isInit )
    {
      std::cerr << "error: ZeqManager is already init!" << std::endl;
      return;
    }

    _publisher = new zeroeq::Publisher(
      session.empty() ? zeroeq::DEFAULT_SESSION : session);

    _subscriber = new zeroeq::Subscriber(
      session.empty( ) ? zeroeq::DEFAULT_SESSION : session );

    _subscriber->subscribe(
      vishnulex::SyncGroup::ZEROBUF_TYPE_IDENTIFIER( ),
        static_cast<zeroeq::EventPayloadFunc>( [&]( const void* data,
        const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedSyncGroupCallback )
          {
            _receivedSyncGroupCallback(
                vishnulex::SyncGroup::create( data, size) );
          }
        }
      }));
    _subscriber->subscribe(
      vishnulex::ChangeColorGroup::ZEROBUF_TYPE_IDENTIFIER( ),
        static_cast<zeroeq::EventPayloadFunc>( [&]( const void* data,
        const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedChangeColorUpdateCallback )
          {
            _receivedChangeColorUpdateCallback(
                vishnulex::ChangeColorGroup::create( data, size) );
          }
        }
      }));
    _subscriber->subscribe(
      vishnulex::DestroyGroup::ZEROBUF_TYPE_IDENTIFIER( ),
        static_cast<zeroeq::EventPayloadFunc>( [&]( const void* data,
        const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedDestroyGroupCallback )
          {
            _receivedDestroyGroupCallback(
                vishnulex::DestroyGroup::create( data, size) );
          }
        }
      }));
    _subscriber->subscribe(
      vishnulex::ChangeNameGroup::ZEROBUF_TYPE_IDENTIFIER( ),
        static_cast<zeroeq::EventPayloadFunc>( [&]( const void* data,
        const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedChangeNameGroupUpdateCallback )
          {
            _receivedChangeNameGroupUpdateCallback(
                vishnulex::ChangeNameGroup::create( data, size) );
          }
        }
      }));
    _subscriber->subscribe(
      vishnulex::SyncNeeded::ZEROBUF_TYPE_IDENTIFIER( ),
        static_cast<zeroeq::EventPayloadFunc>( [&]( const void*, const size_t )
      {
        if ( isListen( ) )
        {
          if ( _receivedSyncNeededCallback )
          {
            _receivedSyncNeededCallback( );
          }
        }
      }));
    _subscriber->subscribe(
      vishnulex::SyncXml::ZEROBUF_TYPE_IDENTIFIER( ),
        static_cast<zeroeq::EventPayloadFunc>( [&]( const void* data,
        const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedSyncXmlCallback )
          {
            _receivedSyncXmlCallback(
                vishnulex::SyncXml::create( data, size) );
          }
        }
      }));
    _subscriber->subscribe(
      vishnulex::SyncTransferFunc::ZEROBUF_TYPE_IDENTIFIER( ),
        static_cast<zeroeq::EventPayloadFunc>( [&]( const void* data,
        const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedSyncTransferFuncCallback )
          {
            _receivedSyncTransferFuncCallback(
                vishnulex::SyncTransferFunc::create( data, size) );
          }
        }
      }));

    th = std::thread( [&]( )
    {
      while( _runThread ) 
      {
        //std::cout << "receiveEvents" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        ZeqManager::subscriber( )->receive( 0 );
      }
    });

    _isInit = true;
  }

  void ZeqManager::publishChangeColor( const std::string& key,
    const unsigned int& red, const unsigned int& green,
    const unsigned int& blue )
  {
    if( _publisher && isListen( ) )
    {
      _publisher->publish( vishnulex::ChangeColorGroup( key,
        { red, green, blue } ) );
    }
  }

  void ZeqManager::publishChangeName( const std::string& key,
    const std::string& name )
  {
    if( _publisher && isListen( ) )
    {
      _publisher->publish( vishnulex::ChangeNameGroup( key, name ) );
    }
  }

  void ZeqManager::publishDestroyGroup( const std::string& key )
  {
    if ( _publisher && isListen( ) )
    {
      _publisher->publish( vishnulex::DestroyGroup( key ) );
    }
  }

  void ZeqManager::publishSyncGroup( const std::string& key,
    const std::string& name, const std::string& owner,
    const std::vector<std::string>& ids, const unsigned int& red,
    const unsigned int& green, const unsigned int& blue )
  {
    if ( _publisher && isListen( ) )
    {
      std::vector<unsigned int> color = { red, green, blue };

      std::vector<std::string> _ids = ids;
      ZeqManager::removeEmptyStrings( _ids );

      if ( !_ids.empty( ) )
      {    
        std::string s;

        for ( std::vector<std::string>::const_iterator it = 
          _ids.begin( ); it != _ids.end( ); ++it )
        {
          s += *it;
          if ( it != _ids.end() - 1 )
          {
            s += DELIMITER;
          }
        }

        _publisher->publish( vishnulex::SyncGroup( key, name, 
          owner, s, color ) );
      }
    }
  }

  void ZeqManager::publishSyncNeeded( )
  {
    if ( _publisher && isListen( ) )
    {
      _publisher->publish( vishnulex::SyncNeeded( ) );
    }
  }

  void ZeqManager::publishSyncXml( const std::string& filename )
  {
    if ( _publisher && isListen( ) )
    {
      _publisher->publish( vishnulex::SyncXml( filename ) );
    }
  }

  void ZeqManager::publishSyncTransferFunc( const std::map<std::string,
    float>& scores, const std::vector<vishnulex::Color>& colors )
  {
    if ( _publisher && isListen( ) )
    {
      if ( !scores.empty( ) )
      {    
        std::vector< float > scoreValue;
        std::string idsValue;
        int scoresSize = static_cast<int>( scores.size( ) );

        for ( const auto& score : scores )
        {
          idsValue += score.first;
          scoreValue.push_back( score.second );
          if ( --scoresSize > 0 )
          {
            idsValue += DELIMITER;
          }   
        }        
        _publisher->publish( vishnulex::SyncTransferFunc( colors, scoreValue,
          idsValue ) );
      }
    }
  }

  void ZeqManager::setReceivedSyncGroupCallback( const std::function<void(
    vishnulex::ConstSyncGroupPtr )>& cb )
  {
    _receivedSyncGroupCallback = cb;
  }

  void ZeqManager::setReceivedChangeColorUpdateCallback(
    const std::function<void( vishnulex::ConstChangeColorGroupPtr )>& cb )
  {
    _receivedChangeColorUpdateCallback = cb;
  }

  void ZeqManager::setReceivedDestroyGroupCallback(
    const std::function<void( vishnulex::ConstDestroyGroupPtr )>& cb )
  {
    _receivedDestroyGroupCallback = cb;
  }

  void ZeqManager::setReceivedChangeNameGroupUpdateCallback(
    const std::function<void( vishnulex::ConstChangeNameGroupPtr )>& cb )
  {
    _receivedChangeNameGroupUpdateCallback = cb;
  }

  void ZeqManager::setReceivedSyncNeededCallback(
    const std::function<void( void )>& cb )
  {
    _receivedSyncNeededCallback = cb;
  }

  void ZeqManager::setReceivedSyncXmlCallback(
    const std::function<void( vishnulex::ConstSyncXmlPtr )>& cb )
  {
    _receivedSyncXmlCallback = cb;
  }

  void ZeqManager::setReceivedSyncTransferFuncCallback(
    const std::function<void( vishnulex::ConstSyncTransferFuncPtr )>& cb )
  {
    _receivedSyncTransferFuncCallback = cb;
  }

  std::string ZeqManager::getKeyOwner( const std::string& name,
    const std::string& owner )
  {
    return name + std::string( KEY_DELIMITER ) + owner;
  }
  
  void ZeqManager::removeEmptyStrings( std::vector<std::string>& strings )
  {
    std::vector<std::string>::iterator it = std::remove_if( strings.begin( ),
      strings.end( ), mem_fun_ref( &std::string::empty ) );
    strings.erase( it, strings.end( ) );
  }  

  std::vector<std::string> ZeqManager::split( const std::string& str,
    const std::string& delimiter )
  {
    std::string s = str;
    std::vector<std::string> v;
    size_t pos = 0;
    std::string token;
    while ( ( pos = s.find( delimiter ) ) != std::string::npos )
    {
      token = s.substr( 0, pos );
      v.push_back( token );
      s.erase( 0, pos + delimiter.length( ) );
    }
    if (s != "") 
    {
      v.push_back( s );
    }
    return v;
  }

}
