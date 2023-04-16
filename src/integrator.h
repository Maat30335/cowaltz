#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "primitive.h"
#include "sampler.h"
#include "camera.h"
#include "spectrum.h"
#include "film.h"
#include "scene.h"

class SamplerIntegrator {
    
    public:
    SamplerIntegrator(std::shared_ptr<Film> film, std::shared_ptr<Camera> camera, std::shared_ptr<PixelSampler> sampler) : film{film}, camera{camera}, sampler{sampler} {};
    virtual void Render(const Scene &scene);
    virtual ~SamplerIntegrator() {};

    private:

    virtual Color rayColor(const Ray &r, const Scene &scene, int depth = 1) const = 0;
    std::shared_ptr<Film> film;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<PixelSampler> sampler;

};

class RedIntegrator : public SamplerIntegrator {
    public:
    RedIntegrator(std::shared_ptr<Film> film, std::shared_ptr<Camera> camera, std::shared_ptr<PixelSampler> sampler) : SamplerIntegrator(film, camera, sampler) {};
    virtual Color rayColor(const Ray &r, const Scene &scene, int depth = 1) const;
};

class LambertIntegrator : public SamplerIntegrator {
    public:
    LambertIntegrator(std::shared_ptr<Film> film, std::shared_ptr<Camera> camera, std::shared_ptr<PixelSampler> sampler) : SamplerIntegrator(film, camera, sampler) {};
    virtual Color rayColor(const Ray &r, const Scene &scene, int depth = 1) const;
};


class DisneyIntegrator : public SamplerIntegrator {
    public:
    DisneyIntegrator(std::shared_ptr<Film> film, std::shared_ptr<Camera> camera, std::shared_ptr<PixelSampler> sampler) : SamplerIntegrator(film, camera, sampler) {};
    virtual Color rayColor(const Ray &r, const Scene &scene, int depth = 1) const;
};


class NormalIntegrator : public SamplerIntegrator {
    public:
    NormalIntegrator(std::shared_ptr<Film> film, std::shared_ptr<Camera> camera, std::shared_ptr<PixelSampler> sampler) : SamplerIntegrator(film, camera, sampler) {};
    virtual Color rayColor(const Ray &r, const Scene &scene, int depth = 1) const;

};

#endif
