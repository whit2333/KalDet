
#include "MeasurementSurfaceStore.h"

#include <stdexcept>
#include <vector>
#include <algorithm>

#include "MeasurementSurface.h"

namespace GearExtensions{

  
  bool MeasurementSurfaceStore::_isInitialised = false ;
  
  MeasurementSurfaceStore::~MeasurementSurfaceStore(){
    
    ms_map_it it = _measurement_surface_map.begin();
    std::vector<MeasurementSurface*> deleted_objects;
    
    for( /**/; it!=_measurement_surface_map.end(); ++it) 
      
      if( std::find( deleted_objects.begin(), deleted_objects.end(), (*it).second ) != deleted_objects.end() ) {
        delete (*it).second ;
        deleted_objects.push_back((*it).second) ;
      }
  }
  
  MeasurementSurface* MeasurementSurfaceStore::GetMeasurementSurface(int ID) const {
    
    ms_map_it it = _measurement_surface_map.find(ID) ;        
    
    if ( it == _measurement_surface_map.end() ) { 
      MeasurementSurfaceStoreException exp;
      throw exp ; 
    } 
    else { 
      return (*it).second ; 
    }
    
  }
  
  void MeasurementSurfaceStore::initialise(gear::GearMgr* gear_mrg){
    
    if ( ! _isInitialised) {
      this->createStore(gear_mrg); 
      _isInitialised = true ;
    }
    
    
  }
  
  void MeasurementSurfaceStore::addMeasurementSurface(MeasurementSurface* ms) {
    
    int ID = ms->getID();
    
    ms_map_it it = _measurement_surface_map.find(ID) ; 
    
    MeasurementSurfaceStoreException exp;
    
    if ( it != _measurement_surface_map.end() ) { 
      throw exp; 
    } 
    else { 
      _measurement_surface_map[ID] = ms  ; 
    }
  }
  
  
  void MeasurementSurfaceStore::createStore(gear::GearMgr* gear_mrg){
    
  }
  
}
