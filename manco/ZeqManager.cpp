/**
 * @file    ZeqManager.cpp
 * @brief
 * @author  Cristian Rodriguez Bernal <ccrisrober@gmail.com>
 * @author  Gonzalo Bayo Martinez <gonzalobayo@gmail.com>
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
 Do not distribute without further notice.
*/


#include "ZeqManager.hpp"
#include <algorithm>
#include <iterator>

namespace manco
{
  ZeqManager& ZeqManager::instance()
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
    if (_isInit)
    {
      std::cerr << "error: ZeqManager is already init!" << std::endl;
      return;
    }

    _publisher = new zeroeq::Publisher(
      session.empty( ) ? zeroeq::DEFAULT_SESSION : session );

    _subscriber = new zeroeq::Subscriber(
      session.empty( ) ? zeroeq::DEFAULT_SESSION : session );

    _subscriber->subscribe(
      zeroeq::gmrv::SyncGroup::ZEROBUF_TYPE_IDENTIFIER( ),
      [&]( const void* data, const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedSyncGroupCallback )
          {
            _receivedSyncGroupCallback(
                zeroeq::gmrv::SyncGroup::create( data, size));
          }
        }
      });
    _subscriber->subscribe(
      zeroeq::gmrv::ChangeColorGroup::ZEROBUF_TYPE_IDENTIFIER( ),
      [&]( const void* data, const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedChangeColorUpdateCallback )
          {
            _receivedChangeColorUpdateCallback(
                zeroeq::gmrv::ChangeColorGroup::create( data, size));
          }
        }
      });
    _subscriber->subscribe(
      zeroeq::gmrv::DestroyGroup::ZEROBUF_TYPE_IDENTIFIER( ),
      [&]( const void* data, const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedDestroyGroupCallback )
          {
            _receivedDestroyGroupCallback(
                zeroeq::gmrv::DestroyGroup::create( data, size));
          }
        }
      });
    _subscriber->subscribe(
      zeroeq::gmrv::ChangeNameGroup::ZEROBUF_TYPE_IDENTIFIER( ),
      [&]( const void* data, const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedChangeNameGroupUpdateCallback )
          {
            _receivedChangeNameGroupUpdateCallback(
                zeroeq::gmrv::ChangeNameGroup::create( data, size));
          }
        }
      });
    _subscriber->subscribe(
      zeroeq::gmrv::SyncNeeded::ZEROBUF_TYPE_IDENTIFIER( ),
      [&]( const void*, const size_t )
      {
        if ( isListen( ) )
        {
          if ( _receivedSyncNeededCallback )
          {
            _receivedSyncNeededCallback( );
          }
        }
      });
    _subscriber->subscribe(
      zeroeq::gmrv::SyncXml::ZEROBUF_TYPE_IDENTIFIER( ),
      [&]( const void* data, const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedSyncXmlCallback )
          {
            _receivedSyncXmlCallback(
                zeroeq::gmrv::SyncXml::create( data, size));
          }
        }
      });
    _subscriber->subscribe(
      zeroeq::gmrv::SyncTransferFunc::ZEROBUF_TYPE_IDENTIFIER( ),
      [&]( const void* data, const size_t size )
      {
        if ( isListen( ) )
        {
          if ( _receivedSyncTransferFuncCallback )
          {
            _receivedSyncTransferFuncCallback(
                zeroeq::gmrv::SyncTransferFunc::create( data, size));
          }
        }
      });

    th = std::thread([&](){
      while( _runThread ) 
      {
        //std::cout << "receiveEvents" << std::endl;
        ZeqManager::subscriber( )->receive( 0 );
      }
    });

    _isInit = true;
  }

  void ZeqManager::publishChangeColor( const std::string& key, const unsigned int& red, 
    const unsigned int& green, const unsigned int& blue )
  {
    if( _publisher && isListen( ) )
    {
      _publisher->publish( zeroeq::gmrv::ChangeColorGroup( key, { red, green, blue } ));
    }
  }

  void ZeqManager::publishChangeName( const std::string& key, const std::string& name )
  {
    if( _publisher && isListen( ) )
    {
      _publisher->publish( zeroeq::gmrv::ChangeNameGroup( key, name ));
    }
  }

  void ZeqManager::publishDestroyGroup( const std::string& key )
  {
    if ( _publisher && isListen( ) )
    {
      _publisher->publish( zeroeq::gmrv::DestroyGroup( key ) );
    }
  }

  void ZeqManager::publishSyncGroup( const std::string& key, const std::string& name, 
    const ApplicationType& owner, const std::vector<std::string>& ids, 
    const unsigned int& red, const unsigned int& green, const unsigned int& blue )
  {
    if ( _publisher && isListen( ) )
    {
      std::vector<unsigned int> color = { red, green, blue };

      std::vector<std::string> _ids = ids;
      ZeqManager::removeEmptyStrings(_ids);

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

        _publisher->publish( zeroeq::gmrv::SyncGroup( key, name, 
          ZeqManager::getOwner( owner ), 
          s, color ) );
      }
    }
  }

  void ZeqManager::publishSyncNeeded( )
  {
    if ( _publisher && isListen( ) )
    {
      _publisher->publish( zeroeq::gmrv::SyncNeeded( ) );
    }
  }

  void ZeqManager::publishSyncXml( const std::string& filename )
  {
    if ( _publisher && isListen( ) )
    {
      _publisher->publish( zeroeq::gmrv::SyncXml( filename ) );
    }
  }

  void ZeqManager::publishSyncTransferFunc( const std::map<std::string, float>& scores, const std::vector<zeroeq::gmrv::Color>& colors )
  {
    if ( _publisher && isListen( ) )
    {
      if ( !scores.empty( ) )
      {    
        std::vector< float > scoreValue;
        std::string idsValue;
        int scoresSize = ( int )scores.size( );

        for ( const auto& score : scores )
        {
          idsValue += score.first;
          scoreValue.push_back( score.second );
          if ( --scoresSize > 0 )
          {
            idsValue += DELIMITER;
          }   
        }        
        _publisher->publish( zeroeq::gmrv::SyncTransferFunc( colors, scoreValue, idsValue ) );
      }
    }
  }

  void ZeqManager::setReceivedSyncGroupCallback( const std::function<void( zeroeq::gmrv::ConstSyncGroupPtr )>& cb)
  {
    _receivedSyncGroupCallback = cb;
  }

  void ZeqManager::setReceivedChangeColorUpdateCallback( const std::function<void( zeroeq::gmrv::ConstChangeColorGroupPtr )>& cb)
  {
    _receivedChangeColorUpdateCallback = cb;
  }

  void ZeqManager::setReceivedDestroyGroupCallback( const std::function<void( zeroeq::gmrv::ConstDestroyGroupPtr )>& cb)
  {
    _receivedDestroyGroupCallback = cb;
  }

  void ZeqManager::setReceivedChangeNameGroupUpdateCallback( const std::function<void( zeroeq::gmrv::ConstChangeNameGroupPtr )>& cb)
  {
    _receivedChangeNameGroupUpdateCallback = cb;
  }

  void ZeqManager::setReceivedSyncNeededCallback( const std::function<void( void )>& cb )
  {
    _receivedSyncNeededCallback = cb;
  }

  void ZeqManager::setReceivedSyncXmlCallback( const std::function<void( zeroeq::gmrv::ConstSyncXmlPtr )>& cb)
  {
    _receivedSyncXmlCallback = cb;
  }

  void ZeqManager::setReceivedSyncTransferFuncCallback( const std::function<void( zeroeq::gmrv::ConstSyncTransferFuncPtr )>& cb)
  {
    _receivedSyncTransferFuncCallback = cb;
  }

  std::string ZeqManager::getOwner( ApplicationType cad )
  {
    std::string result;
    switch(cad)
    {
      case APICOLAT:
      {
        result = std::string( "APICOLAT" );
        break;
      }
      case SPINERET:
      {
        result = std::string( "SPINERET" );
        break;
      }
      case CLINT:
      {
        result = std::string( "CLINT" );
        break;
      }
    }
    return result;
  }
  std::string ZeqManager::getKeyOwner( const std::string& name,
      const ApplicationType& owner )
  {
    return name + std::string( KEY_DELIMITER )
      + ZeqManager::getOwner( owner );
  }
  
  void ZeqManager::removeEmptyStrings(std::vector<std::string>& strings)
  {
    std::vector<std::string>::iterator it = std::remove_if(strings.begin(), strings.end(),mem_fun_ref(&std::string::empty));
    strings.erase(it, strings.end());
  }  

  std::vector<std::string> ZeqManager::split( const std::string& str, const std::string& delimiter )
  {
    std::string s = str;
    std::vector< std::string > v;
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
