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

      std::string s;

      for ( std::vector<std::string>::const_iterator it = 
        ids.begin( ); it != ids.end( ); ++it )
      {
        s += *it;
        if ( it != ids.end() - 1 )
        {
          s += DELIMITER;
        }
      }

      _publisher->publish( zeroeq::gmrv::SyncGroup( key, name, 
        ZeqManager::getOwner( owner ), 
        s, color ) );
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

}
