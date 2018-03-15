#ifndef MANCO_ENUMS_HPP
#define MANCO_ENUMS_HPP

#include <string>
#include <assert.h>

#include "Definitions.hpp"

namespace manco
{

  enum class ApplicationType
  {
    CLINT,
    DCEXPLORER,
    PYRAMIDAL
  };

  inline static ApplicationType toApplicationType( const std::string& appType )
  {
    if ( appType == STR_CLINT )
    {
      return ApplicationType::CLINT;
    }
    else if ( appType == STR_DCEXPLORER )
    {
      return ApplicationType::DCEXPLORER;
    }
    else if ( appType == STR_PYRAMIDAL )
    {
      return ApplicationType::PYRAMIDAL;
    }
    else
    {
      assert( 0 && "Error: Invalid ApplicationType enum constant!" );
      exit( -1 );
    }
  }

  inline static std::string toString( const ApplicationType& appType )
  {
    std::string result;
    switch( appType )
    {
      case ApplicationType::CLINT:
        result = STR_CLINT;
        break;
      case ApplicationType::DCEXPLORER:
        result = STR_DCEXPLORER;
        break;
      case ApplicationType::PYRAMIDAL:
        result = STR_PYRAMIDAL;
        break;
    }
    return result;
  }

}

#endif
