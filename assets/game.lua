root:remove_all_children()

bgm = file_player.create("island.wav")
bgm:play()
root:add_child(bgm)

function make(x, y)
    r = rect.create( vec2.new(20, 20) )
    r.position = vec2.new(x * 21, y * 21)
    root:add_child(r)
end

for y = 0, 10 do
    for x = 0, 10 do
        make(x, y)
    end
end