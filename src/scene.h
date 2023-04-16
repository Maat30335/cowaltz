#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

class Light;
class Primitive;


class Scene {
    public: // surely we make this nicer later
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<Primitive> scene;
};

#endif
