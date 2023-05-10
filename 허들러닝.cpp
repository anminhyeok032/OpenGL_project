#define _CRT_SECURE_NO_WARNINGS
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>
#include <vector>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#define M_PI 3.14159265358979323846
#define CLIENT_WIDTH 1280
#define CLIENT_HIEGHT 800
#define DETECH_RANGE 2.0
#define MOVING_SPEED 0.01


#define NPC_SPEED2 0.015//난이도1
#define NPC_SPEED3 0.018//난이도2
#define NPC_SPEED4 0.021//난이도3
#define KNOCK_BACK 0.007

int pick;

bool SPEED1 = false;//초보난이도
bool SPEED2 = false;//중급난이도
bool SPEED3 = false;//고급난이도
bool game_start_check = true;

typedef enum Event {
    LMOUSE_DOWN, LMOUSE_UP, MOUSE_MOVE,
    A_KEY_DOWN, S_KEY_DOWN, Z_KEY_DOWN, X_KEY_DOWN,
}Event;
#define TIMERCLOCK 13

using namespace std;
using namespace glm;

GLuint ShaderID;
GLuint vao, vbo_position, vbo_color;
GLuint vaoEar, vbo_position_Ear, vbo_color_Ear;



float trap_color_buffer[] = {//장애물색깔
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,//윗면
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,//옆면
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,



};

float trap_color_buffer2[] = {//장애물색깔
   0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,

     1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

   0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,

0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,

    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,


  0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,



};
float cubeVertexPositionBuffer[] = {
   1.0f, -1.0f, 1.0f,         //4
   1.0f, 1.0f, 1.0f,         //0
   -1.0f, -1.0f, 1.0f,         //7

   -1.0f, -1.0f, 1.0f,         //7
   1.0f, 1.0f, 1.0f,         //0
   -1.0f, 1.0f, 1.0f,         //3

   1.0f, 1.0f, 1.0f,         //0
   1.0f, 1.0f, -1.0f,         //1
   -1.0f, 1.0f, -1.0f,         //2

   -1.0f, 1.0f, -1.0f,         //2
   -1.0f, 1.0f, 1.0f,         //3
   1.0f, 1.0f, 1.0f,         //0

   -1.0f, 1.0f, 1.0f,         //3
   -1.0f, 1.0f, -1.0f,         //2
   -1.0f, -1.0f, 1.0f,         //7

   -1.0f, 1.0f, -1.0f,         //2
   -1.0f, -1.0f, -1.0f,      //6
   -1.0f, -1.0f, 1.0f,         //7

   -1.0f, -1.0f, -1.0f,      //6
   -1.0f, 1.0f, -1.0f,         //2
   1.0f, 1.0f, -1.0f,         //1

   -1.0f, -1.0f, -1.0f,      //6
   1.0f, 1.0f, -1.0f,         //1
   1.0f, -1.0f, -1.0f,         //5

   1.0f, -1.0f, -1.0f,         //5
   1.0f, 1.0f, -1.0f,         //1
   1.0f, 1.0f, 1.0f,         //0

   1.0f, -1.0f, 1.0f,         //4
   1.0f, -1.0f, -1.0f,         //5
   1.0f, 1.0f, 1.0f,         //0

   -1.0f, -1.0f, 1.0f,         //7
   1.0f, -1.0f, -1.0f,         //5
   1.0f, -1.0f, 1.0f,         //4

   1.0f, -1.0f, -1.0f,         //5
   -1.0f, -1.0f, 1.0f,         //7
   -1.0f, -1.0f, -1.0f,      //6
};

float EarVertexPositionBuffer[] = {
    0.0f, 1.0f, 1.0f,         //1 앞면
    -1.0f, 0.0f, 1.0f,         //2
    1.0f, 0.0f, 1.0f,         //3

    0.0f, 1.0f, 1.0f,         //1
    0.0f, 0.0f, -1.0f,         //0
    1.0f, 0.0f, 1.0f,         //3

    0.0f, 1.0f, 1.0f,         //1
    0.0f, 0.0f, -1.0f,         //0
    -1.0f, 0.0f, 1.0f,         //3

};

float stage_color_buffer[] = {
   1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
   1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,

   1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
   1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,

 1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
   1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,

 1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
   1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,

 1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
   1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,

  1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
   1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,
    1.0f, 0.7f, 0.7f,



};
float robot_color_buffer[] =
{
    // 0,0,0.5,
  1.0,0.2,0.6,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
};
float robot_head_color_buffer1[] = {
    1.0,0.2,0.6,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,

  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
  1.0,0.2,0.5,
};

float robot_head_color_buffer2[] = {
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
   1,1,0,
};

float robot_head_color_buffer3[] = {
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
   1,0,1,
};

float robot_head_color_buffer4[] = {
   1,1,0,
   1,1,0,
  1,1,0,
   1,1,0,
   1,1,0,
  1,1,0,
   1,1,0,
   1,1,0,
  1,1,0,
   1,1,0,
   1,1,0,
  1,1,0,
  1,1,0,
   1,1,0,
  1,1,0,
   1,1,0,
   1,1,0,
  1,1,0,
   1,1,0,
   1,1,0,
  1,1,0,
   1,1,0,
   1,1,0,
  1,1,0,
  1,1,0,
   1,1,0,
  1,1,0,
   1,1,0,
   1,1,0,
  1,1,0,
   1,1,0,
   1,1,0,
  1,1,0,
   1,1,0,
   1,1,0,
  1,1,0,
};

float robot_head_color_buffer5[] = {
   1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
   1.0,0.0,0.0,
   1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
   1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
   1.0,0.0,0.0,
   1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
   1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
   1.0,0.0,0.0,
   1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
   1.0,0.0,0.0,
   1.0,0.0,0.0,
    1.0,0.0,0.0,
   1.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,0.0,0.0,
   1.0,0.0,0.0,
};

float robot_left_color_buffer[] = {
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,

   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,

   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,

   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,

   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,

   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
   0,0,0.5,
};
float robot_right_color_buffer[] = {

   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
   0.5,0,0,
};


//바닥
mat4 stage_transform = glm::mat4(1);
mat4 stage_translate;
mat4 stage_rotate;

//박스장애물
vec3 box_scale_rate;
vec3 box_scale_rate2;
vec3 box_scale_rate3;
mat4 box_rotate;

mat4 box_translate_p1;//주인공쪽 1번박스
mat4 box_transform_p1 = glm::mat4(1);

mat4 box_translate_p2;//주인공쪽 2번박스
mat4 box_transform_p2 = glm::mat4(1);

mat4 box_translate_p3;//주인공쪽 3번박스
mat4 box_transform_p3 = glm::mat4(1);

mat4 box_translate_p4;//주인공쪽 4번박스 -4,5 세트
mat4 box_transform_p4 = glm::mat4(1);

mat4 box_translate_p5;//주인공쪽 5번박스
mat4 box_transform_p5 = glm::mat4(1);

mat4 box_translate_p6;//주인공쪽 6번박스 -6,7,8 세트
mat4 box_transform_p6 = glm::mat4(1);

mat4 box_translate_p7;//주인공쪽 7번박스
mat4 box_transform_p7 = glm::mat4(1);

mat4 box_translate_p8;//주인공쪽 8번박스
mat4 box_transform_p8 = glm::mat4(1);

mat4 box_translate_p9;//주인공쪽 9번박스
mat4 box_transform_p9 = glm::mat4(1);

//여기까지 긴박스

//여기부터 높은박스
mat4 box_translate_p10;//주인공쪽 10번박스
mat4 box_transform_p10 = glm::mat4(1);

mat4 box_translate_p11;//주인공쪽 11번박스
mat4 box_transform_p11 = glm::mat4(1);

mat4 box_translate_p12;//주인공쪽 12번박스
mat4 box_transform_p12 = glm::mat4(1);

mat4 box_translate_p13;//주인공쪽 13번박스
mat4 box_transform_p13 = glm::mat4(1);

mat4 box_translate_p14;//주인공쪽 14번박스
mat4 box_transform_p14 = glm::mat4(1);

mat4 box_translate_p15;//주인공쪽 15번박스 15,16세트
mat4 box_transform_p15 = glm::mat4(1);

mat4 box_translate_p16;//주인공쪽 16번박스
mat4 box_transform_p16 = glm::mat4(1);

mat4 box_translate_p17;//주인공쪽 17번박스 17,18,19세트
mat4 box_transform_p17 = glm::mat4(1);

mat4 box_translate_p18;//주인공쪽 18번박스
mat4 box_transform_p18 = glm::mat4(1);

mat4 box_translate_p19;//주인공쪽 19번박스
mat4 box_transform_p19 = glm::mat4(1);

//여기부터 피라미드 박스

mat4 box_translate_p20;//주인공쪽 20번박스
mat4 box_transform_p20 = glm::mat4(1);

mat4 box_translate_p21;//주인공쪽 21번박스
mat4 box_transform_p21 = glm::mat4(1);

mat4 box_translate_p22;//주인공쪽 22번박스
mat4 box_transform_p22 = glm::mat4(1);
float box_jump_p22 = -0.75;


mat4 box_translate_p23;//주인공쪽 23번박스
mat4 box_transform_p23 = glm::mat4(1);
float box_jump_p23 = -0.75;

mat4 box_translate_p24;//주인공쪽 24번박스
mat4 box_transform_p24 = glm::mat4(1);

mat4 box_translate_p25;//주인공쪽 25번박스
mat4 box_transform_p25 = glm::mat4(1);

mat4 box_translate_p26;//주인공쪽 26번박스
mat4 box_transform_p26 = glm::mat4(1);

mat4 box_translate_p27;//주인공쪽 27번박스
mat4 box_transform_p27 = glm::mat4(1);

mat4 box_translate_p28;//주인공쪽 28번박스
mat4 box_transform_p28 = glm::mat4(1);

mat4 box_translate_p29;//주인공쪽 29번박스
mat4 box_transform_p29 = glm::mat4(1);


mat4 head_transform_1, head_transform_2, head_transform_3, head_transform_4, head_transform_5;
mat4 nose_transform_2;
mat4 Ear_transform_right, Ear_transform_left;

mat4 robot_transform_body;

mat4 robot_transform_1, robot_transform_2, robot_transform_3, robot_transform_4, robot_transform_5;

mat4 left_arm_transform_1, left_arm_transform_2, left_arm_transform_3, left_arm_transform_4, left_arm_transform_5;
mat4 right_arm_transform_1, right_arm_transform_2, right_arm_transform_3, right_arm_transform_4, right_arm_transform_5;
mat4 left_leg_transform_1, left_leg_transform_2, left_leg_transform_3, left_leg_transform_4, left_leg_transform_5;
mat4 right_leg_transform_1, right_leg_transform_2, right_leg_transform_3, right_leg_transform_4, right_leg_transform_5;

vec3 stage_scale_rate;

vec3 robot_scale_rate_1, robot_scale_rate_2, robot_scale_rate_3, robot_scale_rate_4, robot_scale_rate_5;
vec3 robot_pos_1, robot_pos_2, robot_pos_3, robot_pos_4, robot_pos_5;
float robot_angle_1, robot_angle_2, robot_angle_3, robot_angle_4, robot_angle_5;

vec3 arm_pos_1, arm_pos_2, arm_pos_3, arm_pos_4, arm_pos_5;
vec3 arm_scale_rate_1, arm_scale_rate_2, arm_scale_rate_3, arm_scale_rate_4, arm_scale_rate_5;

float arm_rotate_angle_1, arm_rotate_angle_2, arm_rotate_angle_3, arm_rotate_angle_4, arm_rotate_angle_5;
vec3 left_arm_pibot_1, left_arm_pibot_2, left_arm_pibot_3, left_arm_pibot_4, left_arm_pibot_5;
vec3 right_arm_pibot_1, right_arm_pibot_2, right_arm_pibot_3, right_arm_pibot_4, right_arm_pibot_5;

float leg_rotate_angle_1, leg_rotate_angle_2, leg_rotate_angle_3, leg_rotate_angle_4, leg_rotate_angle_5;
vec3 left_leg_pibot_1, left_leg_pibot_2, left_leg_pibot_3, left_leg_pibot_4, left_leg_pibot_5;
vec3 right_leg_pibot_1, right_leg_pibot_2, right_leg_pibot_3, right_leg_pibot_4, right_leg_pibot_5;

float rotate_rate = 0;
float rotate_rate_2 = 0;

glm::vec3 cameraPos;
glm::vec3 playerPos;//주인공 카메라
glm::vec3 cameraDirection;
float camera_angle;
glm::vec3 cameraTarget;
glm::vec3 cameraUp;
glm::mat4 view;
glm::mat4 projection;


glm::mat4 camera_rotate = mat4(1);

GLuint modelLocation;
GLuint viewLocation;
GLuint projectionLocation;

void init_buffers();
void set_matrix();



bool move_w = false;
bool move_a = false;
bool move_s = true;
bool move_d = false;

bool move_w_2 = true;
bool move_a_2 = false;
bool move_s_2 = false;
bool move_d_2 = false;

bool move_w_3 = false;
bool move_a_3 = false;
bool move_s_3 = true;
bool move_d_3 = false;

bool move_w_4 = false;
bool move_a_4 = false;
bool move_s_4 = false;
bool move_d_4 = true;

bool move_w_5 = false;
bool move_a_5 = true;
bool move_s_5 = false;
bool move_d_5 = false;

//키보드설정
bool jumping = false;//점프
float jump;

bool highjumping = false;//하이점프
float highjump;

bool backstep = false;//백스텝
float back;


bool speedwalk = false;//순보
float speed;

bool bodydown = false;//엎드리기
float down;

bool start = false;//주인공출발
bool startnpc = false;//npc출발
bool finish = false;

//

//카메라 시점 변경용
int counts = 0;
int counts2 = 0;
bool countsStatus = false;
bool countsStatus2 = false;
//

GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLvoid Timer(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid key_up(unsigned char key, int x, int y);



bool cameraangle = false;
//bool camerarotate = false;
bool camerarotate = true;//쿼터뷰
bool playerview = false;//1인칭
bool movecamera = false;//카메라가 주인공 따라가기
bool music = false;
bool music_check = true;


char* get_filedata_to_buf(const char* filename) {
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(filename, "rb");
    if (!fptr) return nullptr;

    fseek(fptr, 0, SEEK_END);   //파일의 끝까지 찾는다
    length = ftell(fptr);      // 파일 포인터의 현재 위치 가져옴
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, sizeof(char), length, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}

bool init_shader_program(GLuint* id, const char* filename_vert, const char* filename_frag) {
    using namespace std;

    GLint result;
    GLchar errorLog[512];

    const GLchar* vertexShaderSource = get_filedata_to_buf(filename_vert);
    if (vertexShaderSource == nullptr) {
        cout << "VertexShaderSource 읽기 실패" << endl;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        cerr << "ERROR: vertex shader 컴파일실패\n" << errorLog << endl;
        return false;
    }

    const GLchar* fragmentShaderSource = get_filedata_to_buf(filename_frag);
    if (fragmentShaderSource == nullptr) {
        cout << "FragmentShaderSource 읽기 실패" << endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        cerr << "ERROR: fragment shader 컴파일실패\n" << errorLog << endl;
        return false;
    }

    *id = glCreateProgram();

    glAttachShader(*id, vertexShader);
    glAttachShader(*id, fragmentShader);
    glLinkProgram(*id);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(*id, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(*id, 512, NULL, errorLog);
        cerr << "ERROR: shader program 연결실패\n" << errorLog << endl;
        return false;
    }
    return true;
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(400, 150);
    glutInitWindowSize(CLIENT_WIDTH, CLIENT_HIEGHT);
    glutCreateWindow("허들러닝");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    cout << " 결승선을 향해 달려가고있는 NPC를 상대로 장애물을 피해서 60미터 지점에있는 결승선에 먼저 도착하라!           " << endl << endl << endl;
    

    
    cout << "****************************************************" << endl;
    cout << "*  d - 앞으로 대쉬                                 *" << endl;
    cout << "*  a - 뒤로 대쉬(백스텝)                           *" << endl;
    cout << "*  w - 기본 점프                                   *" << endl;
    cout << "*  j - 슈퍼 점프                                   *" << endl;
    cout << "*  주인공 하이점프 10번 장애물 이전에 사용불가     *" << endl;
    cout << "*  r - 현재 캐릭터 기준 y축 회전                   *" << endl;
    cout << "*  1 - 기본 쿼터뷰 카메라 위치로 변환              *" << endl;
    cout << "*  2 - 1인칭 시점으로 변환                         *" << endl;
    cout << "*  3 - 3인칭 시점으로 변환                         *" << endl;
    cout << "*  m(기본 점프) - 게임시작(시작시 난이도 선택)     *" << endl;
    cout << "*  q - 게임 나가기                                 *" << endl;
    cout << "****************************************************" << endl << endl;

    cout << "-점프 도중 대쉬를 사용할 수 있으며 슈퍼 점프와 기본 점프를 동시에 사용할 수 있다.               " << endl<<endl;
    cout << "-중간에 그냥 통과는 되지만 점프시 충돌이되는 함정장애물이 숨겨져있다.                           " << endl;


    init_shader_program(&ShaderID, "vertex.glsl", "fragment.glsl");
    init_buffers();

    glUseProgram(ShaderID);

    set_matrix();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);

    glutTimerFunc(TIMERCLOCK, Timer, 0);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(key_up);

    music = PlaySound(TEXT("background.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
    if (!music) {
        cout << "음악 재생 오류" << endl;
    }

    glutMainLoop();
}


void init_buffers()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo_position);
    glGenBuffers(1, &vbo_color);


    glBindVertexArray(vao);

    GLint pAttribute;
    GLint cAttribute;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositionBuffer), cubeVertexPositionBuffer, GL_STATIC_DRAW);
    pAttribute = glGetAttribLocation(ShaderID, "vPos");
    glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(pAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    cAttribute = glGetAttribLocation(ShaderID, "vColor");
    glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(cAttribute);


    glGenVertexArrays(1, &vaoEar);
    glGenBuffers(1, &vbo_position_Ear);
    glGenBuffers(1, &vbo_color_Ear);

    glBindVertexArray(vaoEar);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_Ear);
    glBufferData(GL_ARRAY_BUFFER, sizeof(EarVertexPositionBuffer), EarVertexPositionBuffer, GL_STATIC_DRAW);
    pAttribute = glGetAttribLocation(ShaderID, "vPos");
    glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(pAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_Ear);
    cAttribute = glGetAttribLocation(ShaderID, "vColor");
    glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(cAttribute);


}



void set_matrix()
{
    using namespace glm;

    modelLocation = glGetUniformLocation(ShaderID, "modelTransform");
    viewLocation = glGetUniformLocation(ShaderID, "viewTransform");
    projectionLocation = glGetUniformLocation(ShaderID, "projectionTransform");


    //바닥
    {
        stage_rotate = mat4(1);
        stage_scale_rate = vec3(2.0, 1.0, 70.0);
        stage_translate = translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), stage_scale_rate);
        stage_transform = stage_translate * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

    }

    //박스 장애물
    {
        //1번박스
        box_rotate = mat4(1);


        box_scale_rate = vec3(0.2, 0.1, 0.05);//박스크기
        box_translate_p1 = translate(mat4(1), vec3(0.8, -0.95, 68)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p1 = box_translate_p1 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //2번박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.2, 0.05);//박스크기
        box_translate_p2 = translate(mat4(1), vec3(0.8, -0.95, 65)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p2 = box_translate_p2 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //3번박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.3, 0.05);//박스크기
        box_translate_p3 = translate(mat4(1), vec3(0.8, -0.95, 62)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p3 = box_translate_p3 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //4번박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.2, 0.05);//박스크기
        box_translate_p4 = translate(mat4(1), vec3(0.8, -0.95, 59)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p4 = box_translate_p4 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //5번박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.2, 0.05);//박스크기
        box_translate_p5 = translate(mat4(1), vec3(0.8, -0.95, 58.4)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p5 = box_translate_p5 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //6번박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.23, 0.05);//박스크기
        box_translate_p6 = translate(mat4(1), vec3(0.8, -0.95, 55.5)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p6 = box_translate_p6 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //7번박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.23, 0.05);//박스크기
        box_translate_p7 = translate(mat4(1), vec3(0.8, -0.95, 55.1)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p7 = box_translate_p7 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //8번박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.23, 0.05);//박스크기
        box_translate_p8 = translate(mat4(1), vec3(0.8, -0.95, 54.6)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p8 = box_translate_p8 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //9번박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.20, 0.47);//박스크기
        box_translate_p9 = translate(mat4(1), vec3(0.8, -0.95, 51.6)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p9 = box_translate_p9 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //10번박스-여기부터 높은박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.5, 0.1);//박스크기
        box_translate_p10 = translate(mat4(1), vec3(0.8, -0.95, 48.6)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p10 = box_translate_p10 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //11번박스-여기부터 높은박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.7, 0.1);//박스크기
        box_translate_p11 = translate(mat4(1), vec3(0.8, -0.95, 45.6)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p11 = box_translate_p11 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //12번박스-여기부터 높은박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.8, 0.1);//박스크기
        box_translate_p12 = translate(mat4(1), vec3(0.8, -0.95, 42.6)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p12 = box_translate_p12 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //13번박스-여기부터 높은박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 1.0, 0.1);//박스크기
        box_translate_p13 = translate(mat4(1), vec3(0.8, -0.75, 39.6)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p13 = box_translate_p13 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //14번박스-여기부터 높은박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.7, 0.1);//박스크기
        box_translate_p14 = translate(mat4(1), vec3(0.8, -0.75, 36.6)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p14 = box_translate_p14 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //15번박스-여기부터 높은박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.7, 0.1);//박스크기
        box_translate_p15 = translate(mat4(1), vec3(0.8, -0.75, 36.1)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p15 = box_translate_p15 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //16번박스-여기부터 높은박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.7, 0.1);//박스크기
        box_translate_p16 = translate(mat4(1), vec3(0.8, -0.75, 33.0)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p16 = box_translate_p16 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //17번박스-여기부터 높은박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.7, 0.1);//박스크기
        box_translate_p17 = translate(mat4(1), vec3(0.8, -0.75, 32.75)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p17 = box_translate_p17 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //18번박스-여기부터 높은박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.7, 0.1);//박스크기
        box_translate_p18 = translate(mat4(1), vec3(0.8, -0.75, 32.35)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p18 = box_translate_p18 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //19번박스-여기부터 피라미드박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.3, 0.25);//박스크기
        box_translate_p19 = translate(mat4(1), vec3(0.8, -0.75, 29.4)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p19 = box_translate_p19 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //20번박스-여기부터  피라미드박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.8, 0.25);//박스크기
        box_translate_p20 = translate(mat4(1), vec3(0.8, -0.75, 28.9)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p20 = box_translate_p20 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //21번박스-여기부터  피라미드박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.3, 0.25);//박스크기
        box_translate_p21 = translate(mat4(1), vec3(0.8, -0.75, 28.4)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p21 = box_translate_p21 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //22번박스-점프하는 박스
        box_rotate = mat4(1);
        box_scale_rate3 = vec3(0.2, 0.6, 0.1);//박스크기
        box_translate_p22 = translate(mat4(1), vec3(0.8, box_jump_p22, 26)) * scale(mat4(1), box_scale_rate3);//박스위치
        box_transform_p22 = box_translate_p22 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //23번박스-점프하는 박스
        box_rotate = mat4(1);
        box_scale_rate3 = vec3(0.2, 0.6, 0.1);//박스크기
        box_translate_p23 = translate(mat4(1), vec3(0.8, box_jump_p23, 25)) * scale(mat4(1), box_scale_rate3);//박스위치
        box_transform_p23 = box_translate_p23 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //24번박스-여기부터  피라미드박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.5, 0.05);//박스크기
        box_translate_p24 = translate(mat4(1), vec3(0.8, 0.4, 21)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p24 = box_translate_p24 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //25번박스-여기부터  피라미드박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.25, 0.05);//박스크기
        box_translate_p25 = translate(mat4(1), vec3(0.8, -0.75, 21)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p25 = box_translate_p25 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //26번박스-여기부터  피라미드박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.3, 0.05);//박스크기
        box_translate_p26 = translate(mat4(1), vec3(0.8, 1.4, 17)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p26 = box_translate_p26 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //27번박스-여기부터  피라미드박스
        box_rotate = mat4(1);
        box_scale_rate = vec3(0.2, 0.9, 0.05);//박스크기
        box_translate_p27 = translate(mat4(1), vec3(0.8, -0.5, 17)) * scale(mat4(1), box_scale_rate);//박스위치
        box_transform_p27 = box_translate_p27 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

        //28번박스-도착선
        box_rotate = mat4(1);
        box_scale_rate2 = vec3(2.0, 0.1, 0.2);//박스크기
        box_translate_p28 = translate(mat4(1), vec3(0.2, -0.75, 10)) * scale(mat4(1), box_scale_rate2);//박스위치
        box_transform_p28 = box_translate_p28 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));




    }


    // 주인공
    {
        robot_scale_rate_1 = vec3(0.1);
        robot_pos_1 = vec3(0.8, -0.8, 69.75);
        robot_angle_1 = 180;

        robot_transform_body = translate(mat4(1), robot_pos_1) * scale(mat4(1), vec3(0.1, 0.05, 0.05));


        robot_transform_1 = translate(mat4(1), robot_pos_1) * scale(mat4(1), robot_scale_rate_1);
        head_transform_1 = robot_transform_1 * translate(mat4(1), vec3(1.5, 2.0, 0)) * scale(mat4(1), vec3(0.6, 0.6, 0.6));
        Ear_transform_left = head_transform_1 * translate(mat4(1), vec3(0.5, 1, 0)) * scale(mat4(1), vec3(0.2, 0.2, 1));
        Ear_transform_right = head_transform_1 * translate(mat4(1), vec3(-0.5, 1, 0)) * scale(mat4(1), vec3(0.2, 0.2, 1));

        arm_pos_1 = vec3(1.2, 0, 0);
        arm_scale_rate_1 = vec3(0.3, 1.0, 0.3);

        arm_rotate_angle_1 = 180;

        left_arm_pibot_1 = arm_pos_1 + vec3(0, 1, 0);
        left_arm_transform_1 = robot_transform_1 * translate(mat4(1), -arm_pos_1) * scale(mat4(1), arm_scale_rate_1);

        right_arm_pibot_1 = -arm_pos_1 + vec3(0, 1, 0);
        right_arm_transform_1 = robot_transform_1 * translate(mat4(1), arm_pos_1) * scale(mat4(1), arm_scale_rate_1);

        leg_rotate_angle_1 = 180;
        left_leg_pibot_1 = vec3(0, 1, 0);
        right_leg_pibot_1 = vec3(0, 1, 0);

        left_leg_transform_1 = robot_transform_1 * translate(mat4(1), vec3(-0.5, -1.5, 0)) * scale(mat4(1), vec3(0.3, 0.5, 0.3));
        right_leg_transform_1 = robot_transform_1 * translate(mat4(1), vec3(0.5, -1.5, 0)) * scale(mat4(1), vec3(0.3, 0.5, 0.3));
    }

    //NPC
    {
        robot_scale_rate_2 = vec3(0.1);
        robot_pos_2 = vec3(-0.8, -0.8, 69.75);
        robot_angle_2 = 180;

        robot_transform_2 = translate(mat4(1), robot_pos_2) * scale(mat4(1), robot_scale_rate_2);
        head_transform_2 = robot_transform_2 * translate(mat4(1), vec3(0, 1.5, 0)) * scale(mat4(1), vec3(0.5, 0.5, 0.5));
        nose_transform_2 = head_transform_2 * translate(mat4(1), vec3(0, 0, 1)) * scale(mat4(1), vec3(0.2, 0.2, 1));

        arm_pos_2 = vec3(1.2, 0, 0);
        arm_scale_rate_2 = vec3(0.3, 1, 0.3);

        arm_rotate_angle_2 = 180;

        left_arm_pibot_2 = arm_pos_2 + vec3(0, 1, 0);
        left_arm_transform_2 = robot_transform_2 * translate(mat4(1), -arm_pos_2) * scale(mat4(1), arm_scale_rate_2);

        right_arm_pibot_2 = -arm_pos_2 + vec3(0, 1, 0);
        right_arm_transform_2 = robot_transform_2 * translate(mat4(1), arm_pos_2) * scale(mat4(1), arm_scale_rate_2);

        leg_rotate_angle_2 = 180;
        left_leg_pibot_2 = vec3(0, 1, 0);
        right_leg_pibot_2 = vec3(0, 1, 0);

        left_leg_transform_2 = robot_transform_2 * translate(mat4(1), vec3(-0.5, -1.5, 0)) * scale(mat4(1), vec3(0.3, 0.5, 0.3));
        right_leg_transform_2 = robot_transform_2 * translate(mat4(1), vec3(0.5, -1.5, 0)) * scale(mat4(1), vec3(0.3, 0.5, 0.3));
    }


    //엔딩
    {
        robot_scale_rate_4 = vec3(0.1);
        robot_pos_4 = vec3(0.8, -0.8, 69.75);
        robot_angle_4 = 180;


    }

    cameraPos = glm::vec3(5.0, 2.0, 3.4);//바닥크기
    //stage_rotate += radians(360.0f);


    camera_angle = radians(-90.f);

    cameraTarget = glm::vec3(0, 0, 0);
    //cameraDirection = cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle));
    cameraDirection = robot_pos_1 + vec3(cos(robot_angle_1), 0, sin(robot_angle_1));
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    view = glm::lookAt(cameraPos + vec3(robot_pos_1.x, 0, robot_pos_1.z), cameraDirection + vec3(robot_pos_1.x, 0, robot_pos_1.z), cameraUp);
    projection = glm::perspective(glm::radians(45.0f), (float)CLIENT_WIDTH / (float)CLIENT_HIEGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

}

GLvoid drawScene()
{


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    // 귀
    {
        glBindVertexArray(vaoEar);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color_Ear);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_color_buffer), robot_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Ear_transform_left));
        glDrawArrays(GL_TRIANGLES, 0, 9);
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Ear_transform_right));
        glDrawArrays(GL_TRIANGLES, 0, 9);

    }

    //바닥
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(stage_color_buffer), stage_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(stage_transform));
        glDrawArrays(GL_TRIANGLES, 30, 6);
        //glDisable(GL_DEPTH_TEST);
        glEnable(GL_DEPTH_TEST);
    }
    //박스장애물
    {
        //주인공1번박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p1));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공2번박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p2));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공3번박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p3));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공4번박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p4));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공5번박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p5));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공6번박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p6));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공7번박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p7));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공8번박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p8));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공9번박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p9));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공10번박스 -여기부터 높은박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p10));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공11번박스 -여기부터 높은박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p11));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공12번박스 -여기부터 높은박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p12));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공13번박스 -여기부터 높은박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p13));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공14번박스 -여기부터 높은박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p14));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공15번박스 -여기부터 높은박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p15));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공16번박스 -여기부터 높은박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p16));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공17번박스 -여기부터 높은박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p17));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공18번박스 -여기부터 높은박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p18));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공19번박스 -여기부터 피라미드박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p19));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공20번박스 -여기부터 피라미드박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p20));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공21번박스 -여기부터 피라미드박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer), trap_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p21));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공22번박스 -점프하는 박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer2), trap_color_buffer2, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p22));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공23번박스 -점프하는 박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer2), trap_color_buffer2, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p23));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공24번박스 -점프하는 박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer2), trap_color_buffer2, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p24));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공25번박스 -점프하는 박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer2), trap_color_buffer2, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p25));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공26번박스 -점프하는 박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer2), trap_color_buffer2, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p26));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //주인공27번박스 -점프하는 박스
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(trap_color_buffer2), trap_color_buffer2, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p27));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);

        //도착선
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_head_color_buffer5), robot_head_color_buffer5, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(box_transform_p28));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glEnable(GL_DEPTH_TEST);


    }

    //주인공
    {
        //주인공
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_color_buffer), robot_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robot_transform_body));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_head_color_buffer1), robot_head_color_buffer1, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(head_transform_1));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 주인공 왼쪽
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_right_color_buffer), robot_right_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(left_arm_transform_1));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(left_leg_transform_1));
        glDrawArrays(GL_TRIANGLES, 0, 36);



        // 주인공 오른쪽
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_right_color_buffer), robot_right_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(right_arm_transform_1));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(right_leg_transform_1));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //NPC
    {
        //로봇 몸통
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_head_color_buffer1), robot_head_color_buffer1, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robot_transform_2));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(nose_transform_2));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_head_color_buffer1), robot_head_color_buffer1, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(head_transform_2));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 로봇 왼쪽
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_right_color_buffer), robot_right_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(left_arm_transform_2));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(left_leg_transform_2));
        glDrawArrays(GL_TRIANGLES, 0, 36);



        // 로봇 오른쪽
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(robot_right_color_buffer), robot_right_color_buffer, GL_STATIC_DRAW);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(right_arm_transform_2));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(right_leg_transform_2));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //NPC 현재 위치
    if (robot_pos_2.z < 60 && robot_pos_2.z>59.98)
    {
        cout << endl;
        cout << "10미터 돌파";
    }

    if (robot_pos_2.z < 50 && robot_pos_2.z>49.98)
    {
        cout << endl;
        cout << "20미터 돌파";
    }

    if (robot_pos_2.z < 40 && robot_pos_2.z>39.98)
    {
        cout << endl;
        cout << "30미터 돌파";
    }

    if (robot_pos_2.z < 30 && robot_pos_2.z>29.98)
    {
        cout << endl;
        cout << "40미터 돌파";
    }

    if (robot_pos_2.z < 20 && robot_pos_2.z>19.98)
    {
        cout << endl;
        cout << "50미터 돌파";
    }

    if (robot_pos_2.z < 15 && robot_pos_2.z>14.98)
    {
        cout << endl;
        cout << "NPC가 거의 도착했습니다 서두르세요!";
        cout << endl;
    }

    glutSwapBuffers();
}


GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

GLvoid Timer(int value)
{
    {


        //점프
        if (jumping)
        {
            robot_pos_1.y += cos(radians(jump)) / 40.0;
            robot_pos_1.z -= cos(radians(speed)) / 120.0;
            jump += 2.5;

            if (jump > 180) {
                jumping = false;
                startnpc = true;
                start = true;
                arm_rotate_angle_1 = 180;
                leg_rotate_angle_1 = 180;
                jump = 0;
            }
        }
        //하이점프
        if (highjumping)
        {

            robot_pos_1.y += cos(radians(highjump)) / 15.0;
            robot_pos_1.z -= cos(radians(speed)) / 100.0;
            highjump += 2.5;
            if (highjump > 180)
            {
                highjumping = false;
                startnpc = true;
                start = true;
                arm_rotate_angle_1 = 180;
                leg_rotate_angle_1 = 180;
                highjump = 0;
            }
        }
        //순보
        if (speedwalk)
        {
            robot_pos_1.z -= cos(radians(speed)) / 30.0;
            speed += 3;

            if (speed > 90) {
                speedwalk = false;
                speed = 0;
            }
        }
        //백스텝
        if (backstep) {
            robot_pos_1.z += cos(radians(back)) / 12.0;
            back += 3;

            if (robot_pos_1.z > 70)
            {
                robot_pos_1.z = 69.7;
            }

            if (back > 90) {
                backstep = false;
                back = 0;
            }

        }
        //움직이는 장애물


        //if (robot_pos_3.z > -60.0) //npc가 승리시
        //{
        //    robot_pos_3.z -= MOVING_SPEED;
        //    if (robot_pos_1.z == robot_pos_3.z + MOVING_SPEED)
        //    {
        //        cout << "안녕";
        //    }

        //}

        //robot_transform_3 = translate(mat4(1), robot_pos_3) * rotate(mat4(1), radians(robot_angle_1), vec3(0, 1, 0)) * scale(mat4(1), robot_scale_rate_3);



  
        //주인공 움직임 및 충돌처리
        if (start == true)
        {
            if (move_w)
            {
                rotate_rate += 10;
                arm_rotate_angle_1 += cos(radians(rotate_rate)) * 5;
                leg_rotate_angle_1 += cos(radians(rotate_rate)) * 5;

                if (robot_pos_1.z > -70.0) {
                    robot_pos_1.z -= MOVING_SPEED;
                  

                    if (robot_pos_1.z < 15) {
                        cameraPos = glm::vec3(0, 0.5, -7);
                        camera_angle = radians(0.f);
                        cameraTarget = glm::vec3(0, 0, 0);
                        cameraDirection = cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle));
                        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
                    }

                    if (robot_pos_1.z < 10.2)//승리
                    {


                        if (music_check) {
                            music = PlaySound(TEXT("victory.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
                            if (!music) {
                                cout << "음악 재생 오류" << endl;
                            }
                            music_check = false;
                        }

                        cout << "승리";
                        if (robot_pos_1.z < 8) {
                            start = false;
                            finish = true;
                        }
                    }
                    move_s = true;
                }
                else {
                    robot_angle_1 = 0;
                    move_s = true;
                    move_w = false;
                }

                // 장애물 충돌 판정 모음
                {
                    //1번장애물 부딪히면 백스텝점프
                    if (robot_pos_1.z < 68.2 && robot_pos_1.z>67.8 && robot_pos_1.y < -0.77)
                    {
                        jumping = true;
                        backstep = true;
                        start = false;

                    }
                    //2번장애물 부딪히면 백스텝점프
                    if (robot_pos_1.z < 65.2 && robot_pos_1.z>64.6 && robot_pos_1.y < -0.57)
                    {
                        jumping = true;
                        backstep = true;
                        start = false;

                    }
                    //3번장애물 부딪히면 벡스텝점프
                    if (robot_pos_1.z < 62.2 && robot_pos_1.z>61.6 && robot_pos_1.y < -0.47)//12
                    {
                        jumping = true;
                        backstep = true;
                        start = false;

                    }
                    //4,5번장애물 부딪히면 벡스텝점프
                    if (robot_pos_1.z < 59.2 && robot_pos_1.z>58.6 && robot_pos_1.y < -0.57)//9
                    {
                        jumping = true;
                        backstep = true;
                        start = false;

                    }
                    //4,5번장애물 부딪히면 벡스텝점프 -2형 장애물
                    if (robot_pos_1.z < 59.2 && robot_pos_1.z>58.0 && robot_pos_1.y < -0.57)//8
                    {
                        jumping = true;
                        backstep = true;
                        start = false;

                    }
                    //6,7,8번장애물 부딪히면 벡스텝점프 -3형 장애물
                    if (robot_pos_1.z < 55.7 && robot_pos_1.z>54.25 && robot_pos_1.y < -0.57)//8
                    {
                        jumping = true;
                        backstep = true;
                        start = false;

                    }

                    //9번 긴 장애물
                    if (robot_pos_1.z < 52.5 && robot_pos_1.z>50.9 && robot_pos_1.y < -0.6)//8
                    {
                        jumping = true;
                        backstep = true;
                        start = false;

                    }

                    //10번 높은 장애물(높은점프)
                    if (robot_pos_1.z < 49.0 && robot_pos_1.z>48.2 && robot_pos_1.y < -0.4)//8
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //11번 높은 장애물(높은점프)
                    if (robot_pos_1.z < 46.0 && robot_pos_1.z>45.2 && robot_pos_1.y < -0.2)//8
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //12번 높은 장애물(높은점프)
                    if (robot_pos_1.z < 43.0 && robot_pos_1.z>42.2 && robot_pos_1.y < 0.1)//8
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //13번 높은 장애물(높은점프)
                    if (robot_pos_1.z < 40.0 && robot_pos_1.z>39.2 && robot_pos_1.y < 0.4)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //14번,15번 높은 장애물(높은점프)
                    if (robot_pos_1.z < 37.0 && robot_pos_1.z>35.7 && robot_pos_1.y < 0.1)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }
                    //16번,17번,18번 높은 장애물(높은점프)
                    if (robot_pos_1.z < 33.4 && robot_pos_1.z>32.0 && robot_pos_1.y < 0.1)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //19번 피라미드 장애물(높은2단점프)
                    if (robot_pos_1.z < 30.0 && robot_pos_1.z>28.0 && robot_pos_1.y < -0.4)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //20번 피라미드 장애물(높은2단점프)
                    if (robot_pos_1.z < 29.5 && robot_pos_1.z>28.0 && robot_pos_1.y < 0.1)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //21번 피라미드 장애물(높은2단점프)
                    if (robot_pos_1.z < 30.0 && robot_pos_1.z>28.0 && robot_pos_1.y < -0.4)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }


                    //22번 점프 같이하는 장애물
                    if (jumping) {
                        box_jump_p22 += cos(radians(jump)) / 40.0;
                        box_translate_p22 = translate(mat4(1), vec3(0.8, box_jump_p22, 26)) * scale(mat4(1), box_scale_rate3);//박스위치
                        box_transform_p22 = box_translate_p22 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

                        if (box_jump_p22 > -0.3) {
                            if (robot_pos_1.z < 26.2 && robot_pos_1.z > 25.8 && robot_pos_1.y < 0.1) {
                                highjumping = true;
                                backstep = true;
                                start = false;
                                box_jump_p22 = -0.75;
                                box_translate_p22 = translate(mat4(1), vec3(0.8, box_jump_p22, 26)) * scale(mat4(1), box_scale_rate3);//박스위치
                                box_transform_p22 = box_translate_p22 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));
                            }
                        }
                        else if (box_jump_p22 < -0.3) {
                            if (robot_pos_1.z < 26.2 && robot_pos_1.z > 25.8 && robot_pos_1.y < -0.4) {
                                highjumping = true;
                                backstep = true;
                                start = false;
                                box_jump_p22 = -0.75;
                                box_translate_p22 = translate(mat4(1), vec3(0.8, box_jump_p22, 26)) * scale(mat4(1), box_scale_rate3);//박스위치
                                box_transform_p22 = box_translate_p22 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));
                            }
                        }
                    }
                    else {
                        box_jump_p22 = -0.75;
                    }

                    //23번 점프 같이하는 장애물
                    if (jumping) {
                        box_jump_p23 += cos(radians(jump)) / 40.0;
                        box_translate_p23 = translate(mat4(1), vec3(0.8, box_jump_p23, 25)) * scale(mat4(1), box_scale_rate3);//박스위치
                        box_transform_p23 = box_translate_p23 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));

                        if (box_jump_p23 > -0.3) {
                            if (robot_pos_1.z < 25.2 && robot_pos_1.z > 24.8 && robot_pos_1.y < 0.1) {
                                highjumping = true;
                                backstep = true;
                                start = false;
                                box_jump_p23 = -0.75;
                                box_translate_p23 = translate(mat4(1), vec3(0.8, box_jump_p23, 25)) * scale(mat4(1), box_scale_rate3);//박스위치
                                box_transform_p23 = box_translate_p23 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));
                            }
                        }
                        else if (box_jump_p23 < -0.3) {
                            if (robot_pos_1.z < 25.2 && robot_pos_1.z > 24.8 && robot_pos_1.y < -0.4) {
                                highjumping = true;
                                backstep = true;
                                start = false;
                                box_jump_p23 = -0.75;
                                box_translate_p23 = translate(mat4(1), vec3(0.8, box_jump_p23, 25)) * scale(mat4(1), box_scale_rate3);//박스위치
                                box_transform_p23 = box_translate_p23 * translate(mat4(1), vec3(0, 0, 0)) * scale(mat4(1), vec3(1.0, 1.0, 1.0));
                            }
                        }
                    }
                    else {
                        box_jump_p23 = -0.75;
                    }
                    //24번 링 장애물
                    if (robot_pos_1.z < 21.5 && robot_pos_1.z>20.5 && robot_pos_1.y > -0.2)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //25번 링 장애물
                    if (robot_pos_1.z < 21.5 && robot_pos_1.z>20.5 && robot_pos_1.y < -0.5)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //26번 링 장애물
                    if (robot_pos_1.z < 17.5 && robot_pos_1.z>16.8 && robot_pos_1.y > 1.1)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                    //27번 링 장애물
                    if (robot_pos_1.z < 17.5 && robot_pos_1.z>16.8 && robot_pos_1.y < 0.4)
                    {
                        highjumping = true;
                        backstep = true;
                        start = false;

                    }

                }

            }

            if (jumping) {//팔벌리기
                arm_rotate_angle_1 = sin(radians(jump)) - 90;
                leg_rotate_angle_1 = sin(radians(jump)) - 90;
            }
            if (highjumping) {
                arm_rotate_angle_1 = sin(radians(jump)) - 90;
                leg_rotate_angle_1 = sin(radians(jump)) - 90;
            }




        }
        //초보난이도
        if (SPEED1==true)
        {
            //NPC
            if (move_w_2)
            {
                rotate_rate_2 += 10;
                arm_rotate_angle_2 += cos(radians(rotate_rate_2)) * 5;
                leg_rotate_angle_2 += cos(radians(rotate_rate_2)) * 5;

                if (robot_pos_2.z > -70.0) //npc가 승리시
                {
               

                    robot_pos_2.z -= NPC_SPEED2;
                  

                    if (robot_pos_2.z < 10.2 && !finish)// 패배
                    {
                        if (music_check) {
                            music = PlaySound(TEXT("defeat.wav"), 0, SND_FILENAME | SND_ASYNC);
                            if (!music) {
                                cout << "음악 재생 오류" << endl;
                            }
                            music_check = false;
                            start = false;
                        }
                        cout << "패배";
                        start = false;
                        jumping = false;
                        highjumping = false;
                        finish = true;
                    }


                    move_w = true;
                }
                else {
                    robot_angle_2 = 0;
                    move_s = true;
                    move_w = false;
                }
            }
        }
        //중수 난이도
        if (SPEED2==true)
        {
            //NPC
            if (move_w_2)
            {
                rotate_rate_2 += 10;
                arm_rotate_angle_2 += cos(radians(rotate_rate_2)) * 5;
                leg_rotate_angle_2 += cos(radians(rotate_rate_2)) * 5;

                if (robot_pos_2.z > -70.0) //npc가 승리시
                {

                    robot_pos_2.z -= NPC_SPEED3;


                    if (robot_pos_2.z < 10.2 && !finish)// 패배
                    {
                        if (music_check) {
                            music = PlaySound(TEXT("defeat.wav"), 0, SND_FILENAME | SND_ASYNC);
                            if (!music) {
                                cout << "음악 재생 오류" << endl;
                            }
                            music_check = false;
                            start = false;
                        }
                        cout << "패배";
                        start = false;
                        jumping = false;
                        highjumping = false;
                        finish = true;
                    }


                    move_w = true;
                }
                else {
                    robot_angle_2 = 0;
                    move_s = true;
                    move_w = false;
                }
            }
        }
        //고수 난이도
        if (SPEED3 == true)
        {
            //NPC
            if (move_w_2)
            {
                

                rotate_rate_2 += 10;
                arm_rotate_angle_2 += cos(radians(rotate_rate_2)) * 5;
                leg_rotate_angle_2 += cos(radians(rotate_rate_2)) * 5;

                if (robot_pos_2.z > -70.0) //npc가 승리시
                {

                    robot_pos_2.z -= NPC_SPEED4;

                   
                    if (robot_pos_2.z < 10.2 && !finish)// 패배
                    {
                        if (music_check) {
                            music = PlaySound(TEXT("defeat.wav"), 0, SND_FILENAME | SND_ASYNC);
                            if (!music) {
                                cout << "음악 재생 오류" << endl;
                            }
                            music_check = false;
                            start = false;
                        }
                        cout << "패배";
                        start = false;
                        jumping = false;
                        highjumping = false;
                        finish = true;
                    }


                    move_w = true;
                }
                else {
                    robot_angle_2 = 0;
                    move_s = true;
                    move_w = false;
                }
            }
        }

        //주인공
        mat4 left_arm_rotate_1 = translate(mat4(1), left_arm_pibot_1) * rotate(mat4(1), radians(arm_rotate_angle_1),
            vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot_1);
        mat4 right_arm_rotate_1 = translate(mat4(1), right_arm_pibot_1) * rotate(mat4(1), radians(-arm_rotate_angle_1),
            vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot_1);

        mat4 left_leg_rotate_1 = translate(mat4(1), right_arm_pibot_1) * rotate(mat4(1), radians(leg_rotate_angle_1),
            vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot_1);
        mat4 right_leg_rotate_1 = translate(mat4(1), left_arm_pibot_1) * rotate(mat4(1), radians(-leg_rotate_angle_1),
            vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot_1);

        stage_translate = translate(mat4(1), vec3(0, 0, 0));
        //stage_front_pos = vec3(0, 2, 0);
        if (jumping) {
            left_arm_rotate_1 = translate(mat4(1), left_arm_pibot_1) * rotate(mat4(1), radians(-arm_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot_1);
            right_arm_rotate_1 = translate(mat4(1), right_arm_pibot_1) * rotate(mat4(1), radians(-arm_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot_1);
            left_leg_rotate_1 = translate(mat4(1), right_arm_pibot_1) * rotate(mat4(1), radians(leg_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot_1);
            right_leg_rotate_1 = translate(mat4(1), left_arm_pibot_1) * rotate(mat4(1), radians(leg_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot_1);
        }

        if (highjumping) {
            left_arm_rotate_1 = translate(mat4(1), left_arm_pibot_1) * rotate(mat4(1), radians(-arm_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot_1);
            right_arm_rotate_1 = translate(mat4(1), right_arm_pibot_1) * rotate(mat4(1), radians(-arm_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot_1);
            left_leg_rotate_1 = translate(mat4(1), right_arm_pibot_1) * rotate(mat4(1), radians(leg_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot_1);
            right_leg_rotate_1 = translate(mat4(1), left_arm_pibot_1) * rotate(mat4(1), radians(leg_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot_1);
        }

        if (bodydown) {
            left_arm_rotate_1 = translate(mat4(1), left_arm_pibot_1) * rotate(mat4(1), radians(-arm_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot_1);
            right_arm_rotate_1 = translate(mat4(1), right_arm_pibot_1) * rotate(mat4(1), radians(-arm_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot_1);
            left_leg_rotate_1 = translate(mat4(1), right_arm_pibot_1) * rotate(mat4(1), radians(leg_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot_1);
            right_leg_rotate_1 = translate(mat4(1), left_arm_pibot_1) * rotate(mat4(1), radians(leg_rotate_angle_1),
                vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot_1);
        }

        robot_transform_body = translate(mat4(1), robot_pos_1) * rotate(mat4(1), radians(robot_angle_1), vec3(0, 1, 0)) * scale(mat4(1), vec3(0.1, 0.1, 0.25));
        robot_transform_1 = translate(mat4(1), robot_pos_1) * rotate(mat4(1), radians(robot_angle_1), vec3(0, 1, 0)) * scale(mat4(1), robot_scale_rate_1);
        head_transform_1 = robot_transform_1 * translate(mat4(1), vec3(0, 2.0, 2.5)) * scale(mat4(1), vec3(0.8, 0.8, 0.8));

        Ear_transform_left = head_transform_1 * translate(mat4(1), vec3(0.5, 1.0, 0)) * scale(mat4(1), vec3(0.8));
        Ear_transform_right = head_transform_1 * translate(mat4(1), vec3(-0.5, 1.0, 0)) * scale(mat4(1), vec3(0.8));

        left_arm_transform_1 = robot_transform_1 * translate(mat4(1), vec3(-1.5, -1.5, 1.7)) * left_arm_rotate_1 * scale(mat4(1), arm_scale_rate_1);
        right_arm_transform_1 = robot_transform_1 * translate(mat4(1), vec3(1.5, -1.5, 1.7)) * right_arm_rotate_1 * scale(mat4(1), arm_scale_rate_1);

        left_leg_transform_1 = robot_transform_1 * translate(mat4(1), vec3(1.5, -1.5, -1.7)) * left_leg_rotate_1 * scale(mat4(1), arm_scale_rate_1);
        right_leg_transform_1 = robot_transform_1 * translate(mat4(1), vec3(-1.5, -1.5, -1.7)) * right_leg_rotate_1 * scale(mat4(1), arm_scale_rate_1);





        //NPC
        mat4 left_arm_rotate = translate(mat4(1), left_arm_pibot_2) * rotate(mat4(1), radians(arm_rotate_angle_2),
            vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot_2);
        mat4 right_arm_rotate = translate(mat4(1), right_arm_pibot_2) * rotate(mat4(1), radians(-arm_rotate_angle_2),
            vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot_2);

        mat4 left_leg_rotate = translate(mat4(1), left_leg_pibot_2) * rotate(mat4(1), radians(leg_rotate_angle_2),
            vec3(1, 0, 0)) * translate(mat4(1), left_leg_pibot_2);
        mat4 right_leg_rotate = translate(mat4(1), right_leg_pibot_2) * rotate(mat4(1), radians(-leg_rotate_angle_2),
            vec3(1, 0, 0)) * translate(mat4(1), right_leg_pibot_2);

        stage_translate = translate(mat4(1), vec3(0, 0, 0));
        //stage_front_pos = vec3(0, 2, 0);

        robot_transform_2 = translate(mat4(1), robot_pos_2) * rotate(mat4(1), radians(robot_angle_2), vec3(0, 1, 0)) * scale(mat4(1), robot_scale_rate_2);
        head_transform_2 = robot_transform_2 * translate(mat4(1), vec3(0, 1.5, 0)) * scale(mat4(1), vec3(0.5, 0.5, 0.5));
        nose_transform_2 = head_transform_2 * translate(mat4(1), vec3(0, 0, 1)) * scale(mat4(1), vec3(0.3, 0.3, 1));

        left_arm_transform_2 = robot_transform_2 * translate(mat4(1), -arm_pos_2) * left_arm_rotate * scale(mat4(1), arm_scale_rate_2);
        right_arm_transform_2 = robot_transform_2 * translate(mat4(1), arm_pos_2) * right_arm_rotate * scale(mat4(1), arm_scale_rate_2);

        left_leg_transform_2 = robot_transform_2 * translate(mat4(1), vec3(-0.5, -1.5, 0)) * left_leg_rotate * scale(mat4(1), vec3(0.3, 0.5, 0.3));
        right_leg_transform_2 = robot_transform_2 * translate(mat4(1), vec3(0.5, -1.5, 0)) * right_leg_rotate * scale(mat4(1), vec3(0.3, 0.5, 0.3));
    }





    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    if (cameraangle) {
        cameraDirection = cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle));
    }
    else if (camerarotate) {
        cameraDirection = glm::normalize((cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle)) - cameraTarget));
    }



    if (playerview)
    {

        if (move_w) {
            cameraPos = robot_pos_1 + vec3(0, 0, -1);
            cameraDirection = cameraPos + vec3(0, 0, -1);
        }
        else if (move_a) {
            cameraPos = robot_pos_1 + vec3(-1, 0, 0);
            cameraDirection = cameraPos + vec3(-1, 0, 0);
        }
        else if (move_s) {
            cameraPos = robot_pos_1 + vec3(0, 0, 1);
            cameraDirection = cameraPos + vec3(0, 0, 1);
        }
        else if (move_d) {
            cameraPos = robot_pos_1 + vec3(1, 0, 0);
            cameraDirection = cameraPos + vec3(1, 0, 0);
        }
        view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    }


    else
        view = glm::lookAt(cameraPos + vec3(robot_pos_1.x, 0, robot_pos_1.z), cameraDirection + vec3(robot_pos_1.x, 0, robot_pos_1.z), cameraUp);

    glutPostRedisplay();
    glutTimerFunc(TIMERCLOCK, Timer, 0);
}


GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // 로봇 이동
        //case 'w':
        //    robot_pos_1.z -= 10;
        //    break;


        //// 주인공 일반점프
    case 'W':
    case 'w':
        //if (!jumping) jumping = true;
        if (bodydown == false && !finish)
        {
            jumping = true;
        }
        break;

        //주인공 하이점프 10번 장애물 이전에 사용불가
    case 'J':
    case 'j':
        if (robot_pos_1.z < 50.0 && bodydown == false && !finish)
        {
            highjumping = true;

        }

        break;

        // 주인공 순보
    case 'D':
    case 'd':
        if (!speedwalk && start) speedwalk = true;
        break;

        // 주인공 백스텝
    case 'a':
    case 'A':
        if (!backstep && start) backstep = true;
        if (robot_pos_1.z > 69.3)
        {
            backstep = false;
            back = 0;
        }
        break;
        
        // 출발
    case 'M':
    case 'm':
        if (game_start_check) {
            cout << endl << "----------난이도 선택----------" << endl << "1.초보 난이도" << endl << "2.중수 난이도" << endl << "3.고수 난이도"
                << endl << "난이도를 선택하세요: ";
            cin >> pick;
            //NPC
            if (pick == 1)
            {

                SPEED1 = true;
                robot_pos_1.z = 69.75;
            }
            if (pick == 2)
            {
                SPEED2 = true;
            }
            if (pick == 3)
            {
                SPEED3 = true;
            }


            if (!start) start = true;
            //if (!startnpc) startnpc = true;
            game_start_check = false;
        }
       
        break;


        // 카메라 기준 y축 회전
        /*case 'Y':
            cameraangle = true;
            camerarotate = false;
            camera_angle += radians(10.0f);
            break;
        case 'y':
            cameraangle = true;
            camerarotate = false;
            camera_angle -= radians(10.0f);
            break;*/


            //중심축 y 회전
    case 'r':
    case 'R':
        stage_rotate += radians(10.0f);

        cameraangle = false;
        camerarotate = true;
        if (counts < 60) {
            cameraPos = cameraPos + vec3(0.1, 0.0, -0.1);
            counts++;
        }
        else if (counts < 120) {
            cameraPos = cameraPos + vec3(-0.1, 0.0, -0.1);
            counts++;
        }
        else if (counts < 180) {
            cameraPos = cameraPos - vec3(0.1, 0.0, -0.1);
            counts++;
        }
        else if (counts < 240) {
            cameraPos = cameraPos + vec3(0.1, 0.0, 0.1);
            counts++;
        }
        else if (counts >= 240) {
            counts = 0;
        }
        break; cameraPos = glm::vec3(0, 0.5, 7);
        camera_angle = radians(-90.f);
        cameraTarget = glm::vec3(0, 0, 0);
        cameraDirection = cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle));
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        view = glm::lookAt(cameraPos + vec3(robot_pos_1.x, 0, robot_pos_1.z), cameraDirection + vec3(robot_pos_1.x, 0, robot_pos_1.z), cameraUp);
        playerview = false;
        break;

        //나가기
    case 'Q':
    case 'q':
        glutLeaveMainLoop();
        break;

    case '1'://주인공독점
        playerview = false;

        cameraPos = glm::vec3(5.0, 2.0, 3.4);//바닥크기


        camera_angle = radians(-90.f);

        cameraTarget = glm::vec3(0, 0, 0);
        //cameraDirection = cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle));
        cameraDirection = robot_pos_1 + vec3(cos(robot_angle_1), 0, sin(robot_angle_1));
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        break;

    case '2'://1인칭
        playerview = true;
        movecamera = false;
        break;
    case '3'://정면뷰
        cameraPos = glm::vec3(0, 0.5, 7);
        camera_angle = radians(-90.f);
        cameraTarget = glm::vec3(0, 0, 0);
        cameraDirection = cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle));
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        //view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
        playerview = false;
        movecamera = false;
        break;
    }

}

GLvoid key_up(unsigned char key, int x, int y)
{
    switch (key)
    {


    }
}

