//
//  Created by Boris Vigman on 25/08/2017.
//  Copyright © 2017-2019 Boris Vigman. All rights reserved.
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
    //get current size
    long long size(){return 0;};
    //get content referenced by iterator
    Container getCurrentContent(){
        Container c;
        return c;
    }
    //get ID of item referenced by iterator
    Tid getCurrentId(){
        Tid t;
        return t;
    }
    //set new content for item with given ID
    void setContent(Tid& t,Container& c){

    }
    //reset iterator making int point to the beginning
    void resetIterator(){

    };
    //insert new item
    void insert(Tid& tid,Container& c){

    };
    //clear the map
    void clear(){

    }
    //erase item with given ID
    void erase(Tid& t){

    };
    //find item with given ID; return status of search and item;
    Container find(Tid& t,bool& res){
        Container c;
        return c;
    };
    //if iterator visited all items of map return true, otherwise false
    bool isExgausted(){
        return true;
    }
    //return true if item with given ID exists, false otherwise
    bool exists(Tid& id){
        return false;
    }
    //promote iterator
    void promoteIterator(){}
    //promotoe iterator with guarantee that it references valid object for non-empty map
    void promoteIteratorSafely(){};
};
#endif

#endif /* scud_custom_map_h */
