#include <GL/glut.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

enum GameState { STOPPED, RUNNING, ENDED };
GameState gameState = STOPPED;


float ball_x = 0.0;
float ball_y = 0.0;
float ball_radius = 0.05;


float ball_dx = 0.0;
float ball_dy = 0.0;


float goalkeeper_y = 0.0;
float goalkeeper_dy = 0.001;

float goalkeeper_height = 0.2;


float goalpost_width = 0.4;
float goalpost_height = 0.4;


int score = 0;

std::string instructionText = "Press SPACE to start/pause the game";
std::string scoreText = "Score: 0";


void drawBackground() {
    glColor3f(0.0, 0.8, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();
}

void drawText(float x, float y, const char* text) {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        ++text;
    }
}


void drawGoalkeeper() {

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-1.0, goalkeeper_y - goalkeeper_height / 2);
    glVertex2f(-0.9, goalkeeper_y - goalkeeper_height / 2);
    glVertex2f(-0.9, goalkeeper_y + goalkeeper_height / 2);
    glVertex2f(-1.0, goalkeeper_y + goalkeeper_height / 2);
    glEnd();


    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 180; i++) {
        float theta = i * 3.14159 / 180;
        glVertex2f(-1.0, goalkeeper_y + goalkeeper_height / 2 + 0.05 * cos(theta));
    }
    glEnd();
}

void drawBall() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159 / 180;
        glVertex2f(ball_x + ball_radius * cos(theta), ball_y + ball_radius * sin(theta));
    }
    glEnd();
}


void drawGoalpost() {
    glColor3f(0.5, 0.5, 0.5);


    glBegin(GL_QUADS);
    glVertex2f(-1.0, -goalpost_height / 2);
    glVertex2f(-1.0, goalpost_height / 2);
    glVertex2f(-1.0 - goalpost_width, goalpost_height / 2);
    glVertex2f(-1.0 - goalpost_width, -goalpost_height / 2);
    glEnd();


    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (float y = -goalpost_height / 2 + 0.1; y <= goalpost_height / 2 - 0.1; y += 0.2) {
        glVertex2f(-1.0, y);
        glVertex2f(-1.0 - goalpost_width, y);
    }
    for (float x = -1.0 - goalpost_width + 0.1; x <= -1.0 - 0.1; x += 0.2) {
        glVertex2f(x, -goalpost_height / 2);
        glVertex2f(x, goalpost_height / 2);
    }
    glEnd();
}


void updateScore() {
    if (ball_x + ball_radius > -1.0 - goalpost_width && ball_x - ball_radius < -1.0 && ball_y > -goalpost_height / 2 && ball_y < goalpost_height / 2) {
        score++;
        std::stringstream ss;
        ss << "Score: " << score;
        scoreText = ss.str();
        ball_dx = 0.0;
        ball_dy = 0.0;
        ball_x = 0.0;
        ball_y = 0.0;
        gameState = STOPPED;
        instructionText = "Goal! Press SPACE to continue";

        if (score == 10) {
            instructionText = "Goal! You are a pro! Press SPACE to continue";
        } else if (score >= 20) {
            gameState = ENDED;
            instructionText = "Game Over! You scored 20 goals!";
        }
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();

    drawText(-0.8, 0.9, instructionText.c_str());
    drawText(-0.95, 0.8, scoreText.c_str());

    drawGoalkeeper();
    drawBall();
    drawGoalpost();

    glutSwapBuffers();
}


void idle() {
    if (gameState == RUNNING) {
        ball_x += ball_dx;
        ball_y += ball_dy;

        goalkeeper_y += goalkeeper_dy;

        updateScore();

        if (ball_x + ball_radius > 1.0 || ball_x - ball_radius < -1.0) {
            ball_dx *= -1;
        }
        if (ball_y + ball_radius > 1.0 || ball_y - ball_radius < -1.0) {
            ball_dy *= -1;
        }

        if (goalkeeper_y + goalkeeper_height / 2 > 1.0 || goalkeeper_y - goalkeeper_height / 2 < -1.0) {
            goalkeeper_dy *= -1;
        }
    }

    glutPostRedisplay();
}



void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            if (gameState == STOPPED) {
                gameState = RUNNING;
                instructionText = "Press W/S/A/D to move the ball, ESC to quit";
                ball_dx = 0.002;
                ball_dy = 0.002;
            } else if (gameState == ENDED) {
                exit(0);
            } else {
                gameState = STOPPED;
                instructionText = "Press SPACE to resume the game";
                ball_dx = 0.0;
                ball_dy = 0.0;
            }
            break;
        case 'w':
            ball_dy = 0.002;
            break;
        case 's':
            ball_dy = -0.002;
            break;
        case 'a':
            ball_dx = -0.002;
            break;
        case 'd':
            ball_dx = 0.002;
            break;
        case 27:
            exit(0);
            break;
        default:
            ball_dx = 0.0;
            ball_dy = 0.0;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("BOUNCY BALL FOOTBALL");

    glClearColor(0.0, 0.5, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.2, 1.2, -1.0, 1.0);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}

