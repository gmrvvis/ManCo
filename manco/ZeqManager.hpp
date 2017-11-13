/**
 * @file    ZeqManager.hpp
 * @brief
 * @author  Cristian Rodriguez Bernal <ccrisrober@gmail.com>
 * @author  Gonzalo Bayo Martinez <gonzalobayo@gmail.com>
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
 Do not distribute without further notice.
*/

#ifndef __CSB1_ZEQ_MANAGER__
#define __CSB1_ZEQ_MANAGER__

#include <zeroeq/zeroeq.h>
#include <gmrvlex/gmrvlex.h>
#include <thread>
#include <memory>

#include <manco/api.h>

#define DELIMITER "|&|"
#define KEY_DELIMITER "#!#"

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

    MANCO_API static ZeqManager& instance( );

    MANCO_API void init( const std::string& session );

    MANCO_API void destroy();

    MANCO_API zeroeq::Subscriber* subscriber( void );

    MANCO_API void publishChangeColor( const std::string& key,
      const unsigned int& red, const unsigned int& green,
      const unsigned int& blue );

    MANCO_API void publishChangeName( const std::string& key,
      const std::string& name );

    MANCO_API void publishDestroyGroup( const std::string& key );

    MANCO_API void publishSyncGroup( const std::string& key,
      const std::string& name, const ApplicationType& owner,
      const std::vector<std::string>& ids, const unsigned int& red,
      const unsigned int& green, const unsigned int& blue );

    MANCO_API void publishSyncNeeded( void );

    MANCO_API void publishSyncXml( const std::string& filename );

    MANCO_API void publishSyncTransferFunc( const std::map<std::string,
      float>& scores, const std::vector<zeroeq::gmrv::Color>& colors );

    MANCO_API void setReceivedSyncGroupCallback( 
      const std::function<void( zeroeq::gmrv::ConstSyncGroupPtr )>& cb );
    
    MANCO_API void setReceivedChangeColorUpdateCallback( 
      const std::function<void( zeroeq::gmrv::ConstChangeColorGroupPtr )>& cb );
    
    MANCO_API void setReceivedDestroyGroupCallback( 
      const std::function<void( zeroeq::gmrv::ConstDestroyGroupPtr )>& cb );

    MANCO_API void setReceivedChangeNameGroupUpdateCallback( 
      const std::function<void( zeroeq::gmrv::ConstChangeNameGroupPtr )>& cb );

    MANCO_API void setReceivedSyncNeededCallback( 
      const std::function<void( void )>& cb );

    MANCO_API void setReceivedSyncXmlCallback( 
      const std::function<void( zeroeq::gmrv::ConstSyncXmlPtr )>& cb );

    MANCO_API void setReceivedSyncTransferFuncCallback(
      const std::function<void( zeroeq::gmrv::ConstSyncTransferFuncPtr )>& cb );

    MANCO_API static std::string getOwner( ApplicationType cad );

    MANCO_API static std::string getKeyOwner( const std::string& name,
      const ApplicationType& owner );

    MANCO_API bool isListen( void );

    MANCO_API void enableListen( void );

    MANCO_API void disableListen( void );
    
    MANCO_API static void removeEmptyStrings(
      std::vector<std::string>& strings );

    MANCO_API static std::vector<std::string> split( const std::string& str,
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

    std::function<void( zeroeq::gmrv::ConstSyncGroupPtr )>
      _receivedSyncGroupCallback;
    std::function<void( zeroeq::gmrv::ConstChangeColorGroupPtr )>
      _receivedChangeColorUpdateCallback;
    std::function<void( zeroeq::gmrv::ConstDestroyGroupPtr )>
      _receivedDestroyGroupCallback;
    std::function<void( zeroeq::gmrv::ConstChangeNameGroupPtr )>
      _receivedChangeNameGroupUpdateCallback;
    std::function<void( void )> _receivedSyncNeededCallback;
    std::function<void( zeroeq::gmrv::ConstSyncXmlPtr )>
      _receivedSyncXmlCallback;
    std::function<void( zeroeq::gmrv::ConstSyncTransferFuncPtr )>
      _receivedSyncTransferFuncCallback;

  };
}

#endif /* __CSB1_ZEQ_MANAGER__ */
