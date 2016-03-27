hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
stone = gr.material({0.5, 0.30, 0.30}, {0.25, 0.15, 0.15}, 20)
carpet = gr.material({0.6, 0.02, 0.1}, {0,0,0}, 0)
mirror = gr.material({0.1, 0.16, 0.15}, {1.5,1.5,1.5}, 120)

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

--start script
scene = gr.node('root')

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
plane:set_material(stone)
plane:scale(50,50,100)
plane:rotate('x', 15)
plane:translate(0,-1,0)
scene:add_child(plane)

central_cylinder = gr.cylinder('central_cylinder')
central_cylinder:scale(1/50,1/50,1/100)
central_cylinder:scale(5,5,10)
central_cylinder:translate(0,0.13,-0.1)
central_cylinder:translate(0.08,0.2,.5)
central_cylinder:set_material(obsidian)

central_cone = gr.cone('central_cone')
central_cone:scale(1/50,1/50,1/100)
central_cone:scale(5,5,10)
central_cone:translate(0,0.13,-0.1)
central_cone:translate(0.05,0.23,0.55)
central_cone:set_material(polgold)

coneUcylinder = gr.csg('coneUcylinder')
coneUcylinder:set_csg_children(central_cylinder, central_cone, 'UNION')
coneUcylinder:translate(0,-0.2,0)
plane:add_child(coneUcylinder)

coneIcylinder = gr.csg('coneIcylinder')
coneIcylinder:set_csg_children(central_cylinder, central_cone, 'INTERSECT')
coneIcylinder:translate(0,0.05,0)
plane:add_child(coneIcylinder)

coneDcylinder = gr.csg('coneDcylinder')
coneDcylinder:set_csg_children(central_cylinder, central_cone, 'DIFFERENCE')
coneDcylinder:translate(0,0.2,0)
plane:add_child(coneDcylinder)


central_torus = gr.torus('central_torus', 0.25)
central_torus:scale(1/50,1/50,1/100)
central_torus:scale(4,4,8)
central_torus:translate(0,0.13,-0.1)
central_torus:translate(-0.4,0.2,.5)
central_torus:set_material(turquoise)

central_hyperboloid = gr.hyperboloid('central_hyperboloid', 0.25)
central_hyperboloid:scale(1/50,1/50,1/100)
central_hyperboloid:scale(4,4,8)
central_hyperboloid:translate(0,0.13,-0.1)
central_hyperboloid:translate(-0.4,0.23,0.5)
central_hyperboloid:set_material(hide)

central_hyperboloidUtorus = gr.csg('central_hyperboloidUtorus')
central_hyperboloidUtorus:set_csg_children(central_torus, central_hyperboloid, 'UNION')
central_hyperboloidUtorus:translate(0,-0.25,0)
plane:add_child(central_hyperboloidUtorus)

central_hyperboloidItorus = gr.csg('central_hyperboloidItorus')
central_hyperboloidItorus:set_csg_children(central_torus, central_hyperboloid, 'INTERSECT')
central_hyperboloidItorus:translate(0,0.0,0)
plane:add_child(central_hyperboloidItorus)

central_hyperboloidDtorus = gr.csg('central_hyperboloidDtorus')
central_hyperboloidDtorus:set_csg_children(central_torus, central_hyperboloid, 'DIFFERENCE')
central_hyperboloidDtorus:translate(0,0.25,0)
plane:add_child(central_hyperboloidDtorus)


central_sphere = gr.sphere('central_sphere')
central_sphere:scale(1/50,1/50,1/100)
central_sphere:scale(4,4,4)
central_sphere:translate(0,0.13,-0.1)
central_sphere:translate(0.4,0.2,.5)
central_sphere:set_material(chrome)

central_cube = gr.cube('central_cube')
central_cube:translate(-0.5,-0.753,-0.5)
central_cube:scale(1/50,1/50,1/100)
central_cube:scale(6,6,6)
central_cube:translate(0,0.13,-0.1)
central_cube:translate(0.4,0.23,0.5)
central_cube:set_material(emerald)

central_cubeUsphere = gr.csg('central_cubeUsphere')
central_cubeUsphere:set_csg_children(central_cube, central_sphere, 'UNION')
central_cubeUsphere:translate(0,-0.25,0)
plane:add_child(central_cubeUsphere)

central_cubeIsphere = gr.csg('central_cubeIsphere')
central_cubeIsphere:set_csg_children(central_cube, central_sphere, 'INTERSECT')
central_cubeIsphere:translate(0,0.0,0)
plane:add_child(central_cubeIsphere)

central_cubeDsphere = gr.csg('central_cubeDsphere')
central_cubeDsphere:set_csg_children(central_cube, central_sphere, 'DIFFERENCE')
central_cubeDsphere:translate(0,0.25,0)
plane:add_child(central_cubeDsphere)


gr.render(scene, 'AntiAlias1.png', 700, 500,
    {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {primary_light, secondary_light, frontal_light})

