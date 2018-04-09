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

pub fn game_update(input: &GameControllerInput, game_state: &mut GameState, elapsed_seconds: f32){
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
}
