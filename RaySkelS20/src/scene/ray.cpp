#include "ray.h"
#include "material.h"
#include "scene.h"

const Material &
isect::getMaterial() const
{
    return material ? *material : obj->getMaterial();
}

const Material *ray::getPrevMaterial() const {
    return this->material_stack.top();
}

void ray::rememberMaterial(const Material *material) {
    this->material_stack.push(material);
}

