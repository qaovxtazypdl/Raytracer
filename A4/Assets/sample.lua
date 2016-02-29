hide = gr.material({0.6, 0.53, 0.84}, {0.3, 0.3, 0.3}, 40)
stone = gr.material({0.5, 0.40, 0.01}, {0.2, 0.05, 0.03}, 20)
mirror = gr.material({0.2, 0.3, 0.3}, {0.75, 0.75, 0.75}, 120)
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

mirror_plane = gr.mesh( 'plane', 'Assets/plane.obj' )
mirror_plane:set_material(mirror)

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
scene:add_child(plane)
plane:set_material(stone)
plane:scale(50,50,100)
plane:rotate('x', 15)

mirror_1 = gr.node('plane_instance_1')
mirror_1:rotate('z', 90)
mirror_1:rotate('y', 15)
mirror_1:translate(0.25,1,-0.2)
mirror_1:scale(40/50,16/50,60/100)
mirror_1:add_child(mirror_plane)
plane:add_child(mirror_1)

mirror_2 = gr.node('plane_instance_2')
mirror_2:rotate('z', 90)
mirror_2:rotate('y', -15)
mirror_2:translate(-0.25,1,-0.2)
mirror_2:scale(40/50,16/50,60/100)
mirror_2:add_child(mirror_plane)
plane:add_child(mirror_2)


central_sphere = gr.sphere('central_sphere', {0, 10, -10}, 10)
central_sphere:scale(1/50,1/50,1/100)
central_sphere:scale(10,10,10)
central_sphere:translate(0,10,-10)
scene:add_child(central_sphere)
plane:set_material(chrome)

bottom_sphere_1 = gr.sphere('bottom_sphere_1')
--central_sphere:add_child(bottom_sphere_1)
bottom_sphere_1:set_material(chrome)

bottom_sphere_1 = gr.sphere('bottom_sphere_1')
--central_sphere:add_child(bottom_sphere_1)
bottom_sphere_1:set_material(chrome)

bottom_sphere_1 = gr.sphere('bottom_sphere_1')
--central_sphere:add_child(bottom_sphere_1)
bottom_sphere_1:set_material(chrome)

bottom_sphere_1 = gr.sphere('bottom_sphere_1')
--central_sphere:add_child(bottom_sphere_1)
bottom_sphere_1:set_material(chrome)

gr.render(scene, 'sample.png', 300, 300,
    {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {primary_light, secondary_light, frontal_light})

