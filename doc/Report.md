# Report

The following sections present the features we have implemented, some screenshots of those features, and a small list
of references we used while implementing the shading and shadows.

## Features

| Category | Name                      | Author(s)       |
| -------- | ------------------------- | ----------------|
| Graphics | Diffuse/Specular textures | Wessel          |
|          | Mipmapping                | Andor / Wessel  |
|          | Anisotropic filtering     | Wessel          |
|          | Normal mapping            | Arnoud          |
|          | Lighting                  | Wessel          |
|          | Normalized Blinn-Phong    | Wessel          |
|          | Directional light shadows | Wessel          |
|          | Spot light shadows        | Andor / Wessel  |
|          | Slope-based bias          | Wessel          |
|          | Depth-based bias          | Andor / Wessel  |
|          | PCF                       | Wessel          |
|          | Poisson sampling          | Wessel          |
|          | X-ray vision              | Andor           |
|          | Toon shading              | Andor / Wessel  |
|          | 2D icons                  | Wessel          |
| Modeling | Player character          | Andor           |
|          | Animation                 | Andor           |
|          | House                     | Arnoud          |
|          | Wall                      | Arnoud          |
|          | Hierarchical door         | Arnoud          |
|          | Hierarchical animation    | Arnoud          |
|          | Key                       | Wessel          |
|          | Pedestal                  | Wessel          |
| Logic    | AABB collision detection  | Andor           |
|          | Level design              | Arnoud          |
|          | Gates and keys            | Arnoud / Wessel |
| Other    | Framework                 | Wessel          |
|          | Model loader              | Wessel          |
|          | Gamepad support           | Wessel          |

## Screenshots

### Custom models

![](images/custom_models.png)

### Spot light shadows

![](images/spot_light_shadow.png)

### Directional shadows

![](images/directional_shadows.png)

### Hierarchical door + Normal maps

![](images/hierarchical_door.png)

### X-toon shading

![](images/x-toon_shading.png)

### X-ray vision

![](images/x-ray_vision.png)

## References

- [Blinn–Phong reflection model](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model)
- [The Blinn-Phong Normalization Zoo](http://www.thetenthplanet.de/archives/255)
- [Normalization Article - Fabian Giesen](http://www.farbrausch.de/~fg/stuff/phong.pdf)
- [Shadow mapping tutorial](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/)
- [Adaptive Depth Bias for Shadow Maps](http://jcgt.org/published/0003/04/08/paper-lowres.pdf)
- [How to Create a Low Poly Character in Blender 2.8](https://youtu.be/Ljl_QFs9xhE)
- [Mixamo running animation](https://www.mixamo.com/#/?limit=1&page=1&query=running)
