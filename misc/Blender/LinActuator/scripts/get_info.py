import bpy
import time
import mathutils
import json
from math import radians
from mathutils import Vector
import os

def get_difference(name=None, x=None, y=None, z=None, roll=None, pitch=None, yaw=None, parent_state=False):
    if parent_state:
        dict_part = {
            name:
            {
                'parent': True,
                'position':
                {
                    'x': x,
                    'y': y,
                    'z': z
                },
                'rotation':
                {
                    'r': roll,
                    'p': pitch,
                    'y': yaw
                }
             }
        }
    else:
        dict_part = {
            name:
            {
                'child': True,
                'position':
                {
                    'x': x,
                    'y': y,
                    'z': z
                },
                'rotation':
                {
                    'r': roll,
                    'p': pitch,
                    'y': yaw
                }
             }
        }

    return dict_part


# Select the object considered the parent. 0 indexed of Collection
object_select = 0
parent = bpy.data.objects[object_select].name
round_value = True
round_precision = 4

full_dict = {}
full_dict['Objects'] = []
for obj in bpy.data.objects:

    if round_value:
        get_x = round(obj.location[0], round_precision)
        get_y = round(obj.location[1], round_precision)
        get_z = round(obj.location[2], round_precision)
        get_R = round(obj.rotation_euler[0], round_precision)
        get_P = round(obj.rotation_euler[1], round_precision)
        get_Y = round(obj.rotation_euler[2], round_precision)
    
    else:
        get_x = obj.location[0]
        get_y = obj.location[1]
        get_z = obj.location[2]
        get_R = obj.rotation_euler[0]
        get_P = obj.rotation_euler[1]
        get_Y = obj.rotation_euler[2]
        


    if obj.name == parent:
        full_dict['Objects'].append(get_difference(obj.name, get_x, get_y, get_z, get_R, get_P, get_Y, parent_state=True))
    else:
        full_dict['Objects'].append(get_difference(obj.name, get_x, get_y, get_z, get_R, get_P, get_Y))


with open('/home/sondre/Blender/yale_forklift/pose.json', 'w') as file_out:
    json.dump(full_dict, file_out, indent=2, separators=(',', ':'))

