//
//  test_SCUD
//
//  Created by Boris Vigman on 25/08/2017.
//  Copyright © 2017 bv. All rights reserved.
//

#ifndef scud_custom_map_h
#define scud_custom_map_h
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

#ifdef SCUD_CUSTOM_MAP_AVAILABLE
template<typename Tid, typename Container> class SCMap{
public:
    SCMap(){
        
    }
    long long size(){return 0;};
    
    Container getCurrentContent(){
        Container c;
        return c;
    }
    Tid getCurrentId(){
        Tid t;
        return t;
    }
    void setContent(Tid& t,Container& c){

    }
    void resetIterator(){

    };
    void insert(Tid& tid,Container& c){

    };
    void clear(){

    }
    void erase(Tid& t){

    };
    Container find(Tid& t,bool& res){
        Container c;
        return c;
    };
    bool isExgausted(){
        return true;
    }
    bool exists(Tid& id){
        return false;
    }
    void promoteIterator(){}
};
#endif

#endif /* scud_custom_map_h */
