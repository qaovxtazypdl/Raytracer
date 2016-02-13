shoulder = gr.mesh('sphere', 'shoulder')
left_wing = gr.mesh('sphere', 'left_wing')
right_wing = gr.mesh('sphere', 'right_wing')
left_wing_outer = gr.mesh('sphere', 'left_wing_outer')
right_wing_outer = gr.mesh('sphere', 'right_wing_outer')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

shoulder:add_child(left_wing)
shoulder:add_child(right_wing)
left_wing:add_child(left_wing_outer)
right_wing:add_child(right_wing_outer)

shoulder:scale( 0.05, 0.1, 0.1 )
shoulder:translate(0.0, 0.0, -1.0)
shoulder:set_material(green)
shoulder:scale(1.08,0.27,0.7)

left_wing:set_material(blue)
left_wing:translate(-1.0,0.0,0.0)
left_wing:scale(1.56,0.27,0.4)
left_wing:rotate('z', -15)

right_wing:set_material(blue)
right_wing:translate(1.0,0.30,0.0)
right_wing:scale(1.56,0.27,0.4)
right_wing:rotate('z', 15)

left_wing_outer:set_material(red)
left_wing_outer:translate(-1.7,0.0,0.0)
left_wing_outer:rotate('z', 30)

right_wing_outer:set_material(red)
right_wing_outer:translate(1.7,0.0,0.0)
right_wing_outer:rotate('z', -30)

return shoulder
