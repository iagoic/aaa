#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef PLANE_H
#define PLANE_H


class Plane{
    private:
        float A;
        float B;
        float C;
        glm::vec3 p;
        glm::vec3 n;
    public:
        Plane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2){
            glm::vec3 v = p1 - p0;
            glm::vec3 u = p2 - p0;
            glm::vec3 n = cross(v, u);
            n = normalize(n);
            this->A = n.x;
            this->B = n.y;
            this->C = n.z;
            this->n = n;
            this->p = p0;
        }
        bool estaNoPlano(glm::vec3 pS){
            glm::vec3 v1 = pS-this->p;
            if(dot(v1, n) == 0)
                return true;
            else
                return false;
        }
};

#endif // PLANE_H
