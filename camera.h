#ifndef CAMERA_H
#define CAMERA_H

#include "transform.h"
#include "point.h"

class Camera {
    public:
    Camera(const Transform *CameraToWorld, const Point2f &resolution) : CameraToWorld{CameraToWorld}, resolution{resolution} {}; 
    const Transform *CameraToWorld;
    const Point2f resolution;

    virtual Ray GenerateRay(const Point2f &sample) const = 0; // ray should be returned in world space plz    
};

class PerspectiveCamera : public Camera {
    public:
    PerspectiveCamera(const Transform *CameraToWorld, const Point2f &resolution, float fov) : Camera(CameraToWorld, resolution), fov{fov} {}; // we add fov later
    virtual Ray GenerateRay(const Point2f &sample) const;
    const float fov;
};


#endif
