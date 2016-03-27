hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
stone = gr.material({0.5, 0.40, 0.01}, {0.25, 0.15, 0.15}, 20)
carpet = gr.material({0.6, 0.02, 0.1}, {0,0,0}, 0)
mirror = gr.material({0.1, 0.16, 0.15}, {1.5,1.5,1.5}, 120)

perfectCanvas = gr.material({1.0, 1.0, 1.0}, {0, 0, 0}, 0)
reflectorCanvas = gr.material_extended({0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, 1000, 1, 1, 0, 0)
refractorCanvas = gr.material_extended({0.6, 0.9, 0.9}, {1.5, 0.4, 1.3}, 1000, 1, 0.4, 0, 0)
glossyReflectorCanvas = gr.material_extended({0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, 1000, 1, 1, 2.4, 0)

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

primary_light = gr.light({200.0, 350.0, 200.0}, {1.0, 1.0, 1.0}, {1, 0, 0})

texture_checkerboard = gr.texture('Assets/checkerboard.png')
texture_planks = gr.texture('Assets/planks.png')
texture_bricks = gr.texture('Assets/bricks.png')
texture_globe = gr.texture('Assets/globetexture.png')
texture_cubemap = gr.texture('Assets/cube_map.png')

--start script
scene = gr.node('root')

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
plane:set_material(carpet)
plane:scale(50,50,100)
plane:rotate('x', 15)
plane:translate(0,-1,0)
scene:add_child(plane)

ground_cylinder = gr.cylinder('ground_cylinder')
ground_cylinder:scale(2,0.05,1.2)
ground_cylinder:translate(0,-0.045,0)
ground_cylinder:set_material(stone)
ground_cylinder:set_bumps(texture_checkerboard, 'rgb')
plane:add_child(ground_cylinder)

central_cylinder = gr.cone('central_cylinder')
central_cylinder:translate(0,0.5,0)
central_cylinder:scale(1/50,1/50,1/100)
central_cylinder:scale(5,10,5)
central_cylinder:translate(0,0.13,-0.1)
central_cylinder:translate(0.33,0.2,.5)
central_cylinder:set_material(perfectCanvas)
central_cylinder:set_bumps(texture_planks, 'rgb')
central_cylinder_inner = gr.cylinder('central_cylinder_inner')
central_cylinder_inner:scale(1/50,1/50,1/100)
central_cylinder_inner:scale(4,8,4)
central_cylinder_inner:translate(0,0.13,-0.1)
central_cylinder_inner:translate(0.33,0.2,.5)
central_cylinder_inner:set_material(perfectCanvas)
central_cylinder_inner:set_bumps(texture_bricks, 'rgb')
cupsg = gr.csg('cupsg')
cupsg:set_csg_children(central_cylinder, central_cylinder_inner, 'DIFFERENCE')
cupsg:rotate('x', 30)
cupsg:translate(0,0.2,-0.1)
plane:add_child(cupsg)

central_cone = gr.cylinder('central_cone')
central_cone:scale(1/50,1/50,1/100)
central_cone:scale(5,5,10)
central_cone:translate(0,0.13,-0.1)
central_cone:translate(-0.32,0.2,0.5)
central_cone:set_material(refractorCanvas)
central_cone:set_bumps(texture_bricks, 'rgb')
plane:add_child(central_cone)

central_hyperboloid = gr.hyperboloid('central_hyperboloid', 0.1)
central_hyperboloid:scale(1/50,1/50,1/100)
central_hyperboloid:scale(5,5,10)
central_hyperboloid:rotate('z', 90)
central_hyperboloid:rotate('y', 25)
central_hyperboloid:translate(0,0.13,-0.1)
central_hyperboloid:translate(0,0,0.5)
central_hyperboloid:set_material(glossyReflectorCanvas)
central_hyperboloid:set_bumps(texture_planks, 'rgb')
plane:add_child(central_hyperboloid)

central_torus = gr.torus('central_torus', 0.3)
central_torus:scale(1/50,1/50,1/100)
central_torus:scale(10,10,20)
central_torus:rotate('y', 30)
central_torus:translate(0.1,0.13,-0.1)
central_torus:translate(0,0.2,0.2)
central_torus:set_material(reflectorCanvas)
central_torus:set_bumps(texture_planks, 'rgb')
plane:add_child(central_torus)

central_cube = gr.cube('central_cube')
central_cube:scale(1/50,1/50,1/100)
central_cube:scale(8,8,8)
central_cube:rotate('z', 15)
central_cube:translate(0,0,-0.1)
central_cube:translate(-0.3,0,0.65)
central_cube:set_material(perfectCanvas)
central_cube:set_bumps(texture_cubemap, 'rgb')
plane:add_child(central_cube)

central_sphere = gr.sphere('central_sphere')
central_sphere:scale(1/50,1/50,1/100)
central_sphere:scale(8,8,8)
central_sphere:translate(0,0,-0.1)
central_sphere:translate(0.3,0.075,0.65)
central_sphere:set_material(perfectCanvas)
central_sphere:set_bumps(texture_globe, 'rgb')
plane:add_child(central_sphere)

gr.render(scene, 'bumpRight.png', 800, 700,
    {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {primary_light})

