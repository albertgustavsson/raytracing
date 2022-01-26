#pragma once
#include "scene.h"
#include "hittable_list.h"

hittable_list random_spheres();
scene random_scene();
hittable_list floor_triangles();
scene random_scene_triangles();
scene random_scene_checker();
scene random_scene_light();
hittable_list cornell_box();
scene cornell_box_scene();