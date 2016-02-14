-- puppet.lua
-- A simplified puppet without posable joints, but that
-- looks roughly humanoid.

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
yellow = gr.material({1.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)

root_transforms = gr.node('root_transforms')

rootnode = gr.node('root')
rwing = gr.node('rwing')
root_transforms:add_child(rootnode)
rootnode:scale( 0.25, 0.25, 0.25 )
rootnode:translate(0.0, 0.0, -1.0)


torso = gr.mesh('sphere', 'torso')

--wings are problematic once again
shoulder = gr.mesh('sphere', 'shoulder')
shoulder_j = gr.joint('shoulder_j', {-10,0,10}, {-24,0,24})
right_wing = gr.mesh('sphere', 'right_wing')
right_wing_j = gr.joint('right_wing_j', {-10,0,10}, {-40,0,40})
right_wing_outer = gr.mesh('cube', 'right_wing_outer')
right_wing_outer_j = gr.joint('right_wing_outer_j', {-10,0,10}, {-40,-0,40})
right_wing_o = gr.mesh('sphere', 'right_wing')
right_wing_j_o = gr.joint('right_wing_j', {-10,0,10}, {-40,0,40})
right_wing_outer_o = gr.mesh('cube', 'right_wing_outer')
right_wing_outer_j_o = gr.joint('right_wing_outer_j', {-10,0,10}, {-40,-0,40})

rootnode:add_child(torso)

torso:add_child(shoulder_j)

shoulder:add_child(right_wing_j)
right_wing:add_child(right_wing_outer_j)
shoulder:add_child(right_wing_j_o)
right_wing_o:add_child(right_wing_outer_j_o)

shoulder_j:add_child(shoulder)

right_wing_j:add_child(right_wing)
right_wing_outer_j:add_child(right_wing_outer)

right_wing_j_o:add_child(right_wing_o)
right_wing_outer_j_o:add_child(right_wing_outer_o)

-- start modelling
torso:set_material(blue)
torso:scale(0.25,0.5,0.18)

-- shoulder
shoulder:set_material(green)
shoulder:scale(1/0.25,1/0.5,1/0.18)
shoulder:scale(0.27,0.14,0.13)
shoulder:translate(0.0,0.30,0.0)

right_wing:set_material(blue)
right_wing:scale(0.421,0.07,0.065)
right_wing:translate(0.38,0.0,0.0)
right_wing:rotate('z', 0)
right_wing:translate(0.25,0,0.0)
right_wing:scale(1/0.27,1/0.14,1/0.13)
--right_wing:scale(0.421,0.07,0.065)
--right_wing:rotate('z', 15)
right_wing_o:set_material(blue)
right_wing_o:scale(0.421,0.07,0.065)
right_wing_o:translate(0.38,0.0,0.0)
right_wing_o:rotate('z', 90)
right_wing_o:translate(0.25,0,0.0)
right_wing_o:scale(1/0.27,1/0.14,1/0.13)


right_wing_outer:set_material(red)
right_wing_outer:scale(1,0.5,0.5)
--right_wing_outer:rotate('z', -50)
right_wing_outer:translate(1.75,-0.1,0.0)
right_wing_outer:scale(1.2,1/0.5,1/0.5)



--right_wing_o:scale(0.421,0.07,0.065)
--right_wing_o:rotate('z', 15)

right_wing_outer_o:set_material(red)
right_wing_outer_o:scale(1,0.5,0.5)
--right_wing_outer_o:rotate('z', -50)
right_wing_outer_o:translate(1.75,-0.1,0.0)
right_wing_outer_o:scale(1.2,1/0.5,1/0.5)











return rootnode
