size = vec2.new(100)

p0 = button.create( size )
p0.color = color.new( 0, 0, 1, 1 )
p0.position = vec2.new(100, 100)
p0.scale = vec2.new(0.5, 0.5)
p0.rotation = pi / 4.0
p0.name = "blue rect"

root:add_child(p0)

text = label.create( "ほげほげ", "sample.otf", 48 )
text.position = vec2.new(100, 100)
root:add_child(text)

function make_client(p1)
    client = tcp_client.create("192.168.11.2", "25565")
    client.name = "クライアント"
    p1:add_child( client )
end

p1 = button.create( size )
if p1 then
    p1.position = vec2.new( 200, 200 )
    p1.color = color.new(1, 0, 1, 1)
    p1.scale = vec2.new(2, 2)
    p1.rotation = pi / 3
    p1.name = "p1 button"
    p1.anchor_point = vec2.new(0.5, 0.5)
    p1.pivot = vec2.new(0, 0)

    seq = sequence.create(  spawn.create(   EaseInOutExpo.create( rotate_by.create(0.5, pi / 2 ) ),
                                            EaseInOutExpo.create( scale_by.create(0.5, vec2.new( 1, 1 ) ) ) )
                            ,call_func.create(function() p1:remove_child_by_name("クライアント") end)
                            ,EaseInOutExpo.create(move_to.create(1.0, vec2.new(400, 200)))
                            ,EaseInOutExpo.create( scale_by.create(0.5, vec2.new( -1, -1 ) ) )
                            ,EaseInOutExpo.create(move_to.create(1.0, vec2.new(200, 200)))
                            ,call_func.create(function() make_client(p1) end)
                         )
    p1:run_action( repeat_forever.create( seq ) )

    root:add_child(p1)

    p2 = sprite.create( "hogehoge.png" )
    if p2 then
        p2.position = vec2.new(0, 0)
        p2.scale = vec2.new(0.25, 0.25)
        p2.name = "p2 sprite"
        p1:add_child(p2)

        p3 = button.create( size )
        if p3 then
            p3.position = vec2.new(100, 0)
            p3.color = color.new(0.2, 1, 0.5, 1)
            p3.name = "p3 button"
            p2:add_child(p3)
        end
    end
end