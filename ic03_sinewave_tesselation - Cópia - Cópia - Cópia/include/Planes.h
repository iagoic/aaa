#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef PLANES_H
#define PLANES_H


class Planes{
    private:
        float A;
        float B;
        float C;
        float D;
        glm::vec3 p;
        glm::vec3 n;
    public:
        Planes(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
        bool estaNoPlano(glm::vec3 pS);
        float distancia(glm::vec3 pS);
        float getA();
        float getB();
        float getC();
        float getD();
        glm::vec3 getP();
        glm::vec3 getN();

};

#endif // PLANES_H
