
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "coordinates.h"
#include <math.h>
#include <time.h>


typedef struct Player Player;
typedef struct BodyPart BodyPart;

struct Player {
	Coordinate2 position;
	float direction;
};

struct BodyPart {
	Coordinate2 position;
	float length;
	float direction;
};

enum Color {
	Black = 0b000,
	Blue = 0b001,
	Green = 0b010,
	Red = 0b100,
	Cyan = 0b011,
	Magenta = 0b101,
	Yellow = 0b110,
	White = 0b111,
};

enum Color colors[] = { Red, Green, Blue, Cyan, Magenta, Yellow };

char game_over = 0;
char key_press = 0;
Player player;
GLuint framebuffer;
GLuint renderbuffer;
BodyPart* bodies[1000] = { NULL };
Coordinate2 apple;
Coordinate2 mouse_position;



void draw_line(Coordinate2 *coords, char color) {
	glBegin(GL_LINE);

	glColor3f((color & 0b00000100), (color & 0b00000010), (color & 0b00000001));
	for (int i = 0; i < 2; i++) {
		glVertex2f(coords[i].x, coords[i].y);
	}

	glEnd();
	glFlush();

}

void draw_triangle(Coordinate2 *coords, char color) {
	glBegin(GL_TRIANGLES);
	
	glColor3f((color & 0b00000100), (color & 0b00000010), (color & 0b00000001));
	for (int i = 0; i < 3; i++) {
		glVertex2f(coords[i].x, coords[i].y);
	}

	glEnd();
	glFlush();
}

void draw_cube(Coordinate2 coords, float size, float orientation, char color) {
	size = size / 2;
	float distance = size * sqrt(2);
	Coordinate2 coord_1 = find_coord_rad(coords, add_rad(orientation, 0.125 * 2.0 * PI), distance);
	Coordinate2 coord_2 = find_coord_rad(coords, add_rad(orientation, 0.875 * 2.0 * PI), distance);
	Coordinate2 coord_3 = find_coord_rad(coords, add_rad(orientation, 0.625 * 2.0 * PI), distance);
	Coordinate2 coord_4 = find_coord_rad(coords, add_rad(orientation, 0.375 * 2.0 * PI), distance);


	glBegin(GL_QUADS);

	glColor3f((color & 0b00000100), (color & 0b00000010), (color & 0b00000001));
	glVertex2f(coord_1.x, coord_1.y);
	glVertex2f(coord_2.x, coord_2.y);
	glVertex2f(coord_3.x, coord_3.y);
	glVertex2f(coord_4.x, coord_4.y);

	glEnd();
	glFlush();
}

void draw_end() {
	glColor3f(1.0, 1.0, 1.0);
	int x_position = 100;
	int y_position = 100;
	
	const char *text = "Game Over.";
	glRasterPos2i(x_position, y_position);
	for (int i = 0; i < strlen(text); i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}

	glEnd();
	glFlush();
}

BodyPart* extend_body(Coordinate2 new_position, float new_length, float new_direction) {
	BodyPart* body_part = malloc(sizeof(BodyPart));

	body_part -> position = new_position;
	body_part -> length = new_length;
	body_part -> direction = new_direction;
	
	for (int i = 0; ; i++) {
		if (bodies[i] == NULL) {
			bodies[i] = body_part;
			break;
		}
	}

	return body_part;
}

void mouse(int x, int y) {
	mouse_position = coord2(((float) x - 500.0) / 500.0, ((-1.0 * ((float) y - 500.0))) / 500.0);
	player.direction = find_rad_coord(player.position, mouse_position);
}


void update() {

	if (game_over) {
		glutPostRedisplay();
		return;
	}

	if (distance(apple, player.position) < 0.05 || distance(apple, player.position) < 0.05) {
		apple = coord2(((float) rand() / (float) RAND_MAX) * 2.0 - 1.0 , (float) rand() / (float) RAND_MAX * 2.0 - 1.0);
		for (int i = 0; ; i++) {
			if (bodies[i] == NULL) {
				extend_body(bodies[i - 1] -> position, 0.05, 0.0);
				break;
			}
		}
	}
	if (player.position.x > 1.0 || player.position.x < -1.0) {
		player.position.x *= -1.0;
	}
	if (player.position.y > 1.0 || player.position.y < -1.0) {
		player.position.y *= -1.0;
	}

	if (key_press != 0) {
		key_press = 0;
	}

	// printf("%f\n", player.position.x);
	// printf("%f\n", player.position.y);


	move_coord_rad(&(player.position), player.direction, 0.005);

	for (int i = 0; bodies[i] != 0; i++) {
		if (bodies[i] == NULL) {
			break;
		}
		if (distance(bodies[i] -> position, player.position) < 0.025) {
			game_over = 1;
		}

		if (i == 0) {
			if (distance(bodies[i] -> position, player.position) > bodies[i] -> length) {
				move_coord_rad(&(bodies[i] -> position), find_rad_coord(bodies[i] -> position, player.position),
		   				distance(bodies[i] -> position, player.position) - bodies[i] -> length);
		   		bodies[i] -> direction = find_rad_coord(bodies[i] -> position, player.position);
			}
			continue;
		}

		if (distance(bodies[i] -> position, bodies[i - 1] -> position) > bodies[i] -> length) {
			bodies[i] -> direction = find_rad_coord(bodies[i] -> position, bodies[i - 1] -> position);

			move_coord_rad(&(bodies[i] -> position), find_rad_coord(bodies[i] -> position, bodies[i - 1] -> position),
		  			distance(bodies[i] -> position, bodies[i - 1] -> position) - bodies[i] -> length);
		}
	}


	// printf("%f\n", player.direction);

	glutPostRedisplay();
	glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			player.direction = add_rad(player.direction, 0.05 * 2.0 * PI);
			key_press = 'd';
			break;
		case 'd':
			player.direction = subtract_rad(player.direction, 0.05 * 2 * PI);
			key_press = 'a';
			break;
		case 'w':
			move_coord_rad(&(player.position), player.direction, 0.05);
			break;
		case 27:
			exit(0);

	}
}

void display_function() {

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClear(GL_COLOR_BUFFER_BIT);

	if (game_over) {
		draw_end();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, 1000, 1000, 0, 0, 1000, 1000, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glutSwapBuffers();
		return;

	}
	draw_cube(apple, 0.05, 0.0, Red);
	draw_cube(apple, 0.0125, 0.0, Green);

	Coordinate2 head_triangle[] = { find_coord_rad(player.position, player.direction + (0.75 - 0.125 - (0.125 / 3)) * 2 * PI, 0.05),
		player.position,
		find_coord_rad(player.position, player.direction + (0.25 + 0.125 + (0.125 / 3)) * 2 * PI, 0.05) };
	draw_triangle(head_triangle, Blue);
	
	for (int i = 0; bodies[i] != NULL; i++) {
		draw_cube(bodies[i] -> position, 0.05, bodies[i] -> direction, colors[i % 6]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, 1000, 1000, 0, 0, 1000, 1000, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glutSwapBuffers();


}


void setup_framebuffer() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 1000, 1000);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main(int argc, char **argv) {
	srand(time(NULL));

	player.position = coord2(0.0, 0.0);
	player.direction = 0;
	extend_body(coord2(-0.1, 0.0), 0.1, 0.0);
	apple = coord2(((float) rand() / (float) RAND_MAX) * 2.0 - 1.0 , (float) rand() / (float) RAND_MAX * 2.0 - 1.0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("OpenGL Application");
	setup_framebuffer();
	glutDisplayFunc(display_function);
	glutTimerFunc(0, update, 0);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	return 0;

}
