#include <CrcUtils.h>
#include <CrcXbee.h>
#include <CrcLib.h>


// patches to give us public access to crclib internals
namespace CrcLib
{
    CrcUtility::CrcXbee CrcLib::_crcXbee;

}
