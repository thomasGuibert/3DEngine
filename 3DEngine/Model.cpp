#include "Model.h"

Model::Model(const float vertices[], unsigned int bufferSize, unsigned int attributeCount, Shader shader) : _shader(shader) {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);


    if (attributeCount > 0) {
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        if (attributeCount > 1) {
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            if ((attributeCount > 2)) {
                // normal attribute
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
                glEnableVertexAttribArray(2);
            }
        }
    }
};

void Model::drawOnPositions(const glm::vec3 cubePositions[], const int size) {
    glBindVertexArray(_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    for (unsigned int i = 0; i < size; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        model = glm::scale(model, _scale);
        _shader.updateUniformMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
};

void Model::setScale(const glm::vec3 scale) {
    _scale = scale;
};
