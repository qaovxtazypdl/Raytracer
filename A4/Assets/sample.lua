hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
stone = gr.material({0.5, 0.40, 0.01}, {0.2, 0.05, 0.03}, 20)
carpet = gr.material({0.6, 0.02, 0.1}, {0,0,0}, 0)
mirror = gr.material({0.2, 0.3, 0.3}, {1.5,1.5,1.5}, 120)

--obtained from https://www.student.cs.uwaterloo.ca/~cs488/mat.txt
copper = gr.material({0.7038, 0.27048, 0.0828}, {0.25677, 0.13762, 0.086014}, 12.8)
emerald = gr.material({0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 76.8)
jade = gr.material({0.54, 0.89, 0.63}, {0.316228, 0.316228, 0.316228}, 12.8)
obsidian = gr.material({0.18275, 0.17, 0.22525}, {0.332741, 0.328634, 0.346435}, 38.4)
pearl = gr.material({1, 0.829, 0.829}, {0.296648, 0.296648, 0.296648}, 11.264)
ruby = gr.material({0.61424, 0.04136, 0.04136}, {0.727811, 0.626959, 0.626959}, 76.8)
turquoise = gr.material({0.396, 0.74151, 0.69102}, {0.297254, 0.30829, 0.306678}, 12.8)
brass = gr.material({0.78039, 0.568627, 0.113725}, {0.99215, 0.941176, 0.807843}, 27.9)
bronze = gr.material({0.714, 0.4284, 0.18144}, {0.393548, 0.271906, 0.166721}, 25.6)
chrome = gr.material({0.4, 0.4, 0.4}, {0.774597, 0.774597, 0.774597}, 76.8)
gold = gr.material({0.75164, 0.60648, 0.22648}, {0.628281, 0.555802, 0.366065}, 51.2)
silver = gr.material({0.50754, 0.50754, 0.50754}, {0.508273, 0.508273, 0.508273}, 51.2)
brubber = gr.material({0.01, 0.01, 0.01}, {0.4, 0.4, 0.4}, 10)
polbronze = gr.material({0.4, 0.2368, 0.1036}, {0.774597, 0.458561, 0.200621}, 76.8)
polcopper = gr.material({0.5508, 0.2118, 0.066}, {0.580594, 0.223257, 0.0695701}, 51.2)
polgold = gr.material({0.34615, 0.3143, 0.0903}, {0.797357, 0.723991, 0.208006}, 83.2)
pewter = gr.material({0.427451, 0.470588, 0.541176}, {0.33333, 0.33333, 0.521569}, 9.84615)
polsilver = gr.material({0.50754, 0.50754, 0.50754}, {0.508273, 0.508273, 0.508273}, 51.2)

primary_light = gr.light({-100.0, 350.0, 200.0}, {1.0, 1.0, 1.0}, {1, 0, 0})
secondary_light = gr.light({0, 100.0, 0}, {0.1, 0.7, 0.5}, {1, 0.4, 0})
frontal_light = gr.light({0, 0, 200}, {0.4, 0.4, 0.5}, {1, 0, 0})

--start script
scene = gr.node('root')

--MIRROR instance
mirror_edge = gr.mesh('mirror_edge', 'Assets/cylinder.obj')
mirror_edge:set_material(silver)
mirror_edge:scale(0.01,0.7,0.01)
mirror_edge:translate(0.0,-0.37,0.61)

mirror_plane = gr.mesh( 'mirror_plane', 'Assets/plane.obj' )
mirror_plane:rotate('z', 90)
mirror_plane:scale(40/50,16/50,60/100)
mirror_plane:set_material(mirror)

mirror_root = gr.node('mirror_root')
mirror_root:add_child(mirror_plane)
mirror_root:add_child(mirror_edge)
--end mirror instance

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
plane:set_material(stone)
plane:scale(50,50,100)
plane:rotate('x', 15)
plane:translate(0,-1,0)
scene:add_child(plane)

cow = gr.mesh( 'cow', 'Assets/cow.obj' )
cow:rotate('y', 90)
cow:translate(0,4.3,22)
cow:scale(0.01,0.01,0.01)
cow:set_material(hide)
--plane:add_child(cow)

mirror_1 = gr.node('mirror_instance_1')
mirror_1:rotate('y', 20)
mirror_1:translate(0.2,0.32,-0.12)
mirror_1:add_child(mirror_root)
plane:add_child(mirror_1)

mirror_2 = gr.node('mirror_instance_2')
mirror_2:rotate('y', -20)
mirror_2:translate(-0.2,0.32,-0.12)
mirror_2:add_child(mirror_root)
plane:add_child(mirror_2)


central_sphere = gr.sphere('central_sphere')
central_sphere:scale(1/50,1/50,1/100)
central_sphere:scale(10,10,10)
central_sphere:translate(0,0.13,-0.1)
central_sphere:set_material(chrome)
plane:add_child(central_sphere)

bottom_sphere_1 = gr.sphere('bottom_sphere_1')
bottom_sphere_1:scale(0.25,0.25,0.25)
bottom_sphere_1:translate(-0.9,-0.34,0.8)
bottom_sphere_1:set_material(ruby)
central_sphere:add_child(bottom_sphere_1)

bottom_sphere_2 = gr.sphere('bottom_sphere_2')
bottom_sphere_2:scale(0.25,0.25,0.25)
bottom_sphere_2:translate(-0.45,-0.34,1.15)
bottom_sphere_2:set_material(emerald)
central_sphere:add_child(bottom_sphere_2)

bottom_sphere_3 = gr.sphere('bottom_sphere_3')
bottom_sphere_3:scale(0.25,0.25,0.25)
bottom_sphere_3:translate(0.45,-0.34,1.15)
bottom_sphere_3:set_material(bronze)
central_sphere:add_child(bottom_sphere_3)

bottom_sphere_4 = gr.sphere('bottom_sphere_4')
bottom_sphere_4:scale(0.25,0.25,0.25)
bottom_sphere_4:translate(0.9,-0.34,0.8)
bottom_sphere_4:set_material(brubber)
central_sphere:add_child(bottom_sphere_4)

bottom_slab = gr.cube('bottom_slab')
bottom_slab:scale(0.5,0.25,0.5)
bottom_slab:translate(-0.25,-0.76,0.85)
bottom_slab:set_material(obsidian)
central_sphere:add_child(bottom_slab)

trophy = gr.cube('trophy')
trophy:scale(0.2,3,0.2)
trophy:translate(0.42,0,0.5)
trophy:set_material(gold)
bottom_slab:add_child(trophy)

bauble = gr.sphere('bauble')
bauble:scale(1/0.2,2/3.0,1/0.2)
bauble:scale(0.3,0.2,0.2)
bauble:translate(0.5,0.8,0.5)
bauble:set_material(gold)
trophy:add_child(bauble)

--nonhier sphere and cube outside
nhsphere = gr.nh_sphere('nhsphere', {-11,-13.8,50}, 0.5)
nhsphere:set_material(brubber)
scene:add_child(nhsphere)

nh_box = gr.nh_box('nh_box', {10,-14.7,50}, 1)
nh_box:set_material(pewter)
scene:add_child(nh_box)

--red carpet
redcarpet = gr.cube('redcarpet')
redcarpet:translate(-0.5,0.05,0)
redcarpet:scale(0.1,0.01,1)
redcarpet:set_material(carpet)
plane:add_child(redcarpet)

gr.render(scene, 'sample.png', 350, 350,
    {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {primary_light, secondary_light, frontal_light})

