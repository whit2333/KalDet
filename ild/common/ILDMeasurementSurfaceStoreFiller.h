#ifndef ILDMEASUREMENTSURFACESTOREFILLER_H
#define ILDMEASUREMENTSURFACESTOREFILLER_H

#include "gearsurf/MeasurementSurfaceStore.h"

#include <vector>

namespace gear{
  class GearMgr;
  class ZPlanarParameters;
  class FTDParameters;
}

using namespace gear;

class ILDMeasurementSurfaceStoreFiller : public MeasurementSurfaceStoreFiller{
  
  public:
  
  ILDMeasurementSurfaceStoreFiller(const gear::GearMgr& gear_mgr) {

    this->get_gear_parameters(gear_mgr);
    
  }
    
  void getMeasurementSurfaces( std::vector<MeasurementSurface*>& surface_list ) const;
  
  std::string getName() const { return "ILDMeasurementSurfaceStoreFiller" ; } ;    
  
  private:
  
  /** adds MeasurementSufaces to the store
   * @param param: the ZPlanarParameters pointer of the detector, of which the measurement surfaces shall be added
   * 
   * @param det_id: the detector id (as in ILDConf)
   */
  void storeZPlanar( const gear::ZPlanarParameters* param , int det_id, std::vector<MeasurementSurface*>& surface_list ) const;
  
  void storeFTD( const gear::FTDParameters* param, std::vector<MeasurementSurface*>& surface_list ) const;
  
   
  void get_gear_parameters(const gear::GearMgr& gear_mgr);
  
#define HARDCODEDGEAR 1
#ifdef HARDCODEDGEAR
  
  
  /** the strip angles for every layer */
  std::vector< double > _VTXStripAngles;
  std::vector< double > _SITStripAngles;
  std::vector< double > _SETStripAngles;
  
  /** the strip angles for every layer and sensor */
  std::vector< std::vector< double > > _FTDStripAngles;
  
  unsigned _nVTXLayers;
  unsigned _nSITLayers;
  unsigned _nFTDLayers;
  unsigned _nSETLayers;
  
  const gear::ZPlanarParameters* _paramVXD;
  const gear::ZPlanarParameters* _paramSIT;
  const gear::ZPlanarParameters* _paramSET;
  const gear::FTDParameters* _paramFTD;
  
#endif
  
};

#endif
