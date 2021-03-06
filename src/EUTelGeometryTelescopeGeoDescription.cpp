/* 
 * File:   EUTelGeometryTelescopeGeoDescription.cpp
 * 
 */

#include "EUTelGeometryTelescopeGeoDescription.h"

// C++
#include <algorithm>
#include <string>

// MARLIN
#include "marlin/Global.h"
#include "marlin/VerbosityLevels.h"

// EUTELESCOPE
#include "EUTelExceptions.h"

// ROOT
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoNode.h"
#include "TGeoMedium.h"
#include "TGeoMaterial.h"
#include "TGeoBBox.h"
#include "TVectorD.h"
#include "TVector3.h"
#include "TMath.h"

#include <cstring>

using namespace eutelescope;
using namespace geo;
using namespace std;

EUTelGeometryTelescopeGeoDescription& EUTelGeometryTelescopeGeoDescription::getInstance() {
    static EUTelGeometryTelescopeGeoDescription instance;
    return instance;
}

size_t EUTelGeometryTelescopeGeoDescription::nPlanes( ) const {
    return _nPlanes;
}

const EVENT::DoubleVec& EUTelGeometryTelescopeGeoDescription::siPlanesZPositions( ) const {
    return _siPlaneZPosition;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneXPosition( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneXPosition[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneYPosition( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneYPosition[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneZPosition( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneZPosition[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneXRotation( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneXRotation[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneYRotation( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneYRotation[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneZRotation( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneZRotation[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneXSize( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneSizeX[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneYSize( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneSizeY[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneZSize( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneSizeZ[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

double EUTelGeometryTelescopeGeoDescription::siPlaneMediumRadLen( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return _siPlaneRadLength[ _sensorIDtoZOrderMap[ planeID ] ];
    return -999.;
}

TVector3 EUTelGeometryTelescopeGeoDescription::siPlaneNormal( int planeID ) {
    std::map<int,int>::iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) {
        TVector3 normVec( 0., 0., 1. );
        normVec.RotateX( _siPlaneXRotation[ _sensorIDtoZOrderMap[ planeID ] ] );
        normVec.RotateY( _siPlaneYRotation[ _sensorIDtoZOrderMap[ planeID ] ] );
        normVec.RotateZ( _siPlaneZRotation[ _sensorIDtoZOrderMap[ planeID ] ] );
        return normVec;
    }
    return TVector3(0.,0.,0.);
}

const std::map<int, int>& EUTelGeometryTelescopeGeoDescription::sensorIDstoZOrder( ) const {
    return _sensorIDtoZOrderMap;
}

int EUTelGeometryTelescopeGeoDescription::sensorIDtoZOrder( int planeID ) const {
    std::map<int,int>::const_iterator it;
    it = _sensorIDtoZOrderMap.find(planeID);
    if ( it != _sensorIDtoZOrderMap.end() ) return it->second;
    return -1;
}

/** Sensor ID vector ordered according to their position along the Z axis (beam axis)
 *  Numeration runs from 0 to nPlanes-1 */
int EUTelGeometryTelescopeGeoDescription::sensorZOrderToID( int znumber ) const {
    std::map<int,int>::const_iterator it;
    it = _sensorZOrderToIDMap.find( znumber );
    if ( it != _sensorZOrderToIDMap.end() ) return it->second;
    return -1;
}

const EVENT::IntVec& EUTelGeometryTelescopeGeoDescription::sensorIDsVec( ) const {
    return _sensorIDVec;
}

EUTelGeometryTelescopeGeoDescription::EUTelGeometryTelescopeGeoDescription() :
_siPlanesParameters(0),
_siPlanesLayerLayout(0),
_sensorIDVec(),
_sensorIDVecMap(),
_sensorZOrderToIDMap(),
_sensorIDtoZOrderMap(),
_siPlaneXPosition(),
_siPlaneYPosition(),
_siPlaneZPosition(),
_siPlaneXRotation(),
_siPlaneYRotation(),
_siPlaneZRotation(),
_nPlanes(0),
_geoManager(0)
{

    // Check if the GEAR manager is not corrupted, otherwise stop

    if (!marlin::Global::GEAR) {
        streamlog_out(ERROR2) << "The GearMgr is not available, for an unknown reason." << std::endl;
        throw eutelescope::InvalidGeometryException("GEAR manager is not initialised");
    }

    // sensor-planes in geometry navigation:
    _siPlanesParameters = const_cast<gear::SiPlanesParameters*> (&(marlin::Global::GEAR->getSiPlanesParameters()));
    _siPlanesLayerLayout = const_cast<gear::SiPlanesLayerLayout*> (&(_siPlanesParameters->getSiPlanesLayerLayout()));
    
    // create an array with the z positions of each layer
    for (int iPlane = 0; iPlane < _siPlanesLayerLayout->getNLayers(); iPlane++) {
        _siPlaneXPosition.push_back(_siPlanesLayerLayout->getLayerPositionX(iPlane));
        _siPlaneYPosition.push_back(_siPlanesLayerLayout->getLayerPositionY(iPlane));
        _siPlaneZPosition.push_back(_siPlanesLayerLayout->getLayerPositionZ(iPlane));
        _siPlaneXRotation.push_back(_siPlanesLayerLayout->getLayerRotationZY(iPlane));
        _siPlaneYRotation.push_back(_siPlanesLayerLayout->getLayerRotationZX(iPlane));
        _siPlaneZRotation.push_back(_siPlanesLayerLayout->getLayerRotationXY(iPlane));
        
        _siPlaneSizeX.push_back(_siPlanesLayerLayout->getLayerSizeX(iPlane));
        _siPlaneSizeY.push_back(_siPlanesLayerLayout->getLayerSizeY(iPlane));
        _siPlaneSizeZ.push_back(_siPlanesLayerLayout->getLayerThickness(iPlane));
        
        _siPlaneRadLength.push_back(_siPlanesLayerLayout->getLayerRadLength(iPlane));
    }

    if (_siPlanesParameters->getSiPlanesType() == _siPlanesParameters->TelescopeWithDUT) {
        _siPlaneXPosition.push_back(_siPlanesLayerLayout->getDUTPositionX());
        _siPlaneYPosition.push_back(_siPlanesLayerLayout->getDUTPositionY());
        _siPlaneZPosition.push_back(_siPlanesLayerLayout->getDUTPositionZ());
        // WARNING No DUT rotations in GEAR!!!!!!!!!!
        // TODO: Need this in GEAR
        _siPlaneXRotation.push_back(0.);
        _siPlaneYRotation.push_back(0.);
        _siPlaneZRotation.push_back(0.);
    }

    // sort the array with increasing z
    std::sort(_siPlaneZPosition.begin(), _siPlaneZPosition.end());

    // clear the sensor ID vector
    _sensorIDVec.clear();

    // clear the sensor ID map
    _sensorIDVecMap.clear();
    _sensorIDtoZOrderMap.clear();

    double* keepZPosition = new double[ _siPlanesLayerLayout->getNLayers() ];

    for (int iPlane = 0; iPlane < _siPlanesLayerLayout->getNLayers(); iPlane++) {
        int sensorID = _siPlanesLayerLayout->getID(iPlane);
        _sensorIDVec.push_back(sensorID);
        _sensorIDVecMap.insert(std::make_pair(sensorID, iPlane));

        // count number of the sensors to the left of the current one:
        int sensorsToTheLeft = 0;
        keepZPosition[ iPlane ] = _siPlanesLayerLayout->getLayerPositionZ(iPlane);
        for (int jPlane = 0; jPlane < _siPlanesLayerLayout->getNLayers(); jPlane++)
            if (_siPlanesLayerLayout->getLayerPositionZ(jPlane) + 1e-06 < keepZPosition[ iPlane ]) sensorsToTheLeft++;

        _sensorZOrderToIDMap.insert(std::make_pair(sensorsToTheLeft, sensorID));        
        _sensorIDtoZOrderMap.insert(std::make_pair(sensorID, sensorsToTheLeft));
    }

    delete [] keepZPosition;

    _nPlanes = _siPlanesParameters->getSiPlanesNumber();
    if (_siPlanesParameters->getSiPlanesType() == _siPlanesParameters->TelescopeWithDUT) ++_nPlanes;
    
    
    
    // TGeo manager initialisation
    
}

EUTelGeometryTelescopeGeoDescription::~EUTelGeometryTelescopeGeoDescription() {
    delete _geoManager;
}

/**
 * Initialise ROOT geometry objects from external .root file
 * @param tgeofilename name of .root file
 */
void EUTelGeometryTelescopeGeoDescription::initializeTGeoDescription( string tgeofilename ) {
//    #ifdef USE_TGEO
    // get access to ROOT's geometry manager
    
    _geoManager = TGeoManager::Import( tgeofilename.c_str() );
    if( !_geoManager ) {
        streamlog_out( WARNING ) << "Can't read file " << tgeofilename << endl;
    }
    _geoManager->CloseGeometry();
//    #endif //USE_TGEO
}

/**
 * Initialise ROOT geometry objects from GEAR objects
 * 
 * @param geomName name of ROOT geometry object
 * @param dumpRoot dump automatically generated ROOT geometry file for further inspection
 */
void EUTelGeometryTelescopeGeoDescription::initializeTGeoDescription( std::string& geomName, bool dumpRoot = false ) {
//    #ifdef USE_TGEO
    // get access to ROOT's geometry manager
    
    _geoManager = new TGeoManager("Telescope", "v0.1");;
    if( !_geoManager ) {
        streamlog_out( ERROR3 ) << "Can't instantiate ROOT TGeoManager " << std::endl;
        return;
    }
   
    
    // Create top world volume containing telescope/DUT geometry
    
    
    // Create air mixture
    // see http://pdg.lbl.gov/2013/AtomicNuclearProperties/HTML_PAGES/104.html
    double air_density = 1.2e-3;         // g/cm^3
    double air_radlen  = 36.62;          // g/cm^2
    TGeoMixture* pMatAir = new TGeoMixture("AIR",3,air_density);
    pMatAir->DefineElement(0, 14.007, 7.,  0.755267 );     //Nitrogen
    pMatAir->DefineElement(1, 15.999, 8.,  0.231781 );     //Oxygen
    pMatAir->DefineElement(2, 39.948, 18., 0.012827 );     //Argon
    pMatAir->DefineElement(3, 12.011, 6.,  0.000124 );     //Carbon
    pMatAir->SetRadLen( air_radlen );
    // Medium: medium_World_AIR
    TGeoMedium* pMedAir = new TGeoMedium("medium_World_AIR", 3, pMatAir );
    
    // The World is the 10 x 10m x 10m box filled with air mixture
    Double_t dx,dy,dz;
    dx = 5000.000000; // [mm]
    dy = 5000.000000; // [mm]
    dz = 5000.000000; // [mm]
    TGeoShape *pBoxWorld = new TGeoBBox("Box_World", dx,dy,dz);
    // Volume: volume_World
    TGeoVolume* pvolumeWorld = new TGeoVolume("volume_World",pBoxWorld, pMedAir);
    pvolumeWorld->SetLineColor(4);
    pvolumeWorld->SetLineWidth(3);
    pvolumeWorld->SetVisLeaves(kTRUE);

   // Set top volume of geometry
   gGeoManager->SetTopVolume( pvolumeWorld );
   
 
   
   // Iterate over registered GEAR objects and construct their TGeo representation
   
   const Double_t PI = 3.141592653589793;
   const Double_t DEG = 180./PI;
   
   double xc, yc, zc;   // volume center position 
   double alpha, beta, gamma;
   
   IntVec::const_iterator itrPlaneId;
   for ( itrPlaneId = _sensorIDVec.begin(); itrPlaneId != _sensorIDVec.end(); ++itrPlaneId ) {
       
       std::stringstream strId;
       strId << *itrPlaneId;
       
       // Get sensor center position
       xc = siPlaneXPosition( *itrPlaneId );
       yc = siPlaneYPosition( *itrPlaneId );
       zc = siPlaneZPosition( *itrPlaneId );
       
       // Get sensor orientation
       alpha = siPlaneXRotation( *itrPlaneId ); // [rad]
       beta  = siPlaneYRotation( *itrPlaneId ); // [rad]
       gamma = siPlaneZRotation( *itrPlaneId ); // [rad]
       
       // Spatial translations of the sensor center
       string stTranslationName = "matrixTranslationSensor";
       stTranslationName.append( strId.str() );
       TGeoTranslation* pMatrixTrans = new TGeoTranslation( stTranslationName.c_str(), xc, yc, zc );
       
       // Spatial rotation around sensor center
       // TGeoRotation requires Euler angles in degrees
       string stRotationName = "matrixRotationSensorX";
       stRotationName.append( strId.str() );
       TGeoRotation* pMatrixRotX = new TGeoRotation( stRotationName.c_str(), 0.,  alpha*DEG, 0.);                // around X axis
       stRotationName = "matrixRotationSensorY";
       stRotationName.append( strId.str() );
       TGeoRotation* pMatrixRotY = new TGeoRotation( stRotationName.c_str(), 90., beta*DEG,  0.);                // around Y axis (combination of rotation around Z axis and new X axis)
       stRotationName = "matrixRotationSensorBackY";
       stRotationName.append( strId.str() );
       TGeoRotation* pMatrixRotY1 = new TGeoRotation( stRotationName.c_str(), -90., 0.,  0.);                    // restoration of original orientation (valid in small angle approximataion ~< 15 deg)
       stRotationName = "matrixRotationSensorZ";
       stRotationName.append( strId.str() );
       TGeoRotation* pMatrixRotZ = new TGeoRotation( stRotationName.c_str(), 0. , 0.,        gamma*DEG);         // around Z axis
       
       // Combined rotation in several steps
       TGeoRotation* pMatrixRot = new TGeoRotation( *pMatrixRotX );
       pMatrixRot->MultiplyBy( pMatrixRotY );
       pMatrixRot->MultiplyBy( pMatrixRotY1 );
       pMatrixRot->MultiplyBy( pMatrixRotZ );
       
       // Combined translation and orientation
       TGeoCombiTrans* combi = new TGeoCombiTrans( *pMatrixTrans, *pMatrixRot );
       
       // Construction of sensor objects
       
       // Construct object medium. Required for radiation length determination

       // assume SILICON, though all information except of radiation length is ignored
       double a       = 28.085500;     
       double z       = 14.000000;
       double density = 2.330000;
       double radl    = siPlaneMediumRadLen( *itrPlaneId );
       double absl    = 45.753206;
       string stMatName = "materialSensor";
       stMatName.append( strId.str() );
       TGeoMaterial* pMat = new TGeoMaterial( stMatName.c_str(), a, z, density, radl, absl );
       pMat->SetIndex( 1 );
       // Medium: medium_Sensor_SILICON
       int numed   = 0;  // medium number
       double par[8];
       par[0]  = 0.000000; // isvol
       par[1]  = 0.000000; // ifield
       par[2]  = 0.000000; // fieldm
       par[3]  = 0.000000; // tmaxfd
       par[4]  = 0.000000; // stemax
       par[5]  = 0.000000; // deemax
       par[6]  = 0.000000; // epsil
       par[7]  = 0.000000; // stmin
       string stMedName = "mediumSensor";
       stMedName.append( strId.str() );
       TGeoMedium* pMed = new TGeoMedium( stMedName.c_str(), numed, pMat, par );
       
       // Construct object shape
       // Shape: Box type: TGeoBBox
       // TGeo requires half-width of box side
       dx = siPlaneXSize( *itrPlaneId ) / 2.;
       dy = siPlaneYSize( *itrPlaneId ) / 2.;
       dz = siPlaneZSize( *itrPlaneId ) / 2.;
       TGeoShape *pBoxSensor = new TGeoBBox( "BoxSensor", dx, dy, dz );
       // Volume: volume_Sensor1
       
       // Geometry navigation package requires following names for objects that have an ID
       // name:ID
       string stVolName = "volume_SensorID:";
       stVolName.append( strId.str() );
       TGeoVolume* pvolumeSensor = new TGeoVolume( stVolName.c_str(), pBoxSensor, pMed );
       pvolumeSensor->SetVisLeaves( kTRUE );
       pvolumeWorld->AddNode(pvolumeSensor, (*itrPlaneId), combi);
   } // loop over sensorID
   
   
    _geoManager->CloseGeometry();
    
    // Dump ROOT TGeo object into file
    if ( dumpRoot ) _geoManager->Export( geomName.c_str() );
//    #endif //USE_TGEO
    return;
}

/** Determine id of the sensor in which point is locate
 * 
 * @param globalPos 3D point in global reference frame
 * @return sensorID or -999 if the point in outside of sensor volume
 */
int EUTelGeometryTelescopeGeoDescription::getSensorID( const float globalPos[] ) const {
    streamlog_out(DEBUG2) << "EUTelGeometryTelescopeGeoDescription::getSensorID() " << std::endl;
    
    _geoManager->FindNode( globalPos[0], globalPos[1], globalPos[2] );
    const char* volName = const_cast < char* > ( geo::gGeometry( )._geoManager->GetCurrentVolume( )->GetName( ) );
    streamlog_out( DEBUG0 ) << "Point (" << globalPos[0] << "," << globalPos[1] << "," << globalPos[2] << ") found in volume: " << volName << std::endl;
    std::vector< std::string > tokens = Utility::stringSplit(std::string( volName ), ":", false );
    
    // sensor id must be stored in the last token
    int id = -999;
    std::size_t found = tokens.back().find_first_of("0123456789");
    if ( found != std::string::npos ) id = atoi( tokens.back().c_str() );

    int sensorID = -999;
    if ( std::find( _sensorIDVec.begin(), _sensorIDVec.end(), id ) != _sensorIDVec.end() ) sensorID = id;
    else streamlog_out(DEBUG3) << "Point (" << globalPos[0] << "," << globalPos[1] << "," << globalPos[2] << ") was not found inside any sensor!" << std::endl;
    
    streamlog_out( DEBUG0 ) << "SensorID: " << sensorID << std::endl;
    
    return sensorID;
}

/**
 * Coordinate transformation from local reference frame of sensor with a given sensorID
 * to the global coordinate system
 * 
 * @param sensorID Id of the sensor (specifies local coordinate system)
 * @param localPos (x,y,z) in local coordinate system
 * @param globalPos (x,y,z) in global coordinate system
 */
void EUTelGeometryTelescopeGeoDescription::local2Master( int sensorID, const double localPos[], double globalPos[] ) {
    streamlog_out(DEBUG2) << "EUTelGeometryTelescopeGeoDescription::local2Master() " << std::endl;
    const double sensorCenterX = siPlaneXPosition( sensorID );
    const double sensorCenterY = siPlaneYPosition( sensorID );
    const double sensorCenterZ = siPlaneZPosition( sensorID );
    
    streamlog_out(DEBUG0) << "Senosor id: " << sensorID << std::endl;
    streamlog_out(DEBUG0) << "Senosor center: " << "(" << sensorCenterX << "," << sensorCenterY << "," << sensorCenterZ << ")" << std::endl;
    
    _geoManager->FindNode( sensorCenterX, sensorCenterY, sensorCenterZ );    
    _geoManager->LocalToMaster( localPos, globalPos );
    
    streamlog_out(DEBUG0) << std::fixed;
    streamlog_out(DEBUG0) << "Local coordinates ( sensorID =  " << sensorID << " ) : " << std::endl;
    streamlog_out(DEBUG0) << std::setw(10) << std::setprecision(5) << localPos[0] << std::setw(10) << std::setprecision(5) << localPos[1] << std::setw(10) << std::setprecision(5) << localPos[2] << std::endl;
    streamlog_out(DEBUG0) << "Global coordinates ( sensorID =  " << sensorID << " ) : " << std::endl;
    streamlog_out(DEBUG0) << std::setw(10) << std::setprecision(5) << globalPos[0] << std::setw(10) << std::setprecision(5) << globalPos[1] << std::setw(10) << std::setprecision(5) << globalPos[2] << std::endl;
}

/**
 * Coordinate transformation from global reference frame to local reference frame.
 * Corresponding volume is determined automatically.
 * 
 * @param globalPos (x,y,z) in global coordinate system
 * @param localPos (x,y,z) in local coordinate system
 */
void EUTelGeometryTelescopeGeoDescription::master2Local( const double globalPos[], double localPos[] ) {
    streamlog_out(DEBUG2) << "EUTelGeometryTelescopeGeoDescription::master2Local() " << std::endl;
    _geoManager->FindNode( globalPos[0], globalPos[1], globalPos[2] );    
    _geoManager->MasterToLocal( globalPos, localPos );
    
    streamlog_out(DEBUG0) << std::fixed;
    streamlog_out(DEBUG0) << "Global coordinates:" << std::endl;
    streamlog_out(DEBUG0) << std::setw(10) << std::setprecision(5) << globalPos[0] << std::setw(10) << std::setprecision(5) << globalPos[1] << std::setw(10) << std::setprecision(5) << globalPos[2] << std::endl;
    streamlog_out(DEBUG0) << "Local coordinates: " << std::endl;
    streamlog_out(DEBUG0) << std::setw(10) << std::setprecision(5) << localPos[0] << std::setw(10) << std::setprecision(5) << localPos[1] << std::setw(10) << std::setprecision(5) << localPos[2] << std::endl;
}

/**
 * Vector coordinate transformation from global reference frame to local reference frame.
 * Corresponding volume is determined automatically.
 * 
 * @param globalVec (x,y,z) in global coordinate system
 * @param localVec (x,y,z) in local coordinate system
 */
void EUTelGeometryTelescopeGeoDescription::local2MasterVec( int sensorID, const double localVec[], double globalVec[] ) {
    streamlog_out(DEBUG2) << "EUTelGeometryTelescopeGeoDescription::master2LocalVec() " << std::endl;
    const double sensorCenterX = siPlaneXPosition( sensorID );
    const double sensorCenterY = siPlaneYPosition( sensorID );
    const double sensorCenterZ = siPlaneZPosition( sensorID );
    
    streamlog_out(DEBUG0) << "Senosor id: " << sensorID << std::endl;
    streamlog_out(DEBUG0) << "Senosor center: " << "(" << sensorCenterX << "," << sensorCenterY << "," << sensorCenterZ << ")" << std::endl;
    
    _geoManager->FindNode( sensorCenterX, sensorCenterY, sensorCenterZ );    
    _geoManager->LocalToMasterVect( localVec, globalVec );
    
    streamlog_out(DEBUG0) << std::fixed;
    streamlog_out(DEBUG0) << "Global coordinates:" << std::endl;
    streamlog_out(DEBUG0) << std::setw(10) << std::setprecision(5) << globalVec[0] << std::setw(10) << std::setprecision(5) << globalVec[1] << std::setw(10) << std::setprecision(5) << globalVec[2] << std::endl;
    streamlog_out(DEBUG0) << "Local coordinates: " << std::endl;
    streamlog_out(DEBUG0) << std::setw(10) << std::setprecision(5) << localVec[0] << std::setw(10) << std::setprecision(5) << localVec[1] << std::setw(10) << std::setprecision(5) << localVec[2] << std::endl;
}


/**
 * Vector coordinate transformation from global reference frame to local reference frame.
 * Corresponding volume is determined automatically.
 * 
 * @param globalVec (x,y,z) in global coordinate system
 * @param localVec (x,y,z) in local coordinate system
 */
void EUTelGeometryTelescopeGeoDescription::master2LocalVec( int sensorID, const double globalVec[], double localVec[] ) {
    streamlog_out(DEBUG2) << "EUTelGeometryTelescopeGeoDescription::master2LocalVec() " << std::endl;
    const double sensorCenterX = siPlaneXPosition( sensorID );
    const double sensorCenterY = siPlaneYPosition( sensorID );
    const double sensorCenterZ = siPlaneZPosition( sensorID );
    
    streamlog_out(DEBUG0) << "Senosor id: " << sensorID << std::endl;
    streamlog_out(DEBUG0) << "Senosor center: " << "(" << sensorCenterX << "," << sensorCenterY << "," << sensorCenterZ << ")" << std::endl;
    
    _geoManager->FindNode( sensorCenterX, sensorCenterY, sensorCenterZ );    
    _geoManager->MasterToLocalVect( globalVec, localVec );
    
    streamlog_out(DEBUG0) << std::fixed;
    streamlog_out(DEBUG0) << "Global coordinates:" << std::endl;
    streamlog_out(DEBUG0) << std::setw(10) << std::setprecision(5) << globalVec[0] << std::setw(10) << std::setprecision(5) << globalVec[1] << std::setw(10) << std::setprecision(5) << globalVec[2] << std::endl;
    streamlog_out(DEBUG0) << "Local coordinates: " << std::endl;
    streamlog_out(DEBUG0) << std::setw(10) << std::setprecision(5) << localVec[0] << std::setw(10) << std::setprecision(5) << localVec[1] << std::setw(10) << std::setprecision(5) << localVec[2] << std::endl;
}

/**
 * Global-to-local coordinate transformation matrix.
 * Corresponding volume is determined automatically.
 * 
 * @param globalPos (x,y,z) in global coordinate system
 * @return 
 */
const TGeoHMatrix* EUTelGeometryTelescopeGeoDescription::getHMatrix( const double globalPos[] ) {
    streamlog_out(DEBUG2) << "EUTelGeometryTelescopeGeoDescription::getHMatrix() " << std::endl;
    _geoManager->FindNode( globalPos[0], globalPos[1], globalPos[2] );    
    const TGeoHMatrix* globalH = _geoManager->GetCurrentMatrix();
    return globalH;
}

/**
 * Retrieve magnetic field object.
 * 
 * @return reference to gear::BField object
 */
const gear::BField& EUTelGeometryTelescopeGeoDescription::getMagneticFiled() const {
    streamlog_out(DEBUG2) << "EUTelGeometryTelescopeGeoDescription::getMagneticFiled() " << std::endl;
    return marlin::Global::GEAR->getBField();
}

/**
 * Calculate effective radiation length traversed by particle traveling between two points
 * along straight line.
 * 
 * Calculation is done according to the eq. (27.23)
 * @see http://pdg.lbl.gov/2006/reviews/passagerpp.pdf
 * 
 * @param globalPosStart starting point in the global coordinate system
 * @param globalPosFinish ending point in the global coordinate system
 * @param skipBoundaryVolumes if true subtract rad length of the volumes containing start and finish points
 * 
 * @return radiation length in units of X0
 */
float EUTelGeometryTelescopeGeoDescription::findRadLengthIntegral( const double globalPosStart[], const double globalPosFinish[], bool skipBoundaryPonitsVolumes ) {

    streamlog_out(DEBUG1) << "EUTelGeometryTelescopeGeoDescription::findRadLengthIntegral()" << std::endl;
    
    float rad = 0.;        // integral of radiation length in units of X0
    
    const double mm2cm = 0.1;
    
    /* TGeo uses cm and grams as internal units e.g. in radiation length and density. Telescope/LCIO uses mm. Therefore this routine is full of 
     annoying conversion factors */    
    
    const double stepLenght2 = ( globalPosFinish[0] - globalPosStart[0] )*( globalPosFinish[0] - globalPosStart[0] ) +
                               ( globalPosFinish[1] - globalPosStart[1] )*( globalPosFinish[1] - globalPosStart[1] ) +
                               ( globalPosFinish[2] - globalPosStart[2] )*( globalPosFinish[2] - globalPosStart[2] );
    
    const double stepLenght  = TMath::Sqrt( stepLenght2 );

    // don't need conversion factor to for calculation of directions
    const double xp  = ( globalPosFinish[0] - globalPosStart[0] )/stepLenght;
    const double yp  = ( globalPosFinish[1] - globalPosStart[1] )/stepLenght;
    const double zp  = ( globalPosFinish[2] - globalPosStart[2] )/stepLenght;

    streamlog_out(DEBUG0) << "Start point (x,y,z):" << globalPosStart[0] << "," << globalPosStart[1] << "," << globalPosStart[2] << std::endl;
    streamlog_out(DEBUG0) << "Finish point (x,y,z):" << globalPosFinish[0] << "," << globalPosFinish[1] << "," << globalPosFinish[2] << std::endl;
    streamlog_out(DEBUG0) << "Direction (nx,ny,nz):" << xp << "," << yp << "," << zp << std::endl;
    
    double snext;
    double pt[3], loc[3];
    double epsil = 1.E-7;
    double lastrad = 0.;
    int ismall       = 0;
    int nbound       = 0;
    float length     = 0.;
    TGeoMedium *med;
    TGeoShape *shape;
    
    // Get starting node
    gGeoManager->InitTrack( globalPosStart[0]/*mm*/, globalPosStart[1]/*mm*/, globalPosStart[2]/*mm*/, xp, yp, zp );
    TGeoNode *nextnode = gGeoManager->GetCurrentNode( );
    
    double currentStep = stepLenght /*mm*/;
    // Loop over all, encountered during the propagation, volumes 
    while ( nextnode ) {
        med = NULL;
        
	// Check if current point is inside silicon sensor. Radiation length of silicon sensors is accounted in thin scatterers of GBL.
        bool isBoundaryVolume = false;
        if ( gGeoManager->IsSameLocation( globalPosStart[0], globalPosStart[1], globalPosStart[2] ) ||
             gGeoManager->IsSameLocation( globalPosFinish[0], globalPosFinish[1], globalPosFinish[2] ) ) isBoundaryVolume = true;
        
        if ( nextnode ) med = nextnode->GetVolume()->GetMedium();
        else return 0.;
        
        shape = nextnode->GetVolume()->GetShape();
        
        // make a step to the next intersection point
        if ( currentStep > 1.e-9 /*mm*/ ) nextnode = gGeoManager->FindNextBoundaryAndStep( currentStep /*mm*/ );
        else return rad;
        
        snext  = gGeoManager->GetStep() /*mm*/;
        
        // Small steps treatment
        if ( snext < 1.e-8 /*mm*/ ) {
            ismall++;
            
            // Terminate calculation if too many small steps done
            if ( ismall > 3 ) {
                streamlog_out( WARNING1 ) << "ERROR: Small steps in: " << gGeoManager->GetPath() << " shape=" << shape->ClassName() << endl;
                return rad;
            }

            // increase step size (epsilon) and advance along the particle direction
            memcpy( pt, gGeoManager->GetCurrentPoint(), 3 * sizeof (double) );
            const double *dir = gGeoManager->GetCurrentDirection();
            for ( Int_t i = 0; i < 3; i++ ) pt[i] += epsil * dir[i];
            snext = epsil;
            length += snext;
            
            // Ignore start and finish volumes if required
            if ( skipBoundaryPonitsVolumes && isBoundaryVolume ) {
                rad += 0.;
            } else {
                rad += lastrad*snext;
            }
            
            gGeoManager->CdTop( );
            nextnode = gGeoManager->FindNode( pt[0], pt[1], pt[2] );    // Check if particle is crossed the boundary
            if ( gGeoManager->IsOutside() ) return rad;                 // leave if not
            TGeoMatrix *mat = gGeoManager->GetCurrentMatrix();          
            mat->MasterToLocal( pt, loc );
            if ( !gGeoManager->GetCurrentVolume()->Contains( loc ) ) {
                gGeoManager->CdUp();
                nextnode = gGeoManager->GetCurrentNode();               // move to new volume
            }
            continue;
        } else {
            ismall = 0;
        }
        
        // Normal steps case
        nbound++;
        length += snext;
        currentStep -= snext;
        if ( med ) {
            double radlen = med->GetMaterial()->GetRadLen() /*cm*/;
            if ( radlen > 1.e-9 && radlen < 1.e10 ) {
                
                lastrad = 1. / radlen * mm2cm;
                
                // Ignore start and finish volumes if required
                if ( skipBoundaryPonitsVolumes && isBoundaryVolume ) {
                    rad += 0.;
                } else {
                    rad += lastrad*snext;
                }
                
            } else {
                lastrad = 0.;
            }
            streamlog_out( DEBUG0 ) << "STEP #" << nbound << std::endl;
            streamlog_out( DEBUG0 ) << "   step[mm]=" << snext << "   length[mm]=" << length
                    << " rad[X0]=" << snext * mm2cm / radlen << " " << med->GetName( ) 
                    << " rho[g/cm^3]=" << med->GetMaterial()->GetDensity() <<" radlen[cm]=" << radlen << " Boundary:" << (isBoundaryVolume?"yes":"no")
		    << std::endl;
        }
    }
    
    streamlog_out(DEBUG1) << "--------EUTelGeometryTelescopeGeoDescription::findRadLengthIntegral()--------" << std::endl;
    
    return rad;
}
