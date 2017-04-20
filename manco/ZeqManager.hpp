#ifndef __CSB1_ZEQ_MANAGER__
#define __CSB1_ZEQ_MANAGER__

#include <zeroeq/zeroeq.h>
#include <gmrvlex/gmrvlex.h>
#include <thread>

#define DELIMITER "|&|"

namespace manco
{
  enum ApplicationType
  {
    APICOLAT,
    SPINERET,
    CLINT
  };

  class ZeqManager
  {
  public:

    static ZeqManager& instance();

    void init( const std::string& session );
    zeroeq::Subscriber* subscriber( void );

    void publishChangeColor( const std::string& key, const unsigned int& red,
      const unsigned int& green, const unsigned int& blue );

    void publishChangeName( const std::string& key, const std::string& name );

    void publishDestroyGroup( const std::string& key );

    void publishSyncGroup( const std::string& key, const std::string& name,
      const ApplicationType& owner, const std::vector<std::string>& ids,
      const unsigned int& red, const unsigned int& green, 
      const unsigned int& blue );

    void publishSyncNeeded( void );

    void publishSyncXml( const std::string& filename );

    void setReceivedSyncGroupCallback( 
      const std::function<void( zeroeq::gmrv::ConstSyncGroupPtr )>& cb);
    void setReceivedChangeColorUpdateCallback( 
      const std::function<void( zeroeq::gmrv::ConstChangeColorGroupPtr )>& cb);
    void setReceivedDestroyGroupCallback( 
      const std::function<void( zeroeq::gmrv::ConstDestroyGroupPtr )>& cb);
    void setReceivedChangeNameGroupUpdateCallback( 
      const std::function<void( zeroeq::gmrv::ConstChangeNameGroupPtr )>& cb);
    void setReceivedSyncNeededCallback( 
      const std::function<void( void )>& cb );
    void setReceivedSyncXmlCallback( 
      const std::function<void( zeroeq::gmrv::ConstSyncXmlPtr )>& cb);

    static std::string getOwner( ApplicationType cad );

    bool isListen( void );
    void enableListen( void );
    void disableListen( void );

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

    std::function<void( zeroeq::gmrv::ConstSyncGroupPtr )> _receivedSyncGroupCallback;
    std::function<void( zeroeq::gmrv::ConstChangeColorGroupPtr )> _receivedChangeColorUpdateCallback;
    std::function<void( zeroeq::gmrv::ConstDestroyGroupPtr )> _receivedDestroyGroupCallback;
    std::function<void( zeroeq::gmrv::ConstChangeNameGroupPtr )> _receivedChangeNameGroupUpdateCallback;
    std::function<void( void )> _receivedSyncNeededCallback;
    std::function<void( zeroeq::gmrv::ConstSyncXmlPtr )> _receivedSyncXmlCallback;
  };
}

#endif /* __CSB1_ZEQ_MANAGER__ */
