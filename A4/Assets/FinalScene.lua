hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
stone = gr.material({0.5, 0.40, 0.01}, {0.25, 0.15, 0.15}, 20)
carpet = gr.material({0.6, 0.02, 0.1}, {0,0,0}, 0)
mirror = gr.material({0.1, 0.16, 0.15}, {1.5,1.5,1.5}, 120)

perfectCanvas = gr.material({1.0, 1.0, 1.0}, {0, 0, 0}, 0)
reflectorCanvas = gr.material_extended({0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, 1000, 1, 1, 0, 0)
refractorCanvas = gr.material_extended({0.6, 0.9, 0.9}, {1.5, 0.4, 1.3}, 1000, 1, 0.4, 0, 0)
glossyReflectorCanvas = gr.material_extended({0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, 1000, 1, 1, 2.4, 0)
flame = gr.material_extended({0.8, 0.7, 0.2}, {0.0, 0.0, 0.0}, 0, 1, 1, 0, 1)

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

primary_light = gr.light({-8000, 8000, 0}, {1.0, 1.0, 1.0}, {1, 0, 0})
test_inner_light = gr.light({0,50,50}, {0.4, 0.4, 0.4}, {1,0,0})

scene = gr.node('root')

walls = gr.node('walls')
--scene:add_child(walls)


------------------------------- WINDOW
i_window = gr.node('i_window')

window_flat = gr.cube('window_flat')
window_flat:translate(-0.5, -0.5, -0.5)
window_flat:set_material(ruby)

window_round = gr.cylinder('window_round')
window_round:scale(0.5,1,0.5)
window_round:rotate('x', 90)
window_round:translate(0, 0.5, 0)
window_round:set_material(chrome)

windowcsg = gr.csg('windowcsg')
windowcsg:set_csg_children(window_round, window_flat, 'UNION')
i_window:add_child(windowcsg)
-----------------------------------------

--walls and boundaries=============================================
ground = gr.cube('ground')
ground:translate(-0.5, -35, -0.6)
ground:scale(300,2,750)
ground:set_material(stone)
walls:add_child(ground)
roof = gr.cube('roof')
roof:translate(-0.5, 74, -0.6)
roof:scale(300,2,750)
roof:set_material(stone)
walls:add_child(roof)

--left
leftwall = gr.cube('leftwall')
leftwall:translate(-12, -0.5, -0.6)
leftwall:scale(12,300,750)
leftwall:set_material(stone)

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

leftwallcsg = gr.csg('leftwallcsg')
leftwallcsg:set_csg_children(leftwall, leftwindows, 'DIFFERENCE')
walls:add_child(leftwallcsg)

--right
rightwall = gr.cube('rightwall')
rightwall:translate(12, -0.5, -0.6)
rightwall:scale(12,300,750)
rightwall:set_material(stone)

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

rightwallcsg = gr.csg('rightwallcsg')
rightwallcsg:set_csg_children(rightwall, rightwindows, 'DIFFERENCE')
walls:add_child(rightwallcsg)


--back wall
backwall = gr.cube('backwall')
backwall:translate(-0.5, -0.5, -220)
backwall:scale(300,300,2)
backwall:set_material(stone)

backwindow = gr.node('backwindow')
backwindow:scale(160,120,20)
backwindow:translate(5,22,-440)
backwindow:add_child(i_window)

backwallcsg = gr.csg('backwallcsg')
backwallcsg:set_csg_children(backwall, backwindow, 'DIFFERENCE')
walls:add_child(backwallcsg)
--===========================================================================wall and boundaries

--==============pillar======================
i_pillar = gr.node('i_pillar')

pillar_core = gr.cylinder('pillar_center')
pillar_core:scale(6.6,100,6.6)
pillar_core:translate(0,20,0)
pillar_core:set_material(polsilver)
i_pillar:add_child(pillar_core)

pillar_base = gr.cube('pillar_base')
pillar_base:translate(-0.5, -0.5, -0.5)
pillar_base:scale(30,7.2,30)
pillar_base:translate(0,-80, 0)
pillar_base:set_material(polgold)
i_pillar:add_child(pillar_base)

pillar_base_ring_1 = gr.torus('pillar_base_ring_1',0.2)
pillar_base_ring_1:rotate('x', 90)
pillar_base_ring_1:scale(10.6,10.6,10.6)
pillar_base_ring_1:translate(0,-75, 0)
pillar_base_ring_1:set_material(ruby)
i_pillar:add_child(pillar_base_ring_1)

pillar_base_ring_2 = gr.torus('pillar_base_ring_2',0.18)
pillar_base_ring_2:rotate('x', 90)
pillar_base_ring_2:scale(9.3,9.3,9.3)
pillar_base_ring_2:translate(0,-70.67, 0)
pillar_base_ring_2:set_material(emerald)
i_pillar:add_child(pillar_base_ring_2)

top_cone_decor = gr.cone('top_cone_decor')
top_cone_decor:rotate('x', 180)
top_cone_decor:scale(9.9,30,9.9)
top_cone_decor:translate(0,99,0)
top_cone_decor:set_material(emerald)
i_pillar:add_child(top_cone_decor)

pillar_top = gr.cube('pillar_top')
pillar_top:translate(-0.5, -0.5, -0.5)
pillar_top:scale(22,10,22)
pillar_top:translate(0,133,0)
pillar_top:set_material(polgold)
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
pillars = gr.node('pillars')
--scene:add_child(pillars)

left_pillar_1 = gr.node('left_pillar_1')
left_pillar_1:rotate('y', 180)
left_pillar_1:translate(-66,15.5,-235)
left_pillar_1:add_child(i_pillar)
pillars:add_child(left_pillar_1)

left_pillar_2 = gr.node('left_pillar_2')
left_pillar_2:rotate('y', 180)
left_pillar_2:translate(-66,15.5,-145)
left_pillar_2:add_child(i_pillar)
pillars:add_child(left_pillar_2)

left_pillar_3 = gr.node('left_pillar_3')
left_pillar_3:rotate('y', 180)
left_pillar_3:translate(-66,15.5,-55)
left_pillar_3:add_child(i_pillar)
pillars:add_child(left_pillar_3)

right_pillar_1 = gr.node('right_pillar_1')
right_pillar_1:translate(73,15.5,-235)
right_pillar_1:add_child(i_pillar)
pillars:add_child(right_pillar_1)

right_pillar_2 = gr.node('right_pillar_2')
right_pillar_2:translate(73,15.5,-145)
right_pillar_2:add_child(i_pillar)
pillars:add_child(right_pillar_2)

right_pillar_3 = gr.node('right_pillar_3')
right_pillar_3:translate(73,15.5,-55)
right_pillar_3:add_child(i_pillar)
pillars:add_child(right_pillar_3)
--==================================================

--=====altar==================================
altar = gr.node('altar')
scene:add_child(altar)

altar_base_1 = gr.cube('altar_base_1')
altar_base_1:translate(-0.5, -0.5, -0.5)
altar_base_1:scale(200,11,100)
altar_base_1:translate(5,-65,-320)
altar_base_1:set_material(chrome)
altar:add_child(altar_base_1)

altar_base_2 = gr.cube('altar_base_2')
altar_base_2:translate(-0.5, -0.5, -0.5)
altar_base_2:scale(188,11,100)
altar_base_2:translate(5,-59,-326)
altar_base_2:set_material(chrome)
altar:add_child(altar_base_2)

altar_base_3 = gr.cube('altar_base_3')
altar_base_3:translate(-0.5, -0.5, -0.5)
altar_base_3:scale(172,11,100)
altar_base_3:translate(5,-53,-334)
altar_base_3:set_material(chrome)
altar:add_child(altar_base_3)

artifact = gr.node('artifact')
artifact:translate(0,-80,-150)
altar:add_child(artifact)

artifact_base = gr.cylinder('artifact_base')
artifact_base:scale(50,6,50)
artifact_base:set_material(pearl)
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
carvedpyramid:rotate('x', 180)
carvedpyramid:translate(0,100,0)
carvedpyramid:set_csg_children(pyramid, sphericalcarver, 'DIFFERENCE')
artifact:add_child(carvedpyramid)


artifact_sphere = gr.sphere('artifact_sphere')
artifact_sphere:scale(24,24,24)
artifact_sphere:translate(0,100,0)
artifact_sphere:set_material(chrome)
artifact:add_child(artifact_sphere)

--===========================================

gr.render(scene, 'FinalScene.png', 700, 500,
    {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
    {0.5, 0.5, 0.5}, {primary_light, test_inner_light})
