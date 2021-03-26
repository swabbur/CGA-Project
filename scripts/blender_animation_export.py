# exports each selected object into its own file

import bpy
import os

# export to blend file location
basedir = os.path.dirname("E:/Documenten/Uni/Master Y1/CGA/Project/CGA-Project/res/models/player/")

if not basedir:
    raise Exception("Blend file is not saved")

view_layer = bpy.context.view_layer

obj_active = view_layer.objects.active
selection = bpy.context.selected_objects

fn = os.path.join(basedir, "Human_running_")
scene = bpy.context.scene

for i in range (1, 23):
    scene.frame_set(i)
    bpy.ops.export_scene.fbx(filepath=fn + str(i) + ".fbx", use_selection=True,
                                                            object_types={'MESH'},
                                                            global_scale=0.005,
                                                            bake_space_transform=True)

print("written:", fn)
