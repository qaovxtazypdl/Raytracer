hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
stone = gr.material({0.5, 0.40, 0.01}, {0.25, 0.15, 0.15}, 20)
carpet = gr.material({0.6, 0.02, 0.1}, {0,0,0}, 0)
mirror = gr.material({0.1, 0.16, 0.15}, {1.5,1.5,1.5}, 120)

perfectCanvas = gr.material({1.0, 1.0, 1.0}, {0, 0, 0}, 0)

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

primary_light = gr.light({-300.0, 200, 400.0}, {1.0, 0.6, 0}, {1, 0, 0})
primary_light_oppo = gr.light({300.0, 200, 400.0}, {0, 0.4, 1.0}, {1, 0, 0})

texture_checkerboard = gr.texture('Assets/checkerboard.png')
texture_noise = gr.texture('Assets/perlin1.png')

--start script
scene = gr.node('root')

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
plane:set_material(carpet)
plane:scale(50,50,100)
plane:rotate('x', 15)
plane:translate(0,-1,0)
scene:add_child(plane)

central_cylinder = gr.cylinder('central_cylinder')
central_cylinder:scale(1/50,1/50,1/100)
central_cylinder:scale(7,7,7)
central_cylinder:translate(0,0.13,-0.1)
central_cylinder:translate(0.3,0.2,.5)
central_cylinder:set_material(perfectCanvas)
central_cylinder:set_texture(texture_checkerboard)
central_cylinder:set_bumps(texture_checkerboard, 'rgb')
plane:add_child(central_cylinder)

central_cube = gr.cube('central_cube')
central_cube:scale(1/50,1/50,1/100)
central_cube:scale(10,10,10)
central_cube:translate(0,0,-0.1)
central_cube:translate(-0.3,0,0.65)
central_cube:set_material(perfectCanvas)
central_cube:set_texture(texture_checkerboard)
central_cube:set_bumps(texture_checkerboard, 'rgb')
--plane:add_child(central_cube)

central_cone = gr.cone('central_cone')
central_cone:scale(1/50,1/50,1/100)
central_cone:scale(5,5,5)
central_cone:translate(0,0.13,-0.1)
central_cone:translate(-0.32,0.2,0.5)
central_cone:set_material(perfectCanvas)
central_cone:set_texture(texture_checkerboard)
central_cone:set_bumps(texture_checkerboard, 'rgb')
--plane:add_child(central_cone)

central_sphere = gr.sphere('central_sphere')
central_sphere:scale(1/50,1/50,1/100)
central_sphere:scale(4,4,4)
central_sphere:translate(0,0,-0.1)
central_sphere:translate(0.3,0.075,0.65)
central_sphere:set_material(perfectCanvas)
central_sphere:set_texture(texture_checkerboard)
central_sphere:set_bumps(texture_checkerboard, 'rgb')
--plane:add_child(central_sphere)

central_hyperboloid = gr.hyperboloid('central_hyperboloid', 0.1)
central_hyperboloid:scale(1/50,1/50,1/100)
central_hyperboloid:scale(5,5,5)
central_hyperboloid:rotate('x', -30)
central_hyperboloid:translate(0,0.16,0)
central_hyperboloid:translate(0,0,0.5)
central_hyperboloid:set_material(perfectCanvas)
central_hyperboloid:set_texture(texture_checkerboard)
central_hyperboloid:set_bumps(texture_checkerboard, 'rgb')
--plane:add_child(central_hyperboloid)

central_torus = gr.torus('central_torus', 0.3)
central_torus:scale(1/50,1/50,1/100)
central_torus:scale(10,10,10)
central_torus:translate(0,0.13,-0.1)
central_torus:translate(0,0.2,0.2)
central_torus:set_material(perfectCanvas)
central_torus:set_texture(texture_checkerboard)
central_torus:set_bumps(texture_checkerboard, 'rgb')
--plane:add_child(central_torus)



gr.render(scene, 'bumpall.png', 900, 900,
    {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
    {0.1, 0.1, 0.1}, {primary_light_oppo, primary_light})

