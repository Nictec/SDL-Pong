extern crate sdl2;
extern crate time;
use sdl2::pixels::Color;
use sdl2::rect::Rect;
use sdl2::event::Event;
use sdl2::keyboard::{Keycode, Scancode};
use std::time::Duration;
use sdl2::render::Canvas;
use sdl2::video::Window;

/*
#[derive(Debug, Clone)]
struct GameButtonState {
    half_transition_count: i32,
    ended_down: bool
}
*/

#[derive(Debug)]
struct GameControllerInput {
    move_up: bool,
    move_down: bool,
    move_left: bool,
    move_right: bool,

    action_up: bool,
    action_down: bool,
    action_left: bool,
    action_right: bool,

    /*action_up: GameButtonState,
    action_down: GameButtonState,
    action_left: GameButtonState,
    action_right: GameButtonState,

    left_shoulder: GameButtonState,
    right_shoulder: GameButtonState,

    back: GameButtonState,
    start: GameButtonState,

    // NOTE(casey): All buttons must be added above this line

    terminator: GameButtonState
    */
}
impl GameControllerInput {
    fn new() -> GameControllerInput {
        return GameControllerInput {
            move_down:  false,
            move_up:    false,
            move_left:  false,
            move_right: false,
            action_down:  false,
            action_up:    false,
            action_left:  false,
            action_right: false,
        };
    }
}

#[derive(Debug)]
struct GameState {
    paddle_l_y: f32,
    paddle_r_y: f32,

    ball_x: f32,
    ball_y: f32,
    ball_dx: f32,
    ball_dy: f32,

    game_over: bool,
}
impl GameState {
    fn new() -> GameState {
        return GameState {
            paddle_l_y: 50.0,
            paddle_r_y: 50.0,

            ball_x: 50.0,
            ball_y: 50.0,
            ball_dx: 30.0,
            ball_dy: 0.0,

            game_over: false,
        };
    }
}

pub fn main() {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();

    let window = video_subsystem.window("rust-sdl2 demo", 800, 600)
        .position_centered()
        //.resizable()
        .build()
        .unwrap();

    let mut canvas = window.into_canvas().build().unwrap();

    canvas.set_draw_color(Color::RGB(0, 255, 255));
    canvas.clear();
    canvas.present();

    let mut event_pump = sdl_context.event_pump().unwrap();

    let mut new_input;
    let mut game_state = GameState::new();
    let mut last_counter = time::precise_time_ns();
    'running: loop {
        new_input = GameControllerInput::new();
        canvas.set_draw_color(Color::RGB(0, 0, 0));
        canvas.clear();
        {
            let keyboard_state = event_pump.keyboard_state();
            new_input.move_up       = keyboard_state.is_scancode_pressed(Scancode::W);
            new_input.move_down     = keyboard_state.is_scancode_pressed(Scancode::S);
            new_input.move_left     = keyboard_state.is_scancode_pressed(Scancode::A);
            new_input.move_right    = keyboard_state.is_scancode_pressed(Scancode::D);
            new_input.action_up       = keyboard_state.is_scancode_pressed(Scancode::Up);
            new_input.action_down     = keyboard_state.is_scancode_pressed(Scancode::Down);
            new_input.action_left     = keyboard_state.is_scancode_pressed(Scancode::Left);
            new_input.action_right    = keyboard_state.is_scancode_pressed(Scancode::Right);
        }
        for event in event_pump.poll_iter() {
            let mut should_quit = false;
            match event {
                Event::Quit {..} => {
                    should_quit = true;
                },
                Event::KeyUp { keycode: Some(Keycode::Escape), .. } => {
                    should_quit = true;
                },
                /*
                Event::KeyUp { keycode: Some(keycode), scancode: Some(scancode), repeat, .. } |
                Event::KeyDown { keycode: Some(keycode), scancode: Some(scancode), repeat, .. } => {
                    if !repeat {
                        let was_pressed = keyboard_state.is_scancode_pressed(scancode);
                        match keycode {
                            Keycode::Escape => {
                                should_quit = true;
                            },
                            Keycode::W => { new_input.move_up = was_pressed; },
                            Keycode::S => { new_input.move_down = was_pressed; },
                            Keycode::A => { new_input.move_left = was_pressed; },
                            Keycode::D => { new_input.move_right = was_pressed; },
                            _ => {}
                        }
                    }
                },
                */
                _ => {}
            }
            if should_quit {
                break 'running
            }
        }
        // The rest of the game loop goes here...
        game_update_and_render(&new_input, &mut canvas, &mut game_state);

        let now = time::precise_time_ns();

        let ns_elapsed = now - last_counter;
        if ns_elapsed < TARGET_NS_PER_FRAME {
            let ns_to_sleep = (TARGET_NS_PER_FRAME - ns_elapsed) as u32;
            ::std::thread::sleep(Duration::new(0, ns_to_sleep));
        }


        canvas.present();

        last_counter = time::precise_time_ns();
    }
}

const TARGET_FPS : u32 = 60;
const TARGET_SECONDS_PER_FRAME : f64 = 1.0 / (TARGET_FPS as f64);
const TARGET_NS_PER_FRAME : u64 = (TARGET_SECONDS_PER_FRAME * 1_000_000_000.0) as u64;

const PADDLE_SPEED : f32 = 60.0 * TARGET_SECONDS_PER_FRAME as f32;

const BALL_RADIUS : f32 = 2.0;
const PADDLE_HEIGHT : f32 = 20.0;
const PADDLE_WIDTH : f32 = 2.0;
const MAX_BALL_DY : f32 = 60.0;
const BALL_DDX : f32 = 2.0;

fn game_update_and_render(input: &GameControllerInput, canvas: &mut Canvas<Window>, game_state: &mut GameState){
    if input.move_up {
        game_state.paddle_l_y -= PADDLE_SPEED;
    }
    if input.move_down {
        game_state.paddle_l_y += PADDLE_SPEED;
    }
    if input.action_up {
        game_state.paddle_r_y -= PADDLE_SPEED;
    }
    if input.action_down {
        game_state.paddle_r_y += PADDLE_SPEED;
    }

    // clamp
    if game_state.paddle_l_y < PADDLE_HEIGHT / 2.0 {
        game_state.paddle_l_y = PADDLE_HEIGHT / 2.0;
    }
    if game_state.paddle_l_y > 100.0 - PADDLE_HEIGHT / 2.0 {
        game_state.paddle_l_y = 100.0 - PADDLE_HEIGHT / 2.0;
    }
    if game_state.paddle_r_y < PADDLE_HEIGHT / 2.0 {
        game_state.paddle_r_y = PADDLE_HEIGHT / 2.0;
    }
    if game_state.paddle_r_y > 100.0 - PADDLE_HEIGHT / 2.0 {
        game_state.paddle_r_y = 100.0 - PADDLE_HEIGHT / 2.0;
    }


    game_state.ball_y += game_state.ball_dy * TARGET_SECONDS_PER_FRAME as f32;
    if game_state.ball_y > 100.0 - BALL_RADIUS {
        game_state.ball_y = 100.0 - BALL_RADIUS;
        game_state.ball_dy = -game_state.ball_dy;
    }
    if game_state.ball_y < BALL_RADIUS {
        game_state.ball_y = BALL_RADIUS;
        game_state.ball_dy = -game_state.ball_dy;
    }

    game_state.ball_x += game_state.ball_dx * TARGET_SECONDS_PER_FRAME as f32;

    let ball_left_edge = game_state.ball_x - BALL_RADIUS;
    if ball_left_edge < PADDLE_WIDTH {
        if !game_state.game_over
                && ((game_state.ball_y - BALL_RADIUS < game_state.paddle_l_y + PADDLE_HEIGHT / 2.0)
                && ((game_state.ball_y + BALL_RADIUS > game_state.paddle_l_y - PADDLE_HEIGHT / 2.0))) {
            if game_state.ball_dx < 0.0 {
                game_state.ball_dx = -game_state.ball_dx + BALL_DDX;

                game_state.ball_dy =
                    (game_state.ball_y - game_state.paddle_l_y)
                    * ( MAX_BALL_DY / (PADDLE_HEIGHT / 2.0 + BALL_RADIUS));
            }
        } else {
            game_state.game_over = true;
            if game_state.ball_x < 0.0 - BALL_RADIUS {
                // restart
                *game_state = GameState::new();
            }
        }
    }

    let ball_right_edge = game_state.ball_x + BALL_RADIUS;
    if ball_right_edge > 100.0 - PADDLE_WIDTH {
        if !game_state.game_over
                && ((game_state.ball_y - BALL_RADIUS < game_state.paddle_r_y + PADDLE_HEIGHT / 2.0)
                && ((game_state.ball_y + BALL_RADIUS > game_state.paddle_r_y - PADDLE_HEIGHT / 2.0))) {
            if game_state.ball_dx > 0.0 {
                game_state.ball_dx = -game_state.ball_dx - BALL_DDX;

                game_state.ball_dy =
                    (game_state.ball_y - game_state.paddle_r_y)
                    * ( MAX_BALL_DY / (PADDLE_HEIGHT / 2.0 + BALL_RADIUS));
            }
        } else {
            game_state.game_over = true;
            if game_state.ball_x > 100.0 + BALL_RADIUS {
                // restart
                *game_state = GameState::new();
            }
        }
    }


    let (buffer_width, buffer_height) = canvas.output_size().unwrap();
    let width_correction = buffer_width as f32 / 100.0;
    let height_correction = buffer_height as f32 / 100.0;

    canvas.set_draw_color(Color::RGB(255, 210, 0));

    // left paddle
    canvas.fill_rect(Rect::new(
            0,
            ((game_state.paddle_l_y - PADDLE_HEIGHT / 2.0) * height_correction) as i32,
            (PADDLE_WIDTH * width_correction) as u32,
            (PADDLE_HEIGHT * height_correction) as u32,
        )).unwrap();

    // left paddle
    canvas.fill_rect(Rect::new(
            ((100.0 - PADDLE_WIDTH) * width_correction) as i32,
            ((game_state.paddle_r_y - PADDLE_HEIGHT / 2.0) * height_correction) as i32,
            (PADDLE_WIDTH * width_correction) as u32,
            (PADDLE_HEIGHT * height_correction) as u32,
        )).unwrap();

    // ball
    canvas.fill_rect(Rect::new(
            (game_state.ball_x * width_correction - BALL_RADIUS * height_correction) as i32,
            (game_state.ball_y * height_correction - BALL_RADIUS * height_correction) as i32,
            (BALL_RADIUS * 2.0 * height_correction) as u32,
            (BALL_RADIUS * 2.0 * height_correction) as u32,
            ))
        .unwrap();
}
