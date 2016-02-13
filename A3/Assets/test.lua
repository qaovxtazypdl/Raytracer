right_wing = gr.mesh('sphere', 'right_wing')
right_wing_outer = gr.mesh('sphere', 'right_wing_outer')
right_wing_outer_cube = gr.mesh('cube', 'right_wing_outer')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

right_wing:add_child(right_wing_outer)
--right_wing:add_child(right_wing_outer_cube)
right_wing:set_material(blue)


right_wing:scale(0.1,0.03,0.04)

right_wing:rotate('z', 45)
right_wing:translate(0.0, 0.0, -1.0)

right_wing_outer:set_material(red)
right_wing_outer:scale(1,0.3,0.4)
right_wing_outer:rotate('z', -45)
right_wing_outer:translate(1.7,0.0,0.0)
right_wing_outer:scale(1,1/0.3,1/0.4)


right_wing_outer_cube:set_material(red)
right_wing_outer_cube:scale(3,3,3)
right_wing_outer_cube:translate(1.7,0.0,0.0)
right_wing_outer_cube:rotate('z', -45)
return right_wing
