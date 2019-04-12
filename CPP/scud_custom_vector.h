//
//
//  Created by Boris Vigman on 25/08/2017.
//  Copyright © 2017-2019 Boris Vigman. All rights reserved.
//

#ifndef scud_custom_vector_h
#define scud_custom_vector_h
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

#ifdef SCUD_CUSTOM_VECTOR_AVAILABLE
template<typename T> class SCVector{
public:
    void push_back(T& el){};
    T elementAt(long i){
        T t;
        return t;
    }
    long size(){  return -1;  }
    void clear(){    }
};
#endif

#endif /* scud_custom_vector_h */
