hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
stone = gr.material({0.5, 0.30, 0.30}, {0.25, 0.15, 0.15}, 20)
carpet = gr.material({0.6, 0.02, 0.1}, {0,0,0}, 0)
mirror = gr.material({0.1, 0.16, 0.15}, {1.5,1.5,1.5}, 120)

perfect_crystal = gr.material_extended({0.4, 0.4, 0.4}, {1.6, 1.6, 1.6}, 1000, 1, 0, 0, 0)

--obtained from https://www.student.cs.uwaterloo.ca/~cs488/mat.txt
copper = gr.material({0.7038, 0.27048, 0.0828}, {0.25677, 0.13762, 0.086014}, 12.8)
emerald = gr.material({0.07568, 0.61424, 0.07568}, {0.833, 0.927811, 0.833}, 76.8)
jade = gr.material({0.54, 0.89, 0.63}, {0.316228, 0.316228, 0.316228}, 12.8)
obsidian = gr.material({0.18275, 0.17, 0.22525}, {0.332741, 0.328634, 0.346435}, 38.4)
pearl = gr.material({1, 0.829, 0.829}, {0.296648, 0.296648, 0.296648}, 11.264)
ruby = gr.material({0.61424, 0.04136, 0.04136}, {0.927811, 0.826959, 0.826959}, 76.8)
turquoise = gr.material({0.396, 0.74151, 0.69102}, {0.297254, 0.30829, 0.306678}, 12.8)
brass = gr.material({0.78039, 0.568627, 0.113725}, {0.99215, 0.941176, 0.807843}, 27.9)
bronze = gr.material({0.714, 0.4284, 0.18144}, {0.393548, 0.271906, 0.166721}, 25.6)
chrome = gr.material({0.4, 0.4, 0.4}, {0.974597, 0.974597, 0.974597}, 76.8)
gold = gr.material({0.75164, 0.60648, 0.22648}, {0.628281, 0.555802, 0.366065}, 51.2)
silver = gr.material({0.50754, 0.50754, 0.50754}, {0.908273, 0.908273, 0.908273}, 51.2)
brubber = gr.material({0.01, 0.01, 0.01}, {0.4, 0.4, 0.4}, 10)
polbronze = gr.material({0.4, 0.2368, 0.1036}, {0.774597, 0.458561, 0.200621}, 76.8)
polcopper = gr.material({0.5508, 0.2118, 0.066}, {0.580594, 0.223257, 0.0695701}, 51.2)
polgold = gr.material({0.34615, 0.3143, 0.0903}, {0.797357, 0.723991, 0.208006}, 83.2)
pewter = gr.material({0.427451, 0.470588, 0.541176}, {0.33333, 0.33333, 0.521569}, 9.84615)
polsilver = gr.material({0.50754, 0.50754, 0.50754}, {0.508273, 0.508273, 0.508273}, 51.2)

primary_light = gr.light({-100.0, 350.0, 200.0}, {1.0, 1.0, 1.0}, {1, 0, 0})
secondary_light = gr.light({0, 100.0, 0}, {0.1, 0.7, 0.5}, {1, 0.4, 0})
frontal_light = gr.light({0, 0, 200}, {0.4, 0.4, 0.5}, {1, 0, 0})


texture_checkerboard = gr.texture('Assets/checkerboard.png')
texture_bricks = gr.texture('Assets/bricks.png')


--start script
scene = gr.node('root')

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
plane:set_material(stone)
plane:scale(50,50,100)
plane:rotate('x', 15)
plane:translate(0,-1,0)
scene:add_child(plane)

ground_cylinder = gr.cylinder('ground_cylinder')
ground_cylinder:scale(2,0.05,1.2)
ground_cylinder:translate(0,-0.045,0)
ground_cylinder:set_material(stone)
ground_cylinder:set_texture(texture_checkerboard)
plane:add_child(ground_cylinder)





cylinder_x = gr.cylinder('cylinder_x')
cylinder_x:scale(1/50,1/50,1/100)
cylinder_x:scale(3,10,6)
cylinder_x:rotate('z', 90)
cylinder_x:translate(0,0.13,-0.1)
cylinder_x:translate(0.08,0.2,.5)
cylinder_x:set_material(perfect_crystal)

cylinder_z = gr.cylinder('cylinder_z')
cylinder_z:scale(1/50,1/50,1/100)
cylinder_z:scale(3,10,6)
cylinder_z:rotate('z', 90)
cylinder_z:rotate('y', 90)
cylinder_z:translate(0,0.13,-0.1)
cylinder_z:translate(0.08,0.2,.5)
cylinder_z:set_material(perfect_crystal)

sphere = gr.sphere('sphere')
sphere:scale(1/50,1/50,1/100)
sphere:scale(5,5,10)
sphere:translate(0,0.13,-0.1)
sphere:translate(0.05,0.23,0.55)
sphere:translate(0.025,-0.025,-0.047)
sphere:set_material(perfect_crystal)

xz = gr.csg('xz')
xz:set_csg_children(cylinder_x, cylinder_z, 'UNION')

cubeWithoutCore = gr.csg('cubeWithoutCore')
cubeWithoutCore:scale(2,2,2)
cubeWithoutCore:rotate('y', 45)
cubeWithoutCore:translate(-0.68,-0.3,-0.1)
cubeWithoutCore:set_csg_children(sphere, xz, 'DIFFERENCE')
plane:add_child(cubeWithoutCore)








top_cone = gr.cone('top_cone')
top_cone:scale(1/50,1/50,1/100)
top_cone:scale(3,10,6)
top_cone:translate(0.3,0.13,-0.1)
top_cone:translate(0.08,0.2,.5)
top_cone:set_material(perfect_crystal)

top_cone_2 = gr.cone('top_cone_2')
top_cone_2:scale(1/50,1/50,1/100)
top_cone_2:scale(3,13,6)
top_cone_2:rotate('x', 180)
top_cone_2:translate(0.3,0.06,-0.1)
top_cone_2:translate(0.08,0.2,.5)
top_cone_2:set_material(perfect_crystal)

bottom_sphere = gr.sphere('bottom_sphere')
bottom_sphere:scale(1/50,1/50,1/100)
bottom_sphere:scale(3,10,6)
bottom_sphere:rotate('x', 180)
bottom_sphere:translate(0.3,0,-0.1)
bottom_sphere:translate(0.08,0.18,.5)
bottom_sphere:set_material(perfect_crystal)

center_rod = gr.cylinder('center_rod')
center_rod:scale(1/50,1/50,1/100)
center_rod:scale(1,10,2)
center_rod:translate(0.3,0.13,-0.1)
center_rod:translate(0.08,0.2,.5)
center_rod:set_material(perfect_crystal)

conicsg = gr.csg('conicsg')
conicsg:set_csg_children(top_cone, bottom_sphere, 'INTERSECT')
rodsg = gr.csg('rodsg')
rodsg:set_csg_children(conicsg, center_rod, 'UNION')
spikesg = gr.csg('spikesg')
spikesg:set_csg_children(rodsg, top_cone_2, 'DIFFERENCE')

plane:add_child(spikesg)


gr.render(scene, 'refraction4.png', 700, 500,
    {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {primary_light, secondary_light, frontal_light})

