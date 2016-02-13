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
root_transforms:add_child(rootnode)
rootnode:scale( 0.25, 0.25, 0.32 )
rootnode:translate(0.0, 0.0, -1.0)


torso = gr.mesh('sphere', 'torso')
abdomen = gr.mesh('sphere', 'abdomen')
left_thigh = gr.mesh('sphere', 'left_thigh')
right_thigh = gr.mesh('sphere', 'right_thigh')
left_leg = gr.mesh('sphere', 'left_leg')
right_leg = gr.mesh('sphere', 'right_leg')
left_talons = gr.mesh('cube', 'left_talons')
right_talons = gr.mesh('cube', 'abdomen')
shoulder = gr.mesh('sphere', 'shoulder')
left_wing = gr.mesh('sphere', 'left_wing')
right_wing = gr.mesh('sphere', 'right_wing')
left_wing_outer = gr.mesh('sphere', 'left_wing_outer')
right_wing_outer = gr.mesh('sphere', 'right_wing_outer')
neck = gr.mesh('sphere', 'neck')
head = gr.mesh('sphere', 'head')
tail = gr.mesh('cube', 'tail')
tail_left = gr.mesh('cube', 'tail_left')
tail_right = gr.mesh('cube', 'tail_right')
tail_mid = gr.mesh('cube', 'tail_mid')


rootnode:add_child(torso)

torso:add_child(neck)
torso:add_child(shoulder)
torso:add_child(abdomen)

neck:add_child(head)

shoulder:add_child(left_wing)
shoulder:add_child(right_wing)
left_wing:add_child(left_wing_outer)
right_wing:add_child(right_wing_outer)

abdomen:add_child(left_thigh)
abdomen:add_child(right_thigh)
abdomen:add_child(tail)

left_thigh:add_child(left_leg)
right_thigh:add_child(right_leg)
left_leg:add_child(left_talons)
right_leg:add_child(right_talons)
tail:add_child(tail_left)
tail:add_child(tail_right)
tail:add_child(tail_mid)


-- start modelling
torso:set_material(blue)
torso:scale(0.25,0.5,0.15)

-- neck
neck:set_material(white)
neck:scale(0.57,0.35,0.50)
neck:translate(0.0,0.75,0.0)

head:set_material(white)
head:scale(1.12,1.34,1.5)
head:translate(0.0,1.4,0.0)

-- shoulder
shoulder:set_material(green)
shoulder:scale(1.08,0.27,0.7)
shoulder:translate(0.0,0.30,0.0)

left_wing:set_material(blue)
left_wing:translate(-1.0,0.0,0.0)
left_wing:scale(1.56,0.27,0.4)
left_wing:rotate('z', -15)

right_wing:set_material(blue)
right_wing:translate(1.0,0.30,0.0)
right_wing:scale(1.56,0.27,0.4)
right_wing:rotate('z', 15)

left_wing_outer:set_material(red)
left_wing_outer:scale(1,0.2,0.4)
left_wing_outer:rotate('z', 50)
left_wing_outer:translate(-1.4,-0.75,0.0)
left_wing_outer:scale(1.2,1/0.3,1/0.4)

right_wing_outer:set_material(red)
right_wing_outer:scale(1,0.2,0.4)
right_wing_outer:rotate('z', -50)
right_wing_outer:translate(1.4,-0.75,0.0)
right_wing_outer:scale(1.2,1/0.3,1/0.4)


-- abdomen
abdomen:set_material(blue)
abdomen:scale(0.85,0.6,1.0)
abdomen:translate(0.0,-0.65,0.0)

tail:set_material(white)
tail:scale(1.34,0.9,0.35)
tail:translate(0.0,-0.63,0.0)

tail_mid:set_material(red)
tail_mid:scale(0.54,1.33,0.6)
tail_mid:translate(0.0,-0.93,0.0)

tail_left:set_material(red)
tail_left:scale(0.4,1.15,0.8)
tail_left:rotate('z', -18)
tail_left:translate(-0.5,-0.93,0.0)

tail_right:set_material(red)
tail_right:scale(0.4,1.15,0.6)
tail_right:rotate('z', 18)
tail_right:translate(0.5,-0.93,0.0)

-- legs
left_thigh:set_material(black)
left_thigh:scale(0.3,0.3,1.2)
left_thigh:translate(0.32,0.15,-0.6)

right_thigh:set_material(black)
right_thigh:scale(0.3,0.3,1.2)
right_thigh:translate(-0.32,0.15,-0.6)

left_leg:set_material(white)
left_leg:scale(0.65,0.65,1)
left_leg:translate(0,0,-0.5)

right_leg:set_material(white)
right_leg:scale(0.65,0.65,1)
right_leg:translate(0,0,-0.5)

left_talons:set_material(yellow)
left_talons:scale(1,2.3,0.15)
left_talons:translate(0,0.5,-0.94)

right_talons:set_material(yellow)
right_talons:scale(1,2.3,0.15)
right_talons:translate(0,0.5,-0.94)




return rootnode
