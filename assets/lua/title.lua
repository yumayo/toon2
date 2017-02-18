bgm = file_player.create("home.mp3")
bgm:play()
root:add_child(bgm)

background = button.create("monta.png")
background.position = vec2.new( get_window_size.x * 0.5, get_window_size.y * 0.5 )
background.scale = vec2.new(1.5)
background.on_ended = function() tap() end
root:add_child(background)

logo = label.create( "オセラトゥーン", "sample.otf", 48 )
logo.position = vec2.new(get_window_size.x * 0.5, get_window_size.y * 0.25)
logo:run_action(
    repeat_forever.create(
        sequence.create(
        EaseInOutSine.create( scale_by.create(1.0, vec2.new(0.1, 0.1) ) ), EaseInOutSine.create(scale_by.create(1.0, vec2.new(-0.1,-0.1))))))
root:add_child(logo)

tap_start = label.create( "tap to start", "sample.otf", 36 )
tap_start.position = vec2.new(get_window_size.x * 0.5, get_window_size.y * 0.75)
tap_start:run_action(
    repeat_forever.create(
        sequence.create(
        EaseInOutSine.create( fade_in.create(1.0) ), EaseInOutSine.create(fade_out.create(1.0)))))
root:add_child(tap_start)
