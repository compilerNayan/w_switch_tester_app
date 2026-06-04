#ifndef _SWITCH_REPOSITORY_H_
#define _SWITCH_REPOSITORY_H_

#include <StandardDefines.h>
#include "CpaRepository.h"
#include "../entity/Switch.h"

/* @Repository */
DefineStandardPointers(SwitchRepository)
class SwitchRepository : public CpaRepository<Switch, int> {
    Public Virtual ~SwitchRepository() = default;
    
    // Additional custom repository methods can be added here
    // The base CRUD methods are inherited from CpaRepository
};

#endif // _SWITCH_REPOSITORY_H_

