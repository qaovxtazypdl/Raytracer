-- puppet.lua
-- its a bird
-- its a plane
-- just kidding, it's just a bird

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
orange = gr.material({1.0, 0.45, 0.0}, {0.1, 0.1, 0.1}, 10)
yellow = gr.material({1.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)


rootnode = gr.node('root')
rootnode:scale( 0.25, 0.25, 0.25 )
rootnode:translate(0.0, 0.0, -1.0)

root_transforms = gr.node('root_transforms')
rootnode:add_child(root_transforms)


torso = gr.mesh('sphere', 'torso')


--same problem as wings
left_thigh = gr.mesh('sphere', 'left_thigh')
left_thigh_j = gr.joint('left_thigh_j', {-30,-10,20}, {0,0,0})
right_thigh = gr.mesh('sphere', 'right_thigh')
right_thigh_j = gr.joint('right_thigh_j', {-30,10,20}, {0,0,0})
left_leg = gr.mesh('sphere', 'left_leg')
left_leg_j = gr.joint('left_leg_j', {-46,-10,30}, {0,0,0})
right_leg = gr.mesh('sphere', 'right_leg')
right_leg_j = gr.joint('right_leg_j', {-46,10,30}, {0,0,0})
left_talons = gr.mesh('cube', 'left_talons')
left_talons_j = gr.joint('left_talons_j', {-100,-25,10}, {0,0,0})
right_talons = gr.mesh('cube', 'abdomen')
right_talons_j = gr.joint('right_talons_j', {-100,0,10}, {0,0,0})

neck = gr.mesh('sphere', 'neck')
neck_j = gr.joint('neck_j', {0,0,0}, {-45,0,45})
head = gr.mesh('sphere', 'head')
head_j = gr.joint('head_j', {-45,0,25}, {0,0,0})
beak = gr.mesh('cube', 'beak')
beak_fixed = gr.mesh('cube', 'beak')
beak_j = gr.joint('head_j', {-25,0,0}, {0,0,0})

shoulder = gr.mesh('sphere', 'shoulder')
shoulder_j = gr.joint('shoulder_j', {0,0,0}, {-24,0,24})
left_wing = gr.mesh('sphere', 'left_wing')
left_wing_j = gr.joint('left_wing_j', {0,0,0}, {-30,0,30})
right_wing = gr.mesh('sphere', 'right_wing')
right_wing_j = gr.joint('right_wing_j', {0,0,0}, {-30,0,30})
left_wing_outer = gr.mesh('sphere', 'left_wing_outer')
left_wing_outer_j = gr.joint('left_wing_outer_j', {0,0,0}, {-30,0,30})
right_wing_outer = gr.mesh('sphere', 'right_wing_outer')
right_wing_outer_j = gr.joint('right_wing_outer_j', {0,0,0}, {-30,0,30})

abdomen = gr.mesh('sphere', 'abdomen')
abdomen_j = gr.joint('abdomen_j', {-30,0,30}, {0,0,0})
tail = gr.mesh('cube', 'tail')
tail_j = gr.joint('tail_j', {-26,-10,20}, {0,0,0})
tail_left = gr.mesh('cube', 'tail_left')
tail_left_j = gr.joint('tail_left_j', {-32,0,15}, {0,0,0})
tail_right = gr.mesh('cube', 'tail_right')
tail_right_j = gr.joint('tail_right_j', {-32,0,15}, {0,0,0})
tail_mid = gr.mesh('cube', 'tail_mid')
tail_mid_j = gr.joint('tail_mid_j', {-40,0,40}, {0,0,0})

root_transforms:add_child(torso)

torso:add_child(neck_j)
torso:add_child(shoulder_j)
torso:add_child(abdomen_j)

neck:add_child(head_j)
head:add_child(beak_j)
head:add_child(beak_fixed)

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
beak_j:add_child(beak)
tail_j:add_child(tail)
tail_left_j:add_child(tail_left)
tail_right_j:add_child(tail_right)
tail_mid_j:add_child(tail_mid)

-- start modelling
torso:set_material(black)
torso:scale(0.25,0.4,0.18)

-- shoulder
shoulder:set_material(white)
shoulder:scale(0.27,0.10,0.10)
shoulder:translate(0.0,0.15,0.0)
shoulder_j:scale(1/0.25,1/0.4,1/0.18)

left_wing:set_material(black)
left_wing:scale(0.421,0.07,0.065)
left_wing:translate(-0.38,0.0,0.0)
left_wing:rotate('z', -15)
left_wing_j:translate(-0.25,0,0.0)
left_wing_j:scale(1/0.27,1/0.14,1/0.13)

left_wing_outer:set_material(black)
left_wing_outer:scale(0.421, 0.06, 0.055)
left_wing_outer:translate(-0.38,0.0,0.0)
left_wing_outer:rotate('z', 15)
left_wing_outer_j:translate(-0.40,0,0.0)
left_wing_outer_j:scale(1/0.421,1/0.07,1/0.065)

right_wing:set_material(black)
right_wing:scale(0.421,0.07,0.065)
right_wing:translate(0.38,0.0,0.0)
right_wing:rotate('z', 15)
right_wing_j:translate(0.25,0,0.0)
right_wing_j:scale(1/0.27,1/0.14,1/0.13)

right_wing_outer:set_material(black)
right_wing_outer:scale(0.421, 0.06, 0.055)
right_wing_outer:translate(0.38,0.0,0.0)
right_wing_outer:rotate('z', -15)
right_wing_outer_j:translate(0.40,0,0.0)
right_wing_outer_j:scale(1/0.421,1/0.07,1/0.065)


-- abdomen
abdomen:set_material(black)
abdomen:scale(0.213,0.4,0.18)
abdomen:translate(0.0,-0.19,0.0)
abdomen_j:translate(0.0,-0.13,0.02)
abdomen_j:scale(1/0.25,1/0.4,1/0.18)

tail:set_material(yellow)
tail:scale(0.285,0.27,0.063)
tail:translate(0.0,-0.24,0.0)
tail_j:translate(0.0,-0.1,0.0)
tail_j:scale(1/0.213,1/0.4,1/0.18)

tail_mid:set_material(orange)
tail_mid:scale(0.114,0.211,0.0378)
tail_mid:translate(0.0,-0.15,0.0)
tail_mid_j:translate(0.0,-0.08,0.0)
tail_mid_j:scale(1/0.285,1/0.27,1/0.063)

tail_left:set_material(orange)
tail_left:scale(0.114,0.311,0.0378)
tail_left:translate(0,-0.234,0.0)
tail_left:rotate('y', -10)
tail_left:rotate('z', -18)
tail_left_j:translate(-0.142,-0.08,0.0)
tail_left_j:scale(1/0.285,1/0.27,1/0.063)

tail_right:set_material(orange)
tail_right:scale(0.114,0.311,0.0378)
tail_right:translate(0,-0.234,0.0)
tail_right:rotate('y', 10)
tail_right:rotate('z', 18)
tail_right_j:translate(0.142,-0.08,0.0)
tail_right_j:scale(1/0.285,1/0.27,1/0.063)

-- neck
neck:set_material(white)
neck:scale(0.123,0.155,0.09)
neck_j:translate(0.0,0.3,0.0)
neck_j:scale(1/0.25,1/0.4,1/0.18)

head:set_material(white)
head:scale(0.145,0.238,0.145)
head:translate(0.0,0.153,0.0)
head_j:translate(0.0,0.12,0.0)
head_j:scale(1/0.123,1/0.155,1/0.09)

beak:set_material(yellow)
beak:rotate('z',45)
beak:scale(0.06,0.18,0.015)
beak_j:translate(0,0.20,-0.06)
beak_j:scale(1/0.145,1/0.238,1/0.145)

beak_fixed:set_material(yellow)
beak_fixed:rotate('z',45)
beak_fixed:scale(0.07,0.2,0.015)
beak_fixed:translate(0,0.22,-0.045)
beak_fixed:scale(1/0.145,1/0.238,1/0.145)

-- legs
left_thigh:set_material(black)
left_thigh:scale(0.064,0.09,0.216)
left_thigh:translate(0.0682,0.025,-0.108)
left_thigh_j:translate(0.0,0.0,-0.08)
left_thigh_j:scale(1/0.213,1/0.4,1/0.18)

right_thigh:set_material(black)
right_thigh:scale(0.064,0.09,0.216)
right_thigh:translate(-0.0682,0.025,-0.108)
right_thigh_j:translate(0.0,0.0,-0.08)
right_thigh_j:scale(1/0.213,1/0.4,1/0.18)

left_leg:set_material(white)
left_leg:scale(0.0416,0.0585,0.216)
left_leg:translate(0,0,-0.138)
left_leg_j:translate(0.0,0.0,-0.08)
left_leg_j:scale(1/0.064,1/0.09,1/0.216)

right_leg:set_material(white)
right_leg:scale(0.0416,0.0585,0.216)
right_leg:translate(0,0,-0.138)
right_leg_j:translate(0.0,0.0,-0.08)
right_leg_j:scale(1/0.064,1/0.09,1/0.216)

left_talons:set_material(yellow)
left_talons:scale(0.0116,0.135,0.0324)
left_talons:translate(0,0.06,0)
left_talons_j:translate(0,0.02,-0.19)
left_talons_j:scale(1/0.0416,1/0.0585,1/0.216)

right_talons:set_material(yellow)
right_talons:scale(0.0116,0.135,0.0324)
right_talons:translate(0,0.06,0)
right_talons_j:translate(0,0.02,-0.19)
right_talons_j:scale(1/0.0416,1/0.0585,1/0.216)





--decorations - wing feathers
--right inner
r_feather_sec1_row1_0 = gr.mesh('cube', 'r_feather_sec1_row1_0')
r_feather_sec1_row1_1 = gr.mesh('cube', 'r_feather_sec1_row1_1')
r_feather_sec1_row1_2 = gr.mesh('cube', 'r_feather_sec1_row1_2')
r_feather_sec1_row1_3 = gr.mesh('cube', 'r_feather_sec1_row1_3')
r_feather_sec1_row1_4 = gr.mesh('cube', 'r_feather_sec1_row1_4')
r_feather_sec1_row1_5 = gr.mesh('cube', 'r_feather_sec1_row1_5')
r_feather_sec2_row1_1 = gr.mesh('cube', 'r_feather_sec2_row1_1')
r_feather_sec2_row1_2 = gr.mesh('cube', 'r_feather_sec2_row1_2')
r_feather_sec2_row1_3 = gr.mesh('cube', 'r_feather_sec2_row1_3')
r_feather_sec2_row1_4 = gr.mesh('cube', 'r_feather_sec2_row1_4')
r_feather_sec2_row1_5 = gr.mesh('cube', 'r_feather_sec2_row1_5')

right_wing:add_child(r_feather_sec1_row1_0)
right_wing:add_child(r_feather_sec1_row1_1)
right_wing:add_child(r_feather_sec1_row1_2)
right_wing:add_child(r_feather_sec1_row1_3)
right_wing:add_child(r_feather_sec1_row1_4)
right_wing:add_child(r_feather_sec1_row1_5)
right_wing_outer:add_child(r_feather_sec2_row1_1)
right_wing_outer:add_child(r_feather_sec2_row1_2)
right_wing_outer:add_child(r_feather_sec2_row1_3)
right_wing_outer:add_child(r_feather_sec2_row1_4)
right_wing_outer:add_child(r_feather_sec2_row1_5)

r_feather_sec1_row1_0:set_material(white)
r_feather_sec1_row1_0:scale(0.25,0.25,0.25)
r_feather_sec1_row1_0:scale(0.6,1.9,0.16) --e
r_feather_sec1_row1_0:rotate('y', 2)
r_feather_sec1_row1_0:translate(-0.48,-0.19,0.001) --e
r_feather_sec1_row1_0:scale(1/0.421, 1/0.06, 1/0.055)

r_feather_sec1_row1_1:set_material(white)
r_feather_sec1_row1_1:scale(0.25,0.25,0.25)
r_feather_sec1_row1_1:scale(0.6,1.7,0.16) --e
r_feather_sec1_row1_1:rotate('y', -5)
r_feather_sec1_row1_1:translate(-0.32,-0.18,-0.012) --e
r_feather_sec1_row1_1:scale(1/0.421, 1/0.06, 1/0.055)

r_feather_sec1_row1_2:set_material(white)
r_feather_sec1_row1_2:scale(0.25,0.25,0.25)
r_feather_sec1_row1_2:scale(0.6,1.6,0.16) --e
r_feather_sec1_row1_2:rotate('y', 1)
r_feather_sec1_row1_2:translate(-0.16,-0.165,-0.005) --e
r_feather_sec1_row1_2:scale(1/0.421, 1/0.06, 1/0.055)

r_feather_sec1_row1_3:set_material(white)
r_feather_sec1_row1_3:scale(0.25,0.25,0.25)
r_feather_sec1_row1_3:scale(0.6,1.5,0.16) --e
r_feather_sec1_row1_3:rotate('y', -8)
r_feather_sec1_row1_3:translate(0,-0.16,0.003) --e
r_feather_sec1_row1_3:scale(1/0.421, 1/0.06, 1/0.055)

r_feather_sec1_row1_4:set_material(white)
r_feather_sec1_row1_4:scale(0.25,0.25,0.25)
r_feather_sec1_row1_4:scale(0.6,1.4,0.16) --e
r_feather_sec1_row1_4:rotate('y', 4)
r_feather_sec1_row1_4:translate(0.16,-0.16,-0.02) --e
r_feather_sec1_row1_4:scale(1/0.421, 1/0.06, 1/0.055)

r_feather_sec1_row1_5:set_material(white)
r_feather_sec1_row1_5:scale(0.25,0.25,0.25)
r_feather_sec1_row1_5:scale(0.6,1.3,0.16) --e
r_feather_sec1_row1_5:rotate('y', -9)
r_feather_sec1_row1_5:translate(0.32,-0.16,0.01) --e
r_feather_sec1_row1_5:scale(1/0.421, 1/0.06, 1/0.055)


r_feather_sec2_row1_1:set_material(white)
r_feather_sec2_row1_1:scale(0.25,0.25,0.25)
r_feather_sec2_row1_1:scale(0.6,1.5,0.16) --e
r_feather_sec2_row1_1:rotate('y', 0)
r_feather_sec2_row1_1:translate(-0.32,-0.16,-0.012) --e
r_feather_sec2_row1_1:scale(1/0.421, 1/0.06, 1/0.055)

r_feather_sec2_row1_2:set_material(white)
r_feather_sec2_row1_2:scale(0.25,0.25,0.25)
r_feather_sec2_row1_2:scale(0.6,1.5,0.16) --e
r_feather_sec2_row1_2:rotate('y', -1)
r_feather_sec2_row1_2:translate(-0.16,-0.16,-0.018) --e
r_feather_sec2_row1_2:scale(1/0.421, 1/0.06, 1/0.055)

r_feather_sec2_row1_3:set_material(white)
r_feather_sec2_row1_3:scale(0.25,0.25,0.25)
r_feather_sec2_row1_3:scale(0.6,1.3,0.16) --e
r_feather_sec2_row1_3:rotate('y', 2)
r_feather_sec2_row1_3:translate(0,-0.13,0.01) --e
r_feather_sec2_row1_3:scale(1/0.421, 1/0.06, 1/0.055)

r_feather_sec2_row1_4:set_material(white)
r_feather_sec2_row1_4:scale(0.25,0.25,0.25)
r_feather_sec2_row1_4:scale(0.6,1.1,0.16) --e
r_feather_sec2_row1_4:rotate('y', -7)
r_feather_sec2_row1_4:translate(0.16,-0.11,-0.003) --e
r_feather_sec2_row1_4:scale(1/0.421, 1/0.06, 1/0.055)

r_feather_sec2_row1_5:set_material(white)
r_feather_sec2_row1_5:scale(0.25,0.25,0.25)
r_feather_sec2_row1_5:scale(0.6,0.9,0.16) --e
r_feather_sec2_row1_5:rotate('y', -4)
r_feather_sec2_row1_5:translate(0.32,-0.09,0.012) --e
r_feather_sec2_row1_5:scale(1/0.421, 1/0.06, 1/0.055)


--left inner
l_feathel_sec1_row1_0 = gr.mesh('cube', 'l_feathel_sec1_row1_0')
l_feathel_sec1_row1_1 = gr.mesh('cube', 'l_feathel_sec1_row1_1')
l_feathel_sec1_row1_2 = gr.mesh('cube', 'l_feathel_sec1_row1_2')
l_feathel_sec1_row1_3 = gr.mesh('cube', 'l_feathel_sec1_row1_3')
l_feathel_sec1_row1_4 = gr.mesh('cube', 'l_feathel_sec1_row1_4')
l_feathel_sec1_row1_5 = gr.mesh('cube', 'l_feathel_sec1_row1_5')
l_feathel_sec2_row1_1 = gr.mesh('cube', 'l_feathel_sec2_row1_1')
l_feathel_sec2_row1_2 = gr.mesh('cube', 'l_feathel_sec2_row1_2')
l_feathel_sec2_row1_3 = gr.mesh('cube', 'l_feathel_sec2_row1_3')
l_feathel_sec2_row1_4 = gr.mesh('cube', 'l_feathel_sec2_row1_4')
l_feathel_sec2_row1_5 = gr.mesh('cube', 'l_feathel_sec2_row1_5')

left_wing:add_child(l_feathel_sec1_row1_0)
left_wing:add_child(l_feathel_sec1_row1_1)
left_wing:add_child(l_feathel_sec1_row1_2)
left_wing:add_child(l_feathel_sec1_row1_3)
left_wing:add_child(l_feathel_sec1_row1_4)
left_wing:add_child(l_feathel_sec1_row1_5)
left_wing_outer:add_child(l_feathel_sec2_row1_1)
left_wing_outer:add_child(l_feathel_sec2_row1_2)
left_wing_outer:add_child(l_feathel_sec2_row1_3)
left_wing_outer:add_child(l_feathel_sec2_row1_4)
left_wing_outer:add_child(l_feathel_sec2_row1_5)

l_feathel_sec1_row1_0:set_material(white)
l_feathel_sec1_row1_0:scale(0.25,0.25,0.25)
l_feathel_sec1_row1_0:scale(0.6,1.9,0.16) --e
l_feathel_sec1_row1_0:rotate('y', -6)
l_feathel_sec1_row1_0:translate(0.48,-0.194,-0.001) --e
l_feathel_sec1_row1_0:scale(1/0.421, 1/0.06, 1/0.055)

l_feathel_sec1_row1_1:set_material(white)
l_feathel_sec1_row1_1:scale(0.25,0.25,0.25)
l_feathel_sec1_row1_1:scale(0.6,1.7,0.16) --e
l_feathel_sec1_row1_1:rotate('y', 6)
l_feathel_sec1_row1_1:translate(0.32,-0.184,0.003) --e
l_feathel_sec1_row1_1:scale(1/0.421, 1/0.06, 1/0.055)

l_feathel_sec1_row1_2:set_material(white)
l_feathel_sec1_row1_2:scale(0.25,0.25,0.25)
l_feathel_sec1_row1_2:scale(0.6,1.6,0.16) --e
l_feathel_sec1_row1_2:rotate('y', 7)
l_feathel_sec1_row1_2:translate(0.16,-0.165,-0.012) --e
l_feathel_sec1_row1_2:scale(1/0.421, 1/0.06, 1/0.055)

l_feathel_sec1_row1_3:set_material(white)
l_feathel_sec1_row1_3:scale(0.25,0.25,0.25)
l_feathel_sec1_row1_3:scale(0.6,1.5,0.16) --e
l_feathel_sec1_row1_3:rotate('y', 8)
l_feathel_sec1_row1_3:translate(0,-0.16,-0.001) --e
l_feathel_sec1_row1_3:scale(1/0.421, 1/0.06, 1/0.055)

l_feathel_sec1_row1_4:set_material(white)
l_feathel_sec1_row1_4:scale(0.25,0.25,0.25)
l_feathel_sec1_row1_4:scale(0.6,1.4,0.16) --e
l_feathel_sec1_row1_4:rotate('y', -8)
l_feathel_sec1_row1_4:translate(-0.16,-0.16,-0.012) --e
l_feathel_sec1_row1_4:scale(1/0.421, 1/0.06, 1/0.055)

l_feathel_sec1_row1_5:set_material(white)
l_feathel_sec1_row1_5:scale(0.25,0.25,0.25)
l_feathel_sec1_row1_5:scale(0.6,1.3,0.16) --e
l_feathel_sec1_row1_5:rotate('y', -5)
l_feathel_sec1_row1_5:translate(-0.32,-0.16,-0.009) --e
l_feathel_sec1_row1_5:scale(1/0.421, 1/0.06, 1/0.055)


l_feathel_sec2_row1_1:set_material(white)
l_feathel_sec2_row1_1:scale(0.25,0.25,0.25)
l_feathel_sec2_row1_1:scale(0.6,1.5,0.16) --e
l_feathel_sec2_row1_1:rotate('y', 2)
l_feathel_sec2_row1_1:translate(0.32,-0.16,-0.004) --e
l_feathel_sec2_row1_1:scale(1/0.421, 1/0.06, 1/0.055)

l_feathel_sec2_row1_2:set_material(white)
l_feathel_sec2_row1_2:scale(0.25,0.25,0.25)
l_feathel_sec2_row1_2:scale(0.6,1.5,0.16) --e
l_feathel_sec2_row1_2:rotate('y', -3)
l_feathel_sec2_row1_2:translate(0.16,-0.16,-0.001) --e
l_feathel_sec2_row1_2:scale(1/0.421, 1/0.06, 1/0.055)

l_feathel_sec2_row1_3:set_material(white)
l_feathel_sec2_row1_3:scale(0.25,0.25,0.25)
l_feathel_sec2_row1_3:scale(0.6,1.3,0.16) --e
l_feathel_sec2_row1_3:rotate('y', -5)
l_feathel_sec2_row1_3:translate(0,-0.13,-0.005) --e
l_feathel_sec2_row1_3:scale(1/0.421, 1/0.06, 1/0.055)

l_feathel_sec2_row1_4:set_material(white)
l_feathel_sec2_row1_4:scale(0.25,0.25,0.25)
l_feathel_sec2_row1_4:scale(0.6,1.1,0.16) --e
l_feathel_sec2_row1_4:rotate('y', 2)
l_feathel_sec2_row1_4:translate(-0.16,-0.11,-0.013) --e
l_feathel_sec2_row1_4:scale(1/0.421, 1/0.06, 1/0.055)

l_feathel_sec2_row1_5:set_material(white)
l_feathel_sec2_row1_5:scale(0.25,0.25,0.25)
l_feathel_sec2_row1_5:scale(0.6,0.9,0.16) --e
l_feathel_sec2_row1_5:rotate('y', 3)
l_feathel_sec2_row1_5:translate(-0.32,-0.09,0.002) --e
l_feathel_sec2_row1_5:scale(1/0.421, 1/0.06, 1/0.055)



--right feather tips
r_feather_sec1_row2_0 = gr.mesh('cube', 'r_feather_sec1_row2_0')
r_feather_sec1_row2_1 = gr.mesh('cube', 'r_feather_sec1_row2_1')
r_feather_sec1_row2_2 = gr.mesh('cube', 'r_feather_sec1_row2_2')
r_feather_sec1_row2_3 = gr.mesh('cube', 'r_feather_sec1_row2_3')
r_feather_sec1_row2_4 = gr.mesh('cube', 'r_feather_sec1_row2_4')
r_feather_sec1_row2_5 = gr.mesh('cube', 'r_feather_sec1_row2_5')
r_feather_sec2_row2_0 = gr.mesh('cube', 'r_feather_sec2_row2_0')
r_feather_sec2_row2_1 = gr.mesh('cube', 'r_feather_sec2_row2_1')
r_feather_sec2_row2_2 = gr.mesh('cube', 'r_feather_sec2_row2_2')
r_feather_sec2_row2_3 = gr.mesh('cube', 'r_feather_sec2_row2_3')
r_feather_sec2_row2_4 = gr.mesh('cube', 'r_feather_sec2_row2_4')
r_feather_sec2_row2_5 = gr.mesh('cube', 'r_feather_sec2_row2_5')
r_feather_sec2_row2_6 = gr.mesh('cube', 'r_feather_sec2_row2_6')

r_feather_sec1_row1_0:add_child(r_feather_sec1_row2_0)
r_feather_sec1_row1_1:add_child(r_feather_sec1_row2_1)
r_feather_sec1_row1_2:add_child(r_feather_sec1_row2_2)
r_feather_sec1_row1_3:add_child(r_feather_sec1_row2_3)
r_feather_sec1_row1_4:add_child(r_feather_sec1_row2_4)
r_feather_sec1_row1_5:add_child(r_feather_sec1_row2_5)
r_feather_sec2_row1_1:add_child(r_feather_sec2_row2_0)
r_feather_sec2_row1_1:add_child(r_feather_sec2_row2_1)
r_feather_sec2_row1_2:add_child(r_feather_sec2_row2_2)
r_feather_sec2_row1_3:add_child(r_feather_sec2_row2_3)
r_feather_sec2_row1_4:add_child(r_feather_sec2_row2_4)
r_feather_sec2_row1_5:add_child(r_feather_sec2_row2_5)
r_feather_sec2_row1_5:add_child(r_feather_sec2_row2_6)

r_feather_sec1_row2_0:set_material(yellow)
r_feather_sec1_row2_0:rotate('z',36)
r_feather_sec1_row2_0:scale(0.55,1.7,0.10) --e
r_feather_sec1_row2_0:rotate('y', 2)
r_feather_sec1_row2_0:translate(-0.48,-1.19,0) --e
r_feather_sec1_row2_0:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec1_row2_1:set_material(yellow)
r_feather_sec1_row2_1:rotate('z',36)
r_feather_sec1_row2_1:scale(0.55,1.7,0.10) --e
r_feather_sec1_row2_1:rotate('y', -3)
r_feather_sec1_row2_1:translate(-0.48,-1.19,0.005) --e
r_feather_sec1_row2_1:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec1_row2_2:set_material(yellow)
r_feather_sec1_row2_2:rotate('z',36)
r_feather_sec1_row2_2:scale(0.55,1.7,0.10) --e
r_feather_sec1_row2_2:rotate('y', 8)
r_feather_sec1_row2_2:translate(-0.48,-1.19,-0.009) --e
r_feather_sec1_row2_2:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec1_row2_3:set_material(yellow)
r_feather_sec1_row2_3:rotate('z',36)
r_feather_sec1_row2_3:scale(0.55,1.7,0.10) --e
r_feather_sec1_row2_3:rotate('y', 1)
r_feather_sec1_row2_3:translate(-0.48,-1.19,-0.007) --e
r_feather_sec1_row2_3:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec1_row2_4:set_material(yellow)
r_feather_sec1_row2_4:rotate('z',36)
r_feather_sec1_row2_4:scale(0.55,1.7,0.10) --e
r_feather_sec1_row2_4:rotate('y', -10)
r_feather_sec1_row2_4:translate(-0.48,-1.19,-0.004) --e
r_feather_sec1_row2_4:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec1_row2_5:set_material(yellow)
r_feather_sec1_row2_5:rotate('z',36)
r_feather_sec1_row2_5:scale(0.55,1.7,0.10) --e
r_feather_sec1_row2_5:rotate('y', -8)
r_feather_sec1_row2_5:translate(-0.48,-1.19,-0.014) --e
r_feather_sec1_row2_5:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec2_row2_0:set_material(yellow)
r_feather_sec2_row2_0:rotate('z',36)
r_feather_sec2_row2_0:scale(0.52,1.7,0.10) --e
r_feather_sec2_row2_0:rotate('y', 10)
r_feather_sec2_row2_0:translate(0,-1.29,-0.002) --e
r_feather_sec2_row2_0:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec2_row2_1:set_material(yellow)
r_feather_sec2_row2_1:rotate('z',36)
r_feather_sec2_row2_1:scale(0.52,1.7,0.10) --e
r_feather_sec2_row2_1:rotate('y', -5)
r_feather_sec2_row2_1:translate(0.48,-1.19,0.002) --e
r_feather_sec2_row2_1:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec2_row2_2:set_material(yellow)
r_feather_sec2_row2_2:rotate('z',36)
r_feather_sec2_row2_2:scale(0.52,1.7,0.10) --e
r_feather_sec2_row2_2:rotate('y', 2)
r_feather_sec2_row2_2:translate(0.48,-1.19,0.004) --e
r_feather_sec2_row2_2:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec2_row2_3:set_material(yellow)
r_feather_sec2_row2_3:rotate('z',36)
r_feather_sec2_row2_3:scale(0.52,1.7,0.10) --e
r_feather_sec2_row2_3:rotate('y', -3)
r_feather_sec2_row2_3:translate(0.46,-1.19,0) --e
r_feather_sec2_row2_3:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec2_row2_4:set_material(yellow)
r_feather_sec2_row2_4:rotate('z',36)
r_feather_sec2_row2_4:scale(0.52,1.7,0.10) --e
r_feather_sec2_row2_4:rotate('y', -5)
r_feather_sec2_row2_4:translate(0.42,-1.19,-0.005) --e
r_feather_sec2_row2_4:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec2_row2_5:set_material(yellow)
r_feather_sec2_row2_5:rotate('z',36)
r_feather_sec2_row2_5:scale(0.52,1.7,0.10) --e
r_feather_sec2_row2_5:rotate('y', 2)
r_feather_sec2_row2_5:translate(0.34,-1.19,-0.012) --e
r_feather_sec2_row2_5:scale(1/0.6, 1/1.9, 1/0.16)

r_feather_sec2_row2_6:set_material(yellow)
r_feather_sec2_row2_6:rotate('z',50)
r_feather_sec2_row2_6:scale(1.06,1.0,0.10) --e
r_feather_sec2_row2_6:rotate('y', -5)
r_feather_sec2_row2_6:translate(0.34,-0.10,-0.008) --e
r_feather_sec2_row2_6:scale(1/0.6, 1/1.9, 1/0.16)

--left feather tips
l_feathel_sec1_row2_0 = gr.mesh('cube', 'l_feathel_sec1_row2_0')
l_feathel_sec1_row2_1 = gr.mesh('cube', 'l_feathel_sec1_row2_1')
l_feathel_sec1_row2_2 = gr.mesh('cube', 'l_feathel_sec1_row2_2')
l_feathel_sec1_row2_3 = gr.mesh('cube', 'l_feathel_sec1_row2_3')
l_feathel_sec1_row2_4 = gr.mesh('cube', 'l_feathel_sec1_row2_4')
l_feathel_sec1_row2_5 = gr.mesh('cube', 'l_feathel_sec1_row2_5')
l_feathel_sec2_row2_0 = gr.mesh('cube', 'l_feathel_sec2_row2_0')
l_feathel_sec2_row2_1 = gr.mesh('cube', 'l_feathel_sec2_row2_1')
l_feathel_sec2_row2_2 = gr.mesh('cube', 'l_feathel_sec2_row2_2')
l_feathel_sec2_row2_3 = gr.mesh('cube', 'l_feathel_sec2_row2_3')
l_feathel_sec2_row2_4 = gr.mesh('cube', 'l_feathel_sec2_row2_4')
l_feathel_sec2_row2_5 = gr.mesh('cube', 'l_feathel_sec2_row2_5')
l_feathel_sec2_row2_6 = gr.mesh('cube', 'l_feathel_sec2_row2_6')

l_feathel_sec1_row1_0:add_child(l_feathel_sec1_row2_0)
l_feathel_sec1_row1_1:add_child(l_feathel_sec1_row2_1)
l_feathel_sec1_row1_2:add_child(l_feathel_sec1_row2_2)
l_feathel_sec1_row1_3:add_child(l_feathel_sec1_row2_3)
l_feathel_sec1_row1_4:add_child(l_feathel_sec1_row2_4)
l_feathel_sec1_row1_5:add_child(l_feathel_sec1_row2_5)
l_feathel_sec2_row1_1:add_child(l_feathel_sec2_row2_0)
l_feathel_sec2_row1_1:add_child(l_feathel_sec2_row2_1)
l_feathel_sec2_row1_2:add_child(l_feathel_sec2_row2_2)
l_feathel_sec2_row1_3:add_child(l_feathel_sec2_row2_3)
l_feathel_sec2_row1_4:add_child(l_feathel_sec2_row2_4)
l_feathel_sec2_row1_5:add_child(l_feathel_sec2_row2_5)
l_feathel_sec2_row1_5:add_child(l_feathel_sec2_row2_6)

l_feathel_sec1_row2_0:set_material(yellow)
l_feathel_sec1_row2_0:rotate('z',36)
l_feathel_sec1_row2_0:scale(0.55,1.7,0.10) --e
l_feathel_sec1_row2_0:rotate('y', -3)
l_feathel_sec1_row2_0:translate(0.48,-1.19,-0.008) --e
l_feathel_sec1_row2_0:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec1_row2_1:set_material(yellow)
l_feathel_sec1_row2_1:rotate('z',36)
l_feathel_sec1_row2_1:scale(0.55,1.7,0.10) --e
l_feathel_sec1_row2_1:rotate('y', 10)
l_feathel_sec1_row2_1:translate(0.48,-1.19,-0.012) --e
l_feathel_sec1_row2_1:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec1_row2_2:set_material(yellow)
l_feathel_sec1_row2_2:rotate('z',36)
l_feathel_sec1_row2_2:scale(0.55,1.7,0.10) --e
l_feathel_sec1_row2_2:rotate('y', -4)
l_feathel_sec1_row2_2:translate(0.48,-1.19,-0.002) --e
l_feathel_sec1_row2_2:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec1_row2_3:set_material(yellow)
l_feathel_sec1_row2_3:rotate('z',36)
l_feathel_sec1_row2_3:scale(0.55,1.7,0.10) --e
l_feathel_sec1_row2_3:rotate('y', -6)
l_feathel_sec1_row2_3:translate(0.48,-1.19,-0.015) --e
l_feathel_sec1_row2_3:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec1_row2_4:set_material(yellow)
l_feathel_sec1_row2_4:rotate('z',36)
l_feathel_sec1_row2_4:scale(0.55,1.7,0.10) --e
l_feathel_sec1_row2_4:rotate('y', 7)
l_feathel_sec1_row2_4:translate(0.48,-1.19,-0.004) --e
l_feathel_sec1_row2_4:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec1_row2_5:set_material(yellow)
l_feathel_sec1_row2_5:rotate('z',36)
l_feathel_sec1_row2_5:scale(0.55,1.7,0.10) --e
l_feathel_sec1_row2_5:rotate('y', 3)
l_feathel_sec1_row2_5:translate(0.48,-1.19,-0.017) --e
l_feathel_sec1_row2_5:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec2_row2_0:set_material(yellow)
l_feathel_sec2_row2_0:rotate('z',36)
l_feathel_sec2_row2_0:scale(0.52,1.7,0.10) --e
l_feathel_sec2_row2_0:rotate('y', 8)
l_feathel_sec2_row2_0:translate(0,-1.29,0.002) --e
l_feathel_sec2_row2_0:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec2_row2_1:set_material(yellow)
l_feathel_sec2_row2_1:rotate('z',36)
l_feathel_sec2_row2_1:scale(0.52,1.7,0.10) --e
l_feathel_sec2_row2_1:rotate('y', 0)
l_feathel_sec2_row2_1:translate(-0.48,-1.19,-0.012) --e
l_feathel_sec2_row2_1:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec2_row2_2:set_material(yellow)
l_feathel_sec2_row2_2:rotate('z',36)
l_feathel_sec2_row2_2:scale(0.52,1.7,0.10) --e
l_feathel_sec2_row2_2:rotate('y', -3)
l_feathel_sec2_row2_2:translate(-0.48,-1.19,-0.010) --e
l_feathel_sec2_row2_2:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec2_row2_3:set_material(yellow)
l_feathel_sec2_row2_3:rotate('z',36)
l_feathel_sec2_row2_3:scale(0.52,1.7,0.10) --e
l_feathel_sec2_row2_3:rotate('y', 5)
l_feathel_sec2_row2_3:translate(-0.46,-1.19,-0.008) --e
l_feathel_sec2_row2_3:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec2_row2_4:set_material(yellow)
l_feathel_sec2_row2_4:rotate('z',36)
l_feathel_sec2_row2_4:scale(0.52,1.7,0.10) --e
l_feathel_sec2_row2_4:rotate('y', 1)
l_feathel_sec2_row2_4:translate(-0.42,-1.19,0) --e
l_feathel_sec2_row2_4:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec2_row2_5:set_material(yellow)
l_feathel_sec2_row2_5:rotate('z',36)
l_feathel_sec2_row2_5:scale(0.52,1.7,0.10) --e
l_feathel_sec2_row2_5:rotate('y', -9)
l_feathel_sec2_row2_5:translate(-0.34,-1.19,-0.012) --e
l_feathel_sec2_row2_5:scale(1/0.6, 1/1.9, 1/0.16)

l_feathel_sec2_row2_6:set_material(yellow)
l_feathel_sec2_row2_6:rotate('z',50)
l_feathel_sec2_row2_6:scale(1.06,1.0,0.10) --e
l_feathel_sec2_row2_6:rotate('y', 6)
l_feathel_sec2_row2_6:translate(-0.34,-0.10,-0.005) --e
l_feathel_sec2_row2_6:scale(1/0.6, 1/1.9, 1/0.16)


-- tail extra feathers
tail_feather_middle = gr.mesh('cube', 'tail_feather_middle')
tail_feather_left = gr.mesh('cube', 'tail_feather_left')
tail_feather_right = gr.mesh('cube', 'tail_feather_right')

tail_mid:add_child(tail_feather_middle)
tail_right:add_child(tail_feather_left)
tail_left:add_child(tail_feather_right)

tail_feather_middle:set_material(red)
tail_feather_middle:rotate('z',45)
tail_feather_middle:scale(1.5,1.7,0.34)

tail_feather_left:set_material(red)
tail_feather_left:rotate('z',45)
tail_feather_left:scale(1.5,2.67,0.34)

tail_feather_right:set_material(red)
tail_feather_right:rotate('z',45)
tail_feather_right:scale(1.5,2.67,0.34)

-- talons
l_talon_l = gr.mesh('cube', 'l_talon_l')
l_talon_r = gr.mesh('cube', 'l_talon_r')
r_talon_l = gr.mesh('cube', 'r_talon_l')
r_talon_r = gr.mesh('cube', 'r_talon_r')

left_talons:add_child(l_talon_l)
left_talons:add_child(l_talon_r)
right_talons:add_child(r_talon_l)
right_talons:add_child(r_talon_r)

l_talon_l:set_material(yellow)
l_talon_l:scale(0.0116,0.135,0.0324)
l_talon_l:translate(0,0.06,0)
l_talon_l:rotate('z', 15)
l_talon_l:translate(0,-0.06,0)
l_talon_l:scale(1/0.0116,1/0.135,1/0.0324)

l_talon_r:set_material(yellow)
l_talon_r:scale(0.0116,0.135,0.0324)
l_talon_r:translate(0,0.06,0)
l_talon_r:rotate('z', -15)
l_talon_r:translate(0,-0.06,0)
l_talon_r:scale(1/0.0116,1/0.135,1/0.0324)


r_talon_l:set_material(yellow)
r_talon_l:scale(0.0116,0.135,0.0324)
r_talon_l:translate(0,0.06,0)
r_talon_l:rotate('z', 15)
r_talon_l:translate(0,-0.06,0)
r_talon_l:scale(1/0.0116,1/0.135,1/0.0324)

r_talon_r:set_material(yellow)
r_talon_r:scale(0.0116,0.135,0.0324)
r_talon_r:translate(0,0.06,0)
r_talon_r:rotate('z', -15)
r_talon_r:translate(0,-0.06,0)
r_talon_r:scale(1/0.0116,1/0.135,1/0.0324)

--head/neck features
left_eye = gr.mesh('sphere', 'left_eye')
right_eye = gr.mesh('sphere', 'right_eye')
neck_marker = gr.mesh('sphere', 'neck_marker')

head:add_child(left_eye)
head:add_child(right_eye)
neck:add_child(neck_marker)

left_eye:set_material(black)
left_eye:scale(0.02, 0.04, 0.02)
left_eye:translate(-0.1,0.09,0.077)
left_eye:scale(1/0.145,1/0.238,1/0.145)

right_eye:set_material(black)
right_eye:scale(0.02, 0.04, 0.02)
right_eye:translate(0.1,0.09,0.077)
right_eye:scale(1/0.145,1/0.238,1/0.145)

neck_marker:set_material(blue)
neck_marker:scale(0.01, 0.035, 0.01)
neck_marker:translate(0,0.05,0.09)
neck_marker:scale(1/0.123,1/0.155,1/0.09)

--belly
belly_abdomen = gr.mesh('sphere', 'belly_abdomen')
belly_torso = gr.mesh('sphere', 'belly_torso')

torso:add_child(belly_torso)
abdomen:add_child(belly_abdomen)

belly_torso:set_material(blue)
belly_torso:translate(0,0,-0.001)

belly_abdomen:set_material(white)
belly_abdomen:translate(0,0,-0.001)

return rootnode
