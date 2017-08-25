//
//  scud_custom.cpp
//
//  Created by Boris Vigman on 25/07/2017.
//  Copyright © 2017 Boris Vigman. All rights reserved.
//
/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Affero General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Affero General Public License for more details.
 
 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scud.h"

#ifdef SCUD_CUSTOM_RNG_AVAILABLE
SCUD::SCRng::SCRng(){
        
    };
//returns random number in range 0..RAND_MAX
int SCUD::SCRng::randomInt(){
    return 1;
    };
    //returns random number in range (0..1)
float SCUD::SCRng::randomFloat(){
    return 0.5;
    };
    SCUD::SCRng::~SCRng(){};
#endif

#ifdef SCUD_CUSTOM_MUTEX_AVAILABLE
SCUD::SCLocker::SCLocker(){};
void SCUD::SCLocker::lock(){};
void SCUD::SCLocker::unlock(){};
SCUD::SCLocker::~SCLocker(){};
#endif

#ifdef SCUD_CUSTOM_TIME_AVAILABLE
SCUD::SCTime::SCTime(){
    
}
long long SCUD::SCTime::getTime(){
    return 0;
}
SCUD::SCTime::~SCTime(){
    
}
#endif
