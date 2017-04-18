#include "ZeqManager.hpp"
#include <algorithm>
#include <iterator>

namespace manco
{
  zeroeq::Publisher* ZeqManager::_publisher = nullptr;
  zeroeq::Subscriber* ZeqManager::_subscriber = nullptr;
  std::thread ZeqManager::th;

  bool ZeqManager::_listen = true;
  bool ZeqManager::_runThread = true;

  std::function<void( zeroeq::gmrv::ConstSyncGroupPtr )> ZeqManager::_receivedSyncGroupCallback;
  std::function<void( zeroeq::gmrv::ConstChangeColorGroupPtr )> ZeqManager::_receivedChangeColorUpdateCallback;
  std::function<void( zeroeq::gmrv::ConstDestroyGroupPtr )> ZeqManager::_receivedDestroyGroupCallback;
  std::function<void( zeroeq::gmrv::ConstChangeNameGroupPtr )> ZeqManager::_receivedChangeNameGroupUpdateCallback;
  std::function<void( void )> ZeqManager::_receivedSyncNeededCallback;
  
  ZeqManager::~ZeqManager( void )
  {
    ZeqManager::th.join( );
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

  void ZeqManager::close( void )
  {
    _runThread = false;
    th.join();
  }

  zeroeq::Subscriber* ZeqManager::subscriber( void )
  {
    return _subscriber;
  }

  void ZeqManager::init( const std::string& session )
  {
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

    th = std::thread([&](){
      while( _runThread ) 
      {
        //std::cout << "receiveEvents" << std::endl;
        ZeqManager::subscriber( )->receive( 0 );
      }
    });
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
    const std::string& owner, const std::vector<std::string>& ids, 
    const unsigned int& red, const unsigned int& green, const unsigned int& blue )
  {
    if ( _publisher && isListen( ) )
    {
      std::vector<unsigned int> color = { red, green, blue };

      std::string s;

      for ( std::vector<std::string>::const_iterator it = ids.begin( ); it != ids.end( ); ++it )
      {
        s += *it;
        if ( it != ids.end() - 1 )
        {
          s += DELIMITER;
        }
      }

      _publisher->publish( zeroeq::gmrv::SyncGroup( key, name, owner, s, color ) );
    }
  }

  void ZeqManager::publishSyncNeeded( )
  {
    if ( _publisher && isListen( ) )
    {
      _publisher->publish( zeroeq::gmrv::SyncNeeded( ) );
    }
  }
}
