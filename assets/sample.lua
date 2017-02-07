size = vec2.new(100)

p0 = button.create( size )
p0.color = color.new( 0, 0, 1, 1 )
p0.position = vec2.new(100, 100)
p0.scale = vec2.new(0.5, 0.5)
p0.rotation = pi / 4.0
p0.name = "blue rect"
root:add_child(p0)

p1 = button.create( size )
p1.position = vec2.new(200, 200)
p1.color = color.new(1, 0, 1, 1)
p1.scale = vec2.new(2, 2)
p1.rotation = pi / 3
p1.name = "p1 button"
p1.anchor_point = vec2.new(0.5, 0.5)
p1.pivot = vec2.new(0, 0)

act1 = move_to.create(2.0, vec2.new(400, 200))
act2 = move_to.create(2.0, vec2.new(200, 200))
seq = sequence.create(EaseInOutExpo.create(act1),
                      delay.create(1.0),
                      EaseInOutQuart.create(act2),
                      remove_self.create())
p1:run_action( seq )
root:add_child(p1)

p2 = sprite.create( "hogehoge.png" )
p2.position = vec2.new(0, 0)
p2.scale = vec2.new(0.25, 0.25)
p2.name = "p2 sprite"
p1:add_child(p2)

p3 = button.create( size )
p3.position = vec2.new(100, 0)
p3.color = color.new(0.2, 1, 0.5, 1)
p3.name = "p3 button"
p2:add_child(p3)