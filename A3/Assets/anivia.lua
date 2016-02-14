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


--same problem as wings
left_thigh = gr.mesh('sphere', 'left_thigh')
left_thigh_j = gr.joint('left_thigh_j', {-20,-20,20}, {0,0,0})
right_thigh = gr.mesh('sphere', 'right_thigh')
right_thigh_j = gr.joint('right_thigh_j', {-20,-20,20}, {0,0,0})
left_leg = gr.mesh('sphere', 'left_leg')
left_leg_j = gr.joint('left_leg_j', {-20,-20,20}, {0,0,0})
right_leg = gr.mesh('sphere', 'right_leg')
right_leg_j = gr.joint('right_leg_j', {-20,-20,20}, {0,0,0})
left_talons = gr.mesh('cube', 'left_talons')
left_talons_j = gr.joint('left_talons_j', {-20,-20,20}, {0,0,0})
right_talons = gr.mesh('cube', 'abdomen')
right_talons_j = gr.joint('right_talons_j', {-20,-20,20}, {0,0,0})

neck = gr.mesh('sphere', 'neck')
neck_j = gr.joint('neck_j', {-20,0,20}, {-30,0,30})
head = gr.mesh('sphere', 'head')
head_j = gr.joint('head_j', {-25,0,25}, {-45,0,45})
--wings are problematic once again
shoulder = gr.mesh('sphere', 'shoulder')
shoulder_j = gr.joint('shoulder_j', {-10,0,10}, {-24,0,24})
left_wing = gr.mesh('sphere', 'left_wing')
left_wing_j = gr.joint('left_wing_j', {-10,0,10}, {-40,40,40})
right_wing = gr.mesh('sphere', 'right_wing')
right_wing_j = gr.joint('right_wing_j', {-10,0,10}, {-40,-40,40})
left_wing_outer = gr.mesh('sphere', 'left_wing_outer')
left_wing_outer_j = gr.joint('left_wing_outer_j', {-10,0,10}, {-40,40,40})
right_wing_outer = gr.mesh('sphere', 'right_wing_outer')
right_wing_outer_j = gr.joint('right_wing_outer_j', {-10,0,10}, {-40,-40,40})

abdomen = gr.mesh('sphere', 'abdomen')
abdomen_j = gr.joint('abdomen_j', {-10,0,10}, {-24,0,24})
tail = gr.mesh('cube', 'tail')
tail_j = gr.joint('tail_j', {-30,15,30}, {-10,0,10})
tail_left = gr.mesh('cube', 'tail_left')
tail_left_j = gr.joint('tail_left_j', {-30,-30,30}, {0,0,0})
tail_right = gr.mesh('cube', 'tail_right')
tail_right_j = gr.joint('tail_right_j', {-30,-30,30}, {0,0,0})
tail_mid = gr.mesh('cube', 'tail_mid')
tail_mid_j = gr.joint('tail_mid_j', {-30,-30,30}, {0,0,0})

rootnode:add_child(torso)

torso:add_child(neck_j)
torso:add_child(shoulder_j)
torso:add_child(abdomen_j)

neck:add_child(head_j)

shoulder:add_child(left_wing_j)
shoulder:add_child(right_wing_j)
left_wing:add_child(left_wing_outer_j)
right_wing:add_child(right_wing_outer_j)

abdomen:add_child(left_thigh_j)
abdomen:add_child(right_thigh_j)
abdomen:add_child(tail_j)

left_thigh:add_child(left_leg_j)
right_thigh:add_child(right_leg_j)
left_leg:add_child(left_talons_j)
right_leg:add_child(right_talons_j)
tail:add_child(tail_left_j)
tail:add_child(tail_right_j)
tail:add_child(tail_mid_j)

abdomen_j:add_child(abdomen)
left_thigh_j:add_child(left_thigh)
right_thigh_j:add_child(right_thigh)
left_leg_j:add_child(left_leg)
right_leg_j:add_child(right_leg)
left_talons_j:add_child(left_talons)
right_talons_j:add_child(right_talons)
shoulder_j:add_child(shoulder)
left_wing_j:add_child(left_wing)
right_wing_j:add_child(right_wing)
left_wing_outer_j:add_child(left_wing_outer)
right_wing_outer_j:add_child(right_wing_outer)
neck_j:add_child(neck)
head_j:add_child(head)
tail_j:add_child(tail)
tail_left_j:add_child(tail_left)
tail_right_j:add_child(tail_right)
tail_mid_j:add_child(tail_mid)

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
left_wing:translate(-1.56,0.0,0.0)
left_wing:scale(1.56,0.27,0.4)
left_wing:rotate('z', -15)

right_wing:set_material(blue)
right_wing:translate(1.56,0.30,0.0)
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
tail_left:scale(0.4,1.15,0.6)
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
