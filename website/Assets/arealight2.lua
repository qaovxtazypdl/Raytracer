hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
stone = gr.material({0.5, 0.40, 0.01}, {0.25, 0.15, 0.15}, 20)
carpet = gr.material({0.6, 0.02, 0.1}, {0,0,0}, 0)
mirror = gr.material({0.1, 0.16, 0.15}, {1.5,1.5,1.5}, 120)
light_material_orange = gr.material_extended({1.0, 0.6, 0}, {0.0, 0.0, 0.0}, 0, 1, 0.5, 0, 1)
light_material_blue = gr.material_extended({0.0, 0.4, 1.0}, {0.0, 0.0, 0.0}, 0, 1, 0.5, 0, 1)

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

planar_light = gr.planar_light({20, 18, -5}, {1.0, 0.6, 0}, {1, 0, 0}, {0, 0, 5}, {0, 5, 0})
planar_light_pt = gr.light({20, 18, -5}, {1.0, 0.6, 0}, {1, 0, 0}, {0, 0, 5}, {0, 5, 0})
spherical_light = gr.spherical_light({-21, 27, -15}, {0.0, 0.4, 1.0}, {1, 0, 0}, 4.0)
spherical_light_pt = gr.light({-21, 27, -15}, {0.0, 0.4, 1.0}, {1, 0, 0}, 4.0)

--start script
scene = gr.node('root')

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
plane:set_material(pewter)
plane:scale(50,50,100)
plane:rotate('x', 15)
plane:translate(0,-1,0)
scene:add_child(plane)

central_torus = gr.torus('central_torus', 0.25)
central_torus:scale(1/50,1/50,1/100)
central_torus:scale(12,12,24)
central_torus:rotate('x', 90)
central_torus:translate(0,0.134,-0.1)
central_torus:translate(0.0,0.47,.6)
central_torus:set_material(turquoise)

central_cylinder = gr.cylinder('central_cylinder', 0.25)
central_cylinder:scale(1/50,1/50,1/100)
central_cylinder:scale(12,12,24)
central_cylinder:translate(0,0.11,-0.1)
central_cylinder:translate(0.0,0.5,0.6)
central_cylinder:set_material(hide)

central_cylinderUtorus = gr.csg('central_cylinderUtorus')
central_cylinderUtorus:set_csg_children(central_cylinder, central_torus, 'DIFFERENCE')
central_cylinderUtorus:translate(0,-0.25,-0.5)
plane:add_child(central_cylinderUtorus)

gr.render(scene, 'area_light2.png', 800, 700,
    {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {planar_light, spherical_light})

