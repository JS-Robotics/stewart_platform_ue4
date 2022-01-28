import bpy
import time
import mathutils 
from math import radians
from mathutils import Vector


# This is the path to where the files will be stored. Do not forget the trailing / at the end of the path, as shown in the example path.
# export_path = '/home/sondre/Pictures/all/batch_export/'
#export_path = 'C:/Users/Sondre/Documents/Test/'
export_path = 'C:/Users/Sondre/Documents/Blender/LinActuator/fbx-export/'
#export_path = '../fbx-export'

# Center the cursor to [0, 0, 0]
bpy.context.scene.cursor.location[0] = 0
bpy.context.scene.cursor.location[1] = 0
bpy.context.scene.cursor.location[2] = 0
collection_select = 1


for obj in bpy.data.collections[collection_select].objects:
  
    # Store the current pose of the object
    get_location_x = obj.location[0]
    get_location_y = obj.location[1]
    get_location_z = obj.location[2]
    #get_rotation_R = obj.rotation_euler[0]
    #get_rotation_P = obj.rotation_euler[1]
    #get_rotation_Y = obj.rotation_euler[2]
    
    # select the current object
    if not obj.select_get():
        obj.select_set(True)
        
    # These locations and rotations are in relation to the origin of the object (The orange dot)
    # Center the object to [0, 0, 0]
    obj.location[0] = 0 # X
    obj.location[1] = 0 # Y
    obj.location[2] = 0 # Z
    #obj.rotation_euler[0] = radians(0) # Roll
    #obj.rotation_euler[1] = radians(90) # Pitch
    #obj.rotation_euler[2] = radians(90) # Yaw
    
    
    # Get the current objects name
    file_name = obj.name
    
    # Export as a collada file  (.dae)
    bpy.ops.export_scene.fbx(filepath=export_path+file_name+'.fbx', path_mode='RELATIVE', axis_forward='X', axis_up='Z', use_selection=True)
    time.sleep(1)
    
    # Setting the object back to its original pose
    obj.location[0] = get_location_x
    obj.location[1] = get_location_y
    obj.location[2] = get_location_z
    #obj.rotation_euler[0] = get_rotation_R
    #obj.rotation_euler[1] = get_rotation_P
    #obj.rotation_euler[2] = get_rotation_Y
    
    # De-selecting the object
    if obj.select_get():
        obj.select_set(False)
