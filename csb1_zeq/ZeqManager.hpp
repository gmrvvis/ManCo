#ifndef __CSB1_ZEQ_ZEQ_MANAGER__
#define __CSB1_ZEQ_ZEQ_MANAGER__

#include <zeroeq/zeroeq.h>
#include <gmrvlex/gmrvlex.h>

#include <QObject>
#include <QTimer>

namespace csb1_zeq
{
	class SubscriberTimer: public QTimer
	{
		Q_OBJECT
	public:
		SubscriberTimer( void );
	public slots:
		void receiveEvents( void );
	};

	class ZeqManager
	{
	public:
		static void init( const std::string& /*session*/ );
		~ZeqManager( void );
        static zeroeq::Subscriber* subscriber( void );
		
		static void _receivedSyncGroupCallback( zeroeq::gmrv::ConstSyncGroupPtr );
		static void _changeColorUpdateCallback( zeroeq::gmrv::ConstChangeColorGroupPtr );
		static void _receivedDestroyGroupCallback( zeroeq::gmrv::ConstDestroyGroupPtr );
		static void _changeNameGroupUpdateCallback( zeroeq::gmrv::ConstChangeNameGroupPtr );

		static void _receivedSyncNeededCallback( zeroeq::gmrv::ConstSyncNeededPtr );

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
		static SubscriberTimer* _timer;
	};
}

#endif /* __CSB1_ZEQ_ZEQ_MANAGER__ */
