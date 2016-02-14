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
abdomen = gr.mesh('sphere', 'abdomen')
abdomen_j = gr.joint('abdomen_j', {-10,0,10}, {-24,0,24})

torso:add_child(abdomen_j)

abdomen:add_child(left_thigh_j)
abdomen:add_child(right_thigh_j)
left_thigh:add_child(left_leg_j)
right_thigh:add_child(right_leg_j)
left_leg:add_child(left_talons_j)
right_leg:add_child(right_talons_j)


torso:set_material(blue)
torso:scale(0.25,0.5,0.18)

abdomen:set_material(blue)
abdomen:scale(0.213,0.3,0.18)
abdomen:translate(0.0,-0.33,0.0)
abdomen_j:scale(1/0.25,1/0.5,1/0.18)


left_thigh:set_material(black)
left_thigh:scale(0.064,0.09,0.216)
left_thigh:translate(0.0682,0.045,-0.108)
left_thigh_j:scale(1/0.213,1/0.3,1/0.18)

right_thigh:set_material(black)
right_thigh:scale(0.064,0.09,0.216)
right_thigh:translate(-0.0682,0.045,-0.108)
right_thigh_j:scale(1/0.213,1/0.3,1/0.18)

left_leg:set_material(white)
left_leg:scale(0.0416,0.0585,0.216)
left_leg:translate(0,0,-0.108)
left_leg_j:scale(1/0.064,1/0.09,1/0.216)

right_leg:set_material(white)
right_leg:scale(0.0416,0.0585,0.216)
right_leg:translate(0,0,-0.108)
right_leg_j:scale(1/0.064,1/0.09,1/0.216)

left_talons:set_material(yellow)
left_talons:scale(0.0416,0.135,0.0324)
left_talons:translate(0,0.0292,-0.20)
left_talons_j:scale(1/0.0416,1/0.0585,1/0.216)

right_talons:set_material(yellow)
right_talons:scale(0.0416,0.135,0.0324)
right_talons:translate(0,0.0292,-0.20)
right_talons_j:scale(1/0.0416,1/0.0585,1/0.216)









return rootnode
