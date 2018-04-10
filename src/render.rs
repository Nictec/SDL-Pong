
#[derive(Debug, Clone)]
pub struct Vertex { position: [f32; 2] }
impl_vertex!(Vertex, position);
pub struct RectangleToDraw {
    pub color: [f32; 3],
    // top-left; top-right; bottom-right; bottom-left;
    pub position: [Vertex; 4]
}

pub struct DrawElements {
    pub paddle_l: RectangleToDraw,
    pub paddle_r: RectangleToDraw,
    pub ball: RectangleToDraw,
}

impl DrawElements {
    pub fn new() -> DrawElements {
        return DrawElements {
            paddle_l: RectangleToDraw {
                color: [0.0, 1.0, 0.0],
                position: [
                    Vertex { position: [-0.1, -0.1] },
                    Vertex { position: [-0.1, 0.0] },
                    Vertex { position: [0.0, 0.0] },
                    Vertex { position: [0.0, -0.1] },
                ]
            },
            paddle_r: RectangleToDraw {
                color: [1.0, 0.0, 0.0],
                position: [
                    Vertex { position: [0.0, 0.0] },
                    Vertex { position: [0.0, 0.1] },
                    Vertex { position: [0.1, 0.1] },
                    Vertex { position: [0.1, 0.0] },
                ]
            },
            ball: RectangleToDraw {
                color: [0.0, 0.0, 1.0],
                position: [
                    Vertex { position: [0.0, 0.0] },
                    Vertex { position: [0.0, 0.1] },
                    Vertex { position: [0.1, 0.1] },
                    Vertex { position: [0.1, 0.0] },
                ]
            },
        }
    }
}

#[derive(Debug)]
pub struct GameState {
    paddle_l_y: f32,
    paddle_r_y: f32,

    ball_x: f32,
    ball_y: f32,
    ball_dx: f32,
    ball_dy: f32,

    game_over: bool,
}
impl GameState {
    pub fn new() -> GameState {
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

/*
#[derive(Debug, Clone)]
struct GameButtonState {
    half_transition_count: i32,
    ended_down: bool
}
*/

#[derive(Debug)]
pub struct GameControllerInput {
    pub move_up: bool,
    pub move_down: bool,
    pub move_left: bool,
    pub move_right: bool,

    pub action_up: bool,
    pub action_down: bool,
    pub action_left: bool,
    pub action_right: bool,

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
    pub fn new() -> GameControllerInput {
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

const PADDLE_SPEED_PX_PER_SECONDS : f32 = 60.0;

const BALL_RADIUS : f32 = 2.0;
const PADDLE_HEIGHT : f32 = 20.0;
const PADDLE_WIDTH : f32 = 2.0;
const MAX_BALL_DY : f32 = 60.0;
const BALL_DDX : f32 = 2.0;


pub fn game_update_and_render(input: &GameControllerInput, draw_elements: &mut DrawElements, game_state: &mut GameState, elapsed_seconds: f32){
    if input.move_up {
        game_state.paddle_l_y -= PADDLE_SPEED_PX_PER_SECONDS * elapsed_seconds;
    }
    if input.move_down {
        game_state.paddle_l_y += PADDLE_SPEED_PX_PER_SECONDS * elapsed_seconds;
    }
    if input.action_up {
        game_state.paddle_r_y -= PADDLE_SPEED_PX_PER_SECONDS * elapsed_seconds;
    }
    if input.action_down {
        game_state.paddle_r_y += PADDLE_SPEED_PX_PER_SECONDS * elapsed_seconds;
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


    game_state.ball_y += game_state.ball_dy * elapsed_seconds;
    if game_state.ball_y > 100.0 - BALL_RADIUS {
        game_state.ball_y = 100.0 - BALL_RADIUS;
        game_state.ball_dy = -game_state.ball_dy;
    }
    if game_state.ball_y < BALL_RADIUS {
        game_state.ball_y = BALL_RADIUS;
        game_state.ball_dy = -game_state.ball_dy;
    }

    game_state.ball_x += game_state.ball_dx * elapsed_seconds;

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

    fn set_rect_positions(rect: &mut RectangleToDraw, x: f32, y: f32, width: f32, height: f32) {
        fn normalize(x: f32) -> f32 {
            return (x / 50.0) - 1.0;
        }
        rect.position[0].position[0] = normalize(x);
        rect.position[0].position[1] = normalize(y);

        rect.position[1].position[0] = normalize(x + width);
        rect.position[1].position[1] = normalize(y);

        rect.position[2].position[0] = normalize(x + width);
        rect.position[2].position[1] = normalize(y + height);
        rect.position[3].position[0] = normalize(x);
        rect.position[3].position[1] = normalize(y + height);
    }

    // left paddle
    set_rect_positions(
        &mut draw_elements.paddle_l,
        0.0,
        (game_state.paddle_l_y - PADDLE_HEIGHT / 2.0),
        PADDLE_WIDTH,
        PADDLE_HEIGHT,
    );

    // right paddle
    set_rect_positions(
        &mut draw_elements.paddle_r,
        (100.0 - PADDLE_WIDTH),
        (game_state.paddle_r_y - PADDLE_HEIGHT / 2.0),
        PADDLE_WIDTH,
        PADDLE_HEIGHT,
    );

    // ball
    set_rect_positions(
        &mut draw_elements.ball,
        game_state.ball_x - BALL_RADIUS,
        game_state.ball_y - BALL_RADIUS,
        BALL_RADIUS * 2.0,
        BALL_RADIUS * 2.0,
    );
}
