stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, {0, 0, 0}, 1, 20)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, {0, 0, 0}, 1, 20)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, {0, 0, 0}, 1, 20)


scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
scene_root:add_child(s1)
s1:set_material(stone)

s2 = gr.nh_sphere('s2', {200, 50, -100}, 150)
scene_root:add_child(s2)
s2:set_material(stone)

s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
scene_root:add_child(s3)
s3:set_material(grass)

s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
scene_root:add_child(s4)
s4:set_material(hide)

s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
scene_root:add_child(s5)
s5:set_material(stone)

white_light = gr.light({-100.0, 150.0, 400.0}, {1.0, 1.0, 1.0}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'simple.png', 756, 756,
    {0, 0, 800}, {0, 0, -800}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light, orange_light})
