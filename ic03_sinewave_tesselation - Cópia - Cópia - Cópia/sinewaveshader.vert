#version 330 core

in vec2 position; // receber� a posi��o x e y da malha. contudo, a vari�vel position.y vai se tornar a coordenada z e a coordenada y ficar� em fun��o da fun��o seno*cos.

//uniform mat4 MVP;
uniform float amp;

out float y;
out vec3 vPos;

void main(){
  //float pi = 3.14;
  y = cos((position.x)) * sin((position.y));
  y = y*amp;
  //y=0.0;
//  gl_Position = MVP * vec4(position.x, y, position.y, 1.0);
  vPos = (position.x, y, position.y);
}
