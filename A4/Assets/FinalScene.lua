hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
stone = gr.material({0.5, 0.40, 0.01}, {0.25, 0.15, 0.15}, 20)
carpet = gr.material({0.6, 0.02, 0.1}, {0,0,0}, 0)
mirror = gr.material({0.1, 0.16, 0.15}, {1.5,1.5,1.5}, 120)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)

perfectCanvas = gr.material({1.0, 1.0, 1.0}, {0, 0, 0}, 0)
reflectorCanvas = gr.material_extended({0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, 1000, 1, 1, 0, 0)
refractorCanvas = gr.material_extended({0.6, 0.9, 0.9}, {1.5, 0.4, 1.3}, 1000, 1, 0.4, 0, 0)
glossyReflectorCanvas = gr.material_extended({0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, 1000, 1, 1, 2.4, 0)
flame = gr.material_extended({0.8, 0.7, 0.2}, {0.0, 0.0, 0.0}, 0, 1, 1, 0, 1)

crystal = gr.material_extended({0.4, 0.4, 0.4}, {1.4, 1.4, 1.4}, 76.8, 1.5, 0, 0, 0)

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

t_sunset = gr.texture('Assets/sunset.png')
t_woodplanks = gr.texture('Assets/planks.png')
t_wall_inner = gr.texture('Assets/marble2.png')
t_castlewall = gr.texture('Assets/castlewall.png')
t_stainedglass = gr.texture('Assets/stainedglass.png')
t_marbleground = gr.texture('Assets/marble2.png')
t_pillarcenter = gr.texture('Assets/marble3.png')
t_pillarbase = gr.texture('Assets/marble1.png')
t_wood = gr.texture('Assets/wood1.png')
t_fleurdelis = gr.texture('Assets/fleur_de_lis.png')
t_rings = gr.texture('Assets/circles.png')

primary_light = gr.light({-8000, 8000, 0}, {1.0, 1.0, 1.0}, {1, 0, 0})
test_inner_light = gr.light({0,50,50}, {0.4, 0.4, 0.4}, {1,0.0001,0})

scene = gr.node('root')

walls = gr.node('walls')
pillars = gr.node('pillars')
altar = gr.node('altar')
statues = gr.node('statues')



------------------------------- WINDOW
i_window = gr.node('i_window')

window_flat = gr.cube('window_flat')
window_flat:translate(-0.5, -0.5, -0.5)
window_flat:set_material(perfectCanvas)
window_flat:set_texture(t_wall_inner)
window_flat:set_bumps(t_wall_inner, 'rgb')

window_round = gr.cylinder('window_round')
window_round:scale(0.5,1,0.5)
window_round:rotate('x', 90)
window_round:translate(0, 0.5, 0)
window_round:set_material(perfectCanvas)
window_round:set_texture(t_wall_inner)
window_round:set_bumps(t_wall_inner, 'rgb')

windowcsg = gr.csg('windowcsg')
windowcsg:set_csg_children(window_round, window_flat, 'UNION')
i_window:add_child(windowcsg)



i_window_pane = gr.cube('i_window_pane')
i_window_pane:translate(-0.5, -0.5, -0.5)
i_window_pane:scale(1, 2, 0.05)
i_window_pane:set_material(brass)

i_window_pane_large = gr.cube('i_window_pane_large')
i_window_pane_large:translate(-0.5, -0.5, -0.5)
i_window_pane_large:scale(1, 2, 0.05)
i_window_pane_large:set_material(perfectCanvas)
i_window_pane_large:set_texture(t_stainedglass)
i_window_pane_large:set_bumps(t_stainedglass, 'rgb')
-----------------------------------------

--walls and boundaries=============================================
ground = gr.cube('ground')
ground:translate(-0.5, -35, -0.6)
ground:scale(300,2,750)
ground:set_material(perfectCanvas)
ground:set_texture(t_marbleground)

roof = gr.cube('roof')
roof:translate(-0.5, 74, -0.6)
roof:scale(300,2,750)
roof:set_material(stone)
roof:set_texture(t_woodplanks)
roof:set_bumps(t_woodplanks, 'rgb')

--left
leftwall = gr.cube('leftwall')
leftwall:translate(-18, -0.5, -0.6)
leftwall:scale(8,300,750)
leftwall:set_material(perfectCanvas)
leftwall:set_texture(t_castlewall)
leftwall:set_bumps(t_castlewall, 'rgb')

leftwindows = gr.node('leftwindows')
leftwindow_1 = gr.node('leftwindow_1')
leftwindow_1:scale(50,80,40)
leftwindow_1:rotate('y', 90)
leftwindow_1:translate(-141,-10,-100)
leftwindow_1:add_child(i_window)
leftwindows:add_child(leftwindow_1)

leftwindow_2 = gr.node('leftwindow_2')
leftwindow_2:scale(50,80,40)
leftwindow_2:rotate('y', 90)
leftwindow_2:translate(-141,-10,-190)
leftwindow_2:add_child(i_window)
leftwindows:add_child(leftwindow_2)

leftwindow_3 = gr.node('leftwindow_3')
leftwindow_3:scale(50,80,40)
leftwindow_3:rotate('y', 90)
leftwindow_3:translate(-141,-10,-280)
leftwindow_3:add_child(i_window)
leftwindows:add_child(leftwindow_3)

leftwindow_4 = gr.node('leftwindow_4')
leftwindow_4:scale(50,80,40)
leftwindow_4:rotate('y', 90)
leftwindow_4:translate(-141,-10,-10)
leftwindow_4:add_child(i_window)
leftwindows:add_child(leftwindow_4)


leftwallcsg = gr.csg('leftwallcsg')
leftwallcsg:set_csg_children(leftwall, leftwindows, 'DIFFERENCE')

--right
rightwall = gr.cube('rightwall')
rightwall:translate(18, -0.5, -0.6)
rightwall:scale(8,300,750)
rightwall:set_material(perfectCanvas)
rightwall:set_texture(t_castlewall)
rightwall:set_bumps(t_castlewall, 'rgb')

rightwindows = gr.node('rightwindows')
rightwindow_1 = gr.node('rightwindow_1')
rightwindow_1:scale(50,80,40)
rightwindow_1:rotate('y', 90)
rightwindow_1:translate(137,-10,-100)
rightwindow_1:add_child(i_window)
rightwindows:add_child(rightwindow_1)

rightwindow_2 = gr.node('rightwindow_2')
rightwindow_2:scale(50,80,40)
rightwindow_2:rotate('y', 90)
rightwindow_2:translate(137,-10,-190)
rightwindow_2:add_child(i_window)
rightwindows:add_child(rightwindow_2)

rightwindow_3 = gr.node('rightwindow_3')
rightwindow_3:scale(50,80,40)
rightwindow_3:rotate('y', 90)
rightwindow_3:translate(137,-10,-280)
rightwindow_3:add_child(i_window)
rightwindows:add_child(rightwindow_3)

rightwindow_4 = gr.node('rightwindow_4')
rightwindow_4:scale(50,80,40)
rightwindow_4:rotate('y', 90)
rightwindow_4:translate(137,-10,-10)
rightwindow_4:add_child(i_window)
rightwindows:add_child(rightwindow_4)


rightwallcsg = gr.csg('rightwallcsg')
rightwallcsg:set_csg_children(rightwall, rightwindows, 'DIFFERENCE')


--back wall
backwall = gr.cube('backwall')
backwall:translate(-0.5, -0.5, -220)
backwall:scale(300,300,2)
backwall:set_material(perfectCanvas)
backwall:set_texture(t_castlewall)
backwall:set_bumps(t_castlewall, 'rgb')

backwindow = gr.node('backwindow')
backwindow:scale(160,120,20)
backwindow:translate(5,22,-440)
backwindow:add_child(i_window)

backwindowpane = gr.node('backwindowpane')
backwindowpane:scale(160,120,20)
backwindowpane:translate(5,22,-440)
backwindowpane:add_child(i_window_pane_large)

backwallcsg = gr.csg('backwallcsg')
backwallcsg:set_csg_children(backwall, backwindow, 'DIFFERENCE')
backwallcsg:add_child(backwindowpane)
--===========================================================================wall and boundaries

--==============pillar======================
i_pillar = gr.node('i_pillar')

pillar_bound = gr.cube('pillar_bound')
pillar_bound:translate(-0.5,-0.5,-0.5)
pillar_bound:scale(32,300,32)
pillar_bound:translate(0,50,0)
pillar_bound:set_material(perfectCanvas)
i_pillar:add_bound(pillar_bound)

pillar_core = gr.cylinder('pillar_center')
pillar_core:scale(6.6,100,6.6)
pillar_core:translate(0,20,0)
pillar_core:set_material(perfectCanvas)
pillar_core:set_texture(t_pillarcenter)
i_pillar:add_child(pillar_core)

pillar_base = gr.cube('pillar_base')
pillar_base:translate(-0.5, -0.5, -0.5)
pillar_base:scale(30,7.2,30)
pillar_base:translate(0,-80, 0)
pillar_base:set_material(polsilver)
pillar_base:set_texture(t_pillarbase)
i_pillar:add_child(pillar_base)

pillar_base_ring_1 = gr.torus('pillar_base_ring_1',0.2)
pillar_base_ring_1:rotate('x', 90)
pillar_base_ring_1:scale(10.6,10.6,10.6)
pillar_base_ring_1:translate(0,-75, 0)
pillar_base_ring_1:set_material(gold)
i_pillar:add_child(pillar_base_ring_1)

pillar_base_ring_2 = gr.torus('pillar_base_ring_2',0.18)
pillar_base_ring_2:rotate('x', 90)
pillar_base_ring_2:scale(9.3,9.3,9.3)
pillar_base_ring_2:translate(0,-70.67, 0)
pillar_base_ring_2:set_material(perfectCanvas)
pillar_base_ring_2:set_texture(t_wood)
pillar_base_ring_2:set_bumps(t_wood, 'rgb')
i_pillar:add_child(pillar_base_ring_2)

top_cone_decor = gr.cone('top_cone_decor')
top_cone_decor:rotate('x', 180)
top_cone_decor:scale(9.9,30,9.9)
top_cone_decor:translate(0,99,0)
top_cone_decor:set_material(gold)
top_cone_decor:set_texture(t_fleurdelis)
top_cone_decor:set_bumps(t_fleurdelis, 'rgb')
i_pillar:add_child(top_cone_decor)

pillar_top = gr.cube('pillar_top')
pillar_top:translate(-0.5, -0.5, -0.5)
pillar_top:scale(22,10,22)
pillar_top:translate(0,133,0)
pillar_top:set_material(gold)
pillar_top:set_texture(t_pillarbase)
i_pillar:add_child(pillar_top)

lightholder_cup = gr.cone('lightholder_cup')
lightholder_cup:rotate('x', 180)
lightholder_cup:scale(3.3,3.3,3.3)
lightholder_cup:set_material(polgold)

lightholder_indent = gr.sphere('lightholder_indent')
lightholder_indent:scale(1.93,1.93,1.93)
lightholder_indent:translate(0,5.6,0)
lightholder_indent:set_material(polsilver)

lightholder_left = gr.csg('lightholder_left')
lightholder_left:translate(-8.3,-48,0)
lightholder_left:set_csg_children(lightholder_cup, lightholder_indent, 'DIFFERENCE')
i_pillar:add_child(lightholder_left)

light_left = gr.sphere('light_left')
light_left:scale(2,2,2)
light_left:translate(0,6.3,0)
light_left:set_material(flame)
lightholder_left:add_child(light_left)
--=========================pillar========================

--===========================pillars
left_pillar_1 = gr.node('left_pillar_1')
left_pillar_1:rotate('y', 180)
left_pillar_1:translate(-66,15.5,-235)
left_pillar_1:add_child(i_pillar)

left_pillar_2 = gr.node('left_pillar_2')
left_pillar_2:rotate('y', 180)
left_pillar_2:translate(-66,15.5,-145)
left_pillar_2:add_child(i_pillar)

left_pillar_3 = gr.node('left_pillar_3')
left_pillar_3:rotate('y', 180)
left_pillar_3:translate(-66,15.5,-55)
left_pillar_3:add_child(i_pillar)

right_pillar_1 = gr.node('right_pillar_1')
right_pillar_1:translate(73,15.5,-235)
right_pillar_1:add_child(i_pillar)

right_pillar_2 = gr.node('right_pillar_2')
right_pillar_2:translate(73,15.5,-145)
right_pillar_2:add_child(i_pillar)

right_pillar_3 = gr.node('right_pillar_3')
right_pillar_3:translate(73,15.5,-55)
right_pillar_3:add_child(i_pillar)
--==================================================

--=====altar==================================
altar_bound = gr.cube('altar_bound')
altar_bound:translate(-0.5,-0.5,-0.5)
altar_bound:scale(210,180,130)
altar_bound:translate(5,-40,-330)
altar_bound:set_material(perfectCanvas)
altar:add_bound(altar_bound)

altar_base_1 = gr.cube('altar_base_1')
altar_base_1:translate(-0.5, -0.5, -0.5)
altar_base_1:scale(200,11,100)
altar_base_1:translate(5,-65,-320)
altar_base_1:set_material(perfectCanvas)
altar_base_1:set_texture(t_wood)
altar_base_1:set_bumps(t_wood, 'rgb')
altar:add_child(altar_base_1)

altar_base_2 = gr.cube('altar_base_2')
altar_base_2:translate(-0.5, -0.5, -0.5)
altar_base_2:scale(188,11,100)
altar_base_2:translate(5,-59,-326)
altar_base_2:set_material(perfectCanvas)
altar_base_2:set_texture(t_woodplanks)
altar_base_2:set_bumps(t_woodplanks, 'rgb')
altar:add_child(altar_base_2)

altar_base_3 = gr.cube('altar_base_3')
altar_base_3:translate(-0.5, -0.5, -0.5)
altar_base_3:scale(172,11,100)
altar_base_3:translate(5,-53,-334)
altar_base_3:set_material(perfectCanvas)
altar_base_3:set_texture(t_wood)
altar_base_3:set_bumps(t_wood, 'rgb')
altar:add_child(altar_base_3)

artifact = gr.node('artifact')
artifact:translate(5,-47,-350)
altar:add_child(artifact)

artifact_base = gr.cylinder('artifact_base')
artifact_base:scale(50,6,50)
artifact_base:set_material(chrome)
artifact:add_child(artifact_base)

artifact_block = gr.cube('artifact_block')
artifact_block:translate(-0.5,-0.5,-0.5)
artifact_block:scale(50,50,50)
artifact_block:translate(0,40,0)
artifact_block:set_material(pearl)

artifact_face = gr.cube('artifact_face')
artifact_face:translate(-0.5, -0.5, -0.5)
artifact_face:rotate('x', 45)
artifact_face:scale(50,70,50)
artifact_face:translate(0,10.35+40,25)
artifact_face:set_material(jade)

framecsg = gr.csg('framecsg')
framecsg:rotate('y', 90)
framecsg:set_csg_children(artifact_block, artifact_face, 'DIFFERENCE')
framecsg2 = gr.csg('framecsg2')
framecsg2:rotate('y', 90)
framecsg2:set_csg_children(framecsg, artifact_face, 'DIFFERENCE')
framecsg3 = gr.csg('framecsg3')
framecsg3:rotate('y', 90)
framecsg3:set_csg_children(framecsg2, artifact_face, 'DIFFERENCE')
pyramid = gr.csg('pyramid')
pyramid:rotate('y', 30)
pyramid:scale(2,2,2)
pyramid:set_csg_children(framecsg3, artifact_face, 'DIFFERENCE')

sphericalcarver = gr.sphere('sphericalcarver')
sphericalcarver:scale(28,28,28)
sphericalcarver:translate(0,30,0)
sphericalcarver:set_material(ruby)

carvedpyramid = gr.csg('carvedpyramid')
carvedpyramid:rotate('x', 90)
carvedpyramid:rotate('y', -90)
carvedpyramid:translate(-32,90,0)
carvedpyramid:scale(0.55,0.55,0.55)
carvedpyramid:set_csg_children(pyramid, sphericalcarver, 'DIFFERENCE')
artifact:add_child(carvedpyramid)

carvedpyramid_2 = gr.csg('carvedpyramid_2')
carvedpyramid_2:rotate('x', -90)
carvedpyramid_2:rotate('y', -90)
carvedpyramid_2:translate(32,90,0)
carvedpyramid_2:scale(0.55,0.55,0.55)
carvedpyramid_2:set_csg_children(pyramid, sphericalcarver, 'DIFFERENCE')
artifact:add_child(carvedpyramid_2)

artifact_sphere = gr.sphere('artifact_sphere')
artifact_sphere:scale(24,24,24)
artifact_sphere:translate(0,50,0)
artifact_sphere:set_material(crystal)
artifact:add_child(artifact_sphere)

artifact_cylinder = gr.cylinder('artifact_cylinder')
artifact_cylinder:scale(35,0.5,35)
artifact_cylinder:rotate('z', 61)
artifact_cylinder:rotate('y', 12)
artifact_cylinder:translate(0,50,0)
artifact_cylinder:set_material(perfectCanvas)
artifact_cylinder:set_texture(t_rings)
artifact_cylinder:set_bumps(t_rings, 'rgb')

carvedring = gr.csg('carvedring')
carvedring:scale(1.3,1.3,1.3)
carvedring:translate(0,-15,0)
carvedring:set_csg_children(artifact_cylinder, artifact_sphere, 'DIFFERENCE')
artifact:add_child(carvedring)
--===========================================


--===========================================STATUES
i_statue = gr.node('i_statue')

statue_bound = gr.cube('statue_bound')
statue_bound:translate(-0.5,-0.5,-0.5)
statue_bound:scale(90,160,72)
statue_bound:set_material(perfectCanvas)
i_statue:add_bound(statue_bound)

--helmet
s_helmet = gr.cylinder('s_helmet')
s_helmet:scale(8.5,10,8.5)
s_helmet:translate(0,10,0)
s_helmet:set_material(chrome)

s_helmet_diff = gr.node('s_helmet_diff')

s_helmet_inner = gr.cylinder('s_helmet_inner')
s_helmet_inner:scale(7.8,10,7.8)
s_helmet_inner:translate(0,9,0)
s_helmet_inner:set_material(ruby)
s_helmet_diff:add_child(s_helmet_inner)

s_helmet_eyehole = gr.cylinder('s_helmet_eyehole')
s_helmet_eyehole:scale(2.7,5,2.7)
s_helmet_eyehole:rotate('z', 90)
s_helmet_eyehole:translate(0,10,6.6)
s_helmet_eyehole:set_material(ruby)
s_helmet_diff:add_child(s_helmet_eyehole)

s_helmet_mouthhole = gr.cylinder('s_helmet_mouthhole')
s_helmet_mouthhole:scale(2.7,5,2.7)
s_helmet_mouthhole:translate(0,5,6)
s_helmet_mouthhole:set_material(emerald)
s_helmet_diff:add_child(s_helmet_mouthhole)

s_helmet_csg = gr.csg('s_helmet_csg')
s_helmet_csg:translate(0,23,0)
s_helmet_csg:set_csg_children(s_helmet, s_helmet_diff, 'DIFFERENCE')
i_statue:add_child(s_helmet_csg)

--body
s_body_frame = gr.hyperboloid('s_body_frame', 8)
s_body_frame:scale(6,18,3.8)
s_body_frame:set_material(chrome)
i_statue:add_child(s_body_frame)

s_body_trim_top = gr.torus('s_body_trim_top', 0.07)
s_body_trim_top:scale(17.3,11.7,17.3)
s_body_trim_top:rotate('x', 90)
s_body_trim_top:translate(0,18,0)
s_body_trim_top:set_material(emerald)
i_statue:add_child(s_body_trim_top)

s_body_trim_bottom = gr.torus('s_body_trim_bottom', 0.07)
s_body_trim_bottom:scale(17.3,11.7,17.3)
s_body_trim_bottom:rotate('x', 90)
s_body_trim_bottom:translate(0,-18,0)
s_body_trim_bottom:set_material(emerald)
i_statue:add_child(s_body_trim_bottom)

--legs
leg = gr.node('leg')

thigh_socket = gr.sphere('thigh_socket')
thigh_socket:scale(6.4,6.4,6.4)
thigh_socket:translate(0,-20,0)
thigh_socket:set_material(ruby)
leg:add_child(thigh_socket)

thigh = gr.cone('thigh')
thigh:rotate('x', 180)
thigh:scale(6.8,100,6.8)
thigh:translate(0,-123,0)
thigh:set_material(emerald)

thigh_bounds = gr.cylinder('thigh_bounds')
thigh_bounds:scale(6.3,12,6.3)
thigh_bounds:translate(0,-35,0)
thigh_bounds:set_material(chrome)

lthigh = gr.csg('lthigh')
lthigh:set_csg_children(thigh, thigh_bounds, 'INTERSECT')
leg:add_child(lthigh)

kneecap = gr.sphere('kneecap')
kneecap:scale(5.5,5.5,5.5)
kneecap:translate(0,-47,0)
kneecap:set_material(chrome)
leg:add_child(kneecap)

shin = gr.cylinder('shin')
shin:scale(4.8,12,4.8)
shin:translate(0,-58,0)
shin:set_material(chrome)
leg:add_child(shin)

heel = gr.sphere('heel')
heel:scale(4.7,4.7,4.7)
heel:translate(0,-70,0)
heel:set_material(ruby)
leg:add_child(heel)

shoe = gr.cube('shoe')
shoe:translate(-0.5,-0.5,-0.5)
shoe:scale(9,4,18)
shoe:translate(0,-75,6)
shoe:set_material(emerald)

shoe_differ = gr.cube('shoe_differ')
shoe_differ:translate(-0.5,-0.5,-0.5)
shoe_differ:scale(10,4,18)
shoe_differ:rotate('x', 25)
shoe_differ:translate(0,-73,13)
shoe_differ:set_material(ruby)

shoecsg = gr.csg('shoecsg')
shoecsg:set_csg_children(shoe, shoe_differ, 'DIFFERENCE')
leg:add_child(shoecsg)

left_leg = gr.node('left_leg')
left_leg:rotate('y', -15)
left_leg:translate(-9,0,0)
left_leg:add_child(leg)
i_statue:add_child(left_leg)

right_leg = gr.node('right_leg')
right_leg:rotate('y', 15)
right_leg:translate(9,0,0)
right_leg:add_child(leg)
i_statue:add_child(right_leg)


--------arms
left_arm = gr.node('left_arm')
i_statue:add_child(left_arm)

shoulder = gr.sphere('shoulder')
shoulder:scale(6,6,6)
shoulder:translate(-18,18,0)
shoulder:set_material(jade)
left_arm:add_child(shoulder)

arm_top = gr.cone('arm_top')
arm_top:scale(6.5,80,6.5)
arm_top:rotate('x', 180)
arm_top:translate(-18,-59,0)
arm_top:set_material(emerald)

arm_top_bounds = gr.cube('arm_top_bounds')
arm_top_bounds:translate(-0.5,-0.5,-0.5)
arm_top_bounds:scale(10,25,10)
arm_top_bounds:translate(-18,8,0)
arm_top_bounds:set_material(ruby)

armtop_csg = gr.csg('armtop_csg')
armtop_csg:translate(0,-9,0)
armtop_csg:rotate('z', -50)
armtop_csg:rotate('y',20)
armtop_csg:translate(-16,-4,0)
armtop_csg:set_csg_children(arm_top, arm_top_bounds, 'INTERSECT')
left_arm:add_child(armtop_csg)

elbow = gr.sphere('elbow')
elbow:scale(5,5,5)
elbow:rotate('y',-42)
elbow:translate(-18,-7.6,0)
elbow:set_material(ruby)
armtop_csg:add_child(elbow)

arm = gr.cylinder('arm')
arm:scale(1/5,1/5,1/5)
arm:scale(3.5,20,3.5)
arm:translate(0,4,0)
arm:rotate('z', -55)
arm:set_material(emerald)
elbow:add_child(arm)
------------

right_arm = gr.node('right_arm')
right_arm:scale(-1,1,1)
right_arm:add_child(left_arm)
i_statue:add_child(right_arm)

hands = gr.sphere('hands')
hands:scale(5,5,5)
hands:translate(0,-5,17)
hands:set_material(chrome)
i_statue:add_child(hands)

--sword
handle = gr.cylinder('handle')
handle:scale(1/5,1/5,1/5)
handle:scale(1.3,8.5,1.3)
handle:translate(0,1,0)
handle:set_material(ruby)
hands:add_child(handle)

handle_cap = gr.cylinder('handle_cap')
handle_cap:scale(1/1.3,1/8.5,1/1.3)
handle_cap:scale(1.8,0.5,1.8)
handle_cap:translate(0,1,0)
handle_cap:set_material(emerald)
handle:add_child(handle_cap)

hilt = gr.cylinder('hilt')
hilt:scale(1/5,1/5,1/5)
hilt:scale(1.6,10,1.6)
hilt:rotate('z', 90)
hilt:translate(0,-1.2,0)
hilt:set_material(ruby)
hands:add_child(hilt)

hilt_left = gr.cylinder('hilt_left')
hilt_left:scale(1/1.6,1/10,1/1.6)
hilt_left:scale(2,0.5,2)
hilt_left:translate(0,-1,0)
hilt_left:set_material(emerald)
hilt:add_child(hilt_left)

hilt_right = gr.cylinder('hilt_right')
hilt_right:scale(1/1.6,1/10,1/1.6)
hilt_right:scale(2,0.5,2)
hilt_right:translate(0,1,0)
hilt_right:set_material(emerald)
hilt:add_child(hilt_right)

blade = gr.cube('blade')
blade:translate(-0.5,-0.5,-0.5)
blade:scale(1/5,1/5,1/5)
blade:scale(13,70,2)
blade:translate(0,-8,0)
blade:set_material(turquoise)

blade_edge_carver = gr.cube('blade_edge_carver')
blade_edge_carver:translate(-0.5,-0.5,-0.5)
blade_edge_carver:scale(1/5,1/5,1/5)
blade_edge_carver:rotate('y', 45)
blade_edge_carver:scale(13/1.41421,70,3.2)
blade_edge_carver:translate(0,-8,0)
blade_edge_carver:set_material(ruby)

blade_csg = gr.csg('blade_csg')
blade_csg:set_csg_children(blade, blade_edge_carver, 'INTERSECT')

blade_differ = gr.node('blade_differ')

blade_csg_all = gr.csg('blade_csg_all')
blade_csg_all:set_csg_children(blade_csg, blade_differ, 'DIFFERENCE')
hands:add_child(blade_csg_all)

diag_left = gr.cube('diag_left')
diag_left:translate(-0.5,-0.5,-0.5)
diag_left:scale(1/5,1/5,1/5)
diag_left:scale(10,10,10)
diag_left:rotate('z', 45)
diag_left:translate(-1.4,-14.5,0)
diag_left:set_material(emerald)
blade_differ:add_child(diag_left)

diag_right = gr.cube('diag_right')
diag_right:translate(-0.5,-0.5,-0.5)
diag_right:scale(1/5,1/5,1/5)
diag_right:scale(10,10,10)
diag_right:rotate('z', 45)
diag_right:translate(1.4,-14.5,0)
diag_right:set_material(emerald)
blade_differ:add_child(diag_right)

diag_front = gr.cube('diag_front')
diag_front:translate(-0.5,-0.5,-0.5)
diag_front:scale(1/5,1/5,1/5)
diag_front:scale(10,10,1.5)
diag_front:rotate('z', 45)
diag_front:rotate('x', 15)
diag_front:translate(0,-14,0.3)
diag_front:set_material(emerald)
blade_differ:add_child(diag_front)

diag_back = gr.cube('diag_back')
diag_back:translate(-0.5,-0.5,-0.5)
diag_back:scale(1/5,1/5,1/5)
diag_back:scale(10,10,1.5)
diag_back:rotate('z', 45)
diag_back:rotate('x', -15)
diag_back:translate(0,-14,-0.3)
diag_back:set_material(emerald)
blade_differ:add_child(diag_back)
-------

statue_base = gr.cylinder('statue_base')
statue_base:scale(36,1.5,36)
statue_base:translate(0,-78.3,0)
statue_base:set_material(chrome)
i_statue:add_child(statue_base)

statue_1 = gr.node('statue_1')
statue_1:rotate('y', 90)
statue_1:scale(0.48,0.48,0.48)
statue_1:translate(-66,-30,-190)
statue_1:add_child(i_statue)

statue_2 = gr.node('statue_2')
statue_2:rotate('y', 90)
statue_2:scale(0.48,0.48,0.48)
statue_2:translate(-66,-30,-100)
statue_2:add_child(i_statue)

statue_3 = gr.node('statue_3')
statue_3:rotate('y', 90)
statue_3:scale(0.48,0.48,0.48)
statue_3:translate(-66,-30,-10)
statue_3:add_child(i_statue)

statue_r1 = gr.node('statue_r1')
statue_r1:rotate('y', -90)
statue_r1:scale(0.48,0.48,0.48)
statue_r1:translate(73,-30,-190)
statue_r1:add_child(i_statue)

statue_r2 = gr.node('statue_r2')
statue_r2:rotate('y', -90)
statue_r2:scale(0.48,0.48,0.48)
statue_r2:translate(73,-30,-100)
statue_r2:add_child(i_statue)

statue_r3 = gr.node('statue_r3')
statue_r3:rotate('y', -90)
statue_r3:scale(0.48,0.48,0.48)
statue_r3:translate(73,-30,-10)
statue_r3:add_child(i_statue)
--====================================================


--outside================
outside = gr.node('outside')

background = gr.cube('background')
background:translate(-0.5,-0.5,-0.5)
background:scale(2500,500,1)
background:translate(0,0,-600)
background:set_material(perfectCanvas)
background:set_texture(t_sunset)

outside:add_child(background)
--==========================



scene:add_child(walls)
scene:add_child(outside)
scene:add_child(pillars)
scene:add_child(altar)
--scene:add_child(statues)

statues:add_child(statue_1)
statues:add_child(statue_2)
statues:add_child(statue_3)
statues:add_child(statue_r1)
statues:add_child(statue_r2)
statues:add_child(statue_r3)

pillars:add_child(left_pillar_1)
pillars:add_child(left_pillar_2)
pillars:add_child(left_pillar_3)
pillars:add_child(right_pillar_1)
pillars:add_child(right_pillar_2)
pillars:add_child(right_pillar_3)

walls:add_child(ground)
walls:add_child(roof)
walls:add_child(leftwallcsg)
walls:add_child(rightwallcsg)
walls:add_child(backwallcsg)



gr.render(scene, 'FinalScene.png', 888, 500,
    {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
    {0.5, 0.5, 0.5}, {primary_light, test_inner_light})
