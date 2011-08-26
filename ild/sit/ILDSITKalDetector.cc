
#include "ILDSITKalDetector.h"

#include "MaterialDataBase.h"

#include "ILDPlanarMeasLayer.h"
#include "ILDPlanarHit.h"

#include <UTIL/BitField64.h>
#include <UTIL/ILDConf.h>

#include <gear/GEAR.h>
#include "gear/BField.h"
#include <gear/ZPlanarParameters.h>
#include <gear/ZPlanarLayerLayout.h>
#include "gearimpl/Util.h"

#include "TMath.h"

#include "math.h"
#include <sstream>

#include "streamlog/streamlog.h"

ILDSITKalDetector::ILDSITKalDetector( const gear::GearMgr& gearMgr )
                : TVKalDetector(300) // SJA:FIXME initial size, 300 looks reasonable for ILD, though this would be better stored as a const somewhere
{
   

  streamlog_out(DEBUG4) << "ILDSITKalDetector building SIT detector using GEAR " << std::endl ;

  TMaterial & air       = *MaterialDataBase::Instance().getMaterial("air");
  TMaterial & silicon   = *MaterialDataBase::Instance().getMaterial("silicon");
  TMaterial & carbon   = *MaterialDataBase::Instance().getMaterial("carbon");


  this->setupGearGeom(gearMgr) ;

  //--The Ladder structure (realistic ladder)--
  int nLadders;
  
  Bool_t active = true;
  Bool_t dummy  = false;
  
  static const double eps = 1e-6; 

  UTIL::BitField64 encoder( ILDCellID0::encoder_string ) ; 
 
  for (int layer=0; layer<_nLayers; ++layer) {

    double offset = _SITgeo[layer].offset ;

    if( offset!=0.0 ) {
      std::cerr << "SIT can not have offsets using the current implementation: exit(1)" << std::endl ; 
      exit(1) ;
    }

    nLadders = _SITgeo[layer].nLadders ;
    
    double phi0 = _SITgeo[layer].phi0 ;

    double ladder_distance = _SITgeo[layer].supRMin ;
    double ladder_thickness = _SITgeo[layer].supThickness ;
    
    double sensitive_distance = _SITgeo[layer].senRMin ;
    double sensitive_thickness = _SITgeo[layer].senThickness ;

    double width = _SITgeo[layer].width ;
    double length = _SITgeo[layer].length;
    
    double currPhi;
    double dphi = _SITgeo[layer].dphi ;
    double cosphi, sinphi;

    for (int ladder=0; ladder<nLadders; ++ladder) {
      
      
      currPhi = phi0 + (dphi * ladder);
      cosphi = cos(currPhi);
      sinphi = sin(currPhi);

      TVector3 normal( cosphi, sinphi, 0) ;
      
      encoder.reset() ;  // reset to 0

      encoder[ILDCellID0::subdet] = ILDDetID::SIT ;
      encoder[ILDCellID0::side] = 0 ;
      encoder[ILDCellID0::layer]  = layer ;
      encoder[ILDCellID0::module] = ladder ;
      encoder[ILDCellID0::sensor] = 0 ;

      int layerID = encoder.lowWord() ;
      
      
      TVector3 sen_front_face_centre( sensitive_distance*cosphi, sensitive_distance*sinphi, 0) ; 
      TVector3 sen_back_face_centre( (sensitive_distance+sensitive_thickness)*cosphi, (sensitive_distance+sensitive_thickness)*sinphi, 0) ; 
      TVector3 sup_back_face_centre( (ladder_distance+ladder_thickness)*cosphi, (ladder_distance+ladder_thickness)*sinphi, 0) ; 
      
      double sen_front_sorting_policy = sensitive_distance + (3 * ladder+0) * eps ;
      double sen_back_sorting_policy = sensitive_distance  + (3 * ladder+1) * eps ;
      double sup_back_sorting_policy = ladder_distance     + (3 * ladder+2) * eps ;
      



      // air - sensitive boundary
      Add(new ILDPlanarMeasLayer(air, silicon, sen_front_face_centre, normal, _bZ, sen_front_sorting_policy, width, length, offset, active, layerID )) ;
      streamlog_out(DEBUG3) << "ILDSITKalDetector add surface with layerID = "
			    << layerID
			    << std::endl ;
      
      // sensitive - support boundary 
      Add(new ILDPlanarMeasLayer(silicon, carbon, sen_back_face_centre, normal, _bZ, sen_back_sorting_policy, width, length, offset, dummy )) ; 
      
      // support - air boundary
      Add(new ILDPlanarMeasLayer(carbon, air, sup_back_face_centre, normal, _bZ, sup_back_sorting_policy, width, length, offset, dummy )) ; 
      
    }	 

  }

   SetOwner();			
}

ILDSITKalDetector::~ILDSITKalDetector()
{
}


void ILDSITKalDetector::setupGearGeom( const gear::GearMgr& gearMgr ){

  const gear::ZPlanarParameters& pSITDetMain = gearMgr.getSITParameters();
  const gear::ZPlanarLayerLayout& pSITLayerLayout = pSITDetMain.getZPlanarLayerLayout();

  _bZ = gearMgr.getBField().at( gear::Vector3D( 0.,0.,0.)  ).z() ;

  _nLayers = pSITLayerLayout.getNLayers(); 
  _SITgeo.resize(_nLayers);

  //SJA:FIXME: for now the support is taken as the same size the sensitive
  //           if this is not done then the exposed areas of the support would leave a carbon - air boundary,
  //           which if traversed in the reverse direction to the next boundary then the track be propagated through carbon
  //           for a significant distance 
  
  for( int layer=0; layer < _nLayers; ++layer){
    _SITgeo[layer].nLadders = pSITLayerLayout.getNLadders(layer); 
    _SITgeo[layer].phi0 = pSITLayerLayout.getPhi0(layer); 
    _SITgeo[layer].dphi = 2*M_PI / _SITgeo[layer].nLadders; 
    _SITgeo[layer].senRMin = pSITLayerLayout.getSensitiveDistance(layer); 
    _SITgeo[layer].supRMin = pSITLayerLayout.getLadderDistance(layer); 
    _SITgeo[layer].length = pSITLayerLayout.getSensitiveLength(layer); 
    _SITgeo[layer].width = pSITLayerLayout.getSensitiveWidth(layer); 
    _SITgeo[layer].offset = pSITLayerLayout.getSensitiveOffset(layer); 
    _SITgeo[layer].senThickness = pSITLayerLayout.getSensitiveThickness(layer); 
    _SITgeo[layer].supThickness = pSITLayerLayout.getLadderThickness(layer); 
    }





}