#version 330 core
in vec3 vPos;
in vec3 vColor;
out vec3 passColor;

uniform mat4 modelTransform; //--- 모델링 변환 행렬: uniform 변수로 선언
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

uniform vec3 objectColor;

void main()
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4 (vPos, 1.0);
	passColor = vColor;
}