#ifndef MEASUREMENTSURFACE_H
#define MEASUREMENTSURFACE_H

#include "ICoordinateSystem.h"

/** Measurement Surface Class */
namespace GearExtensions{
   
   
class MeasurementSurface{
   
public:
   
   MeasurementSurface( int ID, ICoordinateSystem* csys ): _ID( ID ), _coordinateSystem( csys ){}
   
   ~MeasurementSurface(){ delete _coordinateSystem; }
   
   int getID(){ return _ID; }
   ICoordinateSystem* getCoordinateSystem(){ return _coordinateSystem; }
   
private:
   
   int _ID;
   ICoordinateSystem* _coordinateSystem;
   
   
   MeasurementSurface(const MeasurementSurface& m){};   // copy constructor is private --> no cpoying allowed
   MeasurementSurface& operator= (MeasurementSurface const& m); // assignment not allowed either
   
   
};
   
   
} //end of namespace

#endif
