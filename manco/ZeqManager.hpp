#ifndef __CSB1_ZEQ_MANAGER__
#define __CSB1_ZEQ_MANAGER__

#include <zeroeq/zeroeq.h>
#include <gmrvlex/gmrvlex.h>

namespace manco
{
  class ZeqManager
  {
  public:
    static void init( const std::string& session );
    ~ZeqManager( void );
    static zeroeq::Subscriber* subscriber( void );

    static std::function<void( zeroeq::gmrv::ConstSyncGroupPtr )> _receivedSyncGroupCallback;
    static std::function<void( zeroeq::gmrv::ConstChangeColorGroupPtr )> _receivedChangeColorUpdateCallback;
    static std::function<void( zeroeq::gmrv::ConstDestroyGroupPtr )> _receivedDestroyGroupCallback;
    static std::function<void( zeroeq::gmrv::ConstChangeNameGroupPtr )> _receivedChangeNameGroupUpdateCallback;
    static std::function<void( void )> _receivedSyncNeededCallback;

    static void publishChangeColor( const std::string& key, const unsigned int& red, 
    const unsigned int& green, const unsigned int& blue );

    static void publishChangeName( const std::string& key, const std::string& name );

    static void publishDestroyGroup( const std::string& key );

    static void publishSyncGroup( const std::string& key, const std::string& name, 
    const std::string& owner, const std::vector<std::string>& ids, 
    const unsigned int& red, const unsigned int& green, const unsigned int& blue );

    static void publishSyncNeeded( );
  protected:
//#ifdef CSB1_ZEQ_USE_ZEROEQ
    static zeroeq::Subscriber* _subscriber;
    static zeroeq::Publisher* _publisher;
//#endif
  };
}

#endif /* __CSB1_ZEQ_MANAGER__ */
