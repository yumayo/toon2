bgm = file_player.create("island.wav")
bgm:play()
root:add_child(bgm)

suf = surface.create( vec2.new(256, 256) )
suf.position = vec2.new(get_window_size.x * 0.5, get_window_size.y * 0.5)
root:add_child(suf)

function paint( pixel )
    suf:set_pixel( pixel, color.new(0.8, 0.3, 0.4, 1.0))
end