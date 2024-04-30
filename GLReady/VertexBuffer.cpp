#include "VertexBuffer.h"

#include <iostream>

#include <glad/glad.h>

namespace GLReady {

    VertexBuffer &VertexBuffer::SetData(float *data, unsigned cntFloats)
    {
        if (!_bufferCreated)
        {
            _VBOSize = cntFloats;

            glGenVertexArrays(1, &_VAO);
            glBindVertexArray(_VAO);

            glGenBuffers(1, &_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cntFloats, data, GL_STATIC_DRAW);

            glBindVertexArray(0);
            _bufferCreated = true;
        }
        else
        {
            glDeleteVertexArrays(1, &_VAO);
            glDeleteBuffers(1, &_VBO);
            _bufferCreated = false;

            return SetData(data, cntFloats);
        }

        return *this;
    }

    VertexBuffer &VertexBuffer::UpdateData(unsigned offsetFloats, unsigned newDataSizeFloats,
                                           float *newData)
    {
        if (offsetFloats + newDataSizeFloats >= _VBOSize)
        {
            std::cout << "[GLReady / VertexBuffer::UpdateData] fatal: Too much new data!"
                      << std::endl;
            return *this;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * offsetFloats,
                        sizeof(float) * newDataSizeFloats, newData);

        return *this;
    }

    VertexBuffer &VertexBuffer::SetUsage(unsigned location, unsigned nFloats)
    {
        if (_bufferCreated)
        {
            _verticesUsages.push_back({location, nFloats});
            _totalOffset += nFloats;
        }
        else
        {
            std::cout << "[GLReady / VertexBuffer::SetUsage] fatal: You must call 'SetData' before "
                         "setting usages!"
                      << std::endl;
        }

        return *this;
    }

    VertexBuffer &VertexBuffer::EndSetUsage()
    {
        unsigned preOffset = 0;

        glBindVertexArray(_VAO);
        for (auto &[location, nFloats] : _verticesUsages)
        {
            glVertexAttribPointer(location, nFloats, GL_FLOAT, GL_FALSE,
                                  _totalOffset * sizeof(float),
                                  (void *)(preOffset * sizeof(float)));
            glEnableVertexAttribArray(location);
            preOffset += nFloats;
        }
        _verticesUsages.resize(0);

        return *this;
    }

    void VertexBuffer::DrawTriangles(unsigned verticesOffset, unsigned verticesCnt)
    {
        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, verticesOffset, verticesCnt);
    }

    void VertexBuffer::DrawLines(unsigned verticesOffset, unsigned verticesCnt)
    {
        glBindVertexArray(_VAO);
        glDrawArrays(GL_LINE_STRIP, verticesOffset, verticesCnt);
    }

} // namespace GLReady
