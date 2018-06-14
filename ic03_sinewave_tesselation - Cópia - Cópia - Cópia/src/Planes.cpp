#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Planes.h"

Planes::Planes(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2){
            glm::vec3 v = p1 - p0;
            glm::vec3 u = p2 - p0;
            glm::vec3 n = cross(v, u);
            n = normalize(n);
            this->A = n.x;
            this->B = n.y;
            this->C = n.z;
            this->n = n;
            this->p = p0;
            this->D = dot(-n, p0);
}

bool Planes::estaNoPlano(glm::vec3 pS){
    glm::vec3 v1 = pS-this->p;
    if(dot(v1, n) == 0)
        return true;
    else
        return false;
}

float getA(){
    return this->A;
}
float getB(){
    return this->B;
}
float getC(){
    return this->C;
}
float getD(){
    return this->D;
}
glm::vec3 getP(){
    return this->p;
}
glm::vec3 getN(){
    return this->n;
}
