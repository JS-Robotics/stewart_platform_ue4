import bpy
import time
import mathutils 
from math import radians
from mathutils import Vector


scale_factor = 0.0254
only_scale = False

# Center the cursor to [0, 0, 0]
bpy.context.scene.cursor.location[0] = 0
bpy.context.scene.cursor.location[1] = 0
bpy.context.scene.cursor.location[2] = 0


for obj in bpy.data.objects:

    # Store the current position of the object
    get_location_x = obj.location[0]
    get_location_y = obj.location[1]
    get_location_z = obj.location[2]    

    # select the current object
    if not obj.select_get():
        obj.select_set(True)

    # Scale the current object
    obj.scale[0] = scale_factor
    obj.scale[1] = scale_factor
    obj.scale[2] = scale_factor
    
    # Move object to relative position applied scaling
    if not only_scale:
        obj.location[0] = get_location_x*scale_factor
        obj.location[1] = get_location_y*scale_factor
        obj.location[2] = get_location_z*scale_factor

    # De-selecting the object
    if obj.select_get():
        obj.select_set(False)
